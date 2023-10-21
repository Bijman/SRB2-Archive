// SU - SRB2 Universe
// By Jason the Echidna
// MINE! NO STEALY!

#include "../../doomstat.h"
#ifdef UNIVERSE

#include "../../d_player.h"
#include "../../g_game.h"
#include "../../p_local.h"
#include "../../r_main.h"
#include "../../st_stuff.h"
#include "../../v_video.h"
#include "../../w_wad.h"
#include "../../z_zone.h"

#include "../JB_Bot.h"
#include "SU_Type.h"
#include "SU_HUD.h"

static patch_t* suhudbg;
static patch_t* suhudsmall;
static patch_t* suhudcolors[8];

static int STRINGY(int y)
{
	if(cv_splitscreen.value)
	{
		y >>= 1;
		if(stplyr != &players[displayplayer])
			y += BASEVIDHEIGHT / 2;
	}
	return y;
}

static int STRINGY2(int y)
{
	if(cv_splitscreen.value)
		if(stplyr != &players[displayplayer])
			y += BASEVIDHEIGHT / 2;
	return y;
}

static int SCY(int y)
{
	y = (int)(y * vid.fdupy); // scale to resolution
	if(cv_splitscreen.value)
	{
		y >>= 1;
		if(stplyr != &players[displayplayer])
			y += vid.height / 2;
	}
	return y;
}

static int SCY2(int y)
{
	y = (int)(y * vid.fdupy); // scale to resolution
	if(cv_splitscreen.value)
		if(stplyr != &players[displayplayer])
			y += vid.height / 2;
	return y;
}

static inline int SCX(int x)
{
	return (int)(x * vid.fdupx);
}

void SU_HUDInit(void)
{
	int i;
	char name[9];

	suhudbg = W_CachePatchName("SUHUDBG", PU_STATIC);
	suhudsmall = W_CachePatchName("SUHUDSM", PU_STATIC);

	for(i = 0; i < 8; i++)
	{
		sprintf(name, "SUHUD%d", i+1);
		suhudcolors[i] = W_CachePatchName(name, PU_STATIC);
	}

	hudinfo[HUD_SUBG].x = 8;
	hudinfo[HUD_SUBG].y = 16;
	hudinfo[HUD_SUHEALTH].x = 74;
	hudinfo[HUD_SUHEALTH].y = 0;
	hudinfo[HUD_SUHEALTHBAR].x = 30;
	hudinfo[HUD_SUHEALTHBAR].y = 8;
	hudinfo[HUD_SUMAGIC].x = 69;
	hudinfo[HUD_SUMAGIC].y = 10;
	hudinfo[HUD_SUMAGICBAR].x = 26;
	hudinfo[HUD_SUMAGICBAR].y = 19;
	hudinfo[HUD_SULEVEL].x = 42;
	hudinfo[HUD_SULEVEL].y = 20;
	hudinfo[HUD_SUNAME].x = 45;
	hudinfo[HUD_SUNAME].y = 23;
	hudinfo[HUD_SUCOLOR].x = 9;
	hudinfo[HUD_SUCOLOR].y = 7;
	hudinfo[HUD_SUSMALLBG].x = 0;
	hudinfo[HUD_SUSMALLBG].y = 3;
	hudinfo[HUD_SUSMALLHEALTHBAR].x = 14;
	hudinfo[HUD_SUSMALLHEALTHBAR].y = 2;
	hudinfo[HUD_SUSMALLLEVEL].x = 45;
	hudinfo[HUD_SUSMALLLEVEL].y = 4;
	hudinfo[HUD_SUSMALLNAME].x = 12;
	hudinfo[HUD_SUSMALLNAME].y = 12;
	hudinfo[HUD_SUSMALLCOLOR].x = 3;
	hudinfo[HUD_SUSMALLCOLOR].y = 2;
}

