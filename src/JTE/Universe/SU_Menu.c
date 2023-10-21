// SU - SRB2 Universe
// By Jason the Echidna
// MINE! NO STEALY!

#include "../../doomstat.h"
#ifdef UNIVERSE

// Headers
#include "../../d_clisrv.h"
#include "../../d_englsh.h"
#include "../../g_game.h"
#include "../../m_menu.h"
#include "../../s_sound.h"
#include "../../v_video.h"
#include "../../z_zone.h"
#include "SU_Command.h"
#include "SU_Menu.h"

// General use functions
static inline void SU_DrawSkillMenu(void)
{
	V_DrawCenteredString(160, currentMenu->y + currentMenu->menuitems[0].alphaKey - 30, 0, "Please choose the\ndifficulty level:");
	M_DrawGenericMenu();
}

static inline void SU_DrawTeleportMenu(void)
{
	V_DrawString(currentMenu->x - 24, currentMenu->y + currentMenu->menuitems[0].alphaKey - 16, 0, "Teleport to:");
	M_DrawGenericMenu();
}

///////////////////
// MENU: SU menu //
///////////////////
typedef enum
{
	newgame,
	savechar,
	loadchar,
	splitgame,
	servgame,
	endgame,
	su_end
} su_e;

static void SU_StartGame(int choice)
{
	if(netgame)
	{
		M_StartMessage(NEWGAME,M_ExitGameResponse,MM_YESNO);
		return;
	}

	universe = true;
	M_ClearMenus(false);

	if(choice == servgame)
	{
		netgame = true;
		multiplayer = true;
		if(demoplayback)
			COM_BufAddText("stopdemo\n");
		D_MapChange(SULOBBYMAP, GT_COOP, sk_medium, 1, 1, false, false);
	}
	else
		G_DeferedInitNew(sk_medium, G_BuildMapName(SULOBBYMAP), 0, (choice == splitgame), false);

	COM_BufAddText("dummyconsvar 1\n");
}

static void SU_Save(int choice)
{
	choice = 0;
	COM_BufAddText("savechar 0\n");
	M_ClearMenus(false);
}

static void SU_Load(int choice)
{
	choice = 0;

	if(Playing() && !(universe && netgame))
	{
		M_StartMessage(
			"You are already playing.\n"
			"Do you wish to end the\n"
			"current game? (Y/N)\n",
				M_ExitGameResponse,MM_YESNO);
		return;
	}

	COM_BufAddText("loadchar 0\n");
	M_ClearMenus(false);
}

static menuitem_t SUMenu[]=
{
	{IT_CALL | IT_STRING, NULL, "New Character",    SU_StartGame, 0},
	{IT_CALL | IT_STRING, NULL, "Save Character",   SU_Save,      0},
	{IT_CALL | IT_STRING, NULL, "Load Character",   SU_Load,      0},
	{IT_CALL | IT_STRING, NULL, "Splitscreen Game", SU_StartGame, 0},
	{IT_CALL | IT_STRING, NULL, "Host Game",        SU_StartGame, 0},
	{IT_CALL | IT_STRING, NULL, "End Game",         M_EndGame,     0},
};

menu_t SUDef =
{
	NULL,
	NULL,
	su_end,             // # of menu items
	NULL,               // previous menu
	SUMenu,             // menuitem_t ->
	M_DrawCenteredMenu, // drawing routine ->
	160, 144,           // x, y
	newgame,            // lastOn
	NULL
};

//////////////////////
// MENU: Skill menu //
//////////////////////
static void SU_ChooseSkill(int choice)
{
	M_ClearMenus(false);
	D_MapChange(SUHUBMAP, gametype, choice+2, 1, 1, false, false);
}

typedef enum
{
	normal,
	hard,
	veryhard,
	ultimate,
	skill_end
} skill_e;

static menuitem_t SkillMenu[]=
{
	// Tails
	{IT_CALL | IT_STRING, NULL, "Normal",    SU_ChooseSkill, 0},
	{IT_CALL | IT_STRING, NULL, "Hard",      SU_ChooseSkill, 0},
	{IT_CALL | IT_STRING, NULL, "Very Hard", SU_ChooseSkill, 0},
	{IT_CALL | IT_STRING, NULL, "Ultimate",  SU_ChooseSkill, 0},
};

