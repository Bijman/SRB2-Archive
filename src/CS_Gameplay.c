#include "CS_Global.h"
#include "CS_Console.h"

#include "p_local.h"
#include "g_game.h"
#include "r_state.h"
#include "p_setup.h" // mapthings
#include "dstrings.h"

// CTF
char flagstate  [3] = {0, FLAGSTATE_BASE, FLAGSTATE_BASE};
int  flagholder [3] = {0, -1, -1};

// RACE
UINT32 finished_flags;
tic_t  finished_times[CSMAXPLAYERS];

INT8 race_rank[CSMAXPLAYERS];
INT8 finished_count;

boolean HACK_YOUAREBEINGNUKED = 0;

extern fixed_t TEST_RINGSPD[MAXPLAYERS];
extern boolean USE_TESTRINGSPD[MAXPLAYERS];

// ringtype_strings uses the same order as the WEP_ macros.
char* ringtype_strings[7] =
    {
        "",
        "automatic ",
        "bounce ",
        "scatter ",
        "grenade ",
        "explosion ",
        "rail ",
    };

fixed_t R_PointToDist3(fixed_t x1, fixed_t y1, fixed_t z1, fixed_t x2, fixed_t y2, fixed_t z2);

// CSTODO: LXShadow: You want to make sure that these functions
// are also declared in CS_Global so there's no "implict declaration" warnings
// This isn't a coding style cleanup, but fixing it so it can compile correctly
void CS_FireWeapons(void)
{
    int i;
	Position *temp;
	fixed_t speed;
	Position *my_old_position, *even_older;

/*    if (dedicated)
        return; // Hah! Take that, server-side processing!*/

    for (i = 0; i < MAXPLAYERS; i ++)
    {
        mobj_t* missile;
        VirtualPlayer* player = &virtualplayers[i];

        mobjtype_t type;
        UINT32 flags2;
        fixed_t slope;
        fixed_t spawn_x, spawn_y, spawn_z;

        if (! player->in_game)
            continue;

        if (! player->throwing_ring)
            continue;

        if (! player->mo) // Would this ever happen?
            continue;

        switch (player->current_weapon)
        {
            case WEP_BOUNCE:
                type = MT_THROWNBOUNCE;
                flags2 = MF2_BOUNCERING; break;
            case WEP_RAIL:
                type = MT_REDRING;
                flags2 = MF2_RAILRING | MF2_DONTDRAW; break;
            case WEP_AUTO:
                type = MT_THROWNAUTOMATIC;
                flags2 = MF2_AUTOMATIC; break;
            case WEP_EXPLODE:
                type = MT_THROWNEXPLOSION;
                flags2 = MF2_EXPLOSION; break;
            case WEP_SCATTER:
                type = MT_THROWNSCATTER;
                flags2 = MF2_SCATTER; break;
            case WEP_GRENADE:
                type = MT_THROWNGRENADE;
                flags2 = MF2_GRENADE; break; // What I would do for an "autobreak" feature on-demand...
            default:
                type = MT_REDRING;
                flags2 = 0;
        }

        spawn_x = player->mo->x;
        spawn_y = player->mo->y;
        spawn_z = player->mo->z;

        temp = Get_HistoryItem(i, virtualplayers[i].player_times[i] - 1);
        if (temp && i != 31)
        {
            spawn_x = temp->x; spawn_y = temp->y; spawn_z = temp->z;
        }

        if (player->sync_type & SYNC_RELATIVEPOS)
        {
            spawn_x = player->fire_x;
            spawn_y = player->fire_y;
            spawn_z = player->fire_z;
        }

        missile = P_SpawnMobj(spawn_x, spawn_y, spawn_z + (player->mo->height / 3), type);
        missile->flags2 = flags2;

        speed = missile->info->speed;

        missile->angle = player->mo->angle;

        my_old_position = Get_HistoryItem(self_id, virtualplayers[i].player_times[self_id]);
        even_older = Get_HistoryItem(self_id, virtualplayers[i].player_times[self_id] - 1);

        if (player->sync_type & SYNC_PREDRELANG)
        {
            if (! my_old_position || ! even_older)
                CONS_Printf("Warning: Cannot properly fire a ring as we do not have enough history of ourselves!\n");
        }

        fixed_t jump_by = 0;

        if (player->sync_type & SYNC_RINGJUMP && cv_ringmove.value == 0)
        {
            fixed_t dist = R_PointToDist2(missile->x, missile->y, players[0].mo->x, players[0].mo->y);

            fixed_t tics_wouldbe = FixedDiv(dist, speed);
            fixed_t tics_left = tics_wouldbe - ((leveltime - player->player_times[self_id]) << FRACBITS);

            if (tics_left)
                speed = FixedDiv(dist, tics_left);
            else
                speed = 60 << FRACBITS; // NO DIVISION BY 0 PLEASE.

            #define RINGSPEEDLIMIT (100 << FRACBITS)
            // When the ring reaches a certain speed limit, it jumps ahead to compensate. jump_by is used below.
            if (speed > RINGSPEEDLIMIT)
            {
                fixed_t new_tics;

                new_tics = FixedDiv(dist, RINGSPEEDLIMIT); // new_tics: amount of tics used at 75/tic
                jump_by = new_tics - tics_left;            // tics_left: amount that would be used at full adjusted speed.

                speed = RINGSPEEDLIMIT;
            }

            if (speed < 70 << FRACBITS) // Speed increase is so slight, it's not worth risking consistency. Jump instead.
            {                           // Also, no rings slower than 60 fracunits/tic, please.
                speed = 60 << FRACBITS;
                jump_by = (leveltime - player->player_times[self_id]) << FRACBITS;
            }
        }
        else if (player->sync_type & SYNC_RINGJUMP && cv_ringmove.value == 1)
            jump_by = (leveltime - player->player_times[self_id]) << FRACBITS;

        if ((player->sync_type & SYNC_PREDRELANG) && USE_TESTRINGSPD[i])
        {
            if (cv_ringmove.value == 0)
                speed = TEST_RINGSPD[i];
            else
                speed = TEST_RINGSPD[i]; // WORK ON THIS
        }

        slope = AIMINGTOSLOPE(player->aiming); // TODO: player->aiming changes, so player->angle should too, really.

        missile->momx = FixedMul(speed, FINECOSINE(missile->angle>>ANGLETOFINESHIFT));
        missile->momy = FixedMul(speed, FINESINE(missile->angle>>ANGLETOFINESHIFT));
        missile->momz = FixedMul(speed, slope);

        // "if aiming" which is pretty much always true.
        missile->momx = FixedMul(missile->momx,FINECOSINE(player->aiming>>ANGLETOFINESHIFT));
        missile->momy = FixedMul(missile->momy,FINECOSINE(player->aiming>>ANGLETOFINESHIFT));

        P_SetTarget(&missile->target, player->mo); // Moved this down from the slope = AIMINGTOSLOPE line.
        P_CheckMissileSpawn(missile);

        if (jump_by)
        {
/*            for (i = 0; i < jump_by >> FRACBITS; i ++) // An effort to stop rings from going through walls. Help?
            {
                //P_CheckPosition(missile, missile->x + missile->momx, missile->y + missile->momy);
                if (missile->z <= missile->subsector->sector->floorheight || missile->z >= missile->subsector->sector->ceilingheight)
                    break;
                P_UnsetThingPosition(missile);
                missile->x += missile->momx;
                missile->y += missile->momy;
                missile->z += missile->momz;
                P_SetThingPosition(missile);
            }*/
            missile->x += FixedMul(missile->momx, jump_by);
            missile->y += FixedMul(missile->momy, jump_by);
            missile->z += FixedMul(missile->momz, jump_by);
        }

        if (player->current_weapon == WEP_BOUNCE)
            missile->fuse = 3 * TICRATE;

        if (player->current_weapon == WEP_GRENADE)
        {
            int throwtics = leveltime - player->player_times[self_id];

            if (throwtics >= missile->info->mass || throwtics < 0)
                throwtics = missile->info->mass - 1;

            P_InstaThrust(missile, player->mo->angle, player->toss_strength);
            missile->momz = player->toss_strength;

            if (player->mo->eflags & MFE_VERTICALFLIP)
                missile->momz = -missile->momz; // Toss "up" not "down."

            for (i = 0; i < throwtics; i ++)
            {
                P_CheckGravity(missile, 1);
                missile->z += missile->momz;

                if (missile->z <= missile->floorz)
                {
                    missile->z = missile->floorz;
                    break;
                }
                if (missile->z + missile->height >= missile->ceilingz)
                {
                    missile->z = missile->ceilingz - missile->height;
                    missile->momz = 0;
                }

                P_TryMove(missile, missile->x + missile->momx, missile->y + missile->momy, 1);
            }

            missile->fuse = missile->info->mass - throwtics;
        }

        if (player->current_weapon == WEP_RAIL)
        {
            const boolean nblockmap = !(missile->flags & MF_NOBLOCKMAP);
            for (i = 0; i < 256; i++)
            {
                if (nblockmap)
                {
                    P_UnsetThingPosition(missile);
                    missile->flags |= MF_NOBLOCKMAP;
                    P_SetThingPosition(missile);
                }

                if (i&1)
                    P_SpawnMobj(missile->x, missile->y, missile->z, MT_SPARK);

                P_RailThinker(missile);
            }
        }

        if (player->current_weapon == WEP_SCATTER)
        {
            #define MAKERING(ang, aiming, zadd) {\
            mobj_t* mo = P_SpawnMobj(spawn_x, spawn_y, spawn_z + (zadd) + (player->mo->height / 3), type); \
            mo->momx = FixedMul(speed, FINECOSINE((ang)>>ANGLETOFINESHIFT)); \
            mo->momy = FixedMul(speed, FINESINE((ang)>>ANGLETOFINESHIFT)); \
            mo->momz = FixedMul(speed, AIMINGTOSLOPE((aiming))); \
            mo->momx = FixedMul(mo->momx,FINECOSINE((aiming)>>ANGLETOFINESHIFT)); \
            mo->momy = FixedMul(mo->momy,FINECOSINE((aiming)>>ANGLETOFINESHIFT)); \
            if (jump_by) \
            { \
                mo->x += FixedMul(mo->momx, jump_by); \
                mo->y += FixedMul(mo->momy, jump_by); \
                mo->z += FixedMul(mo->momz, jump_by); \
            } \
            mo->angle = ang; \
            P_SetTarget(&mo->target, player->mo); \
            P_CheckMissileSpawn(mo);}

            angle_t ang = missile->angle; // Take the old ring's data before abandoning the pointer.

            // Spawn the other scatter rings.
            MAKERING(ang - ANG2, player->aiming, 0);
            MAKERING(ang + ANG2, player->aiming, 0);
            MAKERING(ang, player->aiming + ANG1, 12 << FRACBITS); // CSTODO?: *FRACUNIT may be better than << FRACBITS
            MAKERING(ang, player->aiming - ANG2, 24 << FRACBITS); /* LXShadow: For constant expressions, it makes no difference.
                                                                     For integers, << is usually (and sometimes significantly) faster in terms of performance. */
            P_SetTarget(&missile->target, player->mo);

            P_CheckMissileSpawn(missile);

            #undef MAKERING
        }

        if (missile->info->seesound)
            S_StartSound(player->mo, missile->info->seesound); // Oh look, a necessary hack gone unnecessary!

        // Colour the ring. (Copypaste from P_ColorTeamMissile().)
        if (player->current_weapon == 0)
        {
            if (gametype == GT_CTF || (gametype == GT_MATCH && cv_matchtype.value))
            {
                missile->flags |= MF_TRANSLATION;

                if (player->ctfteam == 1)
                    missile->color = 6;
                else
                    missile->color = 8;
            }
        }

        // Flippity flip flip.
        if (player->mo->eflags & MFE_VERTICALFLIP)
            missile->flags2 |= MF2_OBJECTFLIP;

        player->throwing_ring = 0;
    }
}

