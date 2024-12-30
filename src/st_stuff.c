// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// Copyright (C) 1993-1996 by id Software, Inc.
// Copyright (C) 1998-2000 by DooM Legacy Team.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//      Status bar code.
//      Does the face/direction indicator animatin.
//      Does palette indicators as well (red pain/berserk, bright pickup)
//
//-----------------------------------------------------------------------------
/// \file
/// \brief Status bar code
///
///	Does the face/direction indicator animatin.
///	Does palette indicators as well (red pain/berserk, bright pickup)

#include "doomdef.h"
#include "g_game.h"
#include "r_local.h"
#include "p_local.h"
#include "f_finale.h"
#include "st_stuff.h"
#include "i_video.h"
#include "v_video.h"
#include "z_zone.h"
#include "hu_stuff.h"
#include "s_sound.h"
#include "i_system.h"
#include "m_menu.h"

#ifdef HWRENDER
#include "hardware/hw_main.h"
#endif

UINT16 objectsdrawn = 0;

//
// STATUS BAR DATA
//

// Palette indices.
#define STARTBONUSPALS 9
#define NUMBONUSPALS 4

patch_t *tallnum[10]; // 0-9, tall numbers
static patch_t *nightsnum[10]; // NiGHTS timer numbers

patch_t *faceprefix[MAXSKINS]; // face status patches
patch_t *superprefix[MAXSKINS]; // super face status patches
static patch_t *facenameprefix[MAXSKINS]; // face background
#ifdef RIDERS
patch_t *iconprefix[MAXSKINS];
#endif

// ------------------------------------------
//             status bar overlay
// ------------------------------------------

// icons for overlay
patch_t *sboscore; // Score logo
patch_t *sbotime; // Time logo
patch_t *sbocolon; // Colon for time
static patch_t *sborings;
static patch_t *sboover;
static patch_t *timeover;
static patch_t *stlivex;
static patch_t *rrings;
static patch_t *getall; // Special Stage HUD
static patch_t *timeup; // Special Stage HUD
static patch_t *homing1; // Emerald hunt indicators
static patch_t *homing2; // Emerald hunt indicators
static patch_t *homing3; // Emerald hunt indicators
static patch_t *homing4; // Emerald hunt indicators
static patch_t *homing5; // Emerald hunt indicators
static patch_t *homing6; // Emerald hunt indicators
static patch_t *race1;
static patch_t *race2;
static patch_t *race3;
static patch_t *racego;
static patch_t *supersonic;
static patch_t *ttlnum;
static patch_t *nightslink;
static patch_t *count5;
static patch_t *count4;
static patch_t *count3;
static patch_t *count2;
static patch_t *count1;
static patch_t *count0;
static patch_t *curweapon;
static patch_t *normring;
static patch_t *bouncering;
static patch_t *autoring;
static patch_t *explosionring;
static patch_t *scatterring;
static patch_t *grenadering;
static patch_t *railring;
static patch_t *jumpshield;
static patch_t *forceshield;
static patch_t *ringshield;
static patch_t *watershield;
static patch_t *bombshield;
static patch_t *invincibility;
static patch_t *zmushroom;
static patch_t *starmaning;
static patch_t *wipeout;
static patch_t *megashroom;
static patch_t *minishroom;
static patch_t *dsparks;
static patch_t *ddsparks;
static patch_t *sneakers;
static patch_t *gravboots;
static patch_t *nonicon;
static patch_t *bluestat;
static patch_t *byelstat;
static patch_t *orngstat;
static patch_t *redstat;
static patch_t *yelstat;
static patch_t *nbracket;
static patch_t *nhud[12];
static patch_t *narrow[9];
static patch_t *minicaps;
static patch_t *minus;
static patch_t *gotrflag;
static patch_t *gotbflag;
#ifdef RIDERS
static patch_t *ridrings;
static patch_t *ridrngs2;
static patch_t *ridtime;
static patch_t *ridtime2;
static patch_t *ridspeed;
static patch_t *ridsped2;
static patch_t *ridlvl;
static patch_t *thirty;
static patch_t *sixty;
static patch_t *boost;
static patch_t *wair;
static patch_t *rair;
static patch_t *rrace4;
static patch_t *rrace3;
static patch_t *rrace2;
static patch_t *rrace1;
static patch_t *rrace0;
static patch_t *rracego;
static patch_t *rank;
static patch_t *rankc;
static patch_t *rankb;
static patch_t *ranka;
static patch_t *ranks;
static patch_t *rankx;
static patch_t *blankhd;
static patch_t *mushroom;
static patch_t *doublemushroom;
static patch_t *triplemushroom;
static patch_t *goldmushroom;
static patch_t *fireflower;
static patch_t *rinvincibility;
static patch_t *ringshield2;
static patch_t *shell;
static patch_t *tshell;
static patch_t *tshell1;
static patch_t *tshell2;
static patch_t *tshell3;
static patch_t *redshell;
static patch_t *tredshell;
static patch_t *tredshell1;
static patch_t *tredshell2;
static patch_t *tredshell3;
static patch_t *peel;
static patch_t *tpeel;
static patch_t *tpeel1;
static patch_t *tpeel2;
static patch_t *tpeel3;
static patch_t *tboo;
static patch_t *wboo;
static patch_t *pboo;
static patch_t *gboo;
static patch_t *ksink;
static patch_t *fake;
static patch_t *bomb;
static patch_t *tbomb;
static patch_t *tbomb1;
static patch_t *tbomb2;
static patch_t *tbomb3;
static patch_t *bshell;
static patch_t *tbshell;
static patch_t *tbshell1;
static patch_t *tbshell2;
static patch_t *tbshell3;
static patch_t *star;
static patch_t *thunder;
static patch_t *bluethunder;
static patch_t *megamushroom;
static patch_t *ijumpshield;
static patch_t *iforceshield;
static patch_t *iwatershield;
static patch_t *ibombshield;
static patch_t *iringshield;
static patch_t *iunknown;
static patch_t *ijumpshield2;
static patch_t *iforceshield2;
static patch_t *iwatershield2;
static patch_t *ibombshield2;
static patch_t *iringshield2;
static patch_t *ijumpshield3;
static patch_t *iforceshield3;
static patch_t *iwatershield3;
static patch_t *ibombshield3;
static patch_t *iringshield3;
static patch_t *needle1;
static patch_t *needle2;
static patch_t *needle3;
static patch_t *needle4;
static patch_t *needle5;
static patch_t *needle6;
static patch_t *needle7;
static patch_t *needle8;
static patch_t *needle9;
static patch_t *needle10;
static patch_t *needle11;
static patch_t *needle12;
static patch_t *needle13;
static patch_t *needle14;
static patch_t *needle15;
static patch_t *needle16;
static patch_t *needle17;
static patch_t *needle18;
static patch_t *needle19;
static patch_t *needle20;
static patch_t *needle21;
static patch_t *needle22;
static patch_t *needle23;
static patch_t *needle24;
static patch_t *needle25;
static patch_t *itemused9;
static patch_t *itemused8;
static patch_t *itemused7;
static patch_t *itemused6;
static patch_t *itemused5;
static patch_t *itemused4;
static patch_t *itemused3;
static patch_t *itemused2;
static patch_t *itemused1;
static patch_t *shieldused6;
static patch_t *shieldused5;
static patch_t *shieldused4;
static patch_t *shieldused3;
static patch_t *shieldused2;
static patch_t *shieldused1;
#endif

static boolean facefreed[MAXPLAYERS];
static boolean prefixfreed[MAXPLAYERS];
#ifdef RIDERS
static boolean iconfreed[MAXPLAYERS];
#endif

hudinfo_t hudinfo[NUMHUDITEMS] =
{
	{  16, 166}, // HUD_LIVESNAME
	{  16, 176}, // HUD_LIVESPIC
	{  68, 181}, // HUD_LIVESNUM
	{  36, 184}, // HUD_LIVESX
	{ 220,  10}, // HUD_RINGSSPLIT
	{ 288,  10}, // HUD_RINGSNUMSPLIT
	{  16,  42}, // HUD_RINGS
	{ 112,  42}, // HUD_RINGSNUM
	{  16,  10}, // HUD_SCORE
	{ 128,  10}, // HUD_SCORENUM
	{ 136,  10}, // HUD_TIMESPLIT
	{ 204,  10}, // HUD_LOWSECONDSSPLIT
	{ 212,  10}, // HUD_SECONDSSPLIT
	{ 188,  10}, // HUD_MINUTESSPLIT
	{ 188,  10}, // HUD_TIMECOLONSPLIT
	{  17,  26}, // HUD_TIME
	{ 128,  26}, // HUD_LOWTICS
	{ 136,  26}, // HUD_TICS
	{ 104,  26}, // HUD_LOWSECONDS
	{ 112,  26}, // HUD_SECONDS
	{  88,  26}, // HUD_MINUTES
	{  88,  26}, // HUD_TIMECOLON
	{ 112,  26}, // HUD_TIMETICCOLON
	{ 288,  40}, // HUD_SS_TOTALRINGS_SPLIT
	{ 112,  56}, // HUD_SS_TOTALRINGS
	{ 110,  93}, // HUD_GETRINGS
	{ 160,  93}, // HUD_GETRINGSNUM
	{ 124, 160}, // HUD_TIMELEFT
	{ 168, 176}, // HUD_TIMELEFTNUM
	{ 130,  93}, // HUD_TIMEUP
	{ 132, 168}, // HUD_HUNTPIC1
	{ 152, 168}, // HUD_HUNTPIC2
	{ 172, 168}, // HUD_HUNTPIC3
	{ 152,  24}, // HUD_GRAVBOOTSICO
	{ 240, 160}, // HUD_LAP
};

//
// STATUS BAR CODE
//

boolean ST_SameTeam(player_t *a, player_t *b)
{
	// Just pipe team messages to everyone in co-op or race.
	if (gametype == GT_COOP || gametype == GT_RACE)
		return true;

	// Spectator chat.
	if (a->spectator && b->spectator)
		return true;

	// Team chat.
	if (gametype == GT_CTF || (gametype == GT_MATCH && cv_matchtype.value))
		return a->ctfteam == b->ctfteam;

	if (gametype == GT_TAG)
		return ((a->pflags & PF_TAGIT) == (b->pflags & PF_TAGIT));

	return false;
}

static boolean st_stopped = true;

void ST_Ticker(void)
{
	if (st_stopped)
		return;
}

static INT32 st_palette = 0;

void ST_doPaletteStuff(void)
{
	INT32 palette;

	if (stplyr && stplyr->bonuscount)
	{
		palette = (stplyr->bonuscount+7)>>3;

		if (palette >= NUMBONUSPALS)
			palette = NUMBONUSPALS - 1;

		palette += STARTBONUSPALS;
	}
	else
		palette = 0;

	if (palette != st_palette)
	{
		st_palette = palette;

#if defined (SHUFFLE) && defined (HWRENDER)
		if (rendermode == render_opengl)
			HWR_SetPaletteColor(0);
		else
#endif
		if (rendermode != render_none)
		{
			if (palette >= STARTBONUSPALS && palette <= STARTBONUSPALS + NUMBONUSPALS)
				V_SetPaletteLump("FLASHPAL");
			else
				V_SetPaletteLump(GetPalette());

			if (!splitscreen || !palette)
				V_SetPalette(palette);
		}
	}
}

static void ST_overlayDrawer(void);

void ST_Drawer(boolean refresh)
{
#ifdef SEENAMES
	if (cv_seenames.value && cv_allowseenames.value && displayplayer == consoleplayer && seenplayer && seenplayer->mo)
	{
		if (cv_seenames.value == 1)
			V_DrawCenteredString(BASEVIDWIDTH/2, BASEVIDHEIGHT/2 + 15, V_TRANSLUCENT, player_names[seenplayer-players]);
		else if (cv_seenames.value == 2)
			V_DrawCenteredString(BASEVIDWIDTH/2, BASEVIDHEIGHT/2 + 15, V_TRANSLUCENT,
			va("%s%s", (gametype == GT_CTF || (gametype == GT_MATCH && cv_matchtype.value))
			           ? ((seenplayer->ctfteam == 1) ? "\x85" : "\x84") : "", player_names[seenplayer-players]));
		else //if (cv_seenames.value == 3)
			V_DrawCenteredString(BASEVIDWIDTH/2, BASEVIDHEIGHT/2 + 15, V_TRANSLUCENT,
			va("%s%s", (gametype == GT_COOP || gametype == GT_RACE) || ((gametype == GT_CTF || (gametype == GT_MATCH && cv_matchtype.value))
			           && players[consoleplayer].ctfteam == seenplayer->ctfteam) ? "\x83" : "\x85", player_names[seenplayer-players]));
	}
#endif

	// force a set of the palette by using doPaletteStuff()
	refresh = 0; //?
	if (vid.recalc)
		st_palette = -1;

	// Do red-/gold-shifts from damage/items
#ifdef HWRENDER
	//25/08/99: Hurdler: palette changes is done for all players,
	//                   not only player1! That's why this part
	//                   of code is moved somewhere else.
	if (rendermode == render_soft)
#endif
		if (rendermode != render_none) ST_doPaletteStuff();

	if (st_overlay)
	{
		// No deadview!
		stplyr = &players[displayplayer];
		ST_overlayDrawer();

		if (splitscreen)
		{
			stplyr = &players[secondarydisplayplayer];
			ST_overlayDrawer();
		}
	}
}

void ST_UnloadGraphics(void)
{
	Z_FreeTags(PU_HUDGFX, PU_HUDGFX);
}

