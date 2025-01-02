#include "CS_Global.h"
#include "CS_Compression.h"
#include "CS_Console.h" // cv_unsafenodes

#include "r_things.h" // Skins
#include "g_game.h"
#include "p_local.h"
#include "d_netfil.h" // For WAD stuff.
#include "p_setup.h"
#include "m_menu.h" // M_StartMessage.
#include "d_main.h" // D_StartTitle.

#define WAD_START 10

#define BETAVERSION (MODVERSION+1)

UINT8 saymessage_id = 0; // Wraps around after 15.
UINT8 mapswitches = 0;

SayMessage last_saymessage;

void CS_Host(const char* bindport)
{
#ifndef NATIVENET
    int error = 0;
#endif
    (void) bindport;

    if (CS_host || CS_client)
        return;

    if (! server || ! netgame)
    {
        CONS_Printf("Please enter a stage before doing this.\n");
        return;
    }

    if (! netgame || splitscreen)
    {
        CONS_Printf("This must be done in a hosted netgame.\n");
        return;
    }

    CS_Init();

#ifndef NATIVENET
    game_socket = Create_Socket(IPPROTO_UDP, 0, &error);
    if (error)
		CONS_Printf("Create_Socket error %i\n", error);

    if (game_socket == NULL) // No want crash.
    {
        CONS_Printf("Could not create socket.\n");

        CS_Shutdown();
        return;
    }

    Bind(game_socket, "", bindport, &error);
    if (error)
		CONS_Printf("Bind error %i\n", error);
#endif

    self_id = 0;
    CS_host = 1;

/*    if (dedicated)
        self_id = -1;*/
}

void CS_Connect(const char* ip_address, const char* bindport, const char* connectport)
{
#ifndef NATIVENET
    int error = 0;
#endif
    int i;
    char join_message[7] = "\x00JOIN_";

    tic_t start_time;
    boolean negotiated;
    IntroPacket ip;
    UINT8 buffer[1450];

    int received;
    struct sockaddr server_address;

    boolean failed = 0;
    (void) bindport;

    if (CS_host)
    {
        CONS_Printf("Sorry, cannot connect (already hosting).\n");
        return;
    }

    if (modifiedgame)
    {
        CONS_Printf("This cannot be done in a modified game.\n");
        return;
    }

    if (splitscreen)
    {
        CONS_Printf("This cannot be done in splitscreen mode.\n");
        return;
    }

    CS_Init();

#ifndef NATIVENET
    game_socket = Create_Socket(IPPROTO_UDP, 0, &error);
    if (error) CONS_Printf("Create_Socket error %i\n", error);

    if (game_socket == NULL)
    {
        CONS_Printf("Could not create socket.\n");

        CS_Shutdown();
        return;
    }

    Bind(game_socket, "", bindport, &error);
    if (error) CONS_Printf("Bind error %i\n", error);
#endif

    Make_SockAddr(&server_address, ip_address, connectport, NULL);

    join_message[5] = BETAVERSION;
    Network_SendTo(game_socket, join_message, 7, &server_address);

    start_time = I_GetTime();
    negotiated = 0;

    client_connecting = 1;
    while (! negotiated)
    {
        struct sockaddr temp;

        if (I_GetTime() - start_time > 5 * TICRATE) // Five seconds have passed since we tried to connect.
            break;

        // TODO: Make sure the message is actually coming from the server?
        if ((received = Network_ReceiveFrom(game_socket, buffer, 1450, 1, &temp)) > 0)
        {
            if (buffer[0] == 0x01 && received == sizeof (IntroPacket))
            {
                negotiated = 1;
                break;
            }

            if (buffer[0] == 0x06 && received == sizeof (RefusePacket)) // Uh oh!
            {
                ReceiveRefusePacket((RefusePacket*) buffer);

                return; // CS_Shutdown is typically be called above.
            }
        }
    }

    if (! negotiated)
    {
        CONS_Printf("Connection failed (sorry!)\n");

        CS_Shutdown();
        return;
    }

    memcpy(&ip, buffer, sizeof (IntroPacket));

    // LXShadow's all-too-hacky WAD adding code.
    // I was never too keen on the idea to begin with, so no criticism please.
    for (i = 0; i < ip.wad_count; i ++)
    {
        if (! P_AddWadFile(ip.wad_name[i], NULL)) // Expect this to give its own error message.
        {
            failed = 1;
            break;
        }

        if (memcmp(ip.wad_md5[i], wadfiles[numwadfiles - 1]->md5sum, 16))
        {
            char wad_name[MAX_WADPATH];

            memcpy(wad_name, wadfiles[i + WAD_START]->filename, MAX_WADPATH);
            nameonly(wad_name);

            CONS_Printf("Error: MD5 sum comparison for WAD %s failed; WAD may be the wrong version.\n", wad_name);

            failed = 1;
            break;
        }
    }

    if (ip.wad_count > 0)
        modifiedgame = 1;

    if (failed)
    {
        CONS_Printf("Connection terminated.\n");

        CS_Shutdown();
        return;
    }

    // Boolean stuffs.
    netgame = 1;
    multiplayer = 1;
    server = 1;
    CS_client = 1; // Hack - don't want it to do CS_Host as the "server" starts.

    self_id = ip.player_id; // First things first: Know who we are!

    saymessage_id = ip.saymessage_id;

    for (i = 0; i < CSMAXPLAYERS; i ++)
    {
        if (i == self_id)
            continue;

        virtualplayers[i].in_game         = ip.pi[i].in_game;
        virtualplayers[i].score           = ip.pi[i].score;
        virtualplayers[i].event_count     = ip.pi[i].event_count;
        virtualplayers[i].last_eventcount = ip.pi[i].event_count; // No want re-execute.

        memcpy(&virtualplayers[i].address, &ip.pi[i].address, sizeof (struct sockaddr));

        virtualplayers[i].last_update = I_GetTime();
    }

    memcpy(&virtualplayers[0].address, &server_address, sizeof (struct sockaddr)); // We haven't created the host mobj just yet, but we can still do this.
                                                                                   // Watch out - the above code doesn't bother to exclude the host in its address assignment.
    CONS_Printf("\x82Player %i has joined the game.\n", self_id);

    //D_MapChange(ip.sp.map, ip.sp.gametype, 0, 1, 0, 1, 0); // The server player will be created during/after this.
    ReceiveServerPacket(&ip.sp);

    client_connecting = 0;
}