#define RED 1
#define BLUE 2

void CS_ServerUpdateFlags()
{
    int i, l;
    boolean updated = false;
    char newflagstate [3] = {0, flagstate[RED], flagstate[BLUE]};
    char newflagholder[3] = {0, flagholder[RED], flagholder[BLUE]};

    if (gametype != GT_CTF || gamestate != GS_LEVEL)
        return; // Not relevant here!

    for (i = 1; i <= 2; i ++)
    {
        mapthing_t *flagpoint = i == 1 ? rflagpoint : bflagpoint;
        mobjtype_t motype = i == 1 ? MT_REDFLAG : MT_BLUEFLAG;
		mobj_t *flag;

        if (flagstate[i] == FLAGSTATE_HELD)
        {
            if (flagholder[i] != self_id && ! virtualplayers[flagholder[i]].in_game) // Player has quit with the flag...dummy.
                newflagstate[i] = FLAGSTATE_BASE;
            else
                continue;
        }

        flag = FindFlag(i);

        if (! flag && flagstate[i] != FLAGSTATE_DROPNOW)
        {
            flag = P_SpawnMobj(flagpoint->x << FRACBITS, flagpoint->y << FRACBITS, flagpoint->z << FRACBITS, motype);
            flag->fuse = 1;
            flag->spawnpoint = flagpoint;
            flag->flags2 = MF2_JUSTATTACKED;

            CONS_Printf("Warning: Flag %i was not found and had to be created.\n", i);
        }

        if (flagstate[i] == FLAGSTATE_BASE || flagstate[i] == FLAGSTATE_DROPPED)
        {
            for (l = 0; l < MAXPLAYERS; l ++)
            {
                mobj_t *mo;
                char ctfteam;

                if (l > 0 && ! virtualplayers[l].in_game)
                    continue;
                if ((l == 0 && players[0].powers[pw_flashing] > 0) || (l > 0 && virtualplayers[l].flashing > 0) ||
                    (l == 0 && players[0].playerstate != PST_LIVE) || (l == 0 && players[0].spectator) ||
                    (l > 0 && virtualplayers[l].spectator) ||
                    (l > 0 && (virtualplayers[l].mo->frame & FF_FRAMEMASK) == (states[S_PLAY_PAIN].frame & FF_FRAMEMASK)) ||
                    (l > 0 && virtualplayers[l].toss_delay > 0) || (l == 0 && players[0].tossdelay > 0))
                    continue;

                mo = l == 0 ? players[0].mo : virtualplayers[l].mo; // Note: As a general rule, the host always has a player ID of 0.
                ctfteam = l == 0 ? players[0].ctfteam : virtualplayers[l].ctfteam;

                if (ctfteam != i &&
                    ((l == 0 && players[0].powers[pw_super]) || (l > 0 && virtualplayers[l].powers[pw_super])))
                    continue; // Supers can only return flags.

                if (CollisionCheck(mo, flag))
                {
                    char set_state;

                    if (ctfteam != i) // Player is picking up the enemy flag.
                        set_state = FLAGSTATE_HELD;
                    else // Player is returning the team's flag.
                        set_state = FLAGSTATE_BASE;

                    newflagstate[i] = set_state;
                    newflagholder[i] = l;

                    updated = 1;
                }
            }
        }

        if (flagstate[i] == FLAGSTATE_DROPNOW && flagholder[i] != -1)
        {
            newflagstate[i] = FLAGSTATE_DROPPED;
            newflagholder[i] = -1;

            updated = 1;
        }
    }

    // Check for successful flag captures.
    for (i = 0; i < MAXPLAYERS; i ++)
    {
        char special;
		mobj_t *mo;

        if (i > 0 && ! virtualplayers[i].in_game)
            continue;

        if (i == flagholder[1])
            special = 4; // Blue base - red holder is holding red flag, so they must be blue.
        else if (i == flagholder[2])
            special = 3;
        else continue;

        mo = i > 0 ? virtualplayers[i].mo : players[0].mo;

        if (GETSECSPECIAL(mo->subsector->sector->special, 4) == special && mo->z == mo->floorz) // Test 1.
        {
            if (i == flagholder[RED] && flagstate[BLUE] == FLAGSTATE_BASE)      newflagstate[RED]  = FLAGSTATE_CAPTURED;
            else if (i == flagholder[BLUE] && flagstate[RED] == FLAGSTATE_BASE) newflagstate[BLUE] = FLAGSTATE_CAPTURED;

            updated = 1;
        }
        else if (mo->subsector->sector->ffloors) // Test 2.
        {
			ffloor_t *rover;

			for (rover = mo->subsector->sector->ffloors; rover; rover = rover->next)
			{
				if (!(rover->flags & FF_EXISTS))
					continue;

				if (GETSECSPECIAL(rover->master->frontsector->special, 4) != special)
					continue;

				if (mo->z <= *rover->topheight
					&& mo->z >= *rover->bottomheight)
                {
                    if (i == flagholder[RED] && flagstate[BLUE] == FLAGSTATE_BASE)      newflagstate[RED]  = FLAGSTATE_CAPTURED;
                    else if (i == flagholder[BLUE] && flagstate[RED] == FLAGSTATE_BASE) newflagstate[BLUE] = FLAGSTATE_CAPTURED;

                    updated = 1;
                }
			}
        }
    }

    if (updated)
    {
        CS_UpdateFlags(newflagstate[RED] | ((newflagholder[RED] & 63) << 2) | (newflagstate[BLUE] << 8) | ((newflagholder[BLUE] & 63) << 10));

        for (i = 0; i < MAXPLAYERS; i ++)
        {
            if (virtualplayers[i].in_game)
                SendServerPacket(i);
        }
    }
}

