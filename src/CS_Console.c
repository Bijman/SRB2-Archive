#include "CS_Global.h"
#include "CS_Console.h"
#include "g_game.h" // player_names
#include "doomdef.h"

CV_PossibleValue_t pv_ringmovements[] = {{0, "Fast"}, {1, "Jumpy"}, {-1, NULL}};
CV_PossibleValue_t pv_csdelay[] = {{-1, "Automatic"}, {0, "0"}, {1, "1"}, {2, "2"}, {-2, NULL}};

consvar_t cv_clienttoclient = {"clienttoclient", "On", CV_CALL, CV_OnOff, ClientToClient_OnChange, 1, NULL, NULL, 0, 0, NULL};
consvar_t cv_safenodes = {"safenodes", "Off", 0, CV_OnOff, NULL, 1, NULL, NULL, 0, 0, NULL}; // Zero null zero null null....
consvar_t cv_ringmove = {"ringmove", "Fast", 0, pv_ringmovements, NULL, 0, NULL, NULL, 0, 0, NULL};
consvar_t cv_csdelay = {"csdelay", "Automatic", 0, pv_csdelay, NULL, -1, NULL, NULL, 0, 0, NULL}; // Null null zero zero zero...
consvar_t cv_cssync = {"cssync", "64", 0, NULL, NULL, 64, NULL, NULL, 0, 0, NULL};

void Command_CSConnect_f(void)
{
    if (COM_Argc() == 1 || COM_Argc() > 4)
    {
        CONS_Printf("csconnect can be used in three ways:\n");
        CONS_Printf("1: csconnect ipaddress\n2: csconnect ipaddress clientport serverport\n3: csconnect lxshadow");
        return;
    }

    if (COM_Argc() == 2)
    {
#ifndef NATIVENET
        if (! strcasecmp(COM_Argv(1), "lxshadow"))
            CS_Connect("94.193.236.176", "5030", "5030");
        else
            CS_Connect(COM_Argv(1), "5030", "5030");
#else
        if (! strcasecmp(COM_Argv(1), "lxshadow"))
            CS_Connect("94.193.236.176", "5029", "5029");
        else
            CS_Connect(COM_Argv(1), "5029", "5029");
#endif
    }

    if (COM_Argc() == 4)
        CS_Connect(COM_Argv(1), COM_Argv(2), COM_Argv(3));
}

void Command_CSHost_f(void)
{
    if (COM_Argc() > 2)
    {
        CONS_Printf("Usage: cshost port\n");
        return;
    }

    if (CS_host)
    {
        CONS_Printf("You are already hosting.\n"); // <--- lol idiot
        return;
    }

    if (CS_client)
    {
        CONS_Printf("You are already in a netgame.\n");
        return;
    }

    CONS_Printf("Started server...\n");

    if (COM_Argc() == 1)
#ifdef NATIVENET
        CS_Host("5030");
#else
        CS_Host("5029");
#endif
    else
        CS_Host(COM_Argv(1));
}

int jitter[MAXPLAYERS];

void Command_CSPing_f(void)
{
    int i;

    for (i = 0; i < MAXPLAYERS; i ++)
    {
        if (virtualplayers[i].in_game)
        {
            if (jitter[i]) // It is reasonable to guess that this will be doubled in the total ping.
                CONS_Printf("%s - %i (+-%i) ms", virtualplayers[i].name, ((leveltime - virtualplayers[i].player_times[self_id]) * 1000) / TICRATE, jitter[i] * 2000 / TICRATE);
            else
                CONS_Printf("%s - %i ms", virtualplayers[i].name, ((leveltime - virtualplayers[i].player_times[self_id]) * 1000) / TICRATE);

            if (CS_client && i != 0)
                CONS_Printf(" (%s)", self.player_links & (1 << i) ? "direct link" : "host link");
            else if (CS_client)
                CONS_Printf(" (host)");

            CONS_Printf("\n");
        }
    }
}

#ifndef BUNNIES
void Command_CSDelay_f(void)
{
    if (COM_Argc() != 2)
    {
        CONS_Printf("Artificial delay is currently %i tics (%i ms)\n", cv_csdelay.value, cv_csdelay.value * 1000 / TICRATE);
        return;
    }

    cv_csdelay.value = atoi(COM_Argv(1));
}
#endif