void ST_LoadGraphics(void)
{
	INT32 i;
	char buffer[9];

	// SRB2 border patch
	st_borderpatchnum = W_GetNumForName("GFZFLR01");
	scr_borderpatch = W_CacheLumpNum(st_borderpatchnum, PU_HUDGFX);

	// Load the numbers, tall and short
	for (i = 0; i < 10; i++)
	{
		sprintf(buffer, "STTNUM%d", i);
		tallnum[i] = (patch_t *)W_CachePatchName(buffer, PU_HUDGFX);
		sprintf(buffer, "NGTNUM%d", i);
		nightsnum[i] = (patch_t *) W_CachePatchName(buffer, PU_HUDGFX);
	}

	// the original Doom uses 'STF' as base name for all face graphics
	// Graue 04-08-2004: face/name graphics are now indexed by skins
	//                   but load them in R_AddSkins, that gets called
	//                   first anyway
	// cache the status bar overlay icons (fullscreen mode)
	sborings = W_CachePatchName("SBORINGS", PU_HUDGFX);
	sboscore = W_CachePatchName("SBOSCORE", PU_HUDGFX);
	sboover = W_CachePatchName("SBOOVER", PU_HUDGFX);
	timeover = W_CachePatchName("TIMEOVER", PU_HUDGFX);
	stlivex = W_CachePatchName("STLIVEX", PU_HUDGFX);
	rrings = W_CachePatchName("RRINGS", PU_HUDGFX);
	sbotime = W_CachePatchName("SBOTIME", PU_HUDGFX); // Time logo
	sbocolon = W_CachePatchName("SBOCOLON", PU_HUDGFX); // Colon for time
	getall = W_CachePatchName("GETALL", PU_HUDGFX); // Special Stage HUD
	timeup = W_CachePatchName("TIMEUP", PU_HUDGFX); // Special Stage HUD
	homing1	= W_CachePatchName("HOMING1", PU_HUDGFX); // Emerald hunt indicators
	homing2	= W_CachePatchName("HOMING2", PU_HUDGFX); // Emerald hunt indicators
	homing3	= W_CachePatchName("HOMING3", PU_HUDGFX); // Emerald hunt indicators
	homing4	= W_CachePatchName("HOMING4", PU_HUDGFX); // Emerald hunt indicators
	homing5	= W_CachePatchName("HOMING5", PU_HUDGFX); // Emerald hunt indicators
	homing6	= W_CachePatchName("HOMING6", PU_HUDGFX); // Emerald hunt indicators
	race1 = W_CachePatchName("RACE1", PU_HUDGFX);
	race2 = W_CachePatchName("RACE2", PU_HUDGFX);
	race3 = W_CachePatchName("RACE3", PU_HUDGFX);
	racego = W_CachePatchName("RACEGO", PU_HUDGFX);
	supersonic = W_CachePatchName("SUPERICO", PU_HUDGFX);
	nightslink = W_CachePatchName("NGHTLINK", PU_HUDGFX);
	count5 = W_CachePatchName("DRWNF0", PU_HUDGFX);
	count4 = W_CachePatchName("DRWNE0", PU_HUDGFX);
	count3 = W_CachePatchName("DRWND0", PU_HUDGFX);
	count2 = W_CachePatchName("DRWNC0", PU_HUDGFX);
	count1 = W_CachePatchName("DRWNB0", PU_HUDGFX);
	count0 = W_CachePatchName("DRWNA0", PU_HUDGFX);

	curweapon = W_CachePatchName("CURWEAP", PU_HUDGFX);
	normring = W_CachePatchName("RINGIND", PU_HUDGFX);
	bouncering = W_CachePatchName("BNCEIND", PU_HUDGFX);
	autoring = W_CachePatchName("AUTOIND", PU_HUDGFX);
	explosionring = W_CachePatchName("BOMBIND", PU_HUDGFX);
	scatterring = W_CachePatchName("SCATIND", PU_HUDGFX);
	grenadering = W_CachePatchName("GRENIND", PU_HUDGFX);
	railring = W_CachePatchName("RAILIND", PU_HUDGFX);
	jumpshield = W_CachePatchName("WHTVB0", PU_HUDGFX);
	forceshield = W_CachePatchName("BLTVB0", PU_HUDGFX);
	ringshield = W_CachePatchName("YLTVB0", PU_HUDGFX);
	watershield = W_CachePatchName("GRTVB0", PU_HUDGFX);
	bombshield = W_CachePatchName("BKTVB0", PU_HUDGFX);
	invincibility = W_CachePatchName("PINVB0", PU_HUDGFX);
	zmushroom = W_CachePatchName("MUZVB0", PU_HUDGFX);
	starmaning = W_CachePatchName("SMZVB0", PU_HUDGFX);
	wipeout = W_CachePatchName("BPZVB0", PU_HUDGFX);
	megashroom = W_CachePatchName("MGZVB0", PU_HUDGFX);
	minishroom = W_CachePatchName("MNZVB0", PU_HUDGFX);
	dsparks = W_CachePatchName("DSZVB0", PU_HUDGFX);
	ddsparks = W_CachePatchName("DDZVB0", PU_HUDGFX);
	sneakers = W_CachePatchName("SHTVB0", PU_HUDGFX);
	gravboots = W_CachePatchName("GBTVB0", PU_HUDGFX);

	tagico = W_CachePatchName("TAGICO", PU_HUDGFX);
	rflagico = W_CachePatchName("RFLAGICO", PU_HUDGFX);
	bflagico = W_CachePatchName("BFLAGICO", PU_HUDGFX);
	rmatcico = W_CachePatchName("RMATCICO", PU_HUDGFX);
	bmatcico = W_CachePatchName("BMATCICO", PU_HUDGFX);
	gotrflag = W_CachePatchName("GOTRFLAG", PU_HUDGFX);
	gotbflag = W_CachePatchName("GOTBFLAG", PU_HUDGFX);
	nonicon = W_CachePatchName("NONICON", PU_HUDGFX);

	// NiGHTS HUD things
	bluestat = W_CachePatchName("BLUESTAT", PU_HUDGFX);
	byelstat = W_CachePatchName("BYELSTAT", PU_HUDGFX);
	orngstat = W_CachePatchName("ORNGSTAT", PU_HUDGFX);
	redstat = W_CachePatchName("REDSTAT", PU_HUDGFX);
	yelstat = W_CachePatchName("YELSTAT", PU_HUDGFX);
	nbracket = W_CachePatchName("NBRACKET", PU_HUDGFX);
	nhud[0] = W_CachePatchName("NHUD1", PU_HUDGFX);
	nhud[1] = W_CachePatchName("NHUD2", PU_HUDGFX);
	nhud[2] = W_CachePatchName("NHUD3", PU_HUDGFX);
	nhud[3] = W_CachePatchName("NHUD4", PU_HUDGFX);
	nhud[4] = W_CachePatchName("NHUD5", PU_HUDGFX);
	nhud[5] = W_CachePatchName("NHUD6", PU_HUDGFX);
	nhud[6] = W_CachePatchName("NHUD7", PU_HUDGFX);
	nhud[7] = W_CachePatchName("NHUD8", PU_HUDGFX);
	nhud[8] = W_CachePatchName("NHUD9", PU_HUDGFX);
	nhud[9] = W_CachePatchName("NHUD10", PU_HUDGFX);
	nhud[10] = W_CachePatchName("NHUD11", PU_HUDGFX);
	nhud[11] = W_CachePatchName("NHUD12", PU_HUDGFX);
	minicaps = W_CachePatchName("MINICAPS", PU_HUDGFX);

	narrow[0] = W_CachePatchName("NARROW1", PU_HUDGFX);
	narrow[1] = W_CachePatchName("NARROW2", PU_HUDGFX);
	narrow[2] = W_CachePatchName("NARROW3", PU_HUDGFX);
	narrow[3] = W_CachePatchName("NARROW4", PU_HUDGFX);
	narrow[4] = W_CachePatchName("NARROW5", PU_HUDGFX);
	narrow[5] = W_CachePatchName("NARROW6", PU_HUDGFX);
	narrow[6] = W_CachePatchName("NARROW7", PU_HUDGFX);
	narrow[7] = W_CachePatchName("NARROW8", PU_HUDGFX);

	// non-animated version
	narrow[8] = W_CachePatchName("NARROW9", PU_HUDGFX);

	// minus for negative numbers
	minus = (patch_t *)W_CachePatchName("STTMINUS", PU_HUDGFX);
#ifdef RIDERS
	ridrings = W_CachePatchName("RIDRINGS", PU_HUDGFX);
	ridrngs2 = W_CachePatchName("RIDRNGS2", PU_HUDGFX);
	ridtime = W_CachePatchName("RIDTIME", PU_HUDGFX);
	ridtime2 = W_CachePatchName("RIDTIME2", PU_HUDGFX);
	ridspeed = W_CachePatchName("RIDSPEED", PU_HUDGFX);
	ridsped2 = W_CachePatchName("RIDSPED2", PU_HUDGFX);
	ridlvl = W_CachePatchName("RIDLVL", PU_HUDGFX);
	wair = W_CachePatchName("WAIR", PU_HUDGFX);
	rair = W_CachePatchName("RAIR", PU_HUDGFX);
	thirty = W_CachePatchName("THIRTY", PU_HUDGFX);
	sixty = W_CachePatchName("SIXTY", PU_HUDGFX);
	boost = W_CachePatchName("BOOST", PU_HUDGFX);
	rrace4 = W_CachePatchName("RRACE4", PU_HUDGFX);
	rrace3 = W_CachePatchName("RRACE3", PU_HUDGFX);
	rrace2 = W_CachePatchName("RRACE2", PU_HUDGFX);
	rrace1 = W_CachePatchName("RRACE1", PU_HUDGFX);
	rrace0 = W_CachePatchName("RRACE0", PU_HUDGFX);
	rracego = W_CachePatchName("RRACEGO", PU_HUDGFX);
	rank = W_CachePatchName("RANK", PU_HUDGFX);
	rankc = W_CachePatchName("RANKC", PU_HUDGFX);
	rankb = W_CachePatchName("RANKB", PU_HUDGFX);
	ranka = W_CachePatchName("RANKA", PU_HUDGFX);
	ranks = W_CachePatchName("RANKS", PU_HUDGFX);
	rankx = W_CachePatchName("RANKX", PU_HUDGFX);
	mushroom = W_CachePatchName("MSRMA0", PU_HUDGFX);
	rinvincibility = W_CachePatchName("RINVB0", PU_HUDGFX);
	ringshield2 = W_CachePatchName("YLBXB0", PU_HUDGFX);
	blankhd = W_CachePatchName("BLNKA0", PU_HUDGFX);
	star = W_CachePatchName("MSTRA0", PU_HUDGFX);
	shell = W_CachePatchName("SHELA0", PU_HUDGFX);
	tshell = W_CachePatchName("TSHLA0", PU_HUDGFX);
	tshell1 = W_CachePatchName("TSHLD0", PU_HUDGFX);
	tshell2 = W_CachePatchName("TSHLC0", PU_HUDGFX);
	tshell3 = W_CachePatchName("TSHLB0", PU_HUDGFX);
	redshell = W_CachePatchName("RSHLA0", PU_HUDGFX);
	tredshell = W_CachePatchName("TRSHA0", PU_HUDGFX);
	tredshell1 = W_CachePatchName("TRSHD0", PU_HUDGFX);
	tredshell2 = W_CachePatchName("TRSHC0", PU_HUDGFX);
	tredshell3 = W_CachePatchName("TRSHB0", PU_HUDGFX);
	peel = W_CachePatchName("NANAA0", PU_HUDGFX);
	tpeel = W_CachePatchName("TNANA0", PU_HUDGFX);
	tpeel1 = W_CachePatchName("TNAND0", PU_HUDGFX);
	tpeel2 = W_CachePatchName("TNANC0", PU_HUDGFX);
	tpeel3 = W_CachePatchName("TNANB0", PU_HUDGFX);
	tboo = W_CachePatchName("IBOOA0", PU_HUDGFX);
	wboo = W_CachePatchName("WBOOA0", PU_HUDGFX);
	pboo = W_CachePatchName("PBOOA0", PU_HUDGFX);
	gboo = W_CachePatchName("GBOOA0", PU_HUDGFX);
	ksink = W_CachePatchName("SINKA0", PU_HUDGFX);
	fake = W_CachePatchName("FAKEA0", PU_HUDGFX);
	bomb = W_CachePatchName("BOOMA0", PU_HUDGFX);
	tbomb = W_CachePatchName("TBOMA0", PU_HUDGFX);
	tbomb1 = W_CachePatchName("BOOMD0", PU_HUDGFX);
	tbomb2 = W_CachePatchName("BOOMC0", PU_HUDGFX);
	tbomb3 = W_CachePatchName("BOOMB0", PU_HUDGFX);
	bshell = W_CachePatchName("BLSHA0", PU_HUDGFX);
	tbshell = W_CachePatchName("TBSHA0", PU_HUDGFX);
	tbshell1 = W_CachePatchName("BLSHD0", PU_HUDGFX);
	tbshell2 = W_CachePatchName("BLSHC0", PU_HUDGFX);
	tbshell3 = W_CachePatchName("BLSHB0", PU_HUDGFX);
	thunder = W_CachePatchName("THUNA0", PU_HUDGFX);
	bluethunder = W_CachePatchName("THNBA0", PU_HUDGFX);
	megamushroom = W_CachePatchName("MMSHA0", PU_HUDGFX);
	triplemushroom = W_CachePatchName("MSRTA0", PU_HUDGFX);
	doublemushroom = W_CachePatchName("MSRDA0", PU_HUDGFX);
	goldmushroom = W_CachePatchName("GMSHA0", PU_HUDGFX);
	fireflower = W_CachePatchName("FBLLA0", PU_HUDGFX);
	////
	iunknown = W_CachePatchName("IUSZA0", PU_HUDGFX);
	ijumpshield = W_CachePatchName("IJSZA0", PU_HUDGFX);
	iforceshield = W_CachePatchName("IFSZA0", PU_HUDGFX);
	iwatershield = W_CachePatchName("IWSZA0", PU_HUDGFX);
	ibombshield = W_CachePatchName("IBSZA0", PU_HUDGFX);
	iringshield = W_CachePatchName("IRSZA0", PU_HUDGFX);
	//
	ijumpshield2 = W_CachePatchName("IJSZB0", PU_HUDGFX);
	iforceshield2 = W_CachePatchName("IFSZB0", PU_HUDGFX);
	iwatershield2 = W_CachePatchName("IWSZB0", PU_HUDGFX);
	ibombshield2 = W_CachePatchName("IBSZB0", PU_HUDGFX);
	iringshield2 = W_CachePatchName("IRSZB0", PU_HUDGFX);
	//
	ijumpshield3 = W_CachePatchName("IJSZC0", PU_HUDGFX);
	iforceshield3 = W_CachePatchName("IFSZC0", PU_HUDGFX);
	iwatershield3 = W_CachePatchName("IWSZC0", PU_HUDGFX);
	ibombshield3 = W_CachePatchName("IBSZC0", PU_HUDGFX);
	iringshield3 = W_CachePatchName("IRSZC0", PU_HUDGFX);
	////
	needle1 = W_CachePatchName("NEEDLE1", PU_HUDGFX);
	needle2 = W_CachePatchName("NEEDLE2", PU_HUDGFX);
	needle3 = W_CachePatchName("NEEDLE3", PU_HUDGFX);
	needle4 = W_CachePatchName("NEEDLE4", PU_HUDGFX);
	needle5 = W_CachePatchName("NEEDLE5", PU_HUDGFX);
	needle6 = W_CachePatchName("NEEDLE6", PU_HUDGFX);
	needle7 = W_CachePatchName("NEEDLE7", PU_HUDGFX);
	needle8 = W_CachePatchName("NEEDLE8", PU_HUDGFX);
	needle9 = W_CachePatchName("NEEDLE9", PU_HUDGFX);
	needle10 = W_CachePatchName("NEEDLE10", PU_HUDGFX);
	needle11 = W_CachePatchName("NEEDLE11", PU_HUDGFX);
	needle12 = W_CachePatchName("NEEDLE12", PU_HUDGFX);
	needle13 = W_CachePatchName("NEEDLE13", PU_HUDGFX);
	needle14 = W_CachePatchName("NEEDLE14", PU_HUDGFX);
	needle15 = W_CachePatchName("NEEDLE15", PU_HUDGFX);
	needle16 = W_CachePatchName("NEEDLE16", PU_HUDGFX);
	needle17 = W_CachePatchName("NEEDLE17", PU_HUDGFX);
	needle18 = W_CachePatchName("NEEDLE18", PU_HUDGFX);
	needle19 = W_CachePatchName("NEEDLE19", PU_HUDGFX);
	needle20 = W_CachePatchName("NEEDLE20", PU_HUDGFX);
	needle21 = W_CachePatchName("NEEDLE21", PU_HUDGFX);
	needle22 = W_CachePatchName("NEEDLE22", PU_HUDGFX);
	needle23 = W_CachePatchName("NEEDLE23", PU_HUDGFX);
	needle24 = W_CachePatchName("NEEDLE24", PU_HUDGFX);
	needle25 = W_CachePatchName("NEEDLE25", PU_HUDGFX);
	////
	itemused9 = W_CachePatchName("ITUSA0", PU_HUDGFX);
	itemused8 = W_CachePatchName("ITUSB0", PU_HUDGFX);
	itemused7 = W_CachePatchName("ITUSC0", PU_HUDGFX);
	itemused6 = W_CachePatchName("ITUSD0", PU_HUDGFX);
	itemused5 = W_CachePatchName("ITUSE0", PU_HUDGFX);
	itemused4 = W_CachePatchName("ITUSF0", PU_HUDGFX);
	itemused3 = W_CachePatchName("ITUSG0", PU_HUDGFX);
	itemused2 = W_CachePatchName("ITUSH0", PU_HUDGFX);
	itemused1 = W_CachePatchName("ITUSI0", PU_HUDGFX);
	//
	shieldused6 = W_CachePatchName("SHUSA0", PU_HUDGFX);
	shieldused5 = W_CachePatchName("SHUSB0", PU_HUDGFX);
	shieldused4 = W_CachePatchName("SHUSC0", PU_HUDGFX);
	shieldused3 = W_CachePatchName("SHUSD0", PU_HUDGFX);
	shieldused2 = W_CachePatchName("SHUSE0", PU_HUDGFX);
	shieldused1 = W_CachePatchName("SHUSF0", PU_HUDGFX);
	////
#endif
}

// made separate so that skins code can reload custom face graphics
// Graue 04-07-2004: index by skins
void ST_LoadFaceGraphics(char *facestr, char *superstr, INT32 skinnum)
{
	char namelump[9];

	// hack: make sure base face name is no more than 8 chars
	if (strlen(facestr) > 8)
		facestr[8] = '\0';
	strcpy(namelump, facestr); // copy base name

	faceprefix[skinnum] = W_CachePatchName(namelump, PU_HUDGFX);

	if (strlen(superstr) > 8)
		superstr[8] = '\0';
	strcpy(namelump, superstr); // copy base name

	superprefix[skinnum] = W_CachePatchName(namelump, PU_HUDGFX);

	facefreed[skinnum] = false;
}

#ifdef RIDERS
void ST_LoadIconGraphics(char *facestr, INT32 skinnum)
{
	char namelump[9];

	// hack: make sure base face name is no more than 8 chars
	if (strlen(facestr) > 8)
		facestr[8] = '\0';
	strcpy(namelump, facestr); // copy base name

	iconprefix[skinnum] = W_CachePatchName(namelump, PU_HUDGFX);
	iconfreed[skinnum] = false;
}

#if 0 //unused
static void ST_UnLoadIconGraphics(INT32 skinnum)
{
	Z_Free(iconprefix[skinnum]);
	iconfreed[skinnum] = true;
}
#endif
#endif

void ST_UnLoadFaceGraphics(INT32 skinnum)
{
	Z_Free(faceprefix[skinnum]);
	Z_Free(superprefix[skinnum]);
	facefreed[skinnum] = true;
}

// Tails 03-15-2002
// made separate so that skins code can reload custom face graphics
// Graue 04-07-2004: index by skins
void ST_LoadFaceNameGraphics(char *facestr, INT32 skinnum)
{
	char namelump[9];

	// hack: make sure base face name is no more than 8 chars
	if (strlen(facestr) > 8)
		facestr[8] = '\0';
	strcpy(namelump, facestr); // copy base name

	facenameprefix[skinnum] = W_CachePatchName(namelump, PU_HUDGFX);
	prefixfreed[skinnum] = false;
}

void ST_UnLoadFaceNameGraphics(INT32 skinnum)
{
	Z_Free(facenameprefix[skinnum]);
	prefixfreed[skinnum] = true;

}

void ST_ReloadSkinFaceGraphics(void)
{
	INT32 i;

	for (i = 0; i < numskins; i++)
	{
		ST_LoadFaceGraphics(skins[i].faceprefix, skins[i].superprefix, i);
		ST_LoadFaceNameGraphics(skins[i].nameprefix, i);
	}
}

static inline void ST_InitData(void)
{
	// 'link' the statusbar display to a player, which could be
	// another player than consoleplayer, for example, when you
	// change the view in a multiplayer demo with F12.
	stplyr = &players[displayplayer];

	st_palette = -1;
}

static void ST_Stop(void)
{
	if (st_stopped)
		return;

	V_SetPalette(0);

	st_stopped = true;
}

void ST_Start(void)
{
	if (!st_stopped)
		ST_Stop();

	ST_InitData();
	st_stopped = false;
}

//
// Initializes the status bar, sets the defaults border patch for the window borders.
//

// used by OpenGL mode, holds lumpnum of flat used to fill space around the viewwindow
lumpnum_t st_borderpatchnum;

void ST_Init(void)
{
	INT32 i;

	for (i = 0; i < MAXPLAYERS; i++)
	{
		facefreed[i] = true;
		prefixfreed[i] = true;
	}

	if (dedicated)
		return;

	ST_LoadGraphics();
}

// change the status bar too, when pressing F12 while viewing a demo.
void ST_changeDemoView(void)
{
	// the same routine is called at multiplayer deathmatch spawn
	// so it can be called multiple times
	ST_Start();
}

// =========================================================================
//                         STATUS BAR OVERLAY
// =========================================================================

boolean st_overlay;

static INT32 SCY(INT32 y)
{
	//31/10/99: fixed by Hurdler so it _works_ also in hardware mode
	// do not scale to resolution for hardware accelerated
	// because these modes always scale by default
	y = (INT32)(y * vid.fdupy); // scale to resolution
	if (splitscreen)
	{
		y >>= 1;
		if (stplyr != &players[displayplayer])
			y += vid.height / 2;
	}
	return y;
}

static INT32 STRINGY(INT32 y)
{
	//31/10/99: fixed by Hurdler so it _works_ also in hardware mode
	// do not scale to resolution for hardware accelerated
	// because these modes always scale by default
	if (splitscreen)
	{
		y >>= 1;
		if (stplyr != &players[displayplayer])
			y += BASEVIDHEIGHT / 2;
	}
	return y;
}

static INT32 SCX(INT32 x)
{
	return (INT32)(x * vid.fdupx);
}

// Draw a number, scaled, over the view
// Always draw the number completely since it's overlay
//
void ST_DrawOverlayNum(INT32 x /* right border */, INT32 y, INT32 num,
	patch_t **numpat)
{
	INT32 w = SHORT(numpat[0]->width);
	boolean neg;

	// special case for 0
	if (!num)
	{
		V_DrawScaledPatch(x - (w*vid.dupx), y, V_NOSCALESTART|V_TRANSLUCENT, numpat[0]);
		return;
	}

	neg = num < 0;

	if (neg)
		num = -num;

	// draw the number
	while (num)
	{
		x -= (w * vid.dupx);
		V_DrawScaledPatch(x, y, V_NOSCALESTART|V_TRANSLUCENT, numpat[num % 10]);
		num /= 10;
	}

	// draw a minus sign if necessary
	if (neg)
		V_DrawScaledPatch(x - (8*vid.dupx), y, V_NOSCALESTART|V_TRANSLUCENT,
			minus); // Tails
}

// Draw a number, scaled, over the view
// Always draw the number completely since it's overlay
//
// Supports different colors! woo!
static void ST_DrawNightsOverlayNum(INT32 x /* right border */, INT32 y, INT32 num,
	patch_t **numpat, INT32 colornum)
{
	INT32 w = SHORT(numpat[0]->width);
	const UINT8 *colormap;

	if (colornum == 0)
		colormap = colormaps;
	else
	{
		// Uses the player colors.
		colormap = (UINT8 *)defaulttranslationtables - 256 + (colornum<<8);
	}

	// special case for 0
	if (!num)
	{
		V_DrawMappedPatch(x - (w*vid.dupx), y, V_NOSCALESTART|V_TRANSLUCENT, numpat[0], colormap);
		return;
	}

	I_Assert(num >= 0); // this function does not draw negative numbers

	// draw the number
	while (num)
	{
		x -= (w * vid.dupx);
		V_DrawMappedPatch(x, y, V_NOSCALESTART|V_TRANSLUCENT, numpat[num % 10], colormap);
		num /= 10;
	}

	// Sorry chum, this function only draws UNSIGNED values!
}