extern UINT16 rememberthisdumbthing;

void SendMessages()
{
    int i, l;
    boolean update_data = 0;
    boolean update_player = 0;
    PlayerUpdate send_update;
    PlayerDataUpdate my_du;
    BitStream send_data_update;

    if (! CS_host && ! CS_client)
        return;

    update_player = MakePlayerUpdate(&send_update);

    if ((! (I_GetTime() % TICRATE) && CS_client) || CS_host)
    {
#ifndef NATIVENET
        int namelen = sizeof (struct sockaddr);
#endif
        if (CS_client)
            update_data = 1;
        else
            update_data = I_GetTime() % TICRATE; // Which player should be updated?

        my_du.id = 0x04;
        my_du.player_id = self_id;

#ifndef NATIVENET
        getsockname(game_socket->ws_socket, &my_du.address, &namelen);
        my_du.purportedport = IGetSockaddrPort(&my_du.address);
#else
        my_du.purportedport = rememberthisdumbthing;
#endif

        my_du.skin = players[0].skin;
        my_du.colour = players[0].skincolor;

        if (gametype == GT_CTF || (gametype == GT_MATCH && cv_matchtype.value))
            my_du.team = players[0].spectator ? 0 : players[0].ctfteam;
        else
            my_du.team = players[0].spectator ? 0 : 1;

        my_du.score = players[0].score; // Note: Only clients will actually read this.
        my_du.laps = players[0].laps;

        my_du.real_time = players[0].realtime;
        my_du.permadead = (players[0].lives <= 0);

        my_du.player_links = self.player_links;

        memcpy(&my_du.name, player_names[0], MAXPLAYERNAME);
    }

    // If you're the host, send the server packet every second or so.
    if (CS_host)
    {
        int player_count = 0; // Count of *other* players in the server.

        for (i = 1; i < MAXPLAYERS; i ++) {if (virtualplayers[i].in_game) player_count ++;}

        if (player_count)
        {
            int send_to;
            int frequency = TICRATE / player_count;

            if (frequency == 0)
                frequency = 1; // This can happen when player_count > 17.

            send_to = ((I_GetTime() % TICRATE) / frequency);

            if ((I_GetTime() % frequency) == 0)
            {
                int id = 0;
                for (i = 1; i < MAXPLAYERS; i ++)
                {
                    if (! virtualplayers[i].in_game)
                        continue;

                    if (id == send_to)
                        SendServerPacket(i); // Bother breaking here?

                    id ++;
                }
            }
        }

        // And also re-send SayMessages that have apparently not yet been received.
        for (i = 1; i < MAXPLAYERS; i ++)
        {
            if (! virtualplayers[i].in_game)
                continue;

            if (! (I_GetTime() % 7) && virtualplayers[i].saymessage_id != saymessage_id) // About once every 200 ms, if the player is not up-to-date as far as we know.
                BroadcastSayMessage(&last_saymessage, i);
        }
    }

    // Startup send_data_update.
    StreamStart(&send_data_update);

    CompressPlayerDataUpdate(&my_du, &send_data_update);

    if (CS_client)
    {
        if (update_player)
        {
            BitStream stream;

            StreamStart(&stream);

            for (i = 0; i < MAXPLAYERS; i ++)
            {
                if (! virtualplayers[i].in_game || virtualplayers[i].address.sa_family != AF_INET) // An invalid address is possible.
                    continue;

                if (i > 0 && ! cv_clienttoclient.value) // We want nothing to do with clienttoclient.
                    continue;

                stream.position = 0; stream.data_length = 0;

                CompressPlayerUpdate(&send_update, &stream, i);

                Network_SendTo(game_socket, stream.data, stream.data_length / 8, &virtualplayers[i].address);

                // Send off another message to that player's purported port- but don't treat the host that way.
                if (i != 0 && IGetSockaddrPort(&virtualplayers[i].address) != virtualplayers[i].purportedport &&
                    ! (self.player_links & 1 << i)) // If you're already linked, you should have a valid address.
                {
                    UINT16 backup_port = IGetSockaddrPort(&virtualplayers[i].address);

                    ISetSockaddrPort(&virtualplayers[i].address, virtualplayers[i].purportedport);
                    Network_SendTo(game_socket, stream.data, stream.data_length / 8, &virtualplayers[i].address);
                    ISetSockaddrPort(&virtualplayers[i].address, backup_port);
                }
            }

            StreamEnd(&stream);
        }

        if (update_data)
            Network_SendTo(game_socket, send_data_update.data, send_data_update.data_length / 8, &virtualplayers[0].address);
    }
    else if (CS_host) // Send the messages to everyone.
    {
        BitStream my_update;

        StreamStart(&my_update);

        for (i = 1; i < MAXPLAYERS; i ++)
        {
            if (virtualplayers[i].in_game)
            {
                my_update.position = 0;
                my_update.data_length = 0;

                if (update_player) // Update host player.
                {
                    //Network_SendTo(game_socket, &send_update, sizeof (PlayerUpdate), &virtualplayers[i].address);
                    CompressPlayerUpdate(&send_update, &my_update, i); // CSDONE: Moved this under if (update_player) to fix an intermission bug.
                    Network_SendTo(game_socket, my_update.data, my_update.data_length / 8, &virtualplayers[i].address);
                }

                if (update_data == i) // Update host player data.
                    Network_SendTo(game_socket, send_data_update.data, send_data_update.data_length / 8, &virtualplayers[i].address);

                for (l = 1; l < MAXPLAYERS; l ++) // Send the rest of the players' updates. (Not our own one again!)
                {
                    if (! virtualplayers[l].in_game || l == i) // (and not their own one!)
                        continue;

                    if (virtualplayers[l].update_times[0] == 0xFFFFFFFF) // It may happen...
                        continue;

                    // Todo: Make the server the big boss of cv_clienttoclient?
                    // Let's make the raw assumption that, if we can't send a player update, nobody can.
                    if (update_player && ! (virtualplayers[i].player_links & (1 << l))) // No need to reroute if already linked.
                    {
                        //Network_SendTo(game_socket, &virtualplayers[l].update_history[0], sizeof (PlayerUpdate), &virtualplayers[i].address);
                        BitStream stream;

                        StreamStart(&stream);

                        virtualplayers[l].update_history[0].player_id = l; // Do this in case client l is cheating?
                        CompressPlayerUpdate(&virtualplayers[l].update_history[0], &stream, i);

                        Network_SendTo(game_socket, stream.data, stream.data_length / 8, &virtualplayers[i].address);

                        StreamEnd(&stream);
                    }

                    if (update_data == i) // Send this guy (i) all the data updates during this tic.
                    {
                        PlayerDataUpdate du;
                        BitStream compressed_du;

                        du.id = 0x04;
                        du.player_id = l;

                        memcpy(&du.address, &virtualplayers[l].address, sizeof (struct sockaddr));
                        du.purportedport = virtualplayers[l].purportedport;

                        du.skin = virtualplayers[l].skin;
                        du.colour = virtualplayers[l].colour;
                        du.team = virtualplayers[l].ctfteam;

                        du.score = virtualplayers[l].score;
                        du.laps = virtualplayers[l].laps;

                        du.real_time = virtualplayers[l].real_time;
                        du.permadead = virtualplayers[l].permadead;

                        du.player_links = virtualplayers[l].player_links; // Just in case. Clients actually use it when looking at csnodes.

                        memcpy(&du.name, virtualplayers[l].name, MAXPLAYERNAME);

                        // Compress, then send.
                        StreamStart(&compressed_du);
                        CompressPlayerDataUpdate(&du, &compressed_du);

                        Network_SendTo(game_socket, compressed_du.data, compressed_du.data_length / 8, &virtualplayers[i].address);

                        StreamEnd(&compressed_du);
                    }
                }
            }
        }

        StreamEnd(&my_update);
    }

    StreamEnd(&send_data_update);
}