menu_t SkillDef =
{
	NULL,
	NULL,
	skill_end,          // # of menu items
	NULL,               // previous menu
	SkillMenu,          // menuitem_t ->
	SU_DrawSkillMenu,  // drawing routine ->
	50, 120,            // x, y
	normal,             // lastOn
	NULL
};

void SU_SkillMenu(void)
{
	M_StartControlPanel();
	M_SetupNextMenu(&SkillDef);
}

//////////////////////////////////
// MENU: Teleport to Lobby menu //
//////////////////////////////////
static void SU_Teleport2Lobby(int choice)
{
	choice = SULOBBYMAP;
	M_ClearMenus(false);
	D_MapChange(choice, gametype, sk_medium, 1, 1, false, false);
}

typedef enum
{
	lobby,
	teleport2lobby_end
} teleport2lobby_e;

static menuitem_t Teleport2LobbyMenu[]=
{
	{IT_CALL | IT_STRING, NULL, "Lobby", SU_Teleport2Lobby, 0},
};

menu_t Teleport2LobbyDef =
{
	NULL,
	NULL,
	teleport2lobby_end,  // # of menu items
	NULL,                // previous menu
	Teleport2LobbyMenu,  // menuitem_t ->
	SU_DrawTeleportMenu,// drawing routine ->
	140, 70,             // x, y
	lobby,               // lastOn
	NULL
};

void SU_Teleport2LobbyMenu(void)
{
	M_StartControlPanel();
	M_SetupNextMenu(&Teleport2LobbyDef);
}

////////////////////////////////
// MENU: Teleport to HUB menu //
////////////////////////////////
static void SU_Teleport2HUB(int choice)
{
	choice = SUHUBMAP;
	M_ClearMenus(false);
	D_MapChange(choice, gametype, gameskill, 0, 1, false, false);
}

typedef enum
{
	hub,
	teleport2hub_end
} teleport2hub_e;

static menuitem_t Teleport2HUBMenu[]=
{
	{IT_CALL | IT_STRING, NULL, "Pioneer 2", SU_Teleport2HUB, 0},
};

menu_t Teleport2HUBDef =
{
	NULL,
	NULL,
	teleport2hub_end,    // # of menu items
	NULL,                // previous menu
	Teleport2HUBMenu,    // menuitem_t ->
	SU_DrawTeleportMenu,// drawing routine ->
	140, 70,             // x, y
	hub,                 // lastOn
	NULL
};

void SU_Teleport2HUBMenu(void)
{
	M_StartControlPanel();
	M_SetupNextMenu(&Teleport2HUBDef);
}

////////////////////////////////
// MENU: Teleport to Map menu //
////////////////////////////////
static void SU_Teleport2Map(int choice)
{
	int i, j = 0;

	M_ClearMenus(false);

	for(i = 0; i < NUMMAPS; i++)
	{
		if(mapheaderinfo[i].lvlttl[0] == '\0')
			continue;

		if(i != spstage_start-1
		&& !(mapheaderinfo[i].typeoflevel & TOL_SP) && !(mapheaderinfo[i].typeoflevel & TOL_COOP))
			continue;

		if(i != spstage_start-1 && !mapvisited[i])
			continue;

		if(j == choice)
		{
			prevmap = gamemap-1;
			D_MapChange(i+1, gametype, gameskill, 0, 1, false, false);
			return;
		}

		j++;
	}
}

static menuitem_t Teleport2MapMenu[23];
menu_t Teleport2MapDef =
{
	NULL,
	NULL,
	23,                  // # of menu items
	NULL,                // previous menu
	Teleport2MapMenu,    // menuitem_t ->
	SU_DrawTeleportMenu,// drawing routine ->
	140, 10,             // x, y
	0,                   // lastOn
	NULL
};

static size_t numlen(byte actnum)
{
	return (size_t)(log(actnum)+1);
}