void Command_CSNodes_f(void)
{
    int i;
    for (i = 0; i < MAXPLAYERS; i ++)
    {
        if (virtualplayers[i].in_game)
        {
            struct sockaddr_in* sa = (struct sockaddr_in*) &virtualplayers[i].address;

            CONS_Printf("%02i - %s ", i, virtualplayers[i].name);
            CONS_Printf("[%i.%i.%i.%i:%i PP %i]\n", sa->sin_addr.S_un.S_un_b.s_b1, sa->sin_addr.S_un.S_un_b.s_b2,
                            sa->sin_addr.S_un.S_un_b.s_b3, sa->sin_addr.S_un.S_un_b.s_b4, ntohs(sa->sin_port), virtualplayers[i].purportedport);
        }

        if (i == self_id)
            CONS_Printf("%02i - %s (self)\n", i, player_names[0]);
    }
}

void Command_CSPlayerlinks(void)
{
    int i;

    for (i = 0; i < MAXPLAYERS; i ++)
    {
        if (virtualplayers[i].in_game)
        {
            if (! CS_host)
                CONS_Printf("%02i - %s (%s)\n", i, virtualplayers[i].name, self.player_links & (1 << i) ? "direct link" : "host link");
            else
            {
                int j;

                CONS_Printf("%02i - %s (", i, virtualplayers[i].name);

                for (j = 0; j < MAXPLAYERS; j ++)
                {
                    if (j == 0)
                        CONS_Printf(player_names[0]);
                    else if (virtualplayers[i].player_links & (1 << j))
                        CONS_Printf(", %s", virtualplayers[j].name);
                }

                CONS_Printf(")\n");
            }
        }

        if (i == self_id)
            CONS_Printf("%02i - %s (self)\n", i, player_names[0]);
    }
}

void Command_CSKick_f(void)
{
    int player_id;

    if (COM_Argc() != 2)
    {
        CONS_Printf("Usage: cskick csnode\n");
        return;
    }

    player_id = nametonum(COM_Argv(1));

    if (player_id >= MAXPLAYERS || player_id < 0 || player_id == self_id)
        return;

    if (! CS_host)
        return;

    CONS_Printf("%s has been kicked from your server.\n", virtualplayers[player_id].name);

    SendRefusePacket(&virtualplayers[player_id].address, "You have been kicked by the server.\n");

    RemovePlayer(player_id);
}

void Command_CSBan_f(void)
{
    struct sockaddr_in* sa;
    struct in_addr* inaddr;
	int player_id;

    if (COM_Argc() != 2)
    {
        CONS_Printf("Usage: csban csnode\n");
        return;
    }

    player_id = nametonum(COM_Argv(1));

    if (player_id >= MAXPLAYERS || player_id < 0 || player_id == self_id)
        return;

    if (! CS_host)
        return;

    sa = (struct sockaddr_in*) &virtualplayers[player_id].address;
    inaddr = &(sa->sin_addr);

    ban_list[ban_count ++] = inaddr->S_un.S_addr;

    SendRefusePacket(&virtualplayers[player_id].address, "You have been banned by the server.\n");

    CONS_Printf("%s has been banned from your server.\n", virtualplayers[player_id].name);
    RemovePlayer(player_id);
}

void Command_CSClearbans_f(void)
{
    ban_count = 0; // Easy!

    CONS_Printf("CS Bans have been cleared.\n");
}

extern int missed_hits;

void Command_CSStats(void)
{
    int total_hits = (int) (players[0].score / 50) + missed_hits;

    CONS_Printf("Ring hit/miss stats: ----------\n");
    CONS_Printf("Total observed ring hits: %i\nSucessful hits: %i\nUnsuccessful hits: %i\nSuccess percentage: %i%%\n",
                total_hits, total_hits - missed_hits, missed_hits, total_hits ? (total_hits - missed_hits) * 100 / total_hits : 0);
}

void ClientToClient_OnChange(void)
{
    if (! cv_clienttoclient.value)
        self.player_links = 0;
}