static void ST_drawDebugInfo(void)
{
	char smomx[33];
	char smomy[33];
	char smomz[33];
	char sspeed[33];
	char sfloorz[33];
	char spmomz[33];
	char scability[33];
	char scability2[33];
	char scharsped[33];
	char scharflags[33];
	char sstrcolor[33];
	char sdedtimer[33];
	char sjumpfact[33];
	char sx[33];
	char sy[33];
	char sz[33];
	char sangle[33];
	char sunderwater[33];
	char smfjumped[33];
	char smfspinning[33];
	char smfstartdash[33];
	char sjumping[33];
	char sscoreadd[33];

	if (!stplyr->mo)
		return;

	sprintf(smomx, "%d", stplyr->rmomx>>FRACBITS);
	sprintf(smomy, "%d", stplyr->rmomy>>FRACBITS);
	sprintf(smomz, "%d", stplyr->mo->momz>>FRACBITS);
	sprintf(sspeed, "%d", stplyr->speed);
	sprintf(sfloorz, "%d", stplyr->mo->floorz>>FRACBITS);
	sprintf(spmomz, "%d", stplyr->mo->ceilingz>>FRACBITS);
	sprintf(scability, "%d", stplyr->charability);
	sprintf(scability2, "%d", stplyr->charability2);
	sprintf(scharsped, "%d", stplyr->normalspeed);
	sprintf(scharflags, "%d", stplyr->charflags);
#ifdef TRANSFIX
	sprintf(sstrcolor, "%d", atoi(skins[stplyr->skin].starttranscolor));
#else
	sprintf(sstrcolor, "%d", stplyr->starttranscolor);
#endif
	sprintf(sdedtimer, "%d", stplyr->deadtimer);
	sprintf(sjumpfact, "%d", stplyr->jumpfactor);
	sprintf(sx, "%d", stplyr->mo->x>>FRACBITS);
	sprintf(sy, "%d", stplyr->mo->y>>FRACBITS);
	sprintf(sz, "%d", stplyr->mo->z>>FRACBITS);
	sprintf(sangle, "%d", stplyr->mo->angle>>FRACBITS);
	sprintf(sunderwater, "%d", stplyr->powers[pw_underwater]);
	sprintf(smfjumped, "%u", (stplyr->pflags & PF_JUMPED));
	sprintf(smfspinning, "%u", (stplyr->pflags & PF_SPINNING));
	sprintf(smfstartdash, "%u", (stplyr->pflags & PF_STARTDASH));
	sprintf(sjumping, "%d", stplyr->jumping);
	sprintf(sscoreadd, "%d", stplyr->scoreadd);
	V_DrawString(248, 0, 0, "MOMX =");
	V_DrawString(296, 0, 0, smomx);
	V_DrawString(248, 8, 0, "MOMY =");
	V_DrawString(296, 8, 0, smomy);
	V_DrawString(248, 16, 0, "MOMZ =");
	V_DrawString(296, 16, 0, smomz);
	V_DrawString(240, 24, 0, "SPEED =");
	V_DrawString(296, 24, 0, sspeed);
	V_DrawString(232, 32, 0, "FLOORZ=");
	V_DrawString(288, 32, 0, sfloorz);
	V_DrawString(240, 40, 0, "CEILZ =");
	V_DrawString(296, 40, 0, spmomz);
	V_DrawString(216, 48, 0, "CA =");
	V_DrawString(248, 48, 0, scability);
	V_DrawString(264, 48, 0, "CA2 =");
	V_DrawString(304, 48, 0, scability2);
	V_DrawString(216, 56, 0, "CHARSPED =");
	V_DrawString(296, 56, 0, scharsped);
	V_DrawString(216, 64, 0, "CHARFLGS =");
	V_DrawString(296, 64, 0, scharflags);
	V_DrawString(216, 72, 0, "STRCOLOR =");
	V_DrawString(296, 72, 0, sstrcolor);
	V_DrawString(216, 88, 0, "DEDTIMER =");
	V_DrawString(296, 88, 0, sdedtimer);
	V_DrawString(216, 96, 0, "JUMPFACT =");
	V_DrawString(296, 96, 0, sjumpfact);
	V_DrawString(240, 104, 0, "X =");
	V_DrawString(264, 104, 0, sx);
	V_DrawString(240, 112, 0, "Y =");
	V_DrawString(264, 112, 0, sy);
	V_DrawString(240, 120, 0, "Z =");
	V_DrawString(264, 120, 0, sz);
	V_DrawString(216, 128, 0, "Angle =");
	V_DrawString(272, 128, 0, sangle);
	V_DrawString(192, 152, 0, "Underwater =");
	V_DrawString(288, 152, 0, sunderwater);
	V_DrawString(192, 160, 0, "MF_JUMPED =");
	V_DrawString(288, 160, 0, smfjumped);
	V_DrawString(192, 168, 0, "MF_SPINNING =");
	V_DrawString(296, 168, 0, smfspinning);
	V_DrawString(192, 176, 0, "MF_STARDASH =");
	V_DrawString(296, 176, 0, smfstartdash);
	V_DrawString(192, 184, 0, "Jumping =");
	V_DrawString(288, 184, 0, sjumping);
	V_DrawString(192, 192, 0, "Scoreadd =");
	V_DrawString(288, 192, 0, sscoreadd);
}

static void ST_drawLevelTitle(void)
{
	char *lvlttl = mapheaderinfo[gamemap-1].lvlttl;
	char *subttl = mapheaderinfo[gamemap-1].subttl;
	INT32 lvlttlxpos;
	INT32 subttlxpos = BASEVIDWIDTH/2;
	INT32 ttlnumxpos;
	INT32 zonexpos;
	INT32 actnum = mapheaderinfo[gamemap-1].actnum;
	boolean nonumber = false;

	if (!(timeinmap > 1 && timeinmap < 111))
		return;

	if (actnum > 0)
	{
		ttlnum = W_CachePatchName(va("TTL%.2d", actnum), PU_CACHE);
		lvlttlxpos = ((BASEVIDWIDTH/2) - (V_LevelNameWidth(lvlttl)/2)) - SHORT(ttlnum->width);
	}
	else
	{
		nonumber = true;
		lvlttlxpos = ((BASEVIDWIDTH/2) - (V_LevelNameWidth(lvlttl)/2));
	}

	ttlnumxpos = lvlttlxpos + V_LevelNameWidth(lvlttl);
	zonexpos = ttlnumxpos - V_LevelNameWidth(text[ZONE]);

	if (lvlttlxpos < 0)
		lvlttlxpos = 0;

	if (timeinmap == 2)
	{
		if (!nonumber)
			V_DrawScaledPatch(SCX(ttlnumxpos), (INT32)(200*vid.fdupy), V_NOSCALESTART, ttlnum);
		V_DrawLevelTitle(lvlttlxpos, 0, 0, lvlttl);

		if (!mapheaderinfo[gamemap-1].nozone)
			V_DrawLevelTitle(zonexpos, 200, 0, text[ZONE]);

		V_DrawCenteredString(subttlxpos, 0+48, 0, subttl);
	}
	else if (timeinmap == 3)
	{
		if (!nonumber)
			V_DrawScaledPatch(SCX(ttlnumxpos), (INT32)(188*vid.fdupy), V_NOSCALESTART, ttlnum);
		V_DrawLevelTitle(lvlttlxpos, 12, 0, lvlttl);

		if (!mapheaderinfo[gamemap-1].nozone)
			V_DrawLevelTitle(zonexpos, 188, 0, text[ZONE]);

		V_DrawCenteredString(subttlxpos, 12+48, 0, subttl);
	}
	else if (timeinmap == 4)
	{
		if (!nonumber)
			V_DrawScaledPatch(SCX(0), (INT32)(176*vid.fdupy), V_NOSCALESTART, ttlnum);
		V_DrawLevelTitle(lvlttlxpos, 24, 0, lvlttl);

		if (!mapheaderinfo[gamemap-1].nozone)
			V_DrawLevelTitle(zonexpos, 176, 0, text[ZONE]);

		V_DrawCenteredString(subttlxpos, 24+48, 0, subttl);
	}
	else if (timeinmap == 5)
	{
		if (!nonumber)
			V_DrawScaledPatch(SCX(ttlnumxpos), (INT32)(164*vid.fdupy), V_NOSCALESTART, ttlnum);
		V_DrawLevelTitle(lvlttlxpos, 36, 0, lvlttl);

		if (!mapheaderinfo[gamemap-1].nozone)
			V_DrawLevelTitle(zonexpos, 164, 0, text[ZONE]);

		V_DrawCenteredString(subttlxpos, 36+48, 0, subttl);
	}
	else if (timeinmap == 6)
	{
		if (!nonumber)
			V_DrawScaledPatch(SCX(ttlnumxpos), (INT32)(152*vid.fdupy), V_NOSCALESTART, ttlnum);
		V_DrawLevelTitle(lvlttlxpos, 48, 0, lvlttl);

		if (!mapheaderinfo[gamemap-1].nozone)
			V_DrawLevelTitle(zonexpos, 152, 0, text[ZONE]);

		V_DrawCenteredString(subttlxpos, 48+48, 0, subttl);
	}
	else if (timeinmap == 7)
	{
		if (!nonumber)
			V_DrawScaledPatch(SCX(ttlnumxpos), (INT32)(140*vid.fdupy), V_NOSCALESTART, ttlnum);
		V_DrawLevelTitle(lvlttlxpos, 60, 0, lvlttl);

		if (!mapheaderinfo[gamemap-1].nozone)
			V_DrawLevelTitle(zonexpos, 140, 0, text[ZONE]);

		V_DrawCenteredString(subttlxpos, 60+48, 0, subttl);
	}
	else if (timeinmap == 8)
	{
		if (!nonumber)
			V_DrawScaledPatch(SCX(ttlnumxpos), (INT32)(128*vid.fdupy), V_NOSCALESTART, ttlnum);
		V_DrawLevelTitle(lvlttlxpos, 72, 0, lvlttl);

		if (!mapheaderinfo[gamemap-1].nozone)
			V_DrawLevelTitle(zonexpos, 128, 0, text[ZONE]);

		V_DrawCenteredString(subttlxpos, 72+48, 0, subttl);
	}
	else if (timeinmap == 106)
	{
		if (!nonumber)
			V_DrawScaledPatch(SCX(ttlnumxpos), (INT32)(80*vid.fdupy), V_NOSCALESTART, ttlnum);
		V_DrawLevelTitle(lvlttlxpos, 104, 0, lvlttl);

		if (!mapheaderinfo[gamemap-1].nozone)
			V_DrawLevelTitle(zonexpos, 80, 0, text[ZONE]);

		V_DrawCenteredString(subttlxpos, 104+48, 0, subttl);
	}
	else if (timeinmap == 107)
	{
		if (!nonumber)
			V_DrawScaledPatch(SCX(ttlnumxpos), (INT32)(56*vid.fdupy), V_NOSCALESTART, ttlnum);
		V_DrawLevelTitle(lvlttlxpos, 128, 0, lvlttl);

		if (!mapheaderinfo[gamemap-1].nozone)
			V_DrawLevelTitle(zonexpos, 56, 0, text[ZONE]);

		V_DrawCenteredString(subttlxpos, 128+48, 0, subttl);
	}
	else if (timeinmap == 108)
	{
		if (!nonumber)
			V_DrawScaledPatch(SCX(ttlnumxpos), (INT32)(32*vid.fdupy), V_NOSCALESTART, ttlnum);
		V_DrawLevelTitle(lvlttlxpos, 152, 0, lvlttl);

		if (!mapheaderinfo[gamemap-1].nozone)
			V_DrawLevelTitle(zonexpos, 32, 0, text[ZONE]);

//		V_DrawCenteredString(subttlxpos, 152+48, 0, subttl);
	}
	else if (timeinmap == 109)
	{
		if (!nonumber)
			V_DrawScaledPatch(SCX(ttlnumxpos), (INT32)(8*vid.fdupy), V_NOSCALESTART, ttlnum);
		V_DrawLevelTitle(lvlttlxpos, 176, 0, lvlttl);

		if (!mapheaderinfo[gamemap-1].nozone)
			V_DrawLevelTitle(zonexpos, 8, 0, text[ZONE]);

		//V_DrawCenteredString(subttlxpos, 176+48, 0, subttl);
	}
	else if (timeinmap == 110)
	{
		if (!nonumber)
			V_DrawScaledPatch(SCX(ttlnumxpos), (INT32)(0*vid.fdupy), V_NOSCALESTART, ttlnum);
		V_DrawLevelTitle(lvlttlxpos, 200, 0, lvlttl);

		if (!mapheaderinfo[gamemap-1].nozone)
			V_DrawLevelTitle(zonexpos, 0, 0, text[ZONE]);

		//V_DrawCenteredString(subttlxpos, 200+48, 0, subttl);
	}
	else
	{
		if (!nonumber)
			V_DrawScaledPatch(SCX(ttlnumxpos), (INT32)(104*vid.fdupy), V_NOSCALESTART, ttlnum);
		V_DrawLevelTitle(lvlttlxpos, 80, 0, lvlttl);

		if (!mapheaderinfo[gamemap-1].nozone)
			V_DrawLevelTitle(zonexpos, 104, 0, text[ZONE]);

		V_DrawCenteredString(subttlxpos, 80+48, 0, subttl);
	}
#undef ZONE
}

static void ST_drawFirstPersonHUD(void)
{
	player_t *player = stplyr;
	patch_t *p = NULL;

	/// \todo you wanna do something about those countdown drown numbers?

	// Graue 06-18-2004: no V_NOSCALESTART, no SCX, no SCY, snap to right

	if (player->powers[pw_jumpshield] || (player->powers[pw_zjumpshield] && (leveltime & 1)))
		V_DrawScaledPatch(225, STRINGY(156), V_SNAPTORIGHT|V_TRANSLUCENT, jumpshield);
	else if (player->powers[pw_forceshield] == 2 || (player->powers[pw_forceshield] == 1 && (leveltime & 1)))
		V_DrawScaledPatch(225, STRINGY(156), V_SNAPTORIGHT|V_TRANSLUCENT, forceshield);
	else if (player->powers[pw_watershield] || (player->powers[pw_zwatershield] && (leveltime & 1)))
		V_DrawScaledPatch(225, STRINGY(156), V_SNAPTORIGHT|V_TRANSLUCENT, watershield);
	else if (player->powers[pw_bombshield] || (player->powers[pw_zbombshield] && (leveltime & 1)))
		V_DrawScaledPatch(225, STRINGY(156), V_SNAPTORIGHT|V_TRANSLUCENT, bombshield);
	else if (player->powers[pw_zringshield] || (player->powers[pw_ringshield] && (leveltime & 1)))
		V_DrawScaledPatch(225, STRINGY(156), V_SNAPTORIGHT|V_TRANSLUCENT, ringshield);
	
	// Flashing is unneeded!

	// Mushroom Boost //
	if (player->playerstate != PST_DEAD 
		&& (player->powers[pw_mushroom] >= 3*TICRATE
		|| (player->powers[pw_mushroom]
					&& leveltime & 1)))														// Flicker
		V_DrawScaledPatch(245, STRINGY(156), V_SNAPTORIGHT|V_TRANSLUCENT, zmushroom);

	// Invincibility (Star power!) //
	if (player->playerstate != PST_DEAD 
		&& (player->powers[pw_invulnerability] >= 3*TICRATE
		|| (player->powers[pw_invulnerability]
					&& leveltime & 1)))														// Flicker
		V_DrawScaledPatch(265, STRINGY(156), V_SNAPTORIGHT|V_TRANSLUCENT, starmaning);

	// Painstate //
	if (player->playerstate != PST_DEAD 
		&& (player->powers[pw_flashing]
					&& leveltime & 1))														// Flicker
		V_DrawScaledPatch(265, STRINGY(156), V_SNAPTORIGHT|V_TRANSLUCENT, invincibility);

	// MegaMushroom Power //
	if (player->playerstate != PST_DEAD 
		&& (player->powers[pw_shrink] <= -3*TICRATE
		|| ((player->powers[pw_shrink] > -3*TICRATE && player->powers[pw_shrink] < -1)	//Running out?
					&& leveltime & 1)))														// Flicker
		V_DrawScaledPatch(285, STRINGY(156), V_SNAPTORIGHT|V_TRANSLUCENT, megashroom);

	// Shrunk! //
	if (player->playerstate != PST_DEAD 
		&& (player->powers[pw_shrink] >= 3*TICRATE
		|| ((player->powers[pw_shrink] < 3*TICRATE && player->powers[pw_shrink] > 1)	//Running out?
					&& leveltime & 1)))														// Flicker
		V_DrawScaledPatch(285, STRINGY(156), V_SNAPTORIGHT|V_TRANSLUCENT, minishroom);

	// Drift Sparks (Minor) //
	if (player->playerstate != PST_DEAD 
		&& ((player->powers[pw_driftcharge] >= 40 && player->powers[pw_driftcharge] < 80)))
		V_DrawScaledPatch(305, STRINGY(156), V_SNAPTORIGHT|V_TRANSLUCENT, dsparks);

	// Drift Sparks (Major) //
	if (player->playerstate != PST_DEAD 
		&& (player->powers[pw_driftcharge] >= 80))
		V_DrawScaledPatch(305, STRINGY(156), V_SNAPTORIGHT|V_TRANSLUCENT, ddsparks);

	// Sneakers (Drift Boost!) //
	if (player->playerstate != PST_DEAD 
		&& (player->powers[pw_sneakers] >= 3*TICRATE
		|| (player->powers[pw_sneakers] 
					&& leveltime & 1)))														// Flicker
		V_DrawScaledPatch(305, STRINGY(156), V_SNAPTORIGHT|V_TRANSLUCENT, sneakers);

	// Display the countdown drown numbers!
	if ((player->powers[pw_underwater] <= 11*TICRATE + 1
		&& player->powers[pw_underwater] >= 10*TICRATE + 1)
		|| (player->powers[pw_spacetime] <= 11*TICRATE + 1
		&& player->powers[pw_spacetime] >= 10*TICRATE + 1))
	{
		p = count5;
	}
	else if ((player->powers[pw_underwater] <= 9*TICRATE + 1
		&& player->powers[pw_underwater] >= 8*TICRATE + 1)
		|| (player->powers[pw_spacetime] <= 9*TICRATE + 1
		&& player->powers[pw_spacetime] >= 8*TICRATE + 1))
	{
		p = count4;
	}
	else if ((player->powers[pw_underwater] <= 7*TICRATE + 1
		&& player->powers[pw_underwater] >= 6*TICRATE + 1)
		|| (player->powers[pw_spacetime] <= 7*TICRATE + 1
		&& player->powers[pw_spacetime] >= 6*TICRATE + 1))
	{
		p = count3;
	}
	else if ((player->powers[pw_underwater] <= 5*TICRATE + 1
		&& player->powers[pw_underwater] >= 4*TICRATE + 1)
		|| (player->powers[pw_spacetime] <= 5*TICRATE + 1
		&& player->powers[pw_spacetime] >= 4*TICRATE + 1))
	{
		p = count2;
	}
	else if ((player->powers[pw_underwater] <= 3*TICRATE + 1
		&& player->powers[pw_underwater] >= 2*TICRATE + 1)
		|| (player->powers[pw_spacetime] <= 3*TICRATE + 1
		&& player->powers[pw_spacetime] >= 2*TICRATE + 1))
	{
		p = count1;
	}
	else if ((player->powers[pw_underwater] <= 1*TICRATE + 1
		&& player->powers[pw_underwater] > 1)
		|| (player->powers[pw_spacetime] <= 1*TICRATE + 1
		&& player->powers[pw_spacetime] > 1))
	{
		p = count0;
	}

	if (p)
		V_DrawTranslucentPatch(SCX((BASEVIDWIDTH/2) - (SHORT(p->width)/2) + SHORT(p->leftoffset)), SCY(120 - SHORT(p->topoffset)),
			V_NOSCALESTART, p);
}