void SU_Teleport2MapMenu(void)
{
	int i; // Map currently looking at
	unsigned int pos = 0; // Next menu position to fill
	byte alpha = 1; // Alpha value of menu position
	static char ptext[23][41]; // Text of menu item
	memset(ptext, 0, sizeof(ptext));

	for(i = 0; i < NUMMAPS; i++)
	{
		if(mapheaderinfo[i].lvlttl[0] == '\0')
			continue;

		if(i != spstage_start-1
		&& !(mapheaderinfo[i].typeoflevel & TOL_SP) && !(mapheaderinfo[i].typeoflevel & TOL_COOP))
			continue;

		if(i != spstage_start-1 && !mapvisited[i])
			continue;

		if(i-1 >= 0 && pos > 0
		&& !stricmp(mapheaderinfo[i].lvlttl, mapheaderinfo[i-1].lvlttl)
		&& mapheaderinfo[i].actnum != 0
		&& mapheaderinfo[i-1].actnum != 0
		&& mapheaderinfo[i].actnum != mapheaderinfo[i-1].actnum)
		{
			{
				// Calculate the length allowed for the text...
				size_t len = 40;
				if(!mapheaderinfo[i].nozone)
					len -= strlen(" zone");
				if(mapheaderinfo[i].actnum != 0)
					len -= numlen(mapheaderinfo[i].actnum)+1;
				if(len > 40)
					len = 1;

				// Set the text...
				strncpy(ptext[pos], mapheaderinfo[i].lvlttl, len);
			}
			if(!mapheaderinfo[i].nozone)
				sprintf(ptext[pos], "%s zone", ptext[pos]);

			while(V_StringWidth(ptext[pos]) <= V_StringWidth(ptext[pos-1]) // Add spaces for each act number...
			&& strlen(ptext[pos]) < 40 - (numlen(mapheaderinfo[i].actnum)+1))
				sprintf(ptext[pos], "%s ", ptext[pos]);
			sprintf(ptext[pos], "%s%d", ptext[pos], mapheaderinfo[i].actnum); // And then place the act number at the end.
			if(V_StringWidth(ptext[pos]) > 320)
			{
				// Since this string is too big to fit on the screen, we're gonna start a new line now instead.
				goto settext;
			}
			goto done;
		}

		settext:
		alpha++;
		{
			// Calculate the length allowed for the text...
			size_t len = 40;
			if(!mapheaderinfo[i].nozone)
				len -= strlen(" zone");
			if(mapheaderinfo[i].actnum != 0)
				len -= numlen(mapheaderinfo[i].actnum)+1; // WTF, why are you using a logarithm??
			if(len > 40)
				len = 1;

			// Set the text...
			strncpy(ptext[pos], mapheaderinfo[i].lvlttl, len);
		}
		if(!mapheaderinfo[i].nozone)
			sprintf(ptext[pos], "%s zone", ptext[pos]);
		if(mapheaderinfo[i].actnum != 0)
			sprintf(ptext[pos], "%s %d", ptext[pos], mapheaderinfo[i].actnum);

		done:
		ptext[pos][40] = '\0';
		Teleport2MapMenu[pos].text = ptext[pos];

		if(320/2 - V_StringWidth(ptext[pos])/2 < Teleport2MapDef.x)
			Teleport2MapDef.x = (short)(320/2 - V_StringWidth(ptext[pos])/2);

		Teleport2MapMenu[pos].alphaKey = (byte)(alpha*8);

		Teleport2MapMenu[pos].status = IT_CALL|IT_STRING;
		Teleport2MapMenu[pos].patch = NULL;
		Teleport2MapMenu[pos].itemaction = SU_Teleport2Map;

		// If no more menu items will fit on the menu, then stop adding them.
		if((16 - Teleport2MapMenu[0].alphaKey) + alpha*8 > 200 - 8
		|| pos >= 23)
			break;

		pos++;
	}

	// Make it centered around 80 but move down if it doesn't fit.
	Teleport2MapDef.y = (short)(80 - (alpha*8)/2);
	if(Teleport2MapDef.y + Teleport2MapMenu[0].alphaKey - 16 < 0)
		Teleport2MapDef.y = (short)(16 - Teleport2MapMenu[0].alphaKey);

	for(; pos < 23; pos++)
		Teleport2MapMenu[pos].status = IT_DISABLED;

	M_StartControlPanel();
	M_SetupNextMenu(&Teleport2MapDef);
}

/////////////////////////////////
// MENU: Teleport to next menu //
/////////////////////////////////
static void SU_Teleport2Next(int choice)
{
	choice = 0;
	M_ClearMenus(false);
	G_DoCompleted();
}

typedef enum
{
	nextlevel,
	teleport2next_end
} teleport2next_e;

static menuitem_t Teleport2NextMenu[]=
{
	{IT_CALL | IT_STRING, NULL, "Pioneer 2", SU_Teleport2Next, 0},
};