void CS_UpdateFlags(UINT16 flag_state)
{
    int i;

    char newflagstate  [3] = {0, flag_state & 3, flag_state >> 8 & 3};
    int  newflagholder [3] = {0, flag_state >> 2 & 63, flag_state >> 10 & 63};

    for (i = 1; i <= 2; i ++)
    {
        // Read-only variables here, for each flag colour. Remember to set the right variables while writing to them.
        const char* flagname  = i == 1 ? ("\x85" "red" "\x80") : ("\x84" "blue" "\x80");
		mobjtype_t motype     = i == 1 ? MT_REDFLAG    : MT_BLUEFLAG;
		mapthing_t* flagpoint = i == 1 ? rflagpoint    : bflagpoint;

        if (newflagholder[i] == 63) newflagholder[i] = -1;

        if (newflagstate[i] == FLAGSTATE_DROPPED && flagstate[i] != FLAGSTATE_DROPPED)
        {
            mobj_t *flag;
            mobj_t *holder = NULL;
            fixed_t x, y, z;
            boolean flip = 0;

            if (flagholder[i] != -1)
            {
                if (flagholder[i] == self_id)
					holder = players[0].mo;
                else
					holder = virtualplayers[flagholder[i]].mo;
            }

            while ((flag = FindFlag(i)))
                P_SetMobjState(flag, S_DISS); // Destroy all other flags of this colour, just in case.

            if (holder)
            {
                x = holder->x; y = holder->y; z = holder->z;
                flip = (holder->eflags & MFE_VERTICALFLIP) > 0;

                if (CS_client) // Try to drop the flag in the right place.
                {
                    if (flagholder[i] != self_id)
                    {
                        int u;
                        int update_id = -1;
                        tic_t time_dropped = 0;
                        UINT32 last_eventcount = 0;
                        VirtualPlayer* player = &virtualplayers[flagholder[i]];

                        for (u = UPDHISTORY - 1; u >= 0; u --)
                        {
                            if (player->update_times[u] == 0xFFFFFFFF)
                                continue;

                            /* LXShadow: The below assumes that only one event is happening per tic. This is untrue.
                                         However, until we get "time-of-drop" variables in the server packet,
                                         this function will always be hacky. */
                            UINT ev_id = player->update_history[u].event_list[0] & 0x1F;

                            if ((ev_id == EVENT_PLAYERHIT || ev_id == EVENT_DROPMYFLAG) &&
                                player->update_history[u].event_count != last_eventcount)
                            {
                                time_dropped = player->update_times[u];
                                last_eventcount = player->update_history[u].event_count;
                                update_id = u;
                            }
                        }

                        if (update_id != -1)
                        {
                            x = player->update_history[update_id].x;
                            y = player->update_history[update_id].y;
                            z = player->update_history[update_id].z;
                        }
                    }
                    else
                    {
                        Position* old = Get_HistoryItem(self_id, virtualplayers[0].update_history[0].player_times[self_id] + virtualplayers[0].update_history[0].my_time);

                        if (old)
                        {
                            x = old->x; y = old->y; z = old->z;
                        }
                    }
                }
            }
            else
            {
#ifndef BUNNIES
                CONS_Printf("CS warning: Player %i does not exist! Dropping flag anyway.\n", flagholder[i]);
#endif
                x = flagpoint->x << FRACBITS;
                y = flagpoint->y << FRACBITS;
                z = flagpoint->z << FRACBITS;
            }

            flag = P_SpawnMobj(x, y, z, motype);

            flag->momz = 8 * FRACUNIT;

            if (flip)
            {
                flag->z -= holder->height / 2; // Why divide by 2, anyway? High-res or something?
                flag->momz = -flag->momz;
                flag->flags2 |= MF2_OBJECTFLIP;
            }

            //P_SetTarget(&flag->target, holder);

            flag->spawnpoint = flagpoint;
            flag->fuse = cv_flagtime.value * TICRATE;

            if (flagholder[i] != -1) // Just in case.
            {
                char* playername = virtualplayers[flagholder[i]].name;
                if (flagholder[i] == self_id)
                    playername = player_names[0];

                CONS_Printf("%s dropped the %s flag.\n", playername, flagname);
            }
            else
                CONS_Printf("The %s flag has been dropped.\n", flagname);

            if (flagholder[i] == self_id)
                players[0].gotflag = 0;

            flagstate[i] = FLAGSTATE_DROPPED;
            flagholder[i] = -1;
            if (i == 1) redflag = flag; else blueflag = flag;
        }

        if (newflagstate[i] == FLAGSTATE_HELD && flagstate[i] != FLAGSTATE_HELD)
        {
            mobj_t* flag;
			char *playername;

            while ((flag = FindFlag(i)))
                P_SetMobjState(flag, S_DISS); // Destroy the flag(s).

            flagstate[i] = FLAGSTATE_HELD;
            flagholder[i] = newflagholder[i];

            if (newflagholder[i] == self_id)
            {
                players[0].gotflag |= i == 1 ? MF_REDFLAG : MF_BLUEFLAG;

                players[0].pflags &= ~PF_GLIDING;
				players[0].climbing = 0;
				if (players[0].powers[pw_tailsfly])
					players[0].powers[pw_tailsfly] = 1;
            }

            if (i == 1) redflag  = NULL;
            else        blueflag = NULL;

            if (newflagholder[i] == self_id)
            {
                playername = player_names[0];

                S_StartSound(players[0].mo, sfx_lvpass);
            }
            else
            {
                playername = virtualplayers[newflagholder[i]].name;

                if (virtualplayers[newflagholder[i]].in_game && virtualplayers[newflagholder[i]].mo) // Just in case.
                    S_StartSound(virtualplayers[newflagholder[i]].mo, sfx_lvpass);
            }

            CONS_Printf("%s picked up the %s flag!\n", playername, flagname);
        }

        if (newflagstate[i] == FLAGSTATE_BASE && flagstate[i] != FLAGSTATE_BASE)
        {
            mobj_t* flag = FindFlag(i);

            if (! flag) // Ugh...
                flag = P_SpawnMobj(0, 0, 0, motype);

            flag->spawnpoint = flagpoint;
            flag->fuse = 1;
            flag->flags2 |= MF2_JUSTATTACKED;

            flagstate[i] = FLAGSTATE_BASE;

            CONS_Printf("The %s flag has returned to base.\n", flagname);

            if (flagholder[i] == self_id) // Remove the flag from the player's possession - sorry sir! (Out-of-order packet fix.)
            {
                players[0].gotflag = 0;
                CONS_Printf("CS warning: Forced to remove flag from your posession.\n");
            }
        }

        if (newflagstate[i] == FLAGSTATE_CAPTURED && flagstate[i] != FLAGSTATE_CAPTURED)
        {
            mobj_t* mo;
            mobj_t* player;
            char* player_name;

            if (flagholder[i] == -1)
            {
                CONS_Printf("ERROR: Flagholder is -1 during flag capture! This should never happen!\n");
                continue;
            }

            if (flagholder[i] == self_id)
            {
                player_name = player_names[0];
                player = players[0].mo;
            }
            else
            {
                player_name = virtualplayers[flagholder[i]].name;
                player = virtualplayers[flagholder[i]].mo;

                if (! virtualplayers[flagholder[i]].in_game)
                {
                    CONS_Printf("Warning: flag capturer does not exist to this client.\n");
                    player_name = "(error)";
                    player = players[0].mo; // CSTODO: Make sure he exists to the host! (dedicated servers mean no players[0].mo)
                }
            }

            HU_SetCEchoFlags(0);
            HU_SetCEchoDuration(5);
            HU_DoCEcho(va("%s\\captured the %s flag.\\\\\\\\", player_name, flagname));
            I_OutputMsg("%s captured the %s flag.\n", player_name, flagname);

            if (players[0].ctfteam != i && ! players[0].spectator)
                S_StartSound(NULL, sfx_flgcap);
            else if (players[0].ctfteam == i && ! players[0].spectator)
                S_StartSound(NULL, sfx_lose);

            if (flagholder[i] == self_id)
            {
                players[0].gotflag &= ~(i == 1 ? MF_REDFLAG : MF_BLUEFLAG);
                players[0].score += 250;
            }
            else
                virtualplayers[flagholder[i]].score += 250;

            mo = P_SpawnMobj(player->x,
                            player->y,
                            player->z,
                            i == 1 ? MT_REDFLAG : MT_BLUEFLAG);
            mo->flags &= ~MF_SPECIAL;
            mo->fuse = TICRATE;
            mo->spawnpoint = i == 1 ? rflagpoint : bflagpoint;
            mo->flags2 |= MF2_JUSTATTACKED;

            flagstate[i] = FLAGSTATE_CAPTURED; // Hack: for host the send update to other players
            flagholder[i] = -1;

            if (CS_host) // The clients will actually update their score as soon as they receive the ServerPacket.
            {
                if (i == 1)
                    bluescore ++;
                else
                    redscore ++;
            }
        }
    }
}

