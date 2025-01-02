#include "Networking.h"

#include "doomdef.h"
#include "p_local.h" // AIMINGTOSLOPE
#include "d_player.h"
#include "g_game.h"
#include "p_tick.h" // leveltime
//#include "p_setup.h" // For mapthings.
#include "r_state.h" // For thing lookups.
#include "m_menu.h" // For M_StartMessage and the like.
#include "d_main.h" // D_StartTitle.

#include "CS_Global.h"

// Way to check through all (visible/active) mobjs:
/*
    int i;
    mobj_t* thing;

    for (i = 0; i < numsectors; i ++)
    {
        sector_t* sec = &sectors[i];

        for (thing = sec->thinglist; thing; thing = thing->snext)
            count ++;
    }
*/

sock* game_socket = NULL;

boolean CS_client = 0;
boolean CS_host = 0;
boolean CS_mode = 0;

int self_id; // Player ID of self.
SelfPlayer self; // Some additional data of our own to send off to other players - event list, etc.

int csdisplayplayer = 0;

VirtualPlayer virtualplayers[32];

// Player movement history. (For each player.)
// Please excuse the naming conventions....
Position player_histories[32][MAXHISTORY];
tic_t    history_times[32];

boolean         client_connecting = 0;
struct sockaddr temp_serveraddress;
ServerPacket    temp_serverpacket;

UINT32 ban_list[100];
int ban_count = 0;

int missed_hits = 0;

void CS_Init(void)
{
    int i;
#ifndef NATIVENET
    int error = 0;
#endif

    if (CS_mode)
        return;

#ifndef NATIVENET
    Network_Startup(&error);
    if (error) CONS_Printf("Network_Startup() error %i\n", error);
#endif

    for (i = 0; i < MAXPLAYERS; i ++)
    {
        ResetPlayer(i);

        virtualplayers[i].in_game = 0;
    }

    self.event_count = 0;
    self.player_links = 0;

    CS_mode = 1;
}

void CS_Shutdown(void)
{
    int i;
#ifndef NATIVENET
    int error = 0;
#endif

    if (! CS_mode)
        return;

    // Send shutdown message if you're the host.
    if (CS_host)
    {
        for (i = 1; i < MAXPLAYERS; i ++)
        {
            if (virtualplayers[i].in_game)
                SendRefusePacket(&virtualplayers[i].address, M_GetText("Server has shutdown\n\nPress Esc"));
        }
    }

#ifndef NATIVENET
    // Shut down engine.
    if (game_socket != NULL)
    {
        Destroy_Socket(game_socket, &error);
        if (error) CONS_Printf("Destroy_Socket() error %i\n", error);
    }

    Network_Shutdown(&error);

    if (error) CONS_Printf("Network_Shutdown() error %i\n", error);
#endif

    for (i = 0; i < MAXPLAYERS; i ++)
    {
        int l;

        memset(&virtualplayers[i], 0, sizeof (VirtualPlayer));

        for (l = 0; l < MAXHISTORY; l ++)
            memset(&player_histories[i][l], 0, sizeof (Position));
    }

    memset(&self, 0, sizeof (SelfPlayer));

    CS_mode = CS_host = CS_client = mapswitches = client_connecting = csdisplayplayer = self_id = 0; // That should be about everything that's important.
}