#ifdef RIDERS
static void ST_drawCheckPointTimes(void)
{
	//todo:this doesn't belong here
	if (!kartmode && stplyr->starposttime && !stplyr->starpostnum && stplyr->starposttime+(TICRATE*2) >= stplyr->realtime
		&& timeinmap > (TICRATE*2)+1 && !stplyr->exiting && stplyr->laps)
	{
		char tmpstring[10];
		INT32 xpos;
		INT32 plyrlaps = stplyr->laps+1;

		if (stplyr->laps == (unsigned)(cv_numlaps.value - 1))
			sprintf(tmpstring,"Final Lap");
		else
			sprintf(tmpstring,"Lap %d",plyrlaps);

		if (stplyr->realtime-stplyr->starposttime < 8)
			xpos = (stplyr->realtime-stplyr->starposttime)*20;
		else if (stplyr->realtime-stplyr->starposttime > (TICRATE*2)-8)
			xpos = 160+(((stplyr->realtime-stplyr->starposttime)-((TICRATE*2)-8))*20);
		else
			xpos = 160;

		V_DrawCenteredString(xpos, 52, 0, tmpstring);
	}
	else if (!kartmode && stplyr->exiting && timeinmap > (TICRATE*2)+1)
	{
		INT32 xpos;

		if ((leveltime-4*TICRATE)-stplyr->realtime < 8)
			xpos = ((leveltime-4*TICRATE)-stplyr->realtime)*20;
		else
			xpos = 160;

		V_DrawCenteredString(xpos, 52, V_YELLOWMAP, "Finished!");
	}

	//normal checkpoint times
	if (stplyr->starposttime && stplyr->starposttime+(TICRATE*2) >= stplyr->realtime)
	{
		tic_t startime = stplyr->starposttime;

		V_DrawCenteredString(BASEVIDWIDTH/2, 160, ((leveltime%2) ? V_TRANSLUCENT : 0),
		                     va("%d:%02d.%02d", G_TicsToMinutes(startime, true), G_TicsToSeconds(startime), G_TicsToCentiseconds(startime)));
	}

	//per-lap checkpoint times
	if (circuitmap && stplyr->laps && ((stplyr->starpostnum - 1) + (numstarposts+1)*stplyr->laps) > (unsigned)numstarposts
		&& ((stplyr->starpostnum - 1) + (numstarposts+1)*stplyr->laps) < 256 //SIGSEGV prevention
		&& stplyr->starposttime+(TICRATE*2) >= stplyr->realtime)
	{
		INT32 postnum = ((numstarposts+1)*(stplyr->laps - (stplyr->starpostnum ? 0 : 1))) - 1;
		tic_t laptime = stplyr->starposttime - stplyr->checkpointtimes[postnum];

		V_DrawCenteredString(BASEVIDWIDTH/2, 152, V_YELLOWMAP|((leveltime%2) ? V_TRANSLUCENT : 0),
		                     va("%d:%02d.%02d", G_TicsToMinutes(laptime, true), G_TicsToSeconds(laptime), G_TicsToCentiseconds(laptime)));
	}

	//ahead/behind times for Race gametype only
	//good frikkin' luck trying to read this, it can be a bit confusing for sure!
	//P_CheckPlayerAhead handles SIGSEGV prevention here, so no need to worry -- the values will be correct
	if (gametype == GT_RACE && stplyr->playerahead)
	{
		INT32 playerahead = stplyr->playerahead - 1;
		INT32 postnum = (stplyr->starpostnum - 1) + (numstarposts+1)*stplyr->laps;

		if (playerahead == 65535 && stplyr->starposttime+(TICRATE*2) >= stplyr->realtime) //special case, means we tied
			V_DrawCenteredString(BASEVIDWIDTH/2, 168, ((leveltime%2) ? V_TRANSLUCENT : 0), "0:00.00");

		else if (playerahead & 256) //P_CheckPlayerAhead modified our stat because the second placed man decided to finally show up
		{
			if (players[playerahead & 255].checkpointtimes[postnum]+(TICRATE*2) >= stplyr->realtime)
			{
				INT32 startime = players[playerahead & 255].checkpointtimes[postnum] - stplyr->checkpointtimes[postnum];

				V_DrawCenteredString(BASEVIDWIDTH/2, 168, ((leveltime%2) ? V_TRANSLUCENT : 0),
				                     va("\x84-%d:%02d.%02d", G_TicsToMinutes(startime, true), G_TicsToSeconds(startime), G_TicsToCentiseconds(startime)));
			}
		}

		else //uh oh, WE'RE the one who's behind!
		{
			if (stplyr->starposttime+(TICRATE*2) >= stplyr->realtime)
			{
				INT32 startime = stplyr->checkpointtimes[postnum] - players[playerahead].checkpointtimes[postnum];

				V_DrawCenteredString(BASEVIDWIDTH/2, 168, ((leveltime%2) ? V_TRANSLUCENT : 0),
				                     va("\x85+%d:%02d.%02d", G_TicsToMinutes(startime, true), G_TicsToSeconds(startime), G_TicsToCentiseconds(startime)));
			}
		}
	}
}

static void ST_drawRidersHUD(void)
{
	INT32 xspeed;
	INT32 yspeed;
	INT32 ytime;

	// Ring Patch
	if (stplyr->health < 1002)
	{
		UINT8 *colormap;

		if (stplyr->skincolor == 0)
			colormap = colormaps;
		else
		{
			// Uses the player colors.
			colormap = (UINT8 *)defaulttranslationtables - 256 + (stplyr->skincolor<<8);
		}

		if (!splitscreen)
			V_DrawMappedPatch(SCX(5), SCY(4), V_NOSCALESTART|V_TRANSLUCENT, ridrings, colormap);
		else
			V_DrawMappedPatch(SCX(5), SCY(4), V_NOSCALESTART|V_TRANSLUCENT, ridrngs2, colormap);
	}
	// NiGHTS NHUD Ring
	if (!splitscreen)
		V_DrawScaledPatch(SCX(11), (int)(SCY(7) + 7*vid.fdupy), V_NOSCALESTART|V_TRANSLUCENT, nhud[(leveltime/2)%12]);
	// Ring Counter
	if (!splitscreen)
		ST_DrawOverlayNum(SCX(52), SCY(8), stplyr->health > 0 ? stplyr->health-1 : 0, tallnum);
	else
		ST_DrawOverlayNum(SCX(36), SCY(8), stplyr->health > 0 ? stplyr->health-1 : 0, tallnum);
	// Ring Limit Patches
	if (gametype == GT_RACE)
	{
		if (stplyr->health < 31)
		{
			if (!splitscreen)
				V_DrawScaledPatch(SCX(31), SCX(20), V_NOSCALESTART|V_TRANSLUCENT, thirty);
			else
				V_DrawScaledPatch(SCX(15), SCY(34), V_NOSCALESTART|V_TRANSLUCENT, thirty);
		}
		if (stplyr->health > 30 && stplyr->health < 61)
		{
			if (!splitscreen)
				V_DrawScaledPatch(SCX(31), SCX(20), V_NOSCALESTART|V_TRANSLUCENT, sixty);
			else
				V_DrawScaledPatch(SCX(15), SCY(34), V_NOSCALESTART|V_TRANSLUCENT, sixty);
		}
	}
	// Air Tank Lv.
	if (!splitscreen)
	{
		ST_DrawOverlayNum(SCX(46), SCY(45) - (11*vid.dupy), stplyr->lives, tallnum);
		V_DrawScaledPatch(SCX(10), SCY(45) - (ridlvl->height*vid.dupy), V_NOSCALESTART|V_TRANSLUCENT, ridlvl);
	}
	else
	{
		ST_DrawOverlayNum(SCX(53), SCY(55) - (11*vid.dupy), stplyr->lives, tallnum);
		V_DrawScaledPatch(SCX(40), SCY(30) - (ridlvl->height*vid.dupy), V_NOSCALESTART|V_TRANSLUCENT, ridlvl);
	}
	// Boost Patch.
	if (stplyr->powers[pw_airtank] > 15 && (stplyr->pflags & PF_RUNNINGANIM) && !stplyr->powers[pw_boost] && leveltime/5 & 6)
		V_DrawScaledPatch(67, 17, 0, boost);


	// Time Patch
	if (!splitscreen)
		V_DrawScaledPatch(SCX(115), SCY(4), V_NOSCALESTART|V_TRANSLUCENT, ridtime);
	else
		V_DrawScaledPatch(SCX(115), SCY(4), V_NOSCALESTART|V_TRANSLUCENT, ridtime2);

	if (!splitscreen)
		ytime = 22;
	else
		ytime = 27;

	// minutes time       0 __ __
	ST_DrawOverlayNum(SCX(140), SCY(ytime),  stplyr->realtime/(60*TICRATE), tallnum);
	// colon location     _:__ __
	V_DrawScaledPatch(SCX(140), SCY(ytime), V_NOSCALESTART|V_TRANSLUCENT, sbocolon);
	// zero second        _ 0_ __
	if ((stplyr->realtime/TICRATE % 60) < 10)
		ST_DrawOverlayNum(SCX(156), SCY(ytime), 0, tallnum);
	// seconds time       _ _0 __
	ST_DrawOverlayNum(SCX(164), SCY(ytime), stplyr->realtime/TICRATE % 60, tallnum);
	// colon location 2   _ __:__
	V_DrawScaledPatch(SCX(164), SCY(ytime), V_NOSCALESTART|V_TRANSLUCENT, sbocolon);
	// zero tick          _ __ 0_
	if (G_TicsToCentiseconds(stplyr->realtime) < 10)
		ST_DrawOverlayNum(SCX(180), SCY(ytime), 0, tallnum);
	// tics               _ __ _0
	ST_DrawOverlayNum(SCX(188), SCY(ytime), G_TicsToCentiseconds(stplyr->realtime), tallnum);


	// Speed Patch
	{
		UINT8 *colormap;

		if (stplyr->skincolor == 0)
			colormap = colormaps;
		else
		{
			// Uses the player colors.
			colormap = (UINT8 *)defaulttranslationtables - 256 + (stplyr->skincolor<<8);
		}

		if (!splitscreen)
			V_DrawMappedPatch(SCX(266), SCY(73), V_NOSCALESTART|V_TRANSLUCENT, ridspeed, colormap);
		else
			V_DrawMappedPatch(SCX(266), SCY(32), V_NOSCALESTART|V_TRANSLUCENT, ridsped2, colormap);
	}
	// White Air Patch
	if (stplyr->powers[pw_airtank] > 14)
	{
		for (stplyr->chargei = 0; stplyr->chargei < stplyr->powers[pw_airtank]; stplyr->chargei++)
		{
			if (!splitscreen)
				V_DrawSmallScaledPatch(BASEVIDWIDTH-10, (STRINGY(BASEVIDHEIGHT-45) - stplyr->chargei), 0, wair);
			else
				V_DrawSmallScaledPatch(BASEVIDWIDTH-5, (STRINGY(BASEVIDHEIGHT-15) - stplyr->chargei), 0, wair);
		}
	}
	// Red Air Patch
	if (stplyr->powers[pw_airtank] < 15)
	{
		for (stplyr->chargei = 0; stplyr->chargei < stplyr->powers[pw_airtank]; stplyr->chargei++)
		{
			if (!splitscreen)
				V_DrawSmallScaledPatch(BASEVIDWIDTH-10, (STRINGY(BASEVIDHEIGHT-45) - stplyr->chargei), 0, rair);
			else
				V_DrawSmallScaledPatch(BASEVIDWIDTH-5, (STRINGY(BASEVIDHEIGHT-15) - stplyr->chargei), 0, rair);
		}
	}
	// Speed counter.
	if (!splitscreen)
	{
		xspeed = 45;
		yspeed = 32;
	}
	else
	{
		xspeed = 40;
		yspeed = 49;
	}
	if (stplyr->speed < 10 && stplyr->speed > -1)
		V_DrawString(BASEVIDWIDTH-xspeed, STRINGY(BASEVIDHEIGHT-yspeed), 0, va("%03d", stplyr->speed * 2));
	if (stplyr->speed < 99 && stplyr->speed > 9)
		V_DrawString(BASEVIDWIDTH-xspeed, STRINGY(BASEVIDHEIGHT-yspeed), 0, va("%03d", (stplyr->speed * 3) - 10));
	if (stplyr->speed > 100 || stplyr->speed < 0)
		V_DrawString(BASEVIDWIDTH-xspeed, STRINGY(BASEVIDHEIGHT-yspeed), 0, va("---"));

	if (stplyr->powers[pw_rampscore] < 49 && stplyr->powers[pw_rampscore] > 0)
	{
		V_DrawSmallScaledPatch(60, 20, 0, rank);
		V_DrawSmallScaledPatch(75, 33, 0, rankc);
	}
	if (stplyr->powers[pw_rampscore] < 99 && stplyr->powers[pw_rampscore] > 50)
	{
		V_DrawSmallScaledPatch(60, 20, 0, rank);
		V_DrawSmallScaledPatch(75, 33, 0, rankb);
	}
	if (stplyr->powers[pw_rampscore] < 149 && stplyr->powers[pw_rampscore] > 100)
	{
		V_DrawSmallScaledPatch(60, 20, 0, rank);
		V_DrawSmallScaledPatch(75, 33, 0, ranka);
	}
	if (stplyr->powers[pw_rampscore] < 199 && stplyr->powers[pw_rampscore] > 150)
	{
		V_DrawSmallScaledPatch(60, 20, 0, rank);
		V_DrawSmallScaledPatch(75, 33, 0, ranks);
	}
	if (stplyr->powers[pw_rampscore] < 249 && stplyr->powers[pw_rampscore] > 200)
	{
		V_DrawSmallScaledPatch(60, 20, 0, rank);
		V_DrawSmallScaledPatch(75, 33, 0, rankx);
	}

	if ((leveltime == (TICRATE+1)*3) || (leveltime == (TICRATE+1)*4) || (leveltime ==
		(TICRATE+1)*5) || (leveltime == (TICRATE+1)*6) || (leveltime == (TICRATE+1)*7))
		S_StartSound(NULL, sfx_boop);
	if (leveltime == (TICRATE+1)*8)
		S_StartSound(NULL, sfx_beep);
}