#undef RED
#undef BLUE

mobj_t* FindFlag(INT8 flag_colour)
{
    int i;
    mobj_t* thing;

    for (i = 0; i < (int) numsectors; i ++)
    {
        sector_t* sec = &sectors[i];

        for (thing = sec->thinglist; thing; thing = thing->snext)
        {
            if (thing->state == &states[S_DISS])
                continue;

            if (thing->type == MT_REDFLAG && flag_colour == 1)
                return thing;

            if (thing->type == MT_BLUEFLAG && flag_colour == 2)
                return thing;
        }
    }

    return NULL;
}

void CS_ServerUpdateRace()
{
    int i;

    if (gametype != GT_RACE)
        return;

    tic_t lowest_time = 0xFFFFFFFF;

    // Find out the lowest time for everyone so far.
    for (i = 0; i < CSMAXPLAYERS; i ++)
    {
        if (! virtualplayers[i].in_game && i != self_id)
            continue;

        tic_t time = (i == self_id ? leveltime : virtualplayers[i].player_times[i]);

        if (time < lowest_time)
            lowest_time = time;
    }

    for (i = 0; i < CSMAXPLAYERS; i ++)
    {
        if ((finished_flags & (1 << i)) && finished_times[i] < lowest_time)
        {
            // Add to the ranks.
            race_rank[finished_count ++] = i;

            CONS_Printf(M_GetText("%s has finished the race.\n"), i == self_id ? player_names[0] : virtualplayers[i].name);

            finished_flags &= ~(1 << i);
        }
    }
}