void ReceiveMessages()
{
    int i;

    char buffer[500];
    struct sockaddr from;
    int received;

    if (! CS_host && ! CS_client)
        return;

    while ((received = Network_ReceiveFrom(game_socket, buffer, 500, 1, &from)) > 0)
    {
        int player_id = -1;
        UINT8 message_id = buffer[0] & 0x07;
        int message_sizes[7] = {7, sizeof (IntroPacket), sizeof (PlayerUpdate), sizeof (ServerPacket), sizeof (PlayerDataUpdate), sizeof (SayMessage), sizeof (RefusePacket)}; // Quick hacker check.

        // Identify the player from whom the message was sent.
        // In the past only the host would do this, but with clienttoclient...
        for (i = 0; i < MAXPLAYERS; i ++)
        {
            if (virtualplayers[i].in_game)
            {
                if (IGetSockaddrIP(&from) == IGetSockaddrIP(&virtualplayers[i].address) &&
                    (IGetSockaddrPort(&from) == IGetSockaddrPort(&virtualplayers[i].address) ||
                     IGetSockaddrPort(&from) == virtualplayers[i].purportedport))
                {
                    player_id = i;
                    break; // Added a break - in case anyone has the same address and ports/purported ports as the host.
                }
            }
        }

        if (received != message_sizes[message_id] && message_id != 0x02 && message_id != 0x04 && message_id != 0x03) // Is this guy trying to hack us?
            continue; // At some point I'm going to need to abandon message_sizes. =/

        if (CS_host && message_id == 0x00 && player_id == -1)
        {
            if (buffer[5] != BETAVERSION)
            {
                SendRefusePacket(&from, va("Incompatible game version. (Server is using Beta %i)\n", BETAVERSION));
                continue;
            }

            AcceptPlayer(&from);

            continue; // Note: this blocks off any more messages from player_id -1 for the host.
        }

        if (message_id == 0x02)
        {
            PlayerUpdate update;

            DecompressPlayerUpdate(buffer, received * 8, &update, CS_host ? player_id : 0); // If you're a client, you'll
                                                                                            // always receive client messages.
            if (CS_host)
            {
                if (! virtualplayers[player_id].in_game)
                    continue; // To avoid warning message spam from CheckPlayerUpdate, on bans and such.

                CheckPlayerUpdate(player_id, &update);

                AddPlayerUpdate(player_id, &update);

                virtualplayers[player_id].last_update_playertime = update.my_time;
                virtualplayers[player_id].last_update = I_GetTime();
                virtualplayers[player_id].last_message = I_GetTime();
            }
            else if (CS_client)
            {
                if (player_id == 0) // Host message.
                {
                    if (! virtualplayers[update.player_id].in_game) // Assume it's a new player.
                    {
                        ResetPlayer(update.player_id); // Just in case.

                        virtualplayers[update.player_id].in_game = 1;
                        virtualplayers[update.player_id].mo = P_SpawnMobj(0, 0, -(1024 << FRACBITS), MT_VIRTUALPLAYER);

                        CONS_Printf("\x82Player %i has joined the game.\n", update.player_id);
                    }

                    if (update.player_id > 0 && self.player_links & (1 << update.player_id) && cv_clienttoclient.value)
                        continue; // We don't want the host's word on the matter - we're taking updatess directly from the client.

                    CheckPlayerUpdate(update.player_id, &update);

                    AddPlayerUpdate(update.player_id, &update);

                    virtualplayers[update.player_id].last_update_playertime = update.my_time;
                    virtualplayers[update.player_id].last_update = I_GetTime();
                    virtualplayers[update.player_id].last_message = I_GetTime();
                }
                else if (player_id != 0)
                {
                    if (! cv_clienttoclient.value)  // We don't want any clienttoclient, thanks.
                        continue;

                    if (IGetSockaddrPort(&from) == 0)
                        continue; // Troublesome message.

                    if (player_id == -1) // Unidentified player.
                    {
                        if (cv_safenodes.value || update.player_id == 0) // Go away - you could be a hacker for all we know!
                            continue;                                  // If update.player_id *is* 0 then this is a dodgy bloke!

                        player_id = update.player_id; // Very well - we'll just assume they're telling the truth.
                    }

                    if (! virtualplayers[player_id].in_game) // Wait for the host's greenlight first.
                        continue;

                    CheckPlayerUpdate(player_id, &update);

                    AddPlayerUpdate(player_id, &update); // <--- Could use update.player_id but that would let people impersonate each other.

                    virtualplayers[player_id].last_update_playertime = update.my_time;
                    virtualplayers[player_id].last_update = I_GetTime();
                    virtualplayers[player_id].last_message = I_GetTime();

                    self.player_links |= 1 << player_id;

                    memcpy(&virtualplayers[player_id].address, &from, sizeof (struct sockaddr)); // We'll take this address.
                }
            }
        }

        if (message_id == 0x03)
        {
            ServerPacket sp;

            if (CS_host || player_id != 0)
                continue; // Wait, this shouldn't be happening!

            DecompressServerPacket(buffer, received * 8, &sp);

            ReceiveServerPacket(&sp);

            virtualplayers[0].last_message = I_GetTime();
        }

        if (message_id == 0x04)
        {
            PlayerDataUpdate data_update;

            if (CS_client && player_id != 0)
                continue; // Stop this, you mean hackers. =(

            DecompressPlayerDataUpdate(buffer, received * 8, &data_update);

            CheckPlayerDataUpdate(&data_update);

            if (CS_client)
            {
                player_id = data_update.player_id;

                if (player_id != 0 &&  // We don't want to reset the host's address.
                    ! (self.player_links & (1 << player_id))) // If we have a player link, then we already have a valid address to use.
                {
                    if (IGetSockaddrPort(&data_update.address) != 0)
                        memcpy(&virtualplayers[player_id].address, &data_update.address, sizeof (struct sockaddr));
                }
            }

            virtualplayers[player_id].purportedport = data_update.purportedport;

            if (data_update.purportedport == 0)
                data_update.purportedport = 5029;

            virtualplayers[player_id].skin = data_update.skin;
            virtualplayers[player_id].colour = data_update.colour;
            virtualplayers[player_id].ctfteam = data_update.team;
            virtualplayers[player_id].spectator = ! data_update.team;

            if (virtualplayers[player_id].mo && gamestate == GS_LEVEL)
            {
                virtualplayers[player_id].mo->color = data_update.colour;
                virtualplayers[player_id].mo->skin = &skins[data_update.skin];
            }

            memcpy(virtualplayers[player_id].name, data_update.name, MAXPLAYERNAME);

            if (CS_client) // The host should not receive scores.
                virtualplayers[player_id].score = data_update.score;

            if (virtualplayers[player_id].laps < cv_numlaps.value) // Don't go backwards in case of an out-of-order packet.
                virtualplayers[player_id].laps = data_update.laps; // The host receives laps for the sake of synchronisation.

            virtualplayers[player_id].real_time = data_update.real_time;
            virtualplayers[player_id].permadead = data_update.permadead;

            virtualplayers[player_id].player_links = data_update.player_links;

            virtualplayers[player_id].last_message = I_GetTime();
        }

        if (message_id == 0x05)
        {
            SayMessage* message = (SayMessage*) buffer;

            if (CS_client && player_id != 0)
                continue; // Messages are not wanted from HACKERS.

            if (CS_client && message->message_id == saymessage_id) // We already have this message.
                continue;

            if (CS_host && (message->type & 7) == 1 && player_id == message->type >> 3) // Player sending message to themselves?
                continue;

            if (CS_host && (message->type & 7) == 3) // Only the host can send CSays.
                continue;

            message->message[256 - 1] = 0; // *More* early hacking prevention techniques...

            CS_Say(CS_host ? player_id : message->player_id, message);

            if (CS_host)
            {
                saymessage_id = (saymessage_id + 1) & 0x0F;

                message->player_id = player_id; // Just in case.
                message->message_id = saymessage_id;

                // Send the message to all other players. (If they deserve it!)
                BroadcastSayMessage(message, -1);

                // Then, copy to last_saymessage for possible re-sending.
                memcpy(&last_saymessage, message, sizeof (SayMessage));
            }
            else
            {
                CS_AddEvent(EVENT_GOTSAYMSG | (message->message_id << 5));
                saymessage_id = message->message_id;
            }

            virtualplayers[CS_host ? player_id : message->player_id].last_message = I_GetTime();
        }

        if (message_id == 0x06)
        {
            if (CS_host)
            {
                CONS_Printf("CS warning: received hostile message from %s (player ID %i)!\n", virtualplayers[player_id].name, player_id);
                continue; // ...No.
            }

            ReceiveRefusePacket((RefusePacket*) buffer);
        }
    }
}