static void ST_drawKartHUD(void)
{
	INT32 ICONX, ICONY;
	INT32 X = 236;

	// Draw Mario Kart's icons
	if (!splitscreen && (unsigned)cv_katzshield.value >= 1)
	{
		ICONX = 145;
		ICONY = 20;
	}
	else if (!splitscreen)
	{
		ICONX = 139;
		ICONY = 20;
	}
	else
	{
		ICONX = BASEVIDWIDTH-315;
		ICONY = 60;
	}

	if (stplyr->powers[pw_psychic] == 17) //|| stplyr->powers[pw_psychic] == 18)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, itemused9);
	else if (stplyr->powers[pw_psychic] == 15) //|| stplyr->powers[pw_psychic] == 16)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, itemused8);
	else if (stplyr->powers[pw_psychic] == 13) //|| stplyr->powers[pw_psychic] == 14)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, itemused7);
	else if (stplyr->powers[pw_psychic] == 11) //|| stplyr->powers[pw_psychic] == 12)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, itemused6);
	else if (stplyr->powers[pw_psychic] == 9) //|| stplyr->powers[pw_psychic] == 10)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, itemused5);
	else if (stplyr->powers[pw_psychic] == 7) //|| stplyr->powers[pw_psychic] == 8)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, itemused4);
	else if (stplyr->powers[pw_psychic] == 5) //|| stplyr->powers[pw_psychic] == 6)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, itemused3);
	else if (stplyr->powers[pw_psychic] == 3) //|| stplyr->powers[pw_psychic] == 4)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, itemused2);
	else if (stplyr->powers[pw_psychic] == 1) //|| stplyr->powers[pw_psychic] == 2)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, itemused1);

	// Mushroom
	if (stplyr->powers[pw_starshroom] == 2 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 2))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, mushroom);
	// Star
	else if (stplyr->powers[pw_starshroom] == 1 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 5))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, star);
	// Triple Black Shell
	else if (stplyr->powers[pw_itemslot] == 26 || stplyr->powers[pw_blackshell] == 12 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 8))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, tbshell);
	// Green Shell
	else if (stplyr->powers[pw_itemslot] == 3 || stplyr->powers[pw_shell] == 2 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 11))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, shell);
	// Blue Thunder
	else if (stplyr->powers[pw_thunder] == 2 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 14))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, bluethunder);
	// Triple Mushroom
	else if (stplyr->powers[pw_starshroom] == 4 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 17))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, triplemushroom);
	// Red Shell
	else if (stplyr->powers[pw_itemslot] == 4 || stplyr->powers[pw_redshell] == 2 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 20))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, redshell);
	// Fireflower
	else if (stplyr->powers[pw_starshroom] == 9 || (stplyr->powers[pw_fireflower] > 1 && (leveltime & 1)) || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 23))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, fireflower);
	else if (stplyr->powers[pw_fireflower] > 1 && !(leveltime & 1))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, blankhd);
	// Banana Peel
	else if (stplyr->powers[pw_itemslot] == 5 || stplyr->powers[pw_banana] == 2 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 26))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, peel);
	// Triple Green Shell
	else if (stplyr->powers[pw_itemslot] == 21 || stplyr->powers[pw_shell] == 12 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 29))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, tshell);
	// Fake Item
	else if (stplyr->powers[pw_itemslot] == 6 || stplyr->powers[pw_fakeitem] == 2 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 32))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, fake);
	// Triple Banana Peel
	else if (stplyr->powers[pw_itemslot] == 23 || stplyr->powers[pw_banana] == 12 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 35))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, tpeel);
	// Bomb
	else if (stplyr->powers[pw_itemslot] == 7 || stplyr->powers[pw_bomb] == 2 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 38))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, bomb);
	// Triple Red Shell
	else if (stplyr->powers[pw_itemslot] == 22 || stplyr->powers[pw_redshell] == 12 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 41))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, tredshell);
	// Golden Mushroom
	else if (stplyr->powers[pw_starshroom] == 7 || (stplyr->powers[pw_goldshroom] > 1 && (leveltime & 1)) || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 44))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, goldmushroom);
	else if (stplyr->powers[pw_goldshroom] > 1 && !(leveltime & 1))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, blankhd);
	// Triple Bomb
	else if (stplyr->powers[pw_itemslot] == 24 || stplyr->powers[pw_bomb] == 12 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 47))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, tbomb);
	// Thunder
	else if (stplyr->powers[pw_thunder] == 1 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 50))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, thunder);
	// Mega Mushroom
	else if (stplyr->powers[pw_megamushroom] || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 53))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, megamushroom);
	// Initial Boo
	else if (stplyr->powers[pw_initboo] || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 56))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, tboo);
	// Black Shell
	else if (stplyr->powers[pw_itemslot] == 25 || stplyr->powers[pw_blackshell] == 2 || (stplyr->powers[pw_kartitem] && (stplyr->powers[pw_kartitem] % 59) <= 59))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, bshell);
	////////////////////////////////////////////
	// Double Mushroom
	else if (stplyr->powers[pw_starshroom] == 3)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, doublemushroom);
	// White Boo
	else if (stplyr->powers[pw_whiteboo] > 1 && stplyr->powers[pw_whiteboo] < 100 && (leveltime & 2))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, wboo);
	else if (stplyr->powers[pw_whiteboo] > 1 && stplyr->powers[pw_whiteboo] < 100 && !(leveltime & 2))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, blankhd);
	// Pink Boo
	else if (stplyr->powers[pw_pinkboo] > 1 && stplyr->powers[pw_pinkboo] < 100 && (leveltime & 2))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, pboo);
	else if (stplyr->powers[pw_pinkboo] > 1 && stplyr->powers[pw_pinkboo] < 100 && !(leveltime & 2))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, blankhd);
	// Green Boo
	else if (stplyr->powers[pw_greenboo] > 1 && (leveltime & 2))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, gboo);
	else if (stplyr->powers[pw_greenboo] > 1 && !(leveltime & 2))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, ksink);

	else if (stplyr->powers[pw_kitchensink])
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, ksink);

	if (!(stplyr->powers[pw_kartitem]) && stplyr->powers[pw_whiteboo] > 100 && (leveltime & 2))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, wboo);

	if (!(stplyr->powers[pw_kartitem]) && stplyr->powers[pw_pinkboo] > 100 && (leveltime & 2))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, pboo);

	// Additional Barrier Items for SRB2Kart 12/02/08
	if (!splitscreen)
	{
		ICONX = 95;
		ICONY = 20;
	}
	else
	{
		ICONX = BASEVIDWIDTH-280;
		ICONY = 60;
	}
	// pw_ijumpshield  // pw_jumpshield		// Use
	if (stplyr->powers[pw_zjumpshield] > 1 && (leveltime & 1))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, ijumpshield2);
	else if (stplyr->powers[pw_zjumpshield] > 1 && !(leveltime & 1))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, ijumpshield3);
	// pw_iforceshield // pw_forceshield	(HALF)
	if ((stplyr->powers[pw_iforceshield] == 1 && (leveltime & 1)) 
		|| (stplyr->powers[pw_forceshield] == 1 && (leveltime & 1)))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, iforceshield2);
	else if ((stplyr->powers[pw_iforceshield] == 1 && !(leveltime & 1)) 
		|| (stplyr->powers[pw_forceshield] == 1 && !(leveltime & 1)))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, iforceshield3);
	// pw_iwatershield // pw_watershield	// Use
	if (stplyr->powers[pw_zwatershield] > 1 && (leveltime & 1))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, iwatershield2);
	else if (stplyr->powers[pw_zwatershield] > 1 && !(leveltime & 1))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, iwatershield3);
	// pw_ibombshield  // pw_bombshield		// Use
	if (stplyr->powers[pw_zbombshield] > 1 && (leveltime & 1))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, ibombshield2);
	else if (stplyr->powers[pw_zbombshield] > 1 && !(leveltime & 1))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, ibombshield3);
	// pw_iringshield  // pw_ringshield		// Use   
	if (stplyr->powers[pw_ringshield] > 1 && (leveltime & 1))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, iringshield2);
	else if (stplyr->powers[pw_ringshield] > 1 && !(leveltime & 1))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, iringshield3);

	if ((unsigned)cv_randomshield.value == 1 && (stplyr->powers[pw_ijumpshield] || stplyr->powers[pw_iforceshield]
		|| stplyr->powers[pw_iwatershield] || stplyr->powers[pw_ibombshield] || stplyr->powers[pw_iringshield]))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, iunknown);
	// pw_ijumpshield  // pw_jumpshield 
	else if (stplyr->powers[pw_ijumpshield] == 2 || stplyr->powers[pw_jumpshield] == 1
		|| (stplyr->powers[pw_kartitemz] && (stplyr->powers[pw_kartitemz] % 14) <= 2))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, ijumpshield);
	// pw_iforceshield // pw_forceshield
	else if (stplyr->powers[pw_iforceshield] == 2 || stplyr->powers[pw_forceshield] == 2
		|| (stplyr->powers[pw_kartitemz] && (stplyr->powers[pw_kartitemz] % 14) <= 5))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, iforceshield);
	// pw_iwatershield // pw_watershield
	else if (stplyr->powers[pw_iwatershield] == 2 || stplyr->powers[pw_watershield] == 1
		|| (stplyr->powers[pw_kartitemz] && (stplyr->powers[pw_kartitemz] % 14) <= 8))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, iwatershield);
	// pw_ibombshield  // pw_bombshield
	else if (stplyr->powers[pw_ibombshield] == 2 || stplyr->powers[pw_bombshield] == 1
		|| (stplyr->powers[pw_kartitemz] && (stplyr->powers[pw_kartitemz] % 14) <= 11))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, ibombshield);
	// pw_iringshield  // pw_ringshield
	else if (stplyr->powers[pw_iringshield] == 2 || stplyr->powers[pw_zringshield] == 1
		|| (stplyr->powers[pw_kartitemz] && (stplyr->powers[pw_kartitemz] % 14) <= 14))
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, iringshield);
	
	if (stplyr->powers[pw_psychic] == 46 || stplyr->powers[pw_psychic] == 48)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, shieldused6);
	else if (stplyr->powers[pw_psychic] == 43 || stplyr->powers[pw_psychic] == 45)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, shieldused5);
	else if (stplyr->powers[pw_psychic] == 40 || stplyr->powers[pw_psychic] == 42)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, shieldused4);
	else if (stplyr->powers[pw_psychic] == 37 || stplyr->powers[pw_psychic] == 39)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, shieldused3);
	else if (stplyr->powers[pw_psychic] == 34 || stplyr->powers[pw_psychic] == 36)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, shieldused2);
	else if (stplyr->powers[pw_psychic] == 31 || stplyr->powers[pw_psychic] == 33)
		V_DrawScaledPatch(ICONX, STRINGY(ICONY), V_SNAPTORIGHT|V_TRANSLUCENT, shieldused1);

	if (!splitscreen)
	{
		// Draw HUD Triple items to the left of the speedometer
		if (stplyr->powers[pw_triples] == 1 && stplyr->powers[pw_shell] == 11)
			V_DrawScaledPatch(143, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tshell1);
		else if (stplyr->powers[pw_triples] == 2 && stplyr->powers[pw_shell] == 11)
			V_DrawScaledPatch(143, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tshell2);
		else if (stplyr->powers[pw_triples] == 3 && stplyr->powers[pw_shell] == 11)
			V_DrawScaledPatch(143, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tshell3);

		if (stplyr->powers[pw_triples] == 1 && stplyr->powers[pw_redshell] == 11)
			V_DrawScaledPatch(143, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tredshell1);
		else if (stplyr->powers[pw_triples] == 2 && stplyr->powers[pw_redshell] == 11)
			V_DrawScaledPatch(143, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tredshell2);
		else if (stplyr->powers[pw_triples] == 3 && stplyr->powers[pw_redshell] == 11)
			V_DrawScaledPatch(143, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tredshell3);

		if (stplyr->powers[pw_triples] == 1 && stplyr->powers[pw_banana] == 11)
			V_DrawScaledPatch(146, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tpeel1);
		else if (stplyr->powers[pw_triples] == 2 && stplyr->powers[pw_banana] == 11)
			V_DrawScaledPatch(146, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tpeel2);
		else if (stplyr->powers[pw_triples] == 3 && stplyr->powers[pw_banana] == 11)
			V_DrawScaledPatch(146, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tpeel3);

		if (stplyr->powers[pw_triples] == 1 && stplyr->powers[pw_bomb] == 11)
			V_DrawScaledPatch(143, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tbomb1);
		else if (stplyr->powers[pw_triples] == 2 && stplyr->powers[pw_bomb] == 11)
			V_DrawScaledPatch(143, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tbomb2);
		else if (stplyr->powers[pw_triples] == 3 && stplyr->powers[pw_bomb] == 11)
			V_DrawScaledPatch(143, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tbomb3);

		if (stplyr->powers[pw_triples] == 1 && stplyr->powers[pw_blackshell] == 11)
			V_DrawScaledPatch(146, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tbshell1);
		else if (stplyr->powers[pw_triples] == 2 && stplyr->powers[pw_blackshell] == 11)
			V_DrawScaledPatch(146, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tbshell2);
		else if (stplyr->powers[pw_triples] == 3 && stplyr->powers[pw_blackshell] == 11)
			V_DrawScaledPatch(146, STRINGY(166), V_SNAPTORIGHT|V_TRANSLUCENT, tbshell3);

		// Draw the Speed counter.
		if (stplyr->speed < 2 && stplyr->speed > -1)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle1);
		if (stplyr->speed < 5 && stplyr->speed > 1)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle2);
		if (stplyr->speed < 7 && stplyr->speed > 4)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle3);
		if (stplyr->speed < 10 && stplyr->speed > 6)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle4);
		if (stplyr->speed < 12 && stplyr->speed > 9)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle5);
		if (stplyr->speed < 15 && stplyr->speed > 11)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle6);
		if (stplyr->speed < 17 && stplyr->speed > 14)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle7);
		if (stplyr->speed < 20 && stplyr->speed > 16)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle8);
		if (stplyr->speed < 22 && stplyr->speed > 19)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle9);
		if (stplyr->speed < 25 && stplyr->speed > 21)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle10);
		if (stplyr->speed < 27 && stplyr->speed > 24)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle11);
		if (stplyr->speed < 30 && stplyr->speed > 26)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle12);
		if (stplyr->speed < 32 && stplyr->speed > 29)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle13);
		if (stplyr->speed < 35 && stplyr->speed > 31)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle14);
		if (stplyr->speed < 37 && stplyr->speed > 34)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle15);
		if (stplyr->speed < 40 && stplyr->speed > 36)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle16);
		if (stplyr->speed < 42 && stplyr->speed > 39)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle17);
		if (stplyr->speed < 45 && stplyr->speed > 41)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle18);
		if (stplyr->speed < 47 && stplyr->speed > 44)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle19);
		if (stplyr->speed < 50 && stplyr->speed > 46)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle20);
		if (stplyr->speed < 52 && stplyr->speed > 49)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle21);
		if (stplyr->speed < 55 && stplyr->speed > 51)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle22);
		if (stplyr->speed < 57 && stplyr->speed > 54)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle23);
		if (stplyr->speed < 60 && stplyr->speed > 56)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle24);
		if (stplyr->speed > 59)
			V_DrawScaledPatch(205, STRINGY(170), V_SNAPTORIGHT|V_TRANSLUCENT, needle25);
	}

	V_DrawKartString(X-45, STRINGY(12), 0, va("TIME"), 14);
	if (stplyr->realtime/(60*TICRATE) < 10) // 9:99:99 only
	{
		// minutes time       0 __ __
		V_DrawKartString(X, STRINGY(12), 0, va("%d", stplyr->realtime/(60*TICRATE)), 14);

		// colon location     _:__ __
		V_DrawKartString(X+11, STRINGY(12), 0, va(":"), 14);

		// zero second        _ 0_ __
		if ((stplyr->realtime/TICRATE % 60) < 10)
		{
			V_DrawKartString(X+17, STRINGY(12), 0, va("0"), 14);
		// seconds time       _ _0 __
			V_DrawKartString(X+28, STRINGY(12), 0, va("%d", stplyr->realtime/TICRATE % 60), 14);
		}
		// zero second        _ 00 __
		if ((stplyr->realtime/TICRATE % 60) >= 10)
			V_DrawKartString(X+17, STRINGY(12), 0, va("%d", stplyr->realtime/TICRATE % 60), 14);

		// colon location 2   _ __:__
		V_DrawKartString(X+39, STRINGY(12), 0, va(":"), 14);

		// zero tick          _ __ 0_
		if (G_TicsToCentiseconds(stplyr->realtime) < 10)
		{
			V_DrawKartString(X+45, STRINGY(12), 0, va("0"), 14);
		// tics               _ __ _0
			V_DrawKartString(X+56, STRINGY(12), 0, va("%d", G_TicsToCentiseconds(stplyr->realtime)), 14);
		}
		// zero tick          _ __ 00
		if (G_TicsToCentiseconds(stplyr->realtime) >= 10)
			V_DrawKartString(X+45, STRINGY(12), 0, va("%d", G_TicsToCentiseconds(stplyr->realtime)), 14);
	}
	else
		V_DrawKartString(236, STRINGY(12), 0, va("9:59:99"), 14);

	if (leveltime > 157 && leveltime < (TICRATE+1)*7)
	{
		if (!(mapmusic & 2048)) // TODO: Might not need this here
			mapmusic = mapheaderinfo[gamemap-1].musicslot;

		S_ChangeMusic(mapmusic & 2047, true);
	}
}
#endif
static void ST_drawNiGHTSHUD(void)
{
	if (stplyr->linkcount > 1)
	{
		INT32 colornum;

		colornum = ((stplyr->linkcount-1) / 5)%14;

		if (splitscreen)
		{
			ST_DrawNightsOverlayNum(SCX(256), SCY(160), (stplyr->linkcount-1), nightsnum, colornum);
			V_DrawMappedPatch(SCX(264), SCY(160), V_NOSCALESTART, nightslink,
				colornum == 0 ? colormaps : (UINT8 *)defaulttranslationtables - 256 + (colornum<<8));
		}
		else
		{
			ST_DrawNightsOverlayNum(SCX(160), SCY(176), (stplyr->linkcount-1), nightsnum, colornum);
			V_DrawMappedPatch(SCX(168), SCY(176), V_NOSCALESTART, nightslink,
				colornum == 0 ? colormaps : (UINT8 *)defaulttranslationtables - 256 + (colornum<<8));
		}
	}

	if (stplyr->pflags & PF_NIGHTSMODE)
	{
		INT32 locx, locy;

		if (splitscreen)
		{
			locx = 110;
			locy = 188;
		}
		else
		{
			locx = 16;
			locy = 144;
		}

		if (!(stplyr->drillmeter & 1))
		{
			V_DrawFill(locx-2, STRINGY(locy-2), 100, 8, 48);
			V_DrawFill(locx, STRINGY(locy), 96, 4, 31);
			V_DrawFill(locx, STRINGY(locy), stplyr->drillmeter/20, 4, 160);
		}
		else
		{
			V_DrawFill(locx-2, STRINGY(locy-2), 100, 8, 37);
			V_DrawFill(locx, STRINGY(locy), 96, 4, 8);
			V_DrawFill(locx, STRINGY(locy), stplyr->drillmeter/20, 4, 164);
		}
	}

	if (gametype == GT_RACE)
	{
		// draw score (same in splitscreen as normal, too!)
		ST_DrawOverlayNum(SCX(hudinfo[HUD_SCORENUM].x), SCY(hudinfo[HUD_SCORENUM].y), stplyr->score, tallnum);
		V_DrawScaledPatch(SCX(hudinfo[HUD_SCORE].x), SCY(hudinfo[HUD_SCORE].y), V_NOSCALESTART|V_TRANSLUCENT, sboscore);

		// Draw Time
		if (splitscreen)
		{
			INT32 seconds = G_TicsToSeconds(stplyr->realtime);

			if (seconds < 10)
				ST_DrawOverlayNum(SCX(hudinfo[HUD_LOWSECONDSSPLIT].x), SCY(hudinfo[HUD_LOWSECONDSSPLIT].y), 0, tallnum);

			// seconds time
			ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDSSPLIT].x), SCY(hudinfo[HUD_SECONDSSPLIT].y), G_TicsToSeconds(stplyr->realtime), tallnum);

			// minutes time
			ST_DrawOverlayNum(SCX(hudinfo[HUD_MINUTESSPLIT].x), SCY(hudinfo[HUD_MINUTESSPLIT].y), G_TicsToMinutes(stplyr->realtime, true), tallnum);

			// colon location
			V_DrawScaledPatch(SCX(hudinfo[HUD_TIMECOLONSPLIT].x), SCY(hudinfo[HUD_TIMECOLONSPLIT].y), V_NOSCALESTART|V_TRANSLUCENT, sbocolon);

			// TIME location
			V_DrawScaledPatch(SCX(hudinfo[HUD_TIMESPLIT].x), SCY(hudinfo[HUD_TIMESPLIT].y), V_NOSCALESTART|V_TRANSLUCENT, sbotime);
		}
		else
		{
			if (cv_timetic.value == 1) // show tics instead of MM : SS
				ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDS].x), SCY(hudinfo[HUD_SECONDS].y), stplyr->realtime, tallnum);
			else
			{
				INT32 seconds = G_TicsToSeconds(stplyr->realtime);

				if (seconds < 10)
					ST_DrawOverlayNum(SCX(hudinfo[HUD_LOWSECONDS].x), SCY(hudinfo[HUD_LOWSECONDS].y), 0, tallnum);

				// seconds time
				ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDS].x), SCY(hudinfo[HUD_SECONDS].y), G_TicsToSeconds(stplyr->realtime), tallnum);

				// minutes time
				ST_DrawOverlayNum(SCX(hudinfo[HUD_MINUTES].x), SCY(hudinfo[HUD_MINUTES].y), G_TicsToMinutes(stplyr->realtime, true), tallnum);

				// colon location
				V_DrawScaledPatch(SCX(hudinfo[HUD_TIMECOLON].x), SCY(hudinfo[HUD_TIMECOLON].y), V_NOSCALESTART|V_TRANSLUCENT, sbocolon);
			}

			// Show tics, too!
			if (timeattacking || cv_timetic.value == 2)
			{
				INT32 tics = G_TicsToCentiseconds(stplyr->realtime);

				if (tics < 10)
					ST_DrawOverlayNum(SCX(hudinfo[HUD_LOWTICS].x), SCY(hudinfo[HUD_LOWTICS].y), 0, tallnum);

				// colon location
				V_DrawScaledPatch(SCX(hudinfo[HUD_TIMETICCOLON].x), SCY(hudinfo[HUD_TIMETICCOLON].y), V_NOSCALESTART|V_TRANSLUCENT, sbocolon);
				ST_DrawOverlayNum(SCX(hudinfo[HUD_TICS].x), SCY(hudinfo[HUD_TICS].y), tics, tallnum);
			}

			// TIME location
			V_DrawScaledPatch(SCX(hudinfo[HUD_TIME].x), SCY(hudinfo[HUD_TIME].y), V_NOSCALESTART|V_TRANSLUCENT, sbotime);
		}

		return;
	}

	if (stplyr->bonustime > 1)
		V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(100), 0, "BONUS TIME START!");

	V_DrawScaledPatch(SCX(16), SCY(8), V_NOSCALESTART|V_TRANSLUCENT, nbracket);
	V_DrawScaledPatch(SCX(24), (INT32)(SCY(8) + 8*vid.fdupy), V_NOSCALESTART|V_TRANSLUCENT, nhud[(leveltime/2)%12]);

	if (stplyr->capsule && !cv_objectplace.value)
	{
		INT32 amount;
		INT32 origamount;
		const INT32 length = 88;

		V_DrawScaledPatch(SCX(72), SCY(8), V_NOSCALESTART|V_TRANSLUCENT, nbracket);
		V_DrawScaledPatch(SCX(74), (INT32)(SCY(8) + 4*vid.fdupy), V_NOSCALESTART|V_TRANSLUCENT,
			minicaps);

		if (stplyr->capsule->reactiontime != 0)
		{
			INT32 r;
			const INT32 orblength = 20;

			for (r = 0; r < 5; r++)
			{
				V_DrawScaledPatch(SCX(230 - (7*r)), SCY(144), V_NOSCALESTART|V_TRANSLUCENT,
					redstat);
				V_DrawScaledPatch(SCX(188 - (7*r)), SCY(144), V_NOSCALESTART|V_TRANSLUCENT,
					orngstat);
				V_DrawScaledPatch(SCX(146 - (7*r)), SCY(144), V_NOSCALESTART|V_TRANSLUCENT,
					yelstat);
				V_DrawScaledPatch(SCX(104 - (7*r)), SCY(144), V_NOSCALESTART|V_TRANSLUCENT,
					byelstat);
			}
			origamount = stplyr->capsule->spawnpoint->angle & 1023;

			amount = (origamount - stplyr->capsule->health);

			amount = (amount * orblength)/origamount;

			if (amount > 0)
			{
				INT32 t;

				// Fill up the bar with blue orbs... in reverse! (yuck)
				for (r = amount; r >= 0; r--)
				{
					t = r;

					if (r > 14)
						t += 1;
					if (r > 9)
						t += 1;
					if (r > 4)
						t += 1;

					V_DrawScaledPatch(SCX(76 + (7*t)), SCY(144), V_NOSCALESTART|V_TRANSLUCENT,
						bluestat);
				}
			}
		}
		else
		{
			// Lil' white box!
			V_DrawFill(15, STRINGY(8) + 34, length + 2, 5, 0);
			V_DrawFill(16, STRINGY(8)+35, length/4, 3, 103);
			V_DrawFill(16 + length/4, STRINGY(8) + 35, length/4, 3, 85);
			V_DrawFill(16 + (length/4)*2, STRINGY(8) + 35, length/4, 3, 87);
			V_DrawFill(16 + (length/4)*3, STRINGY(8) + 35, length/4, 3, 131);
			origamount = stplyr->capsule->spawnpoint->angle & 1023;

			if (origamount <= 0)
				CONS_Printf("Give the egg capsule on mare %d a ring requirement.\n", stplyr->capsule->threshold);
			else
			{
				amount = (origamount - stplyr->capsule->health);
				amount = (amount * length)/origamount;

				if (amount > 0)
					V_DrawFill(16, STRINGY(8) + 35, amount, 3, 229);
			}
		}
		V_DrawScaledPatch(SCX(40), (INT32)(SCY(8) + 5*vid.fdupy), V_NOSCALESTART|V_TRANSLUCENT, narrow[(leveltime/2)%8]);
	}
	else
		V_DrawScaledPatch(SCX(40), (INT32)(SCY(8) + 5*vid.fdupy), V_NOSCALESTART|V_TRANSLUCENT, narrow[8]);

	ST_DrawOverlayNum(SCX(68), (INT32)(SCY(8) + 11*vid.fdupy), stplyr->health > 0 ? stplyr->health - 1 : 0, tallnum);

	ST_DrawNightsOverlayNum(SCX(288), SCY(12), stplyr->score, nightsnum, 7); // Blue

	if (stplyr->nightstime > 0)
	{
		INT32 numbersize;

		if (stplyr->nightstime < 10)
			numbersize = SCX(16)/2;
		else if (stplyr->nightstime < 100)
			numbersize = SCX(32)/2;
		else
			numbersize = SCX(48)/2;

		if (stplyr->nightstime < 10)
			ST_DrawNightsOverlayNum(SCX(160) + numbersize, SCY(32), stplyr->nightstime,
				nightsnum, 6); // Red
		else
			ST_DrawNightsOverlayNum(SCX(160) + numbersize, SCY(32), stplyr->nightstime,
				nightsnum, 15); // Yellow
	}
}