void CS_MobjStuff()
{
    size_t i;

    if (gamestate != GS_LEVEL)
        return;

    for (i = 0; i < numsectors; i ++)
    {
        sector_t* sec = &sectors[i];
        mobj_t* thing;

        for (thing = sec->thinglist; thing; thing = thing->snext)
        {
            int l;
            VirtualPlayer* player;

            // While we're at it, make sure there isn't anyone already carrying this emerald.
            if ((thing->type >= MT_EMERALD1 && thing->type <= MT_EMERALD7) || thing->type == MT_FLINGEMERALD)
            {
                INT32 emeraldsinhand = players[0].powers[pw_emeralds];
                boolean fling = (thing->type == MT_FLINGEMERALD);

                for (l = 0; l < MAXPLAYERS; l ++)
                {
                    if (virtualplayers[l].in_game)
                        emeraldsinhand |= virtualplayers[l].powers[pw_emeralds];
                }

                if (((emeraldsinhand & thing->info->speed) && ! fling) ||
                    ((emeraldsinhand & thing->threshold) && fling))
                {
                    if (! fling && thing->target && thing->target->type == MT_EMERALDSPAWN) // Emerald spawn stuff that I don't understand.
                    {
                        if (thing->target->target)
                            P_SetTarget(&thing->target->target, NULL);

                        thing->target->threshold = 0;

                        P_SetTarget(&thing->target, NULL);
                    }

                    P_RemoveMobj(thing);
                }

                continue;
            }

            // Do a hacky collision check to destroy the MAXMOVE issue.
            if ((thing->type == MT_REDRING || thing->type == MT_THROWNAUTOMATIC || thing->type == MT_THROWNBOUNCE ||
                thing->type == MT_THROWNEXPLOSION || thing->type == MT_THROWNSCATTER) && thing->target != players[0].mo &&
                players[0].mo)
            {
                fixed_t check_x = thing->x, check_y = thing->y, check_z = thing->z;
                fixed_t thing_speed = R_PointToDist3(0, 0, 0, thing->momx, thing->momy, thing->momz);
                fixed_t thing_steps = FixedDiv(thing_speed, thing->radius);
                boolean collided = 0;

                if (! thing->momx && ! thing->momy && ! thing->momz) // Uh-oh - frozen ring which didn't properly collide.
                {
                    thing_steps = 0; // Hack - don't do the below loop.
                    P_RemoveMobj(thing);
                }

                for (l = 0; l < thing_steps >> FRACBITS; l ++)
                {
                    check_x -= FixedDiv(thing->momx, thing_steps);
                    check_y -= FixedDiv(thing->momy, thing_steps);
                    check_z -= FixedDiv(thing->momz, thing_steps);

                    if (R_PointToDist2(check_x, check_y, players[0].mo->x, players[0].mo->y) < thing->radius + players[0].mo->radius &&
                        thing->z + thing->height >= players[0].mo->z && thing->z <= players[0].mo->z + players[0].mo->height)
                    {
                        collided = 1;
                        break;
                    }
                }

                if (collided)
                {
                    thing->x = check_x;
                    thing->y = check_y;
                    thing->z = check_z;
                    thing->momx = thing->momy = thing->momz = 0;
                }
            }

            // Do collision checks.
            for (l = 0; l < MAXPLAYERS; l ++)
            {
                boolean collided = 0;
				fixed_t realx, realy, realz;

                if (l == self_id || ! virtualplayers[l].in_game || ! virtualplayers[l].mo)
                    continue;

                player = &virtualplayers[l];

                // Check for two collisions.
                realx = player->mo->x;
				realy = player->mo->y;
				realz = player->mo->z;

                player->mo->x -= FixedDiv(player->mo->momx, 2 << FRACBITS);
                player->mo->y -= FixedDiv(player->mo->momy, 2 << FRACBITS);
                player->mo->z -= FixedDiv(player->mo->momz, 2 << FRACBITS);

                if (thing->type == MT_THROWNGRENADE)
                    thing->radius = thing->info->painchance; // HACK (for collision check).

                if (CollisionCheck(player->mo, thing))
                    collided = 1;

                player->mo->x = realx; player->mo->y = realy; player->mo->z = realz;

                if (CollisionCheck(player->mo, thing))
                    collided = 1;

                // If either were successful, then continue as planned.
                if (collided)
                    CollideMobjWithPlayer(thing, player);

                if (thing->type == MT_THROWNGRENADE)
                    thing->radius = thing->info->radius; // END HACK.
            }
        }
    }
}

void CollideMobjWithPlayer(mobj_t* thing, VirtualPlayer* player)
{
    if (player->spectator)
        return; // Bad idea!

    switch (thing->type)
    {
        default: break; // This is really annoying. Forever hate these new compiler warnings.
        case MT_FLINGRING:
        case MT_RING:
        case MT_REDTEAMRING:
        case MT_BLUETEAMRING:
        {
            if (player->flashing <= (flashingtics/4)*3 &&
                ! (thing->type == MT_REDTEAMRING && player->ctfteam == 2) &&
                ! (thing->type == MT_BLUETEAMRING && player->ctfteam == 1) &&
                (player->mo->frame & FF_FRAMEMASK) != (states[S_PLAY_PAIN].frame & FF_FRAMEMASK))
            {
                S_StartSound(player->mo, sfx_itemup);

                P_SpawnMobj(thing->x, thing->y, thing->z, MT_SPARK);

                P_RemoveMobj(thing);
            }
            break;
        }
        case MT_RAILPICKUP:
        case MT_RAILRING:
        case MT_BOUNCEPICKUP:
        case MT_BOUNCERING:
        case MT_AUTOPICKUP:
        case MT_AUTOMATICRING:
        case MT_GRENADEPICKUP:
        case MT_GRENADERING:
        case MT_SCATTERPICKUP:
        case MT_SCATTERRING:
        case MT_EXPLODEPICKUP:
        case MT_EXPLOSIONRING:
        {
            mobj_t* temp;

            if ((player->mo->frame & FF_FRAMEMASK) == (states[S_PLAY_PAIN].frame & FF_FRAMEMASK))
                break; // Don't want to pick up anything yet.

            temp = P_SpawnMobj(thing->x, thing->y, thing->z, thing->type);
            temp->flags &= ~MF_SPECIAL;
            temp->health = 0;
            temp->momz = FRACUNIT;
            temp->fuse = thing->info->damage;

            if (thing->eflags & MFE_VERTICALFLIP)
                temp->momz = -temp->momz;

            P_SetMobjState(temp, thing->info->raisestate);

            if (thing->type == MT_RAILPICKUP || thing->type == MT_BOUNCEPICKUP || thing->type == MT_AUTOPICKUP ||
                thing->type == MT_GRENADEPICKUP || thing->type == MT_SCATTERPICKUP || thing->type == MT_EXPLODEPICKUP)
                S_StartSound(player->mo, sfx_ncitem);
            else // Ring sound.
                S_StartSound(player->mo, sfx_itemup);

            P_SetMobjState(thing, S_DISS);
        }
        break;
        case MT_THROWNGRENADE:
        case MT_SPINFIRE:
        {
            if (thing->state == &states[thing->info->deathstate] || thing->state == &states[S_DISS] ||
                (thing->state - states >= S_XPLD1 && thing->state - states <= S_XPLD4))
                break;

            if ((player->flashing || (player->powers[pw_watershield] && thing->type == MT_SPINFIRE)) &&
                player->mo != thing->target)
                P_SetMobjState(thing, thing->info->deathstate);
            break;
        }
        case MT_REDRING:
        case MT_THROWNAUTOMATIC:
        case MT_THROWNBOUNCE:
        case MT_THROWNEXPLOSION:
        case MT_THROWNSCATTER:
        {
            if (thing->state == &states[thing->info->deathstate] || thing->state == &states[S_DISS] ||
                (thing->type == MT_THROWNEXPLOSION && thing->state - states >= S_XPLD1 && thing->state - states <= S_XPLD4) ||
                thing->sprite == SPR_SPRK)
                break;
            if (thing->target == player->mo) // The new collision check can cause this to happen.
                break;

            P_SetMobjState(thing, thing->info->deathstate);

            if (player->flashing || thing->target != players[0].mo)
                break;
            if (player->mo->frame == states[S_PLAY_PAIN].frame)
                break; // CSTODO: Sometimes hit events are received before (or maybe the same frame) this client detects the hit.
                       // Is this normal or is the ping a little off?
            if ((gametype == GT_CTF || (gametype == GT_MATCH && cv_matchtype.value)) &&
                player->ctfteam == players[0].ctfteam)
                break;
            else
            {
                player->lasthitring = thing;
                player->lasthittime = leveltime;
            }

            break;
        }
    }
}