void MakeServerPacket(ServerPacket* sp)
{
    int i;

    if (! CS_host)
        return; // Added this because of the host-ish loop below.

    sp->id = 0x03;

    sp->map = gamemap;
    sp->gametype = gametype;
    sp->level_time = leveltime;

    sp->mapswitches = mapswitches;

    sp->timelimit = cv_timelimit.value;
    sp->pointlimit = cv_pointlimit.value;
    sp->itemrespawntime = cv_itemrespawn.value ? cv_itemrespawntime.value : -1;
    sp->matchtype = cv_matchtype.value;

    if (gametype == GT_MATCH || gametype == GT_CTF)
    {
        sp->red_score = redscore;
        sp->blue_score = bluescore;

        sp->flag_state = flagstate[1] | ((flagholder[1] & 63) << 2) | (flagstate[2] << 8) | ((flagholder[2] & 63) << 10);

        for (i = 0; i < 7; i ++) sp->emeralds[i] = -1;

        for (i = 0; i < MAXPLAYERS; i ++) // If we're not in a stage then we're probably at intermission.
        {
            int l;
            INT32* powers;

            if (i != self_id && ! virtualplayers[i].in_game)
                continue;

            powers = virtualplayers[i].powers;

            if (i == self_id) powers = players[0].powers;

            for (l = 0; l < 7; l ++)
            {
                if (powers[pw_emeralds] & 1 << l)
                    sp->emeralds[l] = i;
            }
        }
    }

    if (gametype == GT_RACE)
    {
        sp->finished_count = finished_count;

        memcpy(sp->race_rank, race_rank, sizeof (race_rank));
    }
}