static void ST_drawMatchHUD(void)
{
	INT32 offset = 80;

	if (gametype == GT_MATCH || gametype == GT_TAG || gametype == GT_CTF
		|| cv_ringslinger.value)
	{
		if (stplyr->health > 1)
			V_DrawScaledPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT, normring);
		else
			V_DrawTranslucentPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT|V_8020TRANS, normring);

		if (!stplyr->currentweapon)
			V_DrawScaledPatch(6 + offset, STRINGY(162 - (splitscreen ? 4 : 2)), V_SNAPTOLEFT, curweapon);
	}

	offset += 20;

	if (stplyr->powers[pw_automaticring])
	{
		INT32 yelflag = 0;

		if (stplyr->powers[pw_automaticring] >= MAX_AUTOMATIC)
			yelflag = V_YELLOWMAP;

		if ((stplyr->ringweapons & RW_AUTO) && stplyr->health > 1)
			V_DrawScaledPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT, autoring);
		else
			V_DrawTranslucentPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT|V_8020TRANS, autoring);

		if (stplyr->powers[pw_automaticring] > 99)
			V_DrawTinyNum(8 + offset + 1, STRINGY(162), V_TRANSLUCENT | V_SNAPTOLEFT | yelflag,
				stplyr->powers[pw_automaticring]);
		else
			V_DrawString(8 + offset, STRINGY(162), V_TRANSLUCENT | V_SNAPTOLEFT | yelflag,
				va("%d", stplyr->powers[pw_automaticring]));

		if (stplyr->currentweapon == WEP_AUTO)
			V_DrawScaledPatch(6 + offset, STRINGY(162 - (splitscreen ? 4 : 2)), V_SNAPTOLEFT, curweapon);
	}
	else if (stplyr->ringweapons & RW_AUTO)
		V_DrawTranslucentPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT, autoring);

	offset += 20;

	if (stplyr->powers[pw_bouncering])
	{
		INT32 yelflag = 0;

		if (stplyr->powers[pw_bouncering] >= MAX_BOUNCE)
			yelflag = V_YELLOWMAP;

		if ((stplyr->ringweapons & RW_BOUNCE) && stplyr->health > 1)
			V_DrawScaledPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT, bouncering);
		else
			V_DrawTranslucentPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT|V_8020TRANS, bouncering);

		if (stplyr->powers[pw_bouncering] > 99)
			V_DrawTinyNum(8 + offset + 1, STRINGY(162), V_TRANSLUCENT | V_SNAPTOLEFT | yelflag,
				stplyr->powers[pw_bouncering]);
		else
			V_DrawString(8 + offset, STRINGY(162), V_TRANSLUCENT | V_SNAPTOLEFT | yelflag,
				va("%d", stplyr->powers[pw_bouncering]));

		if (stplyr->currentweapon == WEP_BOUNCE)
			V_DrawScaledPatch(6 + offset, STRINGY(162 - (splitscreen ? 4 : 2)), V_SNAPTOLEFT, curweapon);
	}
	else if (stplyr->ringweapons & RW_BOUNCE)
		V_DrawTranslucentPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT, bouncering);

	offset += 20;

	if (stplyr->powers[pw_scatterring])
	{
		INT32 yelflag = 0;

		if (stplyr->powers[pw_scatterring] >= MAX_SCATTER)
			yelflag = V_YELLOWMAP;

		if ((stplyr->ringweapons & RW_SCATTER) && stplyr->health > 1)
			V_DrawScaledPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT, scatterring);
		else
			V_DrawTranslucentPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT|V_8020TRANS, scatterring);

		if (stplyr->powers[pw_scatterring] > 99)
			V_DrawTinyNum(8 + offset + 1, STRINGY(162), V_TRANSLUCENT | V_SNAPTOLEFT | yelflag,
				stplyr->powers[pw_scatterring]);
		else
			V_DrawString(8 + offset, STRINGY(162), V_TRANSLUCENT | V_SNAPTOLEFT | yelflag,
				va("%d", stplyr->powers[pw_scatterring]));

		if (stplyr->currentweapon == WEP_SCATTER)
			V_DrawScaledPatch(6 + offset, STRINGY(162 - (splitscreen ? 4 : 2)), V_SNAPTOLEFT, curweapon);
	}
	else if (stplyr->ringweapons & RW_SCATTER)
		V_DrawTranslucentPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT, scatterring);

	offset += 20;

	if (stplyr->powers[pw_grenadering])
	{
		INT32 yelflag = 0;

		if (stplyr->powers[pw_grenadering] >= MAX_GRENADE)
			yelflag = V_YELLOWMAP;

		if ((stplyr->ringweapons & RW_GRENADE) && stplyr->health > 1)
			V_DrawScaledPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT, grenadering);
		else
			V_DrawTranslucentPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT|V_8020TRANS, grenadering);

		if (stplyr->powers[pw_grenadering] > 99)
			V_DrawTinyNum(8 + offset + 1, STRINGY(162), V_TRANSLUCENT | V_SNAPTOLEFT | yelflag,
				stplyr->powers[pw_grenadering]);
		else
			V_DrawString(8 + offset, STRINGY(162), V_TRANSLUCENT | V_SNAPTOLEFT | yelflag,
				va("%d", stplyr->powers[pw_grenadering]));

		if (stplyr->currentweapon == WEP_GRENADE)
			V_DrawScaledPatch(6 + offset, STRINGY(162 - (splitscreen ? 4 : 2)), V_SNAPTOLEFT, curweapon);
	}
	else if (stplyr->ringweapons & RW_GRENADE)
		V_DrawTranslucentPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT, grenadering);

	offset += 20;

	if (stplyr->powers[pw_explosionring])
	{
		INT32 yelflag = 0;

		if (stplyr->powers[pw_explosionring] >= MAX_EXPLOSION)
			yelflag = V_YELLOWMAP;

		if ((stplyr->ringweapons & RW_EXPLODE) && stplyr->health > 1)
			V_DrawScaledPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT, explosionring);
		else
			V_DrawTranslucentPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT|V_8020TRANS, explosionring);

		if (stplyr->powers[pw_explosionring] > 99)
			V_DrawTinyNum(8 + offset + 1, STRINGY(162), V_TRANSLUCENT | V_SNAPTOLEFT | yelflag,
				stplyr->powers[pw_explosionring]);
		else
			V_DrawString(8 + offset, STRINGY(162), V_TRANSLUCENT | V_SNAPTOLEFT | yelflag,
				va("%d", stplyr->powers[pw_explosionring]));

		if (stplyr->currentweapon == WEP_EXPLODE)
			V_DrawScaledPatch(6 + offset, STRINGY(162 - (splitscreen ? 4 : 2)), V_SNAPTOLEFT, curweapon);
	}
	else if (stplyr->ringweapons & RW_EXPLODE)
		V_DrawTranslucentPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT, explosionring);

	offset += 20;

	if (stplyr->powers[pw_railring])
	{
		INT32 yelflag = 0;

		if (stplyr->powers[pw_railring] >= MAX_RAIL)
			yelflag = V_YELLOWMAP;

		if ((stplyr->ringweapons & RW_RAIL) && stplyr->health > 1)
			V_DrawScaledPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT, railring);
		else
			V_DrawTranslucentPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT|V_8020TRANS, railring);

		if (stplyr->powers[pw_railring] > 99)
			V_DrawTinyNum(8 + offset + 1, STRINGY(162), V_TRANSLUCENT | V_SNAPTOLEFT | yelflag,
				stplyr->powers[pw_railring]);
		else
			V_DrawString(8 + offset, STRINGY(162), V_TRANSLUCENT | V_SNAPTOLEFT | yelflag,
				va("%d", stplyr->powers[pw_railring]));

		if (stplyr->currentweapon == WEP_RAIL)
			V_DrawScaledPatch(6 + offset, STRINGY(162 - (splitscreen ? 4 : 2)), V_SNAPTOLEFT, curweapon);
	}
	else if (stplyr->ringweapons & RW_RAIL)
		V_DrawTranslucentPatch(8 + offset, STRINGY(162), V_SNAPTOLEFT, railring);

	offset += 20;

	// Power Stones collected
	offset = 136; // Used for Y now

	if (stplyr->powers[pw_emeralds] & EMERALD1)
		V_DrawScaledPatch(28, STRINGY(offset), V_SNAPTOLEFT, tinyemeraldpics[0]);

	offset += 8;

	if (stplyr->powers[pw_emeralds] & EMERALD2)
		V_DrawScaledPatch(40, STRINGY(offset), V_SNAPTOLEFT, tinyemeraldpics[1]);

	if (stplyr->powers[pw_emeralds] & EMERALD6)
		V_DrawScaledPatch(16, STRINGY(offset), V_SNAPTOLEFT, tinyemeraldpics[5]);

	offset += 16;

	if (stplyr->powers[pw_emeralds] & EMERALD3)
		V_DrawScaledPatch(40, STRINGY(offset), V_SNAPTOLEFT, tinyemeraldpics[2]);

	if (stplyr->powers[pw_emeralds] & EMERALD5)
		V_DrawScaledPatch(16, STRINGY(offset), V_SNAPTOLEFT, tinyemeraldpics[4]);

	offset += 8;

	if (stplyr->powers[pw_emeralds] & EMERALD4)
		V_DrawScaledPatch(28, STRINGY(offset), V_SNAPTOLEFT, tinyemeraldpics[3]);

	offset -= 16;

	if (stplyr->powers[pw_emeralds] & EMERALD7)
		V_DrawScaledPatch(28, STRINGY(offset), V_SNAPTOLEFT, tinyemeraldpics[6]);
}

static void ST_drawRaceHUD(void)
{
#ifdef RIDERS
	if (riderslevel)
	{
		if (leveltime > 3*TICRATE && leveltime <= 4*TICRATE)
			V_DrawScaledPatch(SCX((BASEVIDWIDTH - SHORT(rrace4->width))/2), (INT32)(SCY(BASEVIDHEIGHT/2)), V_NOSCALESTART, rrace4);
		else if (leveltime > 4*TICRATE && leveltime <= 5*TICRATE)
			V_DrawScaledPatch(SCX((BASEVIDWIDTH - SHORT(rrace3->width))/2), (INT32)(SCY(BASEVIDHEIGHT/2)), V_NOSCALESTART, rrace3);
		else if (leveltime > 5*TICRATE && leveltime <= 6*TICRATE)
			V_DrawScaledPatch(SCX((BASEVIDWIDTH - SHORT(rrace2->width))/2), (INT32)(SCY(BASEVIDHEIGHT/2)), V_NOSCALESTART, rrace2);
		else if (leveltime > 6*TICRATE && leveltime <= 7*TICRATE)
			V_DrawScaledPatch(SCX((BASEVIDWIDTH - SHORT(rrace1->width))/2), (INT32)(SCY(BASEVIDHEIGHT/2)), V_NOSCALESTART, rrace1);
		else if (leveltime > 7*TICRATE && leveltime <= 8*TICRATE)
			V_DrawScaledPatch(SCX((BASEVIDWIDTH - SHORT(rrace0->width))/2), (INT32)(SCY(BASEVIDHEIGHT/2)), V_NOSCALESTART, rrace0);
		else if (leveltime > 8*TICRATE && leveltime <= 9*TICRATE)
			V_DrawScaledPatch(SCX((BASEVIDWIDTH - SHORT(rracego->width))/2), (INT32)(SCY(BASEVIDHEIGHT/2)), V_NOSCALESTART, rracego);
	}
	else if (!(riderslevel || kartmode))
	{
#endif
	if (leveltime > TICRATE && leveltime <= 2*TICRATE)
		V_DrawScaledPatch(SCX((BASEVIDWIDTH - SHORT(race3->width))/2), (INT32)(SCY(BASEVIDHEIGHT/2)), V_NOSCALESTART, race3);
	else if (leveltime > 2*TICRATE && leveltime <= 3*TICRATE)
		V_DrawScaledPatch(SCX((BASEVIDWIDTH - SHORT(race2->width))/2), (INT32)(SCY(BASEVIDHEIGHT/2)), V_NOSCALESTART, race2);
	else if (leveltime > 3*TICRATE && leveltime <= 4*TICRATE)
		V_DrawScaledPatch(SCX((BASEVIDWIDTH - SHORT(race1->width))/2), (INT32)(SCY(BASEVIDHEIGHT/2)), V_NOSCALESTART, race1);
	else if (leveltime > 4*TICRATE && leveltime <= 5*TICRATE)
		V_DrawScaledPatch(SCX((BASEVIDWIDTH - SHORT(racego->width))/2), (INT32)(SCY(BASEVIDHEIGHT/2)), V_NOSCALESTART, racego);

#ifdef RIDERS
	}
#endif
	if (circuitmap)
	{
#ifdef RIDERS
		// Sonic Riders mode
		if (stplyr->exiting && riderslevel)
			V_DrawString(BASEVIDWIDTH-304, STRINGY(BASEVIDHEIGHT-16), V_YELLOWMAP, "FINISHED!");
		if (!stplyr->exiting && riderslevel)
			V_DrawString(BASEVIDWIDTH-304, STRINGY(BASEVIDHEIGHT-16), 0, va("Lap: %d/%d", stplyr->laps+1, cv_numlaps.value));
		// Mario Kart mode
		if (stplyr->exiting && kartmode)
			V_DrawKartString(BASEVIDWIDTH-304, STRINGY(BASEVIDHEIGHT-189), 0, "FINISHED!", 12);
		if (!stplyr->exiting && kartmode)
			V_DrawKartString(BASEVIDWIDTH-304, STRINGY(BASEVIDHEIGHT-189), 0, va("Lap: %d/%d", stplyr->laps+1, cv_numlaps.value), 12);
		// SRB2 mode
		if (stplyr->exiting && !(riderslevel || kartmode))
			V_DrawString(hudinfo[HUD_LAP].x, STRINGY(hudinfo[HUD_LAP].y+24), V_YELLOWMAP, "FINISHED!");
		if (!stplyr->exiting && !(riderslevel || kartmode))
			V_DrawString(hudinfo[HUD_LAP].x, STRINGY(hudinfo[HUD_LAP].y+24), 0, va("Lap: %d/%d", stplyr->laps+1, cv_numlaps.value));

		// Player's position in race.
		{
			INT32 position;
			INT32 xpos1;
			INT32 ypos1;
			INT32 xpos2;
			INT32 ypos2;

			if (riderslevel || kartmode)
			{
				if (!splitscreen)
				{
					xpos1 = 30;
					ypos1 = 25;
					xpos2 = 31;
					ypos2 = 37;
				}
				else
				{
					xpos1 = 30;
					ypos1 = 32;
					xpos2 = 31;
					ypos2 = 57;
				}
			}
			else
			{
				if (!splitscreen)
				{
					xpos1 = 280;
					ypos1 = 25;
					xpos2 = 281;
					ypos2 = 37;
				}
				else
				{
					xpos1 = 280;
					ypos1 = 25;
					xpos2 = 281;
					ypos2 = 50;
				}
			}
			position = stplyr->position;
			if (position == 1) {
				V_DrawString(xpos1, STRINGY(BASEVIDHEIGHT-ypos1), 0, "\x83st");
				ST_DrawNightsOverlayNum(SCX(xpos2), SCY(BASEVIDHEIGHT-ypos2), position, nightsnum, 17);
			}
			else if (position == 2) {
				V_DrawString(xpos1, STRINGY(BASEVIDHEIGHT-ypos1), 0, "\x84nd");
				ST_DrawNightsOverlayNum(SCX(xpos2), SCY(BASEVIDHEIGHT-ypos2), position, nightsnum, 13);
			}
			else if (position == 3) {
				V_DrawString(xpos1, STRINGY(BASEVIDHEIGHT-ypos1), 0, "\x82rd");
				ST_DrawNightsOverlayNum(SCX(xpos2), SCY(BASEVIDHEIGHT-ypos2), position, nightsnum, 19);
			}
			else {
				V_DrawString(xpos1, STRINGY(BASEVIDHEIGHT-ypos1), 0, "\x85th");
				ST_DrawNightsOverlayNum(SCX(xpos2), SCY(BASEVIDHEIGHT-ypos2), position, nightsnum, 7);
			}
		}
	}
	if (!kartmode && !riderslevel && !(stplyr->pflags & PF_NIGHTSMODE))
	{
		INT32 locx, locy;

		if (splitscreen)
		{
			locx = 110;
			locy = 188;
		}
		else
		{
			locx = 16;
			locy = 144;
		}
		if (stplyr->charability == 1 || stplyr->charability == 8 || stplyr->charability == 9)
		{
			if (stplyr->powers[pw_tailsfly] < 3 && stplyr->powers[pw_tailsfly] > 0 )
			{
				V_DrawFill(locx-2, STRINGY(locy-2), 100, 8, 48);
				V_DrawFill(locx, STRINGY(locy), 96, 4, 31);
				V_DrawFill(locx, STRINGY(locy), 0, 4, 160);
			}
			else if (stplyr->powers[pw_tailsfly] > 2)
			{
				V_DrawFill(locx-2, STRINGY(locy-2), 100, 8, 48);
				V_DrawFill(locx, STRINGY(locy), 96, 4, 31);
				V_DrawFill(locx, STRINGY(locy), stplyr->powers[pw_tailsfly], 4, 160);
			}
			else if (stplyr->powers[pw_tailsfly] == 0 )
			{
				V_DrawFill(locx-2, STRINGY(locy-2), 100, 8, 48);
				V_DrawFill(locx, STRINGY(locy), 96, 4, 31);
				V_DrawFill(locx, STRINGY(locy), 96, 4, 160);
			}
		}
		/*if (stplyr->charability == 4)
		{
			if (stplyr->powers[pw_psychic] < 3 && stplyr->powers[pw_psychic] > 0 )
			{
				V_DrawFill(locx-2, STRINGY(locy-2), 100, 8, 48);
				V_DrawFill(locx, STRINGY(locy), 96, 4, 31);
				V_DrawFill(locx, STRINGY(locy), 0, 4, 160);
			}
			else if (stplyr->powers[pw_psychic] > 2)
			{
				V_DrawFill(locx-2, STRINGY(locy-2), 100, 8, 48);
				V_DrawFill(locx, STRINGY(locy), 96, 4, 31);
				V_DrawFill(locx, STRINGY(locy), stplyr->powers[pw_psychic], 4, 164);
			}
			else if (stplyr->powers[pw_psychic] == 0 )
			{
				V_DrawFill(locx-2, STRINGY(locy-2), 100, 8, 48);
				V_DrawFill(locx, STRINGY(locy), 96, 4, 31);
				V_DrawFill(locx, STRINGY(locy), 96, 4, 164);
			}
		}*/
	}
#else
		if (stplyr->exiting)
			V_DrawString(hudinfo[HUD_LAP].x, STRINGY(hudinfo[HUD_LAP].y), V_YELLOWMAP, "FINISHED!");
		else
			V_DrawString(hudinfo[HUD_LAP].x, STRINGY(hudinfo[HUD_LAP].y), 0, va("Lap: %u/%d", stplyr->laps+1, cv_numlaps.value));
	}