void ExecutePlayerEvents(void)
{
    int i, l;

    for (i = 0; i < MAXPLAYERS; i ++)
    {
        VirtualPlayer* player = &virtualplayers[i];
		int event_start;

        if (i == self_id)
            continue;

        if (! player->in_game)
            continue;

        if (! player->mo)
            continue;

        event_start = (player->event_count - player->last_eventcount) - 1;

        if (player->event_count < player->last_eventcount)
            event_start = 0x0F - (player->last_eventcount - player->event_count);

        if (event_start < 0)
            continue; // No new events.

        if (event_start >= MAXCSEVENTS)
        {
            CONS_Printf("CS warning: %s - %i events missed.\n", player->name, event_start - MAXCSEVENTS + 1);
            event_start = MAXCSEVENTS - 1; // Too many new events! Execute as many as we can.
        }

        for (l = event_start; l >= 0; l --)
        {
            UINT32 event = player->event_list[l];

            switch (event & 0x1F)
            {
                case EVENT_SOUNDPLAY:
                    switch (event >> 5 & 3)
                    {
                        case 0:
                            S_StartSound(player->mo, sfx_jump);
                            break;
                        case 1:
                            S_StartSound(player->mo, sfx_spring);
                            break;
                        case 2:
                            S_StartSound(player->mo, sfx_wdjump);
                            break;
                    }
                    break;
                case EVENT_THOK:
                {
                    mobj_t* mo;

                    if (player->spectator) // Uh, no.
                        break; // LXShadow: didn't bother adding checks to the rest of the events, because we don't want to miss them (we might get an out-of-order packet saying that this player is a spectator when s/he isn't).

                    S_StartSound(player->mo, sfx_thok);

                    mo = P_SpawnMobj(
                        player->mo->x,
                        player->mo->y,
                        player->mo->z - FixedDiv(FIXEDSCALE(player->mo->info->height, player->mo->scale) - player->mo->height, 3 * FRACUNIT),
                        MT_THOK);

                    mo->flags |= MF_TRANSLATION;
                    mo->color = player->mo->color;
                    P_SetTarget(&mo->target, player->mo);
                    mo->floorz = mo->z;
                    mo->ceilingz = mo->z + mo->height;
                    P_SetScale(mo, player->mo->scale);
                    mo->destscale = player->mo->scale;

                    break;
                }
                case EVENT_GOTSAYMSG:
                {
                    if (! CS_host)
                        break;

                    player->saymessage_id = event >> 5 & 0x0F;
                    break;
                }
                case EVENT_PLAYERHIT:
                    ExecuteHitEvent(i, event);
                    break;
                case EVENT_MONITORPOP:
                {
                    UINT16 monitor_id = event >> 5;
                    mobj_t* mo = NULL;

                    if (monitor_id > nummapthings)
                    {
                        CONS_Printf("Error: monitor_id too high.\n");
                        break;
                    }

                    for (i = 0; i < (int) numsectors; i ++)
                    {
                        sector_t* sec = &sectors[i];
                        mobj_t* thing;

                        for (thing = sec->thinglist; thing; thing = thing->snext)
                        {
                            if (thing->x >> 16 == mapthings[monitor_id].x && thing->y >> 16 == mapthings[monitor_id].y &&
                                thing->flags & MF_MONITOR)
                                mo = thing;
                        }
                    }

                    if (! mo)
                    {
                        CONS_Printf("CS warning: monitor doesn't even exist!\n");
                        break;
                    }

                    if (mo->fuse > 0 || mo->state == &states[S_DISS])
                    {
#ifndef BUNNIES
                        CONS_Printf("CS warning: Monitor popped twice.\n");
#endif
                        break;
                    }

                    // Copypaste from P_KillMobj:
                    if ((cv_itemrespawn.value && (modifiedgame || netgame || multiplayer)))
                        mo->fuse = cv_itemrespawntime.value*TICRATE + 2; // Random box generation

                    P_SetTarget(&mo->target, player->mo);
                    P_KillMobj(mo, NULL, player->mo);
                    break;
                }
                case EVENT_NUKE:
                {
                    int a;
					fixed_t x, y, z;
					Position* old;

                    S_StartSound(player->mo, sfx_bkpoof);

                    for (a = 0; a < 16; a ++)
                    {
                        mobj_t* mo;
                        angle_t fa;
                        fa = (a*(FINEANGLES/16));
                        mo = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, MT_SUPERSPARK);
                        mo->momx = FixedMul(FINESINE(fa),60 << FRACBITS)/NEWTICRATERATIO;
                        mo->momy = FixedMul(FINECOSINE(fa),60 << FRACBITS)/NEWTICRATERATIO;
                    }

                    if (! players[0].mo) // Just in case - future dedicated stuff.
                        break;

                    x = players[0].mo->x; y = players[0].mo->y; z = players[0].mo->z;

                    old = Get_HistoryItem(self_id, player->player_times[self_id]);

                    if (old) // Use our old position instead, in case this opponent really wanted to track us down.
                    {
						x = old->x;
						y = old->y;
						z = old->z;
					}

                    if (P_AproxDistance(P_AproxDistance(x - player->mo->x, y - player->mo->y), z - player->mo->z) <
                            1536 << FRACBITS && ! ((gametype == GT_CTF || (gametype == GT_MATCH && cv_matchtype.value)) && players[0].ctfteam == player->ctfteam)) // Hit us hard!
                    {
                        HACK_YOUAREBEINGNUKED = 1; // Bombs away! [hack]
                        P_DamageMobj(players[0].mo, player->mo, player->mo, 1);
                        HACK_YOUAREBEINGNUKED = 0;

                        players[0].bonuscount += 10; // Flash the palette.
                    }

                    break;
                }
                case EVENT_DROPMYFLAG:
                    if (CS_host)
                    {
                        PlayerDropFlag(i);

                        if (event >> 5) // Player is tossing the flag.
                        {
                            player->toss_delay = 2 * TICRATE - (leveltime - player->player_times[self_id]);

                            if ((INT32) player->toss_delay < 0)
                                player->toss_delay = 0;
                        }
                    }
                    break;
                case EVENT_GETEMMY:
                {
                    if (CS_host)
                    {
                        int z;
                        int emID = event >> 5;
                        boolean emerald_held = 0;

                        if (players[0].powers[pw_emeralds] & 1 << emID)
                            emerald_held = 1;

                        for (z = 1; z < MAXPLAYERS; z ++)
                        {
                            if (virtualplayers[z].powers[pw_emeralds] & 1 << emID)
                                emerald_held = 1;
                        }

                        if (emerald_held)
                            break; // Someone already has this emerald.

                        player->powers[pw_emeralds] |= (1 << emID);

                        if (mobjinfo[MT_EMERALD1 + emID].deathsound != sfx_None)
                            S_StartSound(player->mo, mobjinfo[MT_EMERALD1 + emID].deathsound);
                    }
                    break;
                }
                case EVENT_GRAVITY:
                    if (event >> 5)
                        player->powers[pw_gravityboots] = 1;
                    else
                        player->powers[pw_gravityboots] = 0;
                    break;
                case EVENT_NEWLAP:
                    ExecuteLapEvent(i, event);
                    break;
            }
        }
    }
}