void SendServerPacket(int player_id)
{
    ServerPacket send_packet;
    BitStream str;

    if (! CS_host) // What the bleep is going on here?
        return;

    StreamStart(&str);

    MakeServerPacket(&send_packet);

    CompressServerPacket(&send_packet, &str);

    Network_SendTo(game_socket, str.data, str.data_length / 8, &virtualplayers[player_id].address);

    StreamEnd(&str);
}

void ReceiveServerPacket(ServerPacket* sp)
{
    int i;

    if (client_connecting)
    {
        timelimitintics = 0x7FFFFFFF; // Hack is done to fix "instant intermission". =/
        CV_StealthSet(&cv_timelimit, "0");
        CV_StealthSet(&cv_pointlimit, "0");
    }

    CV_SetValue(&cv_timelimit, sp->timelimit);
    CV_SetValue(&cv_pointlimit, sp->pointlimit);
    CV_SetValue(&cv_matchtype, sp->matchtype);

    if (sp->itemrespawntime != -1)
    {
        if (sp->itemrespawntime != atoi(cv_itemrespawntime.defaultvalue)) // Activate cheats.
            CV_SetValue(&cv_cheats, 1);

        CV_StealthSet(&cv_itemrespawn, "1");
        CV_StealthSetValue(&cv_itemrespawntime, sp->itemrespawntime);
    }
    else
        CV_StealthSet(&cv_itemrespawn, "0");

    if (client_connecting ||
        gamemap != sp->map || gametype != sp->gametype || sp->mapswitches != mapswitches) // Unexpected map (or gametype) change. One second of error allowed.
    {
        mapswitches = sp->mapswitches;
        D_MapChange(sp->map, sp->gametype, 0, 1, 0, 1, 0);
    }

    if (leveltime < sp->level_time || abs((INT32) leveltime - (INT32) sp->level_time) > 15) // Clients should not be too far behind *or* ahead.
        leveltime = sp->level_time;                                                         // Else their player_times byte will not fit all data.

    if (sp->gametype == GT_MATCH || sp->gametype == GT_CTF)
    {
        redscore = sp->red_score;
        bluescore = sp->blue_score;

        if (gametype == GT_CTF)
            CS_UpdateFlags(sp->flag_state);

        for (i = 0; i < MAXPLAYERS; i ++)
            virtualplayers[i].powers[pw_emeralds] = 0; // Clear everyone else's emeralds before continuing.

        for (i = 0; i < 7; i ++)
        {
            // Double-check to make sure this player isn't badly synced...
            if (players[0].powers[pw_emeralds] & 1 << i && sp->emeralds[i] != self_id)
            {
    #ifndef BUNNIES
                CONS_Printf("CS warning: Forced to remove emerald from your possession.\n");
    #endif
                players[0].powers[pw_emeralds] &= ~(1 << i); // Sorry!
            }

            if (sp->emeralds[i] == -1)
                continue;

            if (sp->emeralds[i] == self_id)
                players[0].powers[pw_emeralds] |= 1 << i;
            else
                virtualplayers[sp->emeralds[i]].powers[pw_emeralds] |= 1 << i;
        }
    }

    if (sp->gametype == GT_RACE)
    {
        for (i = 0; i < sp->finished_count; i ++)
        {
            if (sp->race_rank[i] != self_id)
                virtualplayers[sp->race_rank[i]].laps = cv_numlaps.value;
        }

        // State the winners of this race - if there are any.
        for (i = finished_count; i < sp->finished_count && ! client_connecting; i ++)
            CONS_Printf(M_GetText("%s has finished the race.\n"), sp->race_rank[i] == self_id ? player_names[0] : virtualplayers[sp->race_rank[i]].name);

        finished_count = sp->finished_count;
    }
}