void CS_Tick(void)
{
    int i;

    if (client_connecting)
        return;

    if (gamestate != GS_LEVEL)
    {
        for (i = 0; i < MAXPLAYERS; i ++)
        {
            virtualplayers[i].mo = NULL; // Test hack.
            virtualplayers[i].powers[pw_emeralds] = 0; // We can lose the emeralds now.
        }
    }

    if (CS_host)
    {
        CS_ServerUpdateFlags();
        CS_ServerUpdateRace();
    }

    if (self.had_gravityboots != (players[0].powers[pw_gravityboots] > 0))
    {
        CS_AddEvent(EVENT_GRAVITY | ((players[0].powers[pw_gravityboots] > 0) << 5));
        self.had_gravityboots = players[0].powers[pw_gravityboots] > 0;
    }

    ReceiveMessages();

    UpdatePlayers();

    UpdateHistories();

    ExecutePlayerEvents();

    CS_MobjStuff();

    CS_FireWeapons();

    for (i = 0; i < MAXPLAYERS; i ++)
    {
        if (virtualplayers[i].in_game)
            // Technically the ping to a client through the server will have the server's ping added.
        {
            if ((INT32) (I_GetTime() - virtualplayers[i].last_message) > TICRATE * 8 &&
                (INT32) (I_GetTime() - virtualplayers[i].last_update) > TICRATE * 8) // INT32 used because leveltime may actually become
            {                                                                        // smaller than last_update in some cases.
                RemovePlayer(i);

                CONS_Printf("\x82%s has timed out of the server.\n", virtualplayers[i].name);

                if (CS_client && i == 0)
                {
                    int l;

                    for (l = 0; l < MAXPLAYERS; l ++)
                        RemovePlayer(l);

                    //CS_Shutdown(); // CS_Shutdown will be called on CL_Reset() instead.

					// If we lose connection, go back to the title screen.
					M_StartMessage("You have timed out.\nTo continue playing, you must\nrejoin or join another server.\nPress ESC", NULL, MM_NOTHING);
					D_QuitNetGame();
					CL_Reset();
					D_StartTitle();
                }
            }

            if ((INT32) (I_GetTime() - virtualplayers[i].last_update) > 5 * TICRATE && CS_client)
                self.player_links &= ~(1 << i); // Just in case they've decided to disable clienttoclient.
        }
    }

    for (i = 0; i < MAXPLAYERS; i ++)
    {
        if (! virtualplayers[i].mo || ! virtualplayers[i].in_game || gamestate != GS_LEVEL)
            continue;

        // lasthittime is a client-side time, remember. It's on our end.
        if (virtualplayers[i].lasthitring && virtualplayers[i].player_times[self_id] >
            virtualplayers[i].lasthittime + 10) // 10-tic leeway for slight inaccuracies.
        {
            virtualplayers[i].lasthitring = NULL;
#ifndef BUNNIES
            CONS_Printf("Ring miss registered.\n");
#endif
            missed_hits ++;
        }
    }

#ifndef BUNNIES
    if (! (leveltime % (299 * TICRATE)) && (missed_hits + players[0].score)) // About once every five minutes.
    {
        CONS_Printf("Ring hit/miss stats:\n");
        CONS_Printf("Total ring hits: %li\nRing misses: %i\nHit percentage: %li/100\n", (ULONG)(missed_hits + players[0].score / 50), missed_hits, (ULONG)((players[0].score * 2) / (missed_hits + players[0].score / 50)));

        missed_hits = 0;
    }
#endif

    tmthing = NULL; // I don't like this thing crashing. Do not want.
}

void CS_LevelInit(void)
{
    int i;

    for (i = 0; i < MAXPLAYERS; i ++)
    {
        if (virtualplayers[i].in_game && i != self_id)
        {
#ifndef BUNNIES
            CONS_Printf("CS: Created %s (%i)\n", virtualplayers[i].name, i);
#endif
            ResetPlayer(i);

            virtualplayers[i].mo = P_SpawnMobj(0, 0, -(1024 << FRACBITS), MT_VIRTUALPLAYER);
            virtualplayers[i].last_update = I_GetTime(); // Kind of a hack.
        }
    }

    // Reset CTF stuff.
    flagstate[1] = flagstate[2] = FLAGSTATE_BASE;
    flagholder[1] = flagholder[2] = -1;

    // Reset Race stuff.
    finished_flags = 0;
    finished_count = 0;

    // Reset view.
    csdisplayplayer = self_id;

    // Reset some self stuff.
    self.had_gravityboots = 0;

/*    self.event_count = 0;

    for (i = 0; i < 5; i ++)
        self.event_list[i] = 0;*/ // BETA 4 - This is no longer done.

    // Reset hit stuff.
    missed_hits = 0;

    if (gametype == GT_RACE || gametype == GT_COOP)
    {
        // Destroy all enemies until they can be synchronised.
        for (i = 0; i < (int) numsectors; i ++)
        {
            sector_t* sec = &sectors[i];
            mobj_t* thing;

            for (thing = sec->thinglist; thing; thing = thing->snext)
            {
                if (thing->flags & MF_ENEMY)
                    P_RemoveMobj(thing);
            }
        }
    }
}