#endif
}

static void ST_drawTagHUD(void)
{
	char pstime[33] = "";
	char pstext[33] = "";

	// Figure out what we're going to print.
	if (leveltime < hidetime * TICRATE) //during the hide time, the seeker and hiders have different messages on their HUD.
	{
		if (cv_hidetime.value)
			sprintf(pstime, "%d", (hidetime - leveltime/TICRATE)); //hide time is in seconds, not tics.

		if (stplyr->pflags & PF_TAGIT && !stplyr->spectator)
			sprintf(pstext, "WAITING FOR PLAYERS TO HIDE...");
		else
		{
			if (!stplyr->spectator) //spectators get a generic HUD message rather than a gametype specific one.
			{
				if (cv_tagtype.value == 1) //hide and seek.
					sprintf(pstext, "HIDE BEFORE TIME RUNS OUT!");
				else //default
					sprintf(pstext, "FLEE BEFORE YOU ARE HUNTED!");
			}
			else
				sprintf(pstext, "HIDE TIME REMAINING:");
		}
	}
	else
	{
		if (cv_timelimit.value && timelimitintics >= leveltime)
			sprintf(pstime, "%d", (timelimitintics-leveltime)/TICRATE);

		if (stplyr->pflags & PF_TAGIT)
			sprintf(pstext, "YOU'RE IT!");
		else
		{
			if (cv_timelimit.value)
				sprintf(pstext, "TIME REMAINING:");
			else //Since having no hud message in tag is not characteristic:
				sprintf(pstext, "NO TIME LIMIT");
		}
	}

	// Print the stuff.
	if (pstext[0])
	{
		if (splitscreen)
			V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(168), 0, pstext);
		else
			V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(184), 0, pstext);
	}
	if (pstime[0])
	{
		if (splitscreen)
			V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(184), 0, pstime);
		else
			V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(192), 0, pstime);
	}
}

static void ST_drawCTFHUD(void)
{
	INT32 i, team;
	UINT16 whichflag;
	team = whichflag = 0;

	for (i = 0; i < MAXPLAYERS; i++)
	{
		if (players[i].gotflag & MF_REDFLAG)
		{
			team = players[i].ctfteam;
			whichflag = players[i].gotflag;
			break; // break, don't continue.
		}
	}

	// Draw the flags
	if (splitscreen)
	{
		V_DrawSmallScaledPatch(256, STRINGY(160), 0, rflagico);
		V_DrawSmallScaledPatch(288, STRINGY(160), 0, bflagico);
	}
	else
	{
		V_DrawSmallScaledPatch(256, STRINGY(176), 0, rflagico);
		V_DrawSmallScaledPatch(288, STRINGY(176), 0, bflagico);
	}

	if (stplyr->ctfteam != team && team > 0 && ((stplyr->ctfteam == 1 && whichflag & MF_REDFLAG)
		|| (stplyr->ctfteam == 2 && whichflag & MF_BLUEFLAG)))
	{
		INT32 x;

		if (whichflag & MF_REDFLAG)
			x = 256;
		else
			x = 288;

		// OTHER TEAM HAS YOUR FLAG!
		if (splitscreen)
			V_DrawScaledPatch(x, STRINGY(156), 0, nonicon);
		else
			V_DrawScaledPatch(x, STRINGY(156+16), 0, nonicon);
	}
	else if (stplyr->ctfteam == team && team > 0)
	{
		INT32 x;

		if (whichflag & MF_REDFLAG)
			x = 256;
		else
			x = 288;

		// YOUR TEAM HAS ENEMY FLAG!
		if (splitscreen)
			V_DrawScaledPatch(x, STRINGY(156), 0, nonicon);
		else
			V_DrawScaledPatch(x, STRINGY(156+16), 0, nonicon);
	}

	team = whichflag = 0;

	for (i = 0; i < MAXPLAYERS; i++)
	{
		if (players[i].gotflag & MF_BLUEFLAG)
		{
			team = players[i].ctfteam;
			whichflag = players[i].gotflag;
			break; // break, don't continue.
		}
	}
	if (stplyr->ctfteam != team && team > 0 && ((stplyr->ctfteam == 1 && whichflag & MF_REDFLAG)
		|| (stplyr->ctfteam == 2 && whichflag & MF_BLUEFLAG)))
	{
		INT32 x;

		if (whichflag & MF_REDFLAG)
			x = 256;
		else
			x = 288;

		// OTHER TEAM HAS YOUR FLAG!
		if (splitscreen)
			V_DrawScaledPatch(x, STRINGY(156), 0, nonicon);
		else
			V_DrawScaledPatch(x, STRINGY(156+16), 0, nonicon);
	}
	else if (stplyr->ctfteam == team && team > 0)
	{
		INT32 x;

		if (whichflag & MF_REDFLAG)
			x = 256;
		else
			x = 288;

		// YOUR TEAM HAS ENEMY FLAG!
		if (splitscreen)
			V_DrawScaledPatch(x, STRINGY(156), 0, nonicon);
		else
			V_DrawScaledPatch(x, STRINGY(156+16), 0, nonicon);
	}

	if (stplyr->gotflag & MF_REDFLAG)
	{
		// YOU HAVE THE RED FLAG
		if (splitscreen)
			V_DrawScaledPatch(224, STRINGY(160), 0, gotrflag);
		else
			V_DrawScaledPatch(224, STRINGY(176), 0, gotrflag);
	}
	else if (stplyr->gotflag & MF_BLUEFLAG)
	{
		// YOU HAVE THE BLUE FLAG
		if (splitscreen)
			V_DrawScaledPatch(224, STRINGY(160), 0, gotbflag);
		else
			V_DrawScaledPatch(224, STRINGY(176), 0, gotbflag);
	}
	if (stplyr->ctfteam == 1)
	{
		if (splitscreen)
			V_DrawString(256, STRINGY(184), V_TRANSLUCENT, "RED TEAM");
		else
			V_DrawString(256, STRINGY(192), V_TRANSLUCENT, "RED TEAM");
	}
	else if (stplyr->ctfteam == 2)
	{
		if (splitscreen)
			V_DrawString(248, STRINGY(184), V_TRANSLUCENT, "BLUE TEAM");
		else
			V_DrawString(248, STRINGY(192), V_TRANSLUCENT, "BLUE TEAM");
	}
	else
	{
		if (splitscreen)
			V_DrawString(244, STRINGY(184), V_TRANSLUCENT, "SPECTATOR");
		else
			V_DrawString(244, STRINGY(192), V_TRANSLUCENT, "SPECTATOR");
	}

	// Display a countdown timer showing how much time left until the flag your team dropped returns to base.
	{
		char timeleft[33];
		if (redflag && redflag->fuse)
		{
			sprintf(timeleft, "%u", (redflag->fuse / TICRATE));
			V_DrawCenteredString(268, STRINGY(184), V_YELLOWMAP, timeleft);
		}

		if (blueflag && blueflag->fuse)
		{
			sprintf(timeleft, "%u", (blueflag->fuse / TICRATE));
			V_DrawCenteredString(300, STRINGY(184), V_YELLOWMAP, timeleft);
		}
	}
}

// Though in some ways similar to CTF's hud, team match has enough
// exclusions to warrant a seperate subroutine.
// At this point, CTF minus the extra stuff. May have other unique stuff later.
static void ST_drawTeamMatchHUD(void)
{
	//Draw team name
	switch (stplyr->ctfteam)
	{
	case 1:
		if (splitscreen)
			V_DrawString(256, STRINGY(184), V_TRANSLUCENT, "RED TEAM");
		else
			V_DrawString(256, STRINGY(192), V_TRANSLUCENT, "RED TEAM");
		break;
	case 2:
		if (splitscreen)
			V_DrawString(248, STRINGY(184), V_TRANSLUCENT, "BLUE TEAM");
		else
			V_DrawString(248, STRINGY(192), V_TRANSLUCENT, "BLUE TEAM");
		break;
	default: //spectators have no team.
		if (splitscreen)
			V_DrawString(244, STRINGY(184), V_TRANSLUCENT, "SPECTATOR");
		else
			V_DrawString(244, STRINGY(192), V_TRANSLUCENT, "SPECTATOR");
	}
}

#ifdef CHAOSISNOTDEADYET
static void ST_drawChaosHUD(void)
{
	char chains[33];
	sprintf(chains, "CHAINS: %u", stplyr->scoreadd);
	V_DrawString(8, STRINGY(184), V_TRANSLUCENT, chains);
}
#endif

static void ST_drawSpecialStageHUD(void)
{
	if (hu_showscores && (netgame || multiplayer))
		return; //hide in netplay only

	if (totalrings > 0)
	{
		if (splitscreen)
			ST_DrawOverlayNum(SCX(hudinfo[HUD_SS_TOTALRINGS_SPLIT].x), SCY(hudinfo[HUD_SS_TOTALRINGS_SPLIT].y), totalrings, tallnum);
		else
			ST_DrawOverlayNum(SCX(hudinfo[HUD_SS_TOTALRINGS].x), SCY(hudinfo[HUD_SS_TOTALRINGS].y), totalrings, tallnum);
	}

	if (leveltime < 5*TICRATE && totalrings > 0)
	{
		V_DrawScaledPatch(hudinfo[HUD_GETRINGS].x, (INT32)(SCY(hudinfo[HUD_GETRINGS].y)/vid.fdupy), V_TRANSLUCENT, getall);
		ST_DrawOverlayNum(SCX(hudinfo[HUD_GETRINGSNUM].x), SCY(hudinfo[HUD_GETRINGSNUM].y), totalrings, tallnum);
	}

	if (sstimer)
	{
		V_DrawString(hudinfo[HUD_TIMELEFT].x, STRINGY(hudinfo[HUD_TIMELEFT].y), 0, "TIME LEFT");
		ST_DrawNightsOverlayNum(SCX(hudinfo[HUD_TIMELEFTNUM].x), SCY(hudinfo[HUD_TIMELEFTNUM].y), sstimer/TICRATE, tallnum, 13);
	}
	else
		V_DrawScaledPatch(SCX(hudinfo[HUD_TIMEUP].x), SCY(hudinfo[HUD_TIMEUP].y), V_NOSCALESTART|V_TRANSLUCENT, timeup);
}

static void ST_drawContinueHUD(void)
{
	char stimeleft[33];
	patch_t *contsonic;
	// Do continue screen here.
	// Initialize music
	// For some reason the code doesn't like a simple ==...
	if (stplyr->deadtimer < gameovertics && stplyr->deadtimer > gameovertics - 2)
	{
		// Force a screen wipe

		stplyr->deadtimer--;

		S_ChangeMusic(mus_contsc, false);
		S_StopSounds();
		oncontinuescreen = true;

		if (rendermode != render_none)
		{
			// First, read the current screen
			F_WipeStartScreen();

			// Then, draw what the new screen will look like.
			V_DrawFill(0, 0, vid.width, vid.height, 31);

			contsonic = W_CachePatchName("CONT1", PU_CACHE);
			V_DrawScaledPatch((BASEVIDWIDTH-SHORT(contsonic->width))/2, 64, 0, contsonic);
			V_DrawString(128,128,0, "CONTINUE?");
			sprintf(stimeleft, "%d", (stplyr->deadtimer - (gameovertics-11*TICRATE))/TICRATE);
			V_DrawString(stplyr->deadtimer >= (gameovertics-TICRATE) ? 152 : 160,144,0, stimeleft);

			// Now, read the end screen we want to fade to.
			F_WipeEndScreen(0, 0, vid.width, vid.height);

			// Do the wipe-io!
			F_RunWipe(2*TICRATE, true);
		}
	}

	V_DrawFill(0, 0, vid.width, vid.height, 31);
	V_DrawString(128, 128, 0, "CONTINUE?");
	// Draw a Sonic!
	contsonic = W_CachePatchName("CONT1", PU_CACHE);
	V_DrawScaledPatch((BASEVIDWIDTH - SHORT(contsonic->width))/2, 64, 0, contsonic);
	sprintf(stimeleft, "%d", (stplyr->deadtimer - (gameovertics-11*TICRATE))/TICRATE);
	V_DrawString(stplyr->deadtimer >= (gameovertics-TICRATE) ? 152 : 160, 144, 0, stimeleft);
	if (stplyr->deadtimer < (gameovertics-10*TICRATE))
		Command_ExitGame_f();
	if (stplyr->deadtimer < gameovertics-TICRATE && (stplyr->cmd.buttons & BT_JUMP || stplyr->cmd.buttons & BT_USE))
	{
		if (stplyr->continues != -1)
			stplyr->continues--;

		// Reset score
		stplyr->score = 0;

		// Allow tokens to come back if not a netgame.
		if (!(netgame || multiplayer))
		{
			tokenlist = 0;
			token = 0;
			imcontinuing = true;
		}

		// Reset # of lives
		if (ultimatemode)
			stplyr->lives = 1;
		else
			stplyr->lives = 3;

		// Clear any starpost data
		stplyr->starpostangle = 0;
		stplyr->starpostbit = 0;
		stplyr->starpostnum = 0;
		stplyr->starposttime = 0;
		stplyr->starpostx = 0;
		stplyr->starposty = 0;
		stplyr->starpostz = 0;
		contsonic = W_CachePatchName("CONT2", PU_CACHE);
		V_DrawScaledPatch((BASEVIDWIDTH - SHORT(contsonic->width))/2, 64, 0, contsonic);
	}
}

static void ST_drawEmeraldHuntIcon(mobj_t *hunt, INT32 graphic)
{
	patch_t *p;
	INT32 interval;
	fixed_t dist = P_AproxDistance(P_AproxDistance(stplyr->mo->x - hunt->x, stplyr->mo->y - hunt->y),
		stplyr->mo->z - hunt->z);

	if (dist < 128<<FRACBITS)
	{
		p = homing6;
		interval = 5;
	}
	else if (dist < 512<<FRACBITS)
	{
		p = homing5;
		interval = 10;
	}
	else if (dist < 1024<<FRACBITS)
	{
		p = homing4;
		interval = 20;
	}
	else if (dist < 2048<<FRACBITS)
	{
		p = homing3;
		interval = 30;
	}
	else if (dist < 3072<<FRACBITS)
	{
		p = homing2;
		interval = 35;
	}
	else
	{
		p = homing1;
		interval = 0;
	}

	V_DrawScaledPatch(hudinfo[graphic].x, STRINGY(hudinfo[graphic].y), V_TRANSLUCENT, p);
	if (interval > 0 && leveltime % interval == 0)
		S_StartSound(NULL, sfx_emfind);
}