void SendIntroPacket(int player_id)
{
    int i;
    int wad_count;
    IntroPacket ip;

    ip.id = 0x01;

    ip.player_id = player_id;
    ip.saymessage_id = saymessage_id;

    MakeServerPacket(&ip.sp);

    ip.pi[0].in_game = 1;
    ip.pi[0].score = players[0].score;
    ip.pi[0].event_count = self.event_count;
    // Note: address not set for host.

    for (i = 1; i < CSMAXPLAYERS; i ++)
    {
        ip.pi[i].in_game = virtualplayers[i].in_game;
        ip.pi[i].score = virtualplayers[i].score;
        ip.pi[i].event_count = virtualplayers[i].event_count;

        memcpy(&ip.pi[i].address, &virtualplayers[i].address, sizeof (struct sockaddr));
    }

    wad_count = 0;

    // More hacky WAD code. WADs are unsupported kthxbai.
    for (i = 0; i < numwadfiles; i ++)
    {
        char name[MAX_WADPATH];

        memcpy(name, wadfiles[i]->filename, MAX_WADPATH);
        nameonly(name);

        if (! strcmp(name, "srb2.srb")  || ! strcmp(name, "sonic.plr") || ! strcmp(name, "knux.plr") ||
            ! strcmp(name, "tails.plr") || ! strcmp(name, "music.dta") || ! strcmp(name, "zones.dta") ||
            ! strcmp(name, "soar.dta")  || ! strcmp(name, "drill.dta") || ! strcmp(name, "rings.wpn"))
            continue; // Cheaters could abuse this.

        if (W_VerifyNMUSlumps(wadfiles[i]->filename))
            continue;

        if (wad_count >= 10)
        {
            CONS_Printf("CS warning: Could not send all WAD files - limit is 10.\n");
            break;
        }

        if (strlen(name) > 20)
        {
            CONS_Printf("CS warning: Could not send WAD file %s", name);
            continue;
        }

        memcpy(ip.wad_name[wad_count], name, 20);
        memcpy(ip.wad_md5[wad_count], wadfiles[i]->md5sum, 16);

        wad_count ++;
    }

    ip.wad_count = wad_count;

    Network_SendTo(game_socket, &ip, sizeof (IntroPacket), &virtualplayers[player_id].address);
}