void UpdateHistories()
{
    int i;

    if (! players[0].mo)
        return; // Hack?

    for (i = 0; i < MAXPLAYERS; i ++)
    {
        int l;

        if ((virtualplayers[i].in_game && virtualplayers[i].mo) || i == self_id)
        {
            tic_t old_history_time = history_times[i];
            tic_t new_history_time = virtualplayers[i].player_times[i];
            mobj_t* mo = virtualplayers[i].mo;

            if (i == self_id)
            {
                mo = players[0].mo;
                new_history_time = leveltime;
            }

            // Shift the histories... don't ask me how it works... it probably doesn't.
            // Missed packets count as an extra skip, so things won't be 100% consistent...
            // (Also, if ! (l > 0), something is seriously wrong.) (Meaning (new_history_time - old_history time) would be < 0.)
            for (l = MAXHISTORY - 1; l >= (int) (new_history_time - old_history_time) && (int) (new_history_time - old_history_time) > 0; l --)
                player_histories[i][l] = player_histories[i][l - (int) (new_history_time - old_history_time)];

            player_histories[i][0].x = mo->x;
            player_histories[i][0].y = mo->y;
            player_histories[i][0].z = mo->z;

            if (i != self_id && virtualplayers[i].sync_type == SYNC_PREDICTMOVE)
            {
                player_histories[i][0].x = virtualplayers[i].realx;
                player_histories[i][0].y = virtualplayers[i].realy;
                player_histories[i][0].z = virtualplayers[i].realz;
            }

            history_times[i] = new_history_time;
        }
    }
}

Position* Get_HistoryItem(int player_id, tic_t time)
{
    if (history_times[player_id] - time > MAXHISTORY)
        return NULL; // Too far back, unfortunately!

    return &player_histories[player_id][history_times[player_id] - time];
}

void CS_AddEvent(UINT32 event)
{
    int i;

    for (i = MAXCSEVENTS - 1; i > 0; i --)
        self.event_list[i] = self.event_list[i - 1];

    self.event_list[0] = event;

    self.event_count ++;

    self.event_count &= 0x0F;
}