menu_t Teleport2NextDef =
{
	NULL,
	NULL,
	teleport2next_end,   // # of menu items
	NULL,                // previous menu
	Teleport2NextMenu,   // menuitem_t ->
	SU_DrawTeleportMenu,// drawing routine ->
	140, 70,             // x, y
	nextlevel,           // lastOn
	NULL
};

void SU_Teleport2NextMenu(void)
{
	int next = mapheaderinfo[gamemap-1].nextlevel;
	static char ptext[41];

	if(next < 1100 // If you're not going to the credits or anything like that...
		&& stricmp(mapheaderinfo[next-1].lvlttl, // And the next map's level title...
		mapheaderinfo[gamemap-1].lvlttl)) // Does not match this map's level title...
			next = SUHUBMAP; // Then go back to the Pioneer 2!

	{
		// Calculate the length allowed for the text...
		size_t len = 40;
		if(!mapheaderinfo[next-1].nozone)
			len -= strlen(" zone");
		if(mapheaderinfo[next-1].actnum != 0)
			len -= numlen(mapheaderinfo[next-1].actnum);

		// Set the text
		strncpy(ptext, mapheaderinfo[next-1].lvlttl, len);
	}
	if(!mapheaderinfo[next-1].nozone)
		sprintf(ptext, "%s zone", ptext);
	if(mapheaderinfo[next-1].actnum != 0)
		sprintf(ptext, "%s %d", ptext, mapheaderinfo[next-1].actnum);
	ptext[40] = '\0';
	Teleport2NextMenu[nextlevel].text = ptext;
	Teleport2NextDef.x = (short)(320/2 - V_StringWidth(ptext)/2);

	M_StartControlPanel();
	M_SetupNextMenu(&Teleport2NextDef);
}

/////////////////////////////////
// MENU: Teleport to last menu //
/////////////////////////////////
static void SU_Teleport2Last(int choice)
{
	choice = prevmap+1;
	M_ClearMenus(false);
	D_MapChange(choice, gametype, gameskill, 0, 1, false, false);
}

typedef enum
{
	lastlevel,
	teleport2last_end
} teleport2last_e;

static menuitem_t Teleport2LastMenu[]=
{
	{IT_CALL | IT_STRING, NULL, "Pioneer 2", SU_Teleport2Last, 0},
};

menu_t Teleport2LastDef =
{
	NULL,
	NULL,
	teleport2last_end,   // # of menu items
	NULL,                // previous menu
	Teleport2LastMenu,   // menuitem_t ->
	SU_DrawTeleportMenu,// drawing routine ->
	140, 70,             // x, y
	lastlevel,           // lastOn
	NULL
};

void SU_Teleport2LastMenu(void)
{
	int i; // Map currently looking at
	static char ptext[41]; // Text of menu item

	for(i = 0; i < NUMMAPS; i++)
	{
		if(mapheaderinfo[i].lvlttl[0] == '\0')
			continue;

		if(i != spstage_start-1
		&& !(mapheaderinfo[i].typeoflevel & TOL_SP) && !(mapheaderinfo[i].typeoflevel & TOL_COOP))
			continue;

		if(i != spstage_start-1 && !mapvisited[i])
			continue;

		if(mapheaderinfo[i].nextlevel != gamemap)
			continue;

		// This is the correct map, so set it to the menu option.
		goto setmap;
	}
	// No map found? You must have come from the HUB!
	i = SUHUBMAP-1;

	// The map has been chosen and is currently set to i.
	setmap:
	prevmap = i;

	{
		// Calculate the length allowed for the text...
		size_t len = 40;
		if(!mapheaderinfo[i].nozone)
			len -= strlen(" zone");
		if(mapheaderinfo[i].actnum != 0)
			len -= numlen(mapheaderinfo[i].actnum);

		// Set the text...
		strncpy(ptext, mapheaderinfo[i].lvlttl, len);
	}
	if(!mapheaderinfo[i].nozone)
		sprintf(ptext, "%s zone", ptext);
	if(mapheaderinfo[i].actnum != 0)
		sprintf(ptext, "%s %d", ptext, mapheaderinfo[i].actnum);
	ptext[40] = '\0';
	Teleport2LastMenu[lastlevel].text = ptext;

	if(320/2 - V_StringWidth(ptext)/2 < Teleport2LastDef.x)
		Teleport2LastDef.x = (short)(320/2 - V_StringWidth(ptext)/2);

	M_StartControlPanel();
	M_SetupNextMenu(&Teleport2LastDef);
}

#endif