void ExecuteHitEvent(int player_id, UINT32 event)
{
    INT8 ring_type = event >> 10 & 7;
    INT8 hit_type = event >> 13 & 31;
    INT8 murderer = event >> 5 & 31;
    boolean killed = event >> 18 & 1;
    INT8 ring_count = (event >> 19 & 31) + 1;
    boolean had_shield = event >> 24 & 1;

    VirtualPlayer* player = &virtualplayers[player_id != -1 ? player_id : 0]; // May not necessarily be used if player_id is us.

    char* playername = (player_id == self_id ? player_names[0] : virtualplayers[player_id != -1 ? player_id : 0].name);
    char* murdername = (murderer  == self_id ? player_names[0] : virtualplayers[murderer].name);

    if (player_id != self_id && ! player->mo)
        return; // I don't know why this would ever happen, but better be safe - see sigsegv point below.

    if (ring_type > 6)
        return;

    if (player_id > -1)
    {
        switch (hit_type)
        {
            case 0:
                CONS_Printf("%s was %s by %s's %sring.\n",playername,
                            killed ? "killed" : "hit",
                            murdername,
                            ringtype_strings[ring_type]);
            break; case 1:
                CONS_Printf("%s was %s by %s's fire trail.\n", playername, killed ? "fried" : "burned", murdername);
            break; case 2:
                CONS_Printf("%s got %s by %s!\n", playername, killed ? "kablooied" : "nuked", murdername);
        }

        if (killed)
        {
            switch (hit_type)
            {
                case 3:
                    CONS_Printf(text[PDEAD_DIED], "", playername, ""); break;
                case 4:
                    CONS_Printf(text[PDEAD_GOOP], "", playername, ""); break;
                case 5:
                    CONS_Printf(text[PDEAD_FIRE], "", playername, ""); break;
                case 6:
                    CONS_Printf(text[PDEAD_ELEC], "", playername, ""); break;
                case 7:
                    CONS_Printf(text[PDEAD_PIT], "", playername, ""); break;
                case 8:
                    CONS_Printf(text[PDEAD_SPAC], "", playername, ""); break;
                case 9:
                    CONS_Printf(text[PDEAD_DROWNED], "", playername, ""); break;
                case 10:
                    CONS_Printf(text[PDEAD_SPIK], "", playername, ""); break;
                case 11:
                    CONS_Printf(text[PDEAD_CRUSHED], "", playername, ""); break;
                case 12:
                    CONS_Printf(text[PDEAD_DIED], "", playername, ""); break;
            }
        }
    }

    if (hit_type >= 0 && hit_type <= 2)
    {
        int score_add = killed ? 100 : 50;
        char team;

        if (murderer == self_id)
        {
            players[0].score += score_add;
            team = players[0].ctfteam;
        }
        else
        {
            virtualplayers[murderer].score += score_add;
            team = virtualplayers[murderer].ctfteam;
        }

        if (gametype == GT_MATCH && cv_matchtype.value)
        {
            if (team == 1) redscore  += score_add;
            if (team == 2) bluescore += score_add;
        }
    }

    if (hit_type > 2 && killed && (gametype == GT_MATCH || gametype == GT_TAG) && cv_match_scoring.value == 0) // Suicide penalty!
    {
        UINT32* score = player_id == self_id ? (UINT32*) &players[0].score : &player->score;

        if (*score >= 50)
            *score -= 50;
    }

    if (player_id != self_id) // Stuff for when this is not us.
    {
        if (! had_shield)
            S_StartSound(player->mo, killed ? sfx_altdi1 : sfx_altow1);
        else
            S_StartSound(player->mo, sfx_shldls); // No death expected.

        if (ring_type == 4) // Grenade ring should be destroyed. Use nearest.
        {
            size_t i;
            fixed_t last_dist = 0x7FFFFFFF;
            mobj_t* last_grenade = NULL;
            for (i = 0; i < numsectors; i ++)
            {
                sector_t* sec = &sectors[i];
                mobj_t* thing;

                for (thing = sec->thinglist; thing; thing = thing->snext)
                {
                    fixed_t dist = R_PointToDist2(thing->x, thing->y, player->mo->x, player->mo->y); // POINT OF SIGSEGV
                    if (thing->type == MT_THROWNGRENADE && dist < last_dist && thing->state != &states[thing->info->deathstate])
                    {
                        last_dist = dist;
                        last_grenade = thing;
                    }
                }
            }

            if (last_grenade)
                P_SetMobjState(last_grenade, last_grenade->info->deathstate); // Destroy it!
        }

        player->lasthitring = NULL; // Debug stuff!
    }

    if (! killed && ! had_shield && hit_type != 13) // Hit type 13 involves no ring loss. Or emerald loss.
        PlayerDropRings(player_id, ring_count);

    if (! had_shield && hit_type != 13)
        PlayerDropEmeralds(player_id); // Player needs to drop emeralds even if they're killed.

    PlayerDropFlag(player_id);
}

void ExecuteLapEvent(int player_id, UINT32 event)
{
    UINT32 laps = event >> 5;
    char* name;

    if (player_id == self_id)
    {
        name = player_names[0];
        players[0].laps = laps;
    }
    else
    {
        name = virtualplayers[player_id].name;
        virtualplayers[player_id].laps = laps;
    }

    if (laps >= (unsigned)cv_numlaps.value)
        ;//CONS_Printf(M_GetText("%s has finished the race.\n"), name);
    else
        CONS_Printf(M_GetText("%s started lap %d\n"), name, laps + 1);

    if (laps >= (unsigned) cv_numlaps.value)
    {
		if (!countdown) // a 60-second wait ala Sonic 2.
			countdown = cv_countdowntime.value*TICRATE + 1; // Use cv_countdowntime

		if (!countdown2)
			countdown2 = (11 + cv_countdowntime.value)*TICRATE + 1; // 11sec more than countdowntime

        if (player_id == self_id)
            players[0].exiting = 3*TICRATE;

        if (P_CheckRacers())
            players[0].exiting = (14*TICRATE)/5 + 1;

        if (CS_host)
        {
            finished_flags |= 1 << player_id;
            finished_times[player_id] = (player_id == self_id ? leveltime : virtualplayers[player_id].player_times[player_id]);
        }
    }
}

void CS_DoHitEvent(mobj_t* inflictor, INT8 hit_type, boolean killed)
{
    INT8 i;
    INT8 ring_type = 0;
    UINT32 ev;

    if (players[0].powers[pw_flashing] || players[0].powers[pw_invulnerability] || (players[0].powers[pw_super] && hit_type != 13))
    {
        if (! killed)
            return;
    }

    for (i = 0; i < MAXPLAYERS; i ++)
    {
        if (! virtualplayers[i].in_game) continue;

        if (hit_type == 0 || hit_type == 1)
        {
            if (inflictor && virtualplayers[i].mo == inflictor->target)
                break;
        }
        else if (hit_type == 2)
        {
            if (virtualplayers[i].mo == inflictor)
                break; // Nuke - different rules.
        }
        else // Suicide.
        {
            i = self_id; // =P
            break;
        }
    }

    if (i == MAXPLAYERS)
		i = self_id; // This used to be -1, but if we send that as an event then it'll think that player 31 did the hit.

    if (hit_type == 0 && inflictor) // Ring hit.
    {
        switch (inflictor->type)
        {
            case MT_THROWNAUTOMATIC:
                ring_type = WEP_AUTO;
                break;
            case MT_THROWNBOUNCE:
                ring_type = WEP_BOUNCE;
                break;
            case MT_THROWNSCATTER:
                ring_type = WEP_SCATTER;
                break;
            case MT_THROWNGRENADE:
                ring_type = WEP_GRENADE;
                break;
            case MT_THROWNEXPLOSION:
                ring_type = WEP_EXPLODE;
                break;
            default:
                ring_type = 0;
        }

        if (ring_type == 0 && inflictor->flags2 & MF2_RAILRING)
            ring_type = WEP_RAIL;
    }

    ev = EVENT_PLAYERHIT;
    ev |= (i & 31) << 5;
    ev |= (ring_type & 7) << 10;
    ev |= (hit_type & 31) << 13;
    ev |= killed << 18;
	ev |= ((players[0].health - 2) & 31) << 19;
    ev |= (HasShield(players[0].powers)) << 24;

    CS_AddEvent(ev);

    ExecuteHitEvent(self_id, ev); // Execute this event on our side too.
}

