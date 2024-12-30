// JB - JTE Bots
// By Jason the Echidna
// MINE! NO STEALY!

#include "../doomstat.h"
#include "../command.h"
#include "../d_clisrv.h"
#include "../d_net.h"
#include "../g_game.h"
#include "../m_menu.h"
#include "../p_local.h"
#include "../r_main.h"
#include "../r_things.h"
#include "../byteptr.h"

#include "JB_Bot.h"
#include "JB_Command.h"

#ifdef JTEBOTS

static void JB_Command_BotAdd(void);
static void JB_MaxBots_OnChange(void);
static void JB_Netvar_OnChange(void);

boolean jb_cmdwait = false;

static CV_PossibleValue_t maxbots_cons_t[] = {{0, "MIN"}, {MAXPLAYERS-1, "MAX"}, {0, NULL}};
consvar_t cv_maxbots = {"maxbots", "MAX", CV_NETVAR|CV_CALL, maxbots_cons_t, JB_MaxBots_OnChange, 0, NULL, NULL, 0, 0, NULL};
consvar_t cv_botcanflyme = {"botcanflyme", "Off", CV_NETVAR|CV_CALL|CV_NOINIT, CV_OnOff, JB_Netvar_OnChange, 0, NULL, NULL, 0, 0, NULL};
consvar_t cv_botcoopai = {"botcoopai", "Off", CV_NETVAR|CV_CALL|CV_NOINIT, CV_OnOff, JB_Netvar_OnChange, 0, NULL, NULL, 0, 0, NULL};

void JB_Init(void)
{
	COM_AddCommand("addbot", JB_Command_BotAdd);
	RegisterNetXCmd(XD_ADDBOT, JB_Got_BotAdd);

	CV_RegisterVar(&cv_maxbots);
	CV_RegisterVar(&cv_botcanflyme);
	CV_RegisterVar(&cv_botcoopai);

	memset(bots, 0, sizeof(bots));
	memset(charselbots, 0, sizeof(charselbots));

	// Sonic & Tails char select menu option
	/*strcpy(description[3].info, "             Fastest\n                 Speed Thok\n             Sonic & Tails,\njust like the good ol\'\nGenesis days. You play\nas Sonic.");
	strcpy(description[3].picname, "STCHAR");
	strcpy(description[3].skinname, "SONIC");
	strcpy(charselbots[3], "TAILS");
	PlayerMenu[3].status = IT_CALL|IT_STRING;
	PlayerMenu[3].text = "SONIC & TAILS";*/
}

static inline char JB_Name2Color(const char *name)
{
	INT32 i;

	if (gametype == GT_CTF)
	{
		if (!stricmp(name,"red"))
			return 1;
		else if (!stricmp(name,"blue"))
			return 2;
		else
		{
			CONS_Printf("CTF team %s not valid.\n", name);
			return (char)players[consoleplayer].ctfteam;
		}
	}
	else
	{
		for(i = 0; i < MAXSKINCOLORS; i++)
		{
			if (!stricmp(name,Color_Names[i]))
				return (char)i;
		}
		CONS_Printf("Color %s not found.\n", name);
		return 0;
	}
}

static void JB_Command_BotAdd(void)
{
	char buf[MAXPLAYERNAME+2];
	INT32 skin;
	const char* skinname;
	INT32 i, numbots;

	// If you already sent a command and are still waiting for it to go through, then don't send another!
	if (jb_cmdwait)
		return;

	// If you're not even playing, you can't add a bot...
	if (!Playing())
	{
		CONS_Printf("You cannot add bots outside of a game...\n");
		return;
	}

	// No bots in singleplayer mode unless I say so!...
	if (!multiplayer && !netgame)
	{
		CONS_Printf("You cannot modify bots in singleplayer mode.\n");
		return;
	}

	if (!server && adminplayer != consoleplayer && gametype != GT_COOP)
	{
		CONS_Printf("You are not the server (or admin) or in co-op mode.\n");
		return;
	}

	if (cv_maxbots.value == 0)
	{
		CONS_Printf("Bots are not allowed on this server.\n");
		return;
	}

	for(i = 0, numbots = 0; i < MAXPLAYERS; i++)
		if (playeringame[i] && players[i].bot && players[i].bot->ownernum == consoleplayer)
			numbots++;
	if (numbots >= cv_maxbots.value // If you've added all your bots already...
	|| D_NumPlayers() >= MAXPLAYERS) // Or the max player limit has been reached
	{ // Then don't allow it!
		CONS_Printf("The maximum number of bots on this server has been reached.\n");
		return;
	}

	if (COM_Argc() == 2
	&& (!stricmp(COM_Argv(1),"help")
	|| !stricmp(COM_Argv(1),"info")
	|| !stricmp(COM_Argv(1),"use")))
	{
		CONS_Printf("Usage: addbot [skin] [color/ctfteam] [altname]\n");
		return;
	}

	// Set the arguments...
	if (COM_Argc() < 2)
	{
		// Defaults.
		buf[0] = (char)((players[consoleplayer].skin+numbots+1)%numskins);
		if (gametype == GT_CTF)
			buf[1] = (char)players[consoleplayer].ctfteam;
		else
			buf[1] = (char)atoi(skins[(int)buf[0]].prefcolor);
		strncpy(&buf[2],va("%sBot",skins[(int)buf[0]].name),MAXPLAYERNAME);
	}
	else
	{
		// Skin name
		skinname = COM_Argv(1);
		for(skin = 0; skin < numskins; skin++)
			if (!stricmp(skins[skin].name, skinname))
				break;
		if (skin == numskins)
		{
			CONS_Printf("Skin %s not found.\n", skinname);
			skin = (players[consoleplayer].skin+numbots+1)%numskins;
		}
		buf[0] = (char)skin;

		// CTF team / Bot color
		if (COM_Argc() > 2)
		{
			if (!isalpha(*COM_Argv(2)))
				buf[1] = (char)atoi(COM_Argv(2));
			else
			{
				buf[1] = JB_Name2Color(COM_Argv(2));
				if (buf[1] == 0)
					buf[1] = (char)atoi(skins[skin].prefcolor);
			}
		}
		else if (gametype == GT_CTF)
			buf[1] = (char)players[consoleplayer].ctfteam;
		else
			buf[1] = (char)atoi(skins[skin].prefcolor);

		// Player name
		if (COM_Argc() > 3)
			strncpy(&buf[2],COM_Argv(3),MAXPLAYERNAME);
		else
			strncpy(&buf[2],va("%sBot",skins[skin].name),MAXPLAYERNAME);
	}

	SendNetXCmd(XD_ADDBOT, &buf, 2+strlen(&buf[2])+1);
	jb_cmdwait = true;

	CONS_Printf("Added bot %s with the skin %s ", &buf[2], skins[(size_t)buf[0]].name);
	if (gametype == GT_CTF)
		CONS_Printf("on the %s team.\n", (buf[1] == 2) ? "blue" : "red");
	else
		CONS_Printf("colored %s.\n", Color_Names[(size_t)buf[1]]);
}

static void JB_MaxBots_OnChange(void)
{
	// Max of MAXBOTS bots...
	if (server && !stricmp(cv_maxbots.string, "MAX"))
		CV_StealthSetValue(&cv_maxbots, MAXPLAYERS-1);
}

static void JB_Netvar_OnChange(void)
{
	if (!multiplayer && !netgame)
	{
		if (cv_botcanflyme.value != 0)
			CV_StealthSetValue(&cv_botcanflyme, 0);
		if (cv_botcoopai.value != 0)
			CV_StealthSetValue(&cv_botcoopai, 0);
	}
}

#endif