void AddPlayerUpdate(int id, PlayerUpdate* update)
{
    VirtualPlayer* player = &virtualplayers[id];
    int i, l;
    int shift;
    int hole_start = -1;

    shift = (update->my_time - player->update_times[0]);

    //if (shift != 1)
    //    CONS_Printf("Shift: %i\n", shift);

    if (player->update_times[0] == 0xFFFFFFFF) // Player has just started updating.
        shift = 0;

    if (shift >= UPDHISTORY || shift <= -UPDHISTORY)
    {
        // HACK. This is probably a safer thing to do. (Previously we did shift = 1)
        shift = 0;
        for (i = 0; i < UPDHISTORY; i ++)
            player->update_times[i] = 0xFFFFFFFF;
    }

    if (shift > 0)
    {
        for (i = UPDHISTORY - 1; i >= shift; i --)
        {
            memcpy(&player->update_history[i], &player->update_history[i - shift], sizeof (PlayerUpdate));
            player->update_times[i] = player->update_times[i - shift];
        }

        for (i = 0; i < shift; i ++)
            player->update_times[i] = 0xFFFFFFFF; // "Clear" this value.

        // Write the newest packet to the history.
        memcpy(&player->update_history[0], update, sizeof (PlayerUpdate));
        player->update_times[0] = update->my_time;
    }

    if (shift <= 0) // Odd-order receive, special case. Or even receiving the same update twice?
    {
        fixed_t x, y, z; // New plan: update the player, but use our old predicted position. It is less jumpy.
        x = player->update_history[-shift].x; y = player->update_history[-shift].y; z = player->update_history[-shift].z;

        memcpy(&player->update_history[-shift], update, sizeof (PlayerUpdate));
        player->update_times[-shift] = update->my_time;

        player->update_history[-shift].x = x; player->update_history[-shift].y = y; player->update_history[-shift].z = z;

        /*for (i = (-shift) - 1; i >= 0; i --) // Fix the "repeated event" bug by copying these events over to any apparently-outdated updates.
        {
            if (player->update_history[i].event_count < player->update_history[-shift].event_count)
            {
                memcpy(&player->update_history[i].event_list, &player->update_history[-shift].event_list, sizeof (player->update_history[i].event_list));
                player->update_history[i].event_count = player->update_history[-shift].event_count;
            }
        }*/

        if (player->update_times[-shift + 1] < player->update_times[-shift]) // Events are cleared during level init. So don't remove!
            player->update_history[-shift].event_count = player->update_history[-shift + 1].event_count; // No repeat events. Beta 4; probably better than the above code.
    }

    // Now is the time to fill up the gaps in the update history. We'll make up our own values based on logic.
    for (i = 0; i < UPDHISTORY; i ++)
    {
        if (player->update_times[i] == 0xFFFFFFFF && hole_start == -1)
        {
            hole_start = i;
            continue;
        }

        if (player->update_times[i] != 0xFFFFFFFF && hole_start != -1) // We've got a hole!
        {
            float x, y, z;
            int div;

            if (hole_start == 0) // ...but sorry, we can't do this.
            {
                hole_start = -1;
                continue;
            }

            // By logic, i = hole_end. hole_start is a hole, while hole_end is the first non-hole after.
			div = i - hole_start + 1;

            x = (float) (player->update_history[hole_start - 1].x - player->update_history[i].x) / div;
            y = (float) (player->update_history[hole_start - 1].y - player->update_history[i].y) / div;
            z = (float) (player->update_history[hole_start - 1].z - player->update_history[i].z) / div;

            for (l = 0; l < i - hole_start; l ++)
            {
                memcpy(&player->update_history[hole_start + l], &player->update_history[i], sizeof (PlayerUpdate));

                player->update_history[hole_start + l].x += (INT32) (x * ((i - hole_start) - l));
                player->update_history[hole_start + l].y += (INT32) (y * ((i - hole_start) - l));
                player->update_history[hole_start + l].z += (INT32) (z * ((i - hole_start) - l));

                player->update_history[hole_start + l].my_time =
                player->update_times[hole_start + l] = player->update_times[hole_start - 1] - l - 1; // Better do this!

                player->update_history[hole_start + l].throwing_ring = 0; // Best do this as well.
            }

            //CONS_Printf("Filled %i holes. Count %i. Shift %i\n", i - hole_start, count, shift);

            hole_start = -1;
        }
    }

    for (l = 1; l < UPDHISTORY; l ++)
    {
        if (player->update_times[l] != 0xFFFFFFFF && player->update_times[l - 1] != 0xFFFFFFFF &&
            player->update_times[l] != player->update_times[l - 1] - 1)
            CONS_Printf("ERRREUROEUROERR (%i %i)\n", player->update_times[l], player->update_times[l - 1]);
    }
}