// Draw the status bar overlay, customisable: the user chooses which
// kind of information to overlay
//
/// \todo Split up this 1400 line function into multiple functions!
//
static void ST_overlayDrawer(void)
{
	// lives status
#ifdef RIDERS
	if (!(riderslevel || kartmode) && ((gametype == GT_COOP || gametype == GT_RACE) && !(hu_showscores && (netgame || multiplayer))))
#else
	if ((gametype == GT_COOP || gametype == GT_RACE) && !(hu_showscores && (netgame || multiplayer)))
#endif
	{
		if ((stplyr->powers[pw_super]) || (stplyr->pflags & PF_NIGHTSMODE))
		{
			if (!stplyr->skincolor) // 'default' color
			{
				V_DrawSmallScaledPatch(SCX(hudinfo[HUD_LIVESPIC].x), SCY(hudinfo[HUD_LIVESPIC].y - (splitscreen ? 8 : 0)),
					V_NOSCALESTART|V_TRANSLUCENT,superprefix[stplyr->skin]);
			}
			else
			{
				const UINT8 *colormap = (UINT8 *)translationtables[stplyr->skin] - 256 + (((stplyr->powers[pw_super]) ? 15 : stplyr->skincolor)<<8);
				V_DrawSmallMappedPatch(SCX(hudinfo[HUD_LIVESPIC].x), SCY(hudinfo[HUD_LIVESPIC].y - (splitscreen ? 8 : 0)),
					V_NOSCALESTART|V_TRANSLUCENT,superprefix[stplyr->skin], colormap);
			}
		}
		else
		{
			if (!stplyr->skincolor) // 'default' color
			{
				V_DrawSmallScaledPatch(SCX(hudinfo[HUD_LIVESPIC].x), SCY(hudinfo[HUD_LIVESPIC].y - (splitscreen ? 8 : 0)),
					V_NOSCALESTART|V_TRANSLUCENT,faceprefix[stplyr->skin]);
			}
			else
			{
				const UINT8 *colormap = (UINT8 *)translationtables[stplyr->skin] - 256 + ((stplyr->skincolor)<<8);
				V_DrawSmallMappedPatch(SCX(hudinfo[HUD_LIVESPIC].x), SCY(hudinfo[HUD_LIVESPIC].y - (splitscreen ? 8 : 0)),
					V_NOSCALESTART|V_TRANSLUCENT,faceprefix[stplyr->skin], colormap);
			}
		}

		if (splitscreen) // raise a bit
		{
			V_DrawScaledPatch(SCX(hudinfo[HUD_LIVESNAME].x), SCY(hudinfo[HUD_LIVESNAME].y - 16),
				V_NOSCALESTART|V_TRANSLUCENT, facenameprefix[stplyr->skin]);

			// draw the number of lives
			ST_DrawOverlayNum(SCX(hudinfo[HUD_LIVESNUM].x), SCY(hudinfo[HUD_LIVESNUM].y - 4), stplyr->lives, tallnum);

			// now draw the "x"
			V_DrawScaledPatch(SCX(hudinfo[HUD_LIVESX].x), SCY(hudinfo[HUD_LIVESX].y - 2), V_NOSCALESTART|V_TRANSLUCENT, stlivex);
		}
		else
		{
			V_DrawScaledPatch(SCX(hudinfo[HUD_LIVESNAME].x), SCY(hudinfo[HUD_LIVESNAME].y),
				V_NOSCALESTART|V_TRANSLUCENT, facenameprefix[stplyr->skin]);

			// draw the number of lives
			ST_DrawOverlayNum(SCX(hudinfo[HUD_LIVESNUM].x), SCY(hudinfo[HUD_LIVESNUM].y), stplyr->lives, tallnum);

			// now draw the "x"
			V_DrawScaledPatch(SCX(hudinfo[HUD_LIVESX].x), SCY(hudinfo[HUD_LIVESX].y), V_NOSCALESTART|V_TRANSLUCENT, stlivex);
		}
	}

	if ((maptol & TOL_NIGHTS) && !(hu_showscores && (netgame || multiplayer)))
	{
		ST_drawNiGHTSHUD();
	}
#ifdef RIDERS
	if (riderslevel)
	{
		ST_drawRidersHUD();
	}
	if (kartmode)
	{
		ST_drawKartHUD();
	}
	if (gametype == GT_RACE)
	{
		//draw the starpost time!
		ST_drawCheckPointTimes();
	}
	if (!(riderslevel || kartmode) && !(hu_showscores && (netgame || multiplayer))) //hu_showscores = auto hide score/time/rings when tab rankings are shown
#else
	else if (!(hu_showscores && (netgame || multiplayer))) //hu_showscores = auto hide score/time/rings when tab rankings are shown
#endif
	{
		if (splitscreen)
		{
			// rings counter
			ST_DrawOverlayNum(SCX(hudinfo[HUD_RINGSNUMSPLIT].x), SCY(hudinfo[HUD_RINGSNUMSPLIT].y), stplyr->health > 0 ? stplyr->health - 1 : 0,
				tallnum);

			if (stplyr->health <= 1 && leveltime/5 & 1)
				V_DrawScaledPatch(SCX(hudinfo[HUD_RINGSSPLIT].x), SCY(hudinfo[HUD_RINGSSPLIT].y), V_NOSCALESTART|V_TRANSLUCENT, rrings);
			else
				V_DrawScaledPatch(SCX(hudinfo[HUD_RINGSSPLIT].x), SCY(hudinfo[HUD_RINGSSPLIT].y), V_NOSCALESTART|V_TRANSLUCENT, sborings);
		}
		else
		{
			if (!useNightsSS && gamemap >= sstage_start && gamemap <= sstage_end)
			{
				INT32 ringscollected = 0; // Total # everyone has collected
				INT32 i;

				for (i = 0; i < MAXPLAYERS; i++)
					if (playeringame[i] && players[i].mo && players[i].mo->health > 1)
						ringscollected += players[i].mo->health - 1;

				ST_DrawOverlayNum(SCX(hudinfo[HUD_RINGSNUM].x), SCY(hudinfo[HUD_RINGSNUM].y), ringscollected, tallnum);
			}
			else
			{
				ST_DrawOverlayNum(SCX(hudinfo[HUD_RINGSNUM].x), SCY(hudinfo[HUD_RINGSNUM].y), stplyr->health > 0 ? stplyr->health-1 : 0,
					tallnum);
			}

			if (stplyr->health <= 1 && leveltime/5 & 1)
				V_DrawScaledPatch(SCX(hudinfo[HUD_RINGS].x), SCY(hudinfo[HUD_RINGS].y), V_NOSCALESTART|V_TRANSLUCENT, rrings);
			else
				V_DrawScaledPatch(SCX(hudinfo[HUD_RINGS].x), SCY(hudinfo[HUD_RINGS].y), V_NOSCALESTART|V_TRANSLUCENT, sborings);
		}

		// draw score (same in splitscreen as normal, too!)
		ST_DrawOverlayNum(SCX(hudinfo[HUD_SCORENUM].x), SCY(hudinfo[HUD_SCORENUM].y), stplyr->score, tallnum);
		V_DrawScaledPatch(SCX(hudinfo[HUD_SCORE].x), SCY(hudinfo[HUD_SCORE].y), V_NOSCALESTART|V_TRANSLUCENT, sboscore);

		if (splitscreen)
		{
			INT32 seconds;

			if (cv_objectplace.value)
				seconds = objectsdrawn%100;
			else
				seconds = stplyr->realtime/TICRATE % 60;

			if (seconds < 10)
				ST_DrawOverlayNum(SCX(hudinfo[HUD_LOWSECONDSSPLIT].x), SCY(hudinfo[HUD_LOWSECONDSSPLIT].y), 0, tallnum);

			// seconds time
			if (cv_objectplace.value)
				ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDSSPLIT].x), SCY(hudinfo[HUD_SECONDSSPLIT].y), objectsdrawn%100, tallnum);
			else
				ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDSSPLIT].x), SCY(hudinfo[HUD_SECONDSSPLIT].y), stplyr->realtime/TICRATE % 60, tallnum);

			// minutes time
			if (cv_objectplace.value)
				ST_DrawOverlayNum(SCX(hudinfo[HUD_MINUTESSPLIT].x), SCY(hudinfo[HUD_MINUTESSPLIT].y), objectsdrawn/100, tallnum);
			else
				ST_DrawOverlayNum(SCX(hudinfo[HUD_MINUTESSPLIT].x), SCY(hudinfo[HUD_MINUTESSPLIT].y), stplyr->realtime/(60*TICRATE), tallnum);

			// colon location
			V_DrawScaledPatch(SCX(hudinfo[HUD_TIMECOLONSPLIT].x), SCY(hudinfo[HUD_TIMECOLONSPLIT].y), V_NOSCALESTART|V_TRANSLUCENT, sbocolon);

			// TIME location
			V_DrawScaledPatch(SCX(hudinfo[HUD_TIMESPLIT].x), SCY(hudinfo[HUD_TIMESPLIT].y), V_NOSCALESTART|V_TRANSLUCENT, sbotime);
		}
		else if (cv_timetic.value == 1) // show tics instead of MM : SS
		{
			ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDS].x), SCY(hudinfo[HUD_SECONDS].y), stplyr->realtime, tallnum);

			// TIME location
			V_DrawScaledPatch(SCX(hudinfo[HUD_TIME].x), SCY(hudinfo[HUD_TIME].y), V_NOSCALESTART|V_TRANSLUCENT, sbotime);
		}
		else
		{
			INT32 seconds;

			if (cv_objectplace.value)
				seconds = objectsdrawn%100;
			else
				seconds = G_TicsToSeconds(stplyr->realtime);

			if (seconds < 10)
				ST_DrawOverlayNum(SCX(hudinfo[HUD_LOWSECONDS].x), SCY(hudinfo[HUD_LOWSECONDS].y), 0, tallnum);

			// seconds time
			if (cv_objectplace.value)
				ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDS].x), SCY(hudinfo[HUD_SECONDS].y), objectsdrawn%100, tallnum);
			else
				ST_DrawOverlayNum(SCX(hudinfo[HUD_SECONDS].x), SCY(hudinfo[HUD_SECONDS].y), G_TicsToSeconds(stplyr->realtime), tallnum);

				// minutes time
			if (cv_objectplace.value)
				ST_DrawOverlayNum(SCX(hudinfo[HUD_MINUTES].x), SCY(hudinfo[HUD_MINUTES].y), objectsdrawn/100, tallnum);
			else
				ST_DrawOverlayNum(SCX(hudinfo[HUD_MINUTES].x), SCY(hudinfo[HUD_MINUTES].y), G_TicsToMinutes(stplyr->realtime, true), tallnum);

			// colon location
			V_DrawScaledPatch(SCX(hudinfo[HUD_TIMECOLON].x), SCY(hudinfo[HUD_TIMECOLON].y), V_NOSCALESTART|V_TRANSLUCENT, sbocolon);

			// Show tics, too!
			if (timeattacking || cv_timetic.value == 2)
			{
				INT32 tics = G_TicsToCentiseconds(stplyr->realtime);

				if (tics < 10)
					ST_DrawOverlayNum(SCX(hudinfo[HUD_LOWTICS].x), SCY(hudinfo[HUD_LOWTICS].y), 0, tallnum);

				// colon location
				V_DrawScaledPatch(SCX(hudinfo[HUD_TIMETICCOLON].x), SCY(hudinfo[HUD_TIMETICCOLON].y), V_NOSCALESTART|V_TRANSLUCENT, sbocolon);
				ST_DrawOverlayNum(SCX(hudinfo[HUD_TICS].x), SCY(hudinfo[HUD_TICS].y), tics, tallnum);
			}

			// TIME location
			V_DrawScaledPatch(SCX(hudinfo[HUD_TIME].x), SCY(hudinfo[HUD_TIME].y), V_NOSCALESTART|V_TRANSLUCENT, sbotime);

		}
	}

	// GAME OVER pic
	if ((gametype == GT_COOP || gametype == GT_RACE) && stplyr->lives <= 0 && !(hu_showscores && (netgame || multiplayer)))
	{
		patch_t *p;

		if (countdown == 1)
			p = timeover;
		else
			p = sboover;

		V_DrawScaledPatch((BASEVIDWIDTH - SHORT(p->width))/2, STRINGY(BASEVIDHEIGHT/2 - (SHORT(p->height)/2)), 0, p);
	}

	if (cv_objectplace.value && stplyr->mo && stplyr->mo->target)
	{
		char x[8], y[8], z[8];
		char doomednum[8], thingflags[8];
		sprintf(x, "%d", stplyr->mo->x>>FRACBITS);
		sprintf(y, "%d", stplyr->mo->y>>FRACBITS);
		sprintf(z, "%d", stplyr->mo->z>>FRACBITS);
		sprintf(doomednum, "%d", stplyr->mo->target->info->doomednum);
		sprintf(thingflags, "%d", cv_objflags.value);
		V_DrawString(16, 98, 0, "X =");
		V_DrawString(48, 98, 0, x);
		V_DrawString(16, 108, 0, "Y =");
		V_DrawString(48, 108, 0, y);
		V_DrawString(16, 118, 0, "Z =");
		V_DrawString(48, 118, 0, z);
		V_DrawString(16, 128, 0, "thing # =");
		V_DrawString(16+84, 128, 0, doomednum);
		V_DrawString(16, 138, 0, "flags =");
		V_DrawString(16+56, 138, 0, thingflags);
		V_DrawString(16, 148, 0, "snap =");
		V_DrawString(16+48, 148, 0, cv_snapto.string);
	}

	if (!hu_showscores) // hide the following if TAB is held
	{
		// Countdown timer for Race Mode
		if (countdown)
		{
			char scountdown[33];
			sprintf(scountdown, "%d", countdown/TICRATE);
			V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(176), 0, scountdown);
		}

		// If you are in overtime, put a big honkin' flashin' message on the screen.
		if ((gametype == GT_MATCH || gametype == GT_CTF) && cv_overtime.value
			&& (leveltime > (timelimitintics + TICRATE/2)) && cv_timelimit.value && (leveltime/TICRATE % 2 == 0))
		{
			if (splitscreen)
				V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(168), 0, "OVERTIME!");
			else
				V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(184), 0, "OVERTIME!");
		}

		// Draw Match-related stuff
		//\note Match HUD is drawn no matter what gametype.
		// ... just not if you're a spectator.
		if (!((((gametype == GT_MATCH && !cv_matchtype.value) || gametype == GT_TAG) && stplyr->spectator)
		 || (((gametype == GT_MATCH && cv_matchtype.value) || gametype == GT_CTF) && !stplyr->ctfteam)))
			ST_drawMatchHUD();

		// Race HUD Stuff
		if (gametype == GT_RACE)
			ST_drawRaceHUD();
		// Tag HUD Stuff
		else if (gametype == GT_TAG)
			ST_drawTagHUD();
		// CTF HUD Stuff
		else if (gametype == GT_CTF)
			ST_drawCTFHUD();
		// Team Match HUD Stuff
		else if (gametype == GT_MATCH && cv_matchtype.value)
			ST_drawTeamMatchHUD();
		// Chaos HUD Stuff
#ifdef CHAOSISNOTDEADYET
		else if (gametype == GT_CHAOS)
			ST_drawChaosHUD();
#endif
	}

	// Special Stage HUD
	if (!useNightsSS && gamemap >= sstage_start && gamemap <= sstage_end)
		ST_drawSpecialStageHUD();

	if (!hu_showscores) // again, hide the following if TAB is held
	{
		// Emerald Hunt Indicators
		if (hunt1 && hunt1->health)
			ST_drawEmeraldHuntIcon(hunt1, HUD_HUNTPIC1);
		if (hunt2 && hunt2->health)
			ST_drawEmeraldHuntIcon(hunt2, HUD_HUNTPIC2);
		if (hunt3 && hunt3->health)
			ST_drawEmeraldHuntIcon(hunt3, HUD_HUNTPIC3);

		if (stplyr->powers[pw_gravityboots] > 3*TICRATE || (stplyr->powers[pw_gravityboots] && leveltime & 1))
			V_DrawScaledPatch(hudinfo[HUD_GRAVBOOTSICO].x, STRINGY(hudinfo[HUD_GRAVBOOTSICO].y), V_SNAPTORIGHT, gravboots);

		if(!P_IsLocalPlayer(stplyr))
		{
			char name[MAXPLAYERNAME+1];
			// shorten the name if its more than twelve characters.
			strlcpy(name, player_names[stplyr-players], 13);

			// Show name of player being displayed
			V_DrawCenteredString((BASEVIDWIDTH/6), BASEVIDHEIGHT-80, 0, "Viewpoint:");
			V_DrawCenteredString((BASEVIDWIDTH/6), BASEVIDHEIGHT-64, V_ALLOWLOWERCASE, name);
		}

		// This is where we draw all the fun cheese if you have the chasecam off!
		if ((stplyr == &players[consoleplayer] && !cv_chasecam.value)
			|| ((splitscreen && stplyr == &players[secondarydisplayplayer]) && !cv_chasecam2.value)
			|| (stplyr == &players[displayplayer] && !cv_chasecam.value))
		{
			ST_drawFirstPersonHUD();
		}
	}

	if (!(netgame || multiplayer) && !modifiedgame && gamemap == 11 && ALL7EMERALDS(emeralds)
		&& stplyr->mo && stplyr->mo->subsector && stplyr->mo->subsector->sector-sectors == 1361)
	{
		if (grade & 2048)
		{
			V_DrawCenteredString(BASEVIDWIDTH/2, 70, 0, "I, Pope Rededict XVI proclaim");
			V_DrawCenteredString(BASEVIDWIDTH/2, 80, 0, "AJ & Amy");
			V_DrawCenteredString(BASEVIDWIDTH/2, 90, 0, "Husband & Wife");
			V_DrawCenteredString(BASEVIDWIDTH/2, 100, 0, "on this day");
			V_DrawCenteredString(BASEVIDWIDTH/2, 110, 0, "May 16, 2009");

			P_GivePlayerRings(stplyr, 9999, true);
		}
		else
		{
			V_DrawCenteredString(BASEVIDWIDTH/2,  60, 0, "Oh... it's you again...");
			V_DrawCenteredString(BASEVIDWIDTH/2,  80, 0, "Look, I wanted to apologize for the way");
			V_DrawCenteredString(BASEVIDWIDTH/2,  90, 0, "I've acted in the past.");
			V_DrawCenteredString(BASEVIDWIDTH/2, 110, 0, "I've seen the error of my ways");
			V_DrawCenteredString(BASEVIDWIDTH/2, 120, 0, "and turned over a new leaf.");
			V_DrawCenteredString(BASEVIDWIDTH/2, 140, 0, "Instead of sending people to hell,");
			V_DrawCenteredString(BASEVIDWIDTH/2, 150, 0, "I now send them to heaven!");

			P_LinedefExecute(4200, stplyr->mo, stplyr->mo->subsector->sector);
			P_LinedefExecute(4201, stplyr->mo, stplyr->mo->subsector->sector);
			stplyr->mo->momx = stplyr->mo->momy = 0;
		}
	}

	if (mariomode && stplyr->exiting)
	{
		/// \todo doesn't belong in status bar code AT ALL
		thinker_t *th;
		mobj_t *mo2;
		boolean foundtoad = false;

		// scan the remaining thinkers
		// to find toad
		for (th = thinkercap.next; th != &thinkercap; th = th->next)
		{
			if (th->function.acp1 != (actionf_p1)P_MobjThinker)
				continue;

			mo2 = (mobj_t *)th;
			if (mo2->type == MT_TOAD)
			{
				foundtoad = true;
				break;
			}
		}

		if (foundtoad)
		{
			V_DrawCenteredString(160, 32+16, 0, "Thank you!");
			V_DrawCenteredString(160, 44+16, 0, "But our earless leader is in");
			V_DrawCenteredString(160, 56+16, 0, "another castle!");
		}
	}

	// draw level title Tails
	if (*mapheaderinfo[gamemap-1].lvlttl != '\0' && !(hu_showscores && (netgame || multiplayer)))
		ST_drawLevelTitle();

	if (!hu_showscores && netgame && (gametype == GT_RACE || gametype == GT_COOP) && stplyr->lives <= 0 && displayplayer == consoleplayer && countdown != 1)
		V_DrawCenteredString(BASEVIDWIDTH/2, BASEVIDHEIGHT/3, 0, "Press F12 to watch another player.");

	if (!hu_showscores && netgame && (gametype == GT_TAG && cv_tagtype.value) && displayplayer == consoleplayer &&
		(!stplyr->spectator && !(stplyr->pflags & PF_TAGIT)) && (leveltime > hidetime * TICRATE))
	{
		V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(84), 0, "You cannot move while hiding.");
		V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(116), 0, "Press F12 to watch another player.");
	}

	if (!hu_showscores && (netgame || splitscreen))
	{
		if ((gametype == GT_MATCH || gametype == GT_TAG || gametype == GT_CTF)
			&& stplyr->playerstate == PST_DEAD && stplyr->lives) //Death overrides spectator text.
		{
			V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(132), V_TRANSLUCENT, "Press Jump to respawn.");
			if (((gametype == GT_MATCH && !cv_matchtype.value) || gametype == GT_TAG) && !stplyr->spectator)
				V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(148), V_TRANSLUCENT, "Press 'Toss Flag' to Spectate.");
		}
		else if ((((gametype == GT_RACE && !cv_matchtype.value) || gametype == GT_TAG) && stplyr->spectator)
		 || (((gametype == GT_RACE && cv_matchtype.value) || gametype == GT_CTF) && !stplyr->ctfteam))
		{
			V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(60), V_TRANSLUCENT, "You are a spectator.");
			if ((gametype == GT_RACE && cv_matchtype.value) || gametype == GT_CTF)
				V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(132), V_TRANSLUCENT, "Press Fire to be assigned to a team.");
			else
				V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(132), V_TRANSLUCENT, "Use 'Spectate' again to enter the game.");
			V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(148), V_TRANSLUCENT, "Press F12 to watch another player.");
			//V_DrawCenteredString(BASEVIDWIDTH/2, STRINGY(164), V_TRANSLUCENT, "Press Jump to float and Spin to sink.");
		}
	}

	if (stplyr->deadtimer > 0 && (stplyr->deadtimer < gameovertics) && stplyr->lives <= 0)
	{
		if (!netgame && !multiplayer)
		{
			if (stplyr->continues != 0) // Player has continues, so let's use them!
				ST_drawContinueHUD();
			else // Just go to the title screen
				Command_ExitGame_f();
		}
	}

	if (cv_debug == 2)
		ST_drawDebugInfo();
	else if (cv_debug)
	{
		if (stplyr->mo)
		{
			const fixed_t d = AngleFixed(stplyr->mo->angle);
			V_DrawString(252, 168, 0, va("X: %d", stplyr->mo->x>>FRACBITS));
			V_DrawString(252, 176, 0, va("Y: %d", stplyr->mo->y>>FRACBITS));
			V_DrawString(252, 184, 0, va("Z: %d", stplyr->mo->z>>FRACBITS));
			V_DrawString(252, 192, 0, va("A: %d", FixedInt(d)));
		}
	}
}