void SendRefusePacket(struct sockaddr* to, const char* reason)
{
    RefusePacket rp;

    rp.id = 0x06;

    strncpy(rp.reason, reason, 128);

    Network_SendTo(game_socket, &rp, sizeof (RefusePacket), to);
}

void ReceiveRefusePacket(RefusePacket* rp)
{
    M_StartMessage(M_GetText(rp->reason), NULL, MM_NOTHING);
    D_QuitNetGame();
    CL_Reset();
    D_StartTitle();
}

void CheckPlayerUpdate(int player_id, PlayerUpdate* pu)
{
//    int i;
    boolean warn = ((I_GetTime() % (TICRATE * 3)) == 0);

    VirtualPlayer* player = &virtualplayers[player_id];

    if (pu->player_id != player_id)
    {
        if (warn)
            CONS_Printf("CS WARNING: %s sent an invalid player ID (got: %i exp: %i).\n", player->name, pu->player_id, player_id);
        pu->player_id = player_id;
    }

    if (pu->state >= 62)
    {
        if (warn)
            CONS_Printf("CS WARNING: %s sent an invalid state.\n", player->name);
        pu->state = 0;
    }

/*    for (i = 0; i < 5; i ++)
    {
        if (pu->event_list[i] & 0x0F > MAXEVENT)
        {
            pu->event_list[i] = 0;
            CONS_Printf("CS WARNING: %s sent an invalid event.\n", player->name);
        }
    }*/

    // Only the host checks the below, as the clients are slightly delayed and don't need unnecessary warnings.
    // ..in fact they don't need anything that's unnecessary.
    // I knew that.
    if (CS_host && (pu->powers & POWER_SUPER) && virtualplayers[player_id].powers[pw_emeralds] != 127)
    {
        if (warn)
            CONS_Printf("CS WARNING: %s does not have the emeralds!\n", player->name);
    }
}