void CS_DoMonitorPopEvent(mobj_t* monitor) //P_SetMobjState(mobj, mobj->deathstate)
{
    size_t i;
    UINT32 ev;
    UINT16 monitor_id = 0xFFFF;

    //return; // REMOVED FOR DECENT PLAYTESTING.

    /*if (! monitor->spawnpoint) Well there's your problem.
    {
        CONS_Printf("??\n");
        return;
    }*/

    for (i = 0; i < nummapthings; i ++)
    {
        if (mapthings[i].x == monitor->x >> 16 && mapthings[i].y == monitor->y >> 16)
            monitor_id = i;
    }

    if (monitor_id == 0xFFFF)
    {
        CONS_Printf("Error: could not find monitor ID!\n");
        return;
    }

    ev = EVENT_MONITORPOP;
    ev |= monitor_id << 5;

    CS_AddEvent(ev);
}

void CS_DoNukeEvent(void)
{
    UINT32 ev = 0;

    ev |= EVENT_NUKE;

    CS_AddEvent(ev); // ..That's pretty much it.
}

void CS_DoGetEmmyEvent(mobj_t* emerald)
{
    int emerald_id;// = emerald->type - MT_EMERALD1;

    if (emerald->type == MT_FLINGEMERALD)
        for (emerald_id = 0; (1 << emerald_id) != emerald->threshold && emerald_id < 7; emerald_id ++);
    else
        for (emerald_id = 0; (1 << emerald_id) != emerald->info->speed && emerald_id < 7; emerald_id ++);

    if (CS_client)
        CS_AddEvent(EVENT_GETEMMY | (emerald_id << 5));
    else if (CS_host)
    {
        int i;
        boolean emerald_held = 0;

        for (i = 1; i < MAXPLAYERS; i ++)
        {
            if (! virtualplayers[i].in_game) continue;

            if (virtualplayers[i].powers[pw_emeralds] & 1 << emerald_id)
                emerald_held = 1;
        }

        // Add the emerald to this player's collection, if possible.
        if (! emerald_held)
        {
            players[0].powers[pw_emeralds] |= 1 << emerald_id;

            if (emerald->info->deathsound != sfx_None)
                S_StartSound(players[0].mo, emerald->info->deathsound);
        }
#ifndef BUNNIES
        else
            CONS_Printf("CS warning: Forced to remove emerald from your possession.\n");
#endif
    }
}

void PlayerDropFlag(int player_id)
{
    int i;

    if (CS_host)
    {
        for (i = 1; i <= 2; i ++)
        {
            if (flagholder[i] == player_id) flagstate[i] = FLAGSTATE_DROPNOW;
        }
    }
}

void PlayerDropRings(int player_id, int num_rings)
{
    int i;
    mobj_t* playermo;
	UINT8 randomangle;
	angle_t fa;
	fixed_t ns;
	int skip_tics = 0;

    if (player_id != self_id && ! virtualplayers[player_id].in_game)
        return;

    playermo = player_id == self_id ? players[0].mo : virtualplayers[player_id].mo;

    if (! playermo)
        return;

    for (i = 0; i < MAXPLAYERS && player_id != self_id; i ++)
    {
        if (virtualplayers[i].mo == playermo && virtualplayers[i].in_game)
            skip_tics = leveltime - virtualplayers[i].player_times[self_id];
    }

    P_SetRandIndex(0); // For randomangle - hopefully players won't be smart enough to notice that it isn't random. =P

    for (i = 0; i < num_rings; i++)
	{
		int l;
		mobj_t *mo = P_SpawnMobj(playermo->x,
                         playermo->y,
                         playermo->z,
                         MT_FLINGRING);

        mo->fuse = (8-0)*TICRATE - skip_tics; // 8-player->losscount
        if (mo->fuse < 1) mo->fuse = 1;

        P_SetTarget(&mo->target, playermo);

		mo->destscale = playermo->scale;
		P_SetScale(mo,playermo->scale);

		randomangle = P_Random();
		fa = (randomangle+i*FINEANGLES/16) & FINEMASK;

        if (i > 15)
        {
            ns = FIXEDSCALE(3 * FRACUNIT, mo->scale);

            if (maptol & TOL_ERZ3)
                ns >>= 2;

            mo->momx = FixedMul(FINESINE(fa),ns);
            mo->momy = FixedMul(FINECOSINE(fa),ns);

            P_SetObjectMomZ(mo, (maptol & TOL_ERZ3) ? (4*FRACUNIT) >> 2 : 4*FRACUNIT, false);

            if (i & 1)
                P_SetObjectMomZ(mo, (maptol & TOL_ERZ3) ? (4*FRACUNIT) >> 2 : 4*FRACUNIT, true);
        }
        else
        {
            ns = FIXEDSCALE(2 * FRACUNIT, mo->scale);

            if (maptol & TOL_ERZ3)
                ns >>= 2;

            mo->momx = FixedMul(FINESINE(fa), ns);
            mo->momy = FixedMul(FINECOSINE(fa),ns);

            P_SetObjectMomZ(mo, (maptol & TOL_ERZ3) ? (3*FRACUNIT) >> 2 : 3*FRACUNIT, false);

            if (i & 1)
                P_SetObjectMomZ(mo, (maptol & TOL_ERZ3) ? (3*FRACUNIT) >> 2 : 3*FRACUNIT, true);
        }

        for (l = 0; l < skip_tics && mo; l ++)
        {
            if (mo->state == &states[S_DISS])
                break;
            P_SecretMove(mo);
        }
	}
}

void PlayerDropEmeralds(int player_id)
{
	int i;
	angle_t fa;
	fixed_t ns;
	int amt;
	fixed_t z = 0, momx = 0, momy = 0;
	mobj_t* playermo = players[0].mo;
	INT32* powers = players[0].powers;
    int num_stones = 0;

    if (player_id != self_id)
    {
        playermo = virtualplayers[player_id].mo;
        powers   = virtualplayers[player_id].powers;
    }

    for (i = 0; i < 7; i ++)
    {
        if (powers[pw_emeralds] & 1 << i)
            num_stones++;
    }

    if (! num_stones)
        return; // Nothing to drop!

    amt = 32/num_stones;

    for (i = 0; i < num_stones; i++)
    {
        int l;
        int stoneflag = 0;
        statenum_t statenum = 0;
        mobj_t *mo;

        for (l = 0; l < 7; l ++)
        {
            if (powers[pw_emeralds] & 1 << l)
            {
                stoneflag = 1 << l;
                statenum = S_CEMG1 + l;
                break; // Must break here.
            }
        }

        fa = ((255 / num_stones) * i) * FINEANGLES/256;

        if (!(playermo->eflags & MFE_VERTICALFLIP))
            z = playermo->z + (playermo->height / 2);
        else
            z = playermo->z - (playermo->height / 4);

        ns = 4 * FRACUNIT;

        momx = FixedMul(FINECOSINE(fa), ns);

        if (!(twodlevel || (playermo->flags2 & MF2_TWOD)))
            momy = FixedMul(FINESINE(fa),ns);
        else
            momy = 0;

        mo = P_SpawnMobj(playermo->x, playermo->y, z, MT_FLINGEMERALD);
        mo->health = 1;
        mo->threshold = stoneflag;
        mo->flags2 |= MF2_DONTRESPAWN;
        mo->flags2 |= MF2_SLIDEPUSH;
        mo->flags &= ~MF_NOGRAVITY;
        mo->flags &= ~MF_NOCLIPHEIGHT;
        P_SetTarget(&mo->target, playermo);
        mo->fuse = 12*TICRATE;
        P_SetMobjState(mo, statenum);

        mo->momx = momx;
        mo->momy = momy;

        P_SetObjectMomZ(mo, 3*FRACUNIT, false);

        if (playermo->eflags & MFE_VERTICALFLIP)
            mo->momz = -mo->momz;

        powers[pw_emeralds] &= ~stoneflag;
    }
}