void CS_Say(int player_id, SayMessage* msg)
{
    const char *cstart = "\x80", *cend = "\x80";
    char* my_name;
    UINT8 type = msg->type & 7;
    INT8 to_player = msg->type >> 3;

//    if (dedicated && to_player == 0) to_player = -1; // Hack!

    if (CS_host) // Will be easier to identify bugs this way.
    {            // If a client receives a message it shouldn't, we can correct that easily without being fooled.
        if (type == 1 && to_player != self_id && player_id != self_id) // This message is not for you.
            return;

        if (type == 2 && player_id != self_id && virtualplayers[player_id].ctfteam != (players[0].spectator ? 0 : players[0].ctfteam))
            return;

        if (type == 3 && player_id > 0)
            return;
    }

    // In CTF and team match, color the player's name.
    if (gametype == GT_CTF || (gametype == GT_MATCH && cv_matchtype.value))
    {
        INT8 sender_id = player_id;

        if (type == 1 && player_id == self_id && to_player != self_id)
            sender_id = to_player;

        if (sender_id == self_id)
        {
            if (players[0].ctfteam == 1) // red
                cstart = "\x85";
            else if (players[0].ctfteam == 2) // blue
                cstart = "\x84";
        }
        else
        {
            if (virtualplayers[sender_id].ctfteam == 1)
                cstart = "\x85";
            else if (virtualplayers[sender_id].ctfteam == 2)
                cstart = "\x84";
        }
    }

    my_name = player_names[0];//dedicated ? "Server" : player_names[0];

    if (type == 0) // Normal say message.
        CONS_Printf("\3<%s%s%s> %s\n", cstart, player_id == self_id ? my_name : virtualplayers[player_id].name, cend, msg->message);
    else if (type == 1 && to_player == self_id) // Sayto to you.
        CONS_Printf("\3*%s%s%s* %s\n", cstart, player_id != -1 ? virtualplayers[player_id].name : "Server", cend, msg->message);
    else if (type == 1 && to_player != self_id) // Sayto to another player.
        CONS_Printf("\3->*%s%s%s* %s\n", cstart, virtualplayers[to_player].name, cend, msg->message);
    else if (type == 2) // Sayteam.
        CONS_Printf("\3>>%s%s%s<< (team) %s\n", cstart, player_id == self_id ? my_name : virtualplayers[player_id].name, cend, msg->message);
    else if (type == 3) // CSay.
    {
		DEBPRINT("Server message: ");
        HU_SetCEchoDuration(5);
		HU_DoCEcho(msg->message);
    }
}

int CS_PlayerCount()
{
    int i;
    int count = 0;

    if (! CS_host && ! CS_client)
        return 0;

    for (i = 0; i < MAXPLAYERS; i ++)
    {
        if (virtualplayers[i].in_game || i == self_id)
            count ++;
    }

    return count;
}

boolean HasShield(INT32* powers)
{
    return (powers[pw_forceshield] || powers[pw_ringshield] || powers[pw_jumpshield] || powers[pw_watershield] ||
            powers[pw_bombshield]);
}

boolean CollisionCheck(mobj_t* good, mobj_t* evil)
{
    if (good->z > evil->z + evil->height)
        return 0;

    if (good->z + good->height < evil->z)
        return 0;

// if (abs(good->x - evil->x) > good->radius + evil->radius || abs(good->y - evil->y) > good->radius + evil->radius)
    /* LXShadow: Added halfway-check. Todo: Add halfway-check for Z? Need to be careful, because ordinary-looking
                 code here could yield some messy results. */
    if (R_PointToDist2(good->x, good->y, evil->x, evil->y) > good->radius + evil->radius &&
        R_PointToDist2(good->x - FixedDiv(good->momx, 2 << FRACBITS), good->y - FixedDiv(good->momy, 2 << FRACBITS),
                       evil->x - FixedDiv(evil->momx, 2 << FRACBITS), evil->y - FixedDiv(evil->momy, 2 << FRACBITS)) > good->radius + evil->radius)
        return 0;

    return 1;
}