void CheckPlayerDataUpdate(PlayerDataUpdate* du)
{
    int i;
    int invalid_characters = 0;
    boolean warn = ((I_GetTime() % (TICRATE * 3)) == 0);

    for (i = 0; i < MAXPLAYERNAME; i ++)
    {
        if ((du->name[i] >= 'A' && du->name[i] <= 'Z') ||
            (du->name[i] >= 'a' && du->name[i] <= '}') ||
            (du->name[i] >= ' ' && du->name[i] <= '@') ||
            (du->name[i] >= '[' && du->name[i] <= '`' && du->name[i] != '\\') || // Backslashes are abused in CTF mode.
            ! du->name[i]);
        else
        {
            invalid_characters ++;
            du->name[i] = ' ';
        }

        if (! du->name[i])
            break;
    }

    if (i == MAXPLAYERNAME || invalid_characters > 0)
    {
        du->name[MAXPLAYERNAME - 1] = 0;
#ifndef BUNNIES
        CONS_Printf("CS WARNING: Player name %s invalid; corrected.\n", du->name);
#endif
    }

    if (du->skin >= numskins)
    {
        du->skin = 0;
        if (warn)
            CONS_Printf("CS WARNING: %s using an invalid skin.\n", du->name);
    }

    if (du->colour >= MAXSKINCOLORS) // || du->colour == 0; removed for now but should be re-added when the joiner issue is fixed.
    {
        du->colour = 1; // Don't use a blank colour.
        if (warn)
            CONS_Printf("CS WARNING: %s using an invalid colour.\n", du->name);
    }

    if (du->team > 2)
    {
        du->team = 0;
        if (warn)
            CONS_Printf("CS WARNING: %s using invalid team.\n", du->name);
    }
}

void CS_SendPlayerFire(INT32 currentweapon, fixed_t tossstrength)
{
    self.throwing_ring = 1;

    self.last_weapon       = currentweapon;
    self.last_tossstrength = tossstrength;
}

void CS_SendSay(char* message, UINT8 type)
{
    SayMessage send;

    send.id = 0x05;
    send.player_id = self_id; // If we're the client, the host doesn't care what player we say we are; but if we're the host...
    send.type = type;

    memcpy(send.message, message, 256);

    if (CS_client)
        Network_SendTo(game_socket, &send, sizeof (SayMessage), &virtualplayers[0].address);
    else if (CS_host)
    {
        saymessage_id = (saymessage_id + 1) & 0x0F;

        send.message_id = saymessage_id;

        BroadcastSayMessage(&send, -1);

        memcpy(&last_saymessage, &send, sizeof (SayMessage));

        CS_Say(self_id, &send);
    }
    else // Single-player?
        CS_Say(self_id, &send);
}

void BroadcastSayMessage(SayMessage* msg, int player_id)
{
    int i;
    UINT8 type = msg->type & 7;
    UINT8 to_player = msg->type >> 3;

    if (! CS_host)
        return;

    for (i = 1; i < MAXPLAYERS; i ++)
    {
        if (! virtualplayers[i].in_game || (i != player_id && player_id != -1))
            continue;

        if (type == 1 && msg->player_id != i && to_player != i)
            continue;

        if (type == 2 && virtualplayers[i].ctfteam != (virtualplayers[msg->player_id].spectator ? 0 : virtualplayers[msg->player_id].ctfteam))
            continue;

        Network_SendTo(game_socket, msg, sizeof (SayMessage), &virtualplayers[i].address);
    }
}

void AcceptPlayer(struct sockaddr* from)
{
    int new_player_id = -1;
    int i;

    // Check for bans.
    struct sockaddr_in* sa = (struct sockaddr_in*) from;
    struct in_addr* inaddr = &(sa->sin_addr);

    for (i = 0; i < ban_count; i ++)
    {
        if (ban_list[i] == inaddr->S_un.S_addr)
        {
            SendRefusePacket(from, "You are banned from the server.\n");
            return; // Don't let them in!
        }
    }

    // Check playerlimit.
    if (CS_PlayerCount() >= cv_maxplayers.value)
    {
        SendRefusePacket(from, va("Maximum amount of players reached (%i).\n", cv_maxplayers.value));
        return;
    }

    // Add this new player.
    for (i = 1; i < CSMAXPLAYERS; i ++) // Make sure i isn't 0 - that's us!
    { // Also HACK: do not allow more than 8 players for now.
        if (! virtualplayers[i].in_game)
        {
            new_player_id = i;

            break;
        }
    }

    if (new_player_id == -1)
        return; // Cannot accept a new player for now...

    CONS_Printf("\x82Player %i has joined the game.\n", new_player_id);

    ResetPlayer(new_player_id);

    virtualplayers[new_player_id].in_game = 1;
    memcpy(&virtualplayers[new_player_id].address, from, sizeof (struct sockaddr));

    virtualplayers[new_player_id].mo = P_SpawnMobj(0, 0, -(1024 << FRACBITS), MT_VIRTUALPLAYER);
    virtualplayers[new_player_id].last_update = I_GetTime();

    // Send off the intro packet so that the client knows what's going on.
    SendIntroPacket(new_player_id);
}