void SU_DrawHUD(void)
{
	int i, y;
	char health[5];
	char magic[5];
	char level[4];
	char name[13];

	int amount;
	int origamount;
	const int length = 44;
	const int lengthsmall = 32;
	player_t* player;

	if(timeinmap <= 1)
		return;

	// HUD background
	V_DrawScaledPatch(SCX(hudinfo[HUD_SUBG].x), SCY(hudinfo[HUD_SUBG].y), V_NOSCALESTART, suhudbg);
	V_DrawScaledPatch(SCX(hudinfo[HUD_SUBG].x + hudinfo[HUD_SUCOLOR].x), SCY(hudinfo[HUD_SUBG].y) + (int)(hudinfo[HUD_SUCOLOR].y*vid.fdupy), V_NOSCALESTART, suhudcolors[(stplyr-players)%8]);

	// HUD numbers.
	sprintf(health, "%d", stplyr->health   % 10000);
	sprintf(magic,  "%d", stplyr->sutp    % 10000);
	sprintf(level,  "%d", stplyr->sulevel %  1000);

	// Name!
	strncpy(name, player_names[stplyr-players], 12);
	// Only use the first part of the name... "Jason the Echidna" becomes "Jason"
	for(i = 0; i < 12; i++)
		if(name[i] == ' ')
			name[i] = '\0';
	name[i] = '\0'; // And of course, terminate the string when you reach the end either way.

	// Draw the numbers and the name.
	V_DrawRightAlignedString(hudinfo[HUD_SUBG].x + hudinfo[HUD_SUHEALTH].x, STRINGY(hudinfo[HUD_SUBG].y) + hudinfo[HUD_SUHEALTH].y, 0, health);
	V_DrawRightAlignedString(hudinfo[HUD_SUBG].x + hudinfo[HUD_SUMAGIC].x, STRINGY(hudinfo[HUD_SUBG].y) + hudinfo[HUD_SUMAGIC].y, 0, magic);
	V_DrawRightAlignedString(hudinfo[HUD_SUBG].x + hudinfo[HUD_SULEVEL].x, STRINGY(hudinfo[HUD_SUBG].y) + hudinfo[HUD_SULEVEL].y, 0, level);
	V_DrawString(hudinfo[HUD_SUBG].x + hudinfo[HUD_SUNAME].x, STRINGY(hudinfo[HUD_SUBG].y) + hudinfo[HUD_SUNAME].y, V_YELLOWMAP, name);

	// Health bar dupletion
	origamount = stplyr->sumaxhp;

	amount = (origamount - (stplyr->health%10000));
	amount = (int)(amount * ((float)length/origamount));

	if(amount > 0)
		V_DrawFill(hudinfo[HUD_SUBG].x + hudinfo[HUD_SUHEALTHBAR].x + length - amount, STRINGY(hudinfo[HUD_SUBG].y) + hudinfo[HUD_SUHEALTHBAR].y, amount, 3, 5);

	// Magic bar dupletion
	origamount = stplyr->sumaxtp;

	amount = (origamount - (stplyr->sutp%10000));
	amount = (int)(amount * ((float)length/origamount));

	if(amount > 0)
		V_DrawFill(hudinfo[HUD_SUBG].x + hudinfo[HUD_SUMAGICBAR].x + length - amount, STRINGY(hudinfo[HUD_SUBG].y) + hudinfo[HUD_SUMAGICBAR].y, amount, 3, 5);

	y = hudinfo[HUD_SUBG].y + hudinfo[HUD_SUNAME].y + 9;
	for(i = 0; i < MAXPLAYERS; i++)
	{
		if(!playeringame[i])
			continue;

		if(&players[i] == stplyr)
			continue;

		player = &players[i];

		// HUD background
		V_DrawScaledPatch(SCX(hudinfo[HUD_SUBG].x + hudinfo[HUD_SUSMALLBG].x), SCY2(y), V_NOSCALESTART, suhudsmall);
		V_DrawScaledPatch(SCX(hudinfo[HUD_SUBG].x + hudinfo[HUD_SUSMALLBG].x + hudinfo[HUD_SUSMALLCOLOR].x), SCY2(y + hudinfo[HUD_SUSMALLCOLOR].y), V_NOSCALESTART, suhudcolors[(player-players)%8]);

		// HUD numbers.
		sprintf(level, "%d", player->sulevel%1000);

		// Name!
		strncpy(name, player_names[player-players], 12);
		// Only use the first part of the name... "Jason the Echidna" becomes "Jason"
		for(i = 0; i < 12; i++)
			if(name[i] == ' ')
				name[i] = '\0';
		name[i] = '\0'; // And of course, terminate the string when you reach the end either way.

		V_DrawRightAlignedString(hudinfo[HUD_SUBG].x + hudinfo[HUD_SUSMALLBG].x + hudinfo[HUD_SUSMALLLEVEL].x, STRINGY2(y) + hudinfo[HUD_SUSMALLLEVEL].y, 0, level);
		V_DrawString(hudinfo[HUD_SUBG].x + hudinfo[HUD_SUSMALLBG].x + hudinfo[HUD_SUSMALLNAME].x, STRINGY2(y) + hudinfo[HUD_SUSMALLNAME].y, V_YELLOWMAP, name);

		// Health bar dupletion
		origamount = player->sumaxhp;

		amount = (origamount - (player->health%10000));
		amount = (int)(amount * ((float)lengthsmall/origamount));

		if(amount > 0)
			V_DrawFill(hudinfo[HUD_SUBG].x + hudinfo[HUD_SUSMALLBG].x + hudinfo[HUD_SUSMALLHEALTHBAR].x + lengthsmall - amount, STRINGY2(y) + hudinfo[HUD_SUSMALLHEALTHBAR].y, amount, 3, 5);

		y += hudinfo[HUD_SUSMALLNAME].y + 9;
	}
}

#endif
