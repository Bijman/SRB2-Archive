// SC - SA City
// By Jason the Echidna
// MINE! NO STEALY!

#include "../doomstat.h"
#ifdef SACITY
#include "../d_player.h"
#include "../g_game.h"
#include "../i_net.h"
#include "../p_local.h"
#include "../r_main.h"
#include "../st_stuff.h"
#include "../v_video.h"
#include "../w_wad.h"
#include "../z_zone.h"

#include "SC_Info.h"
#include "SC_HUD.h"

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

#if 0
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

#endif

static inline int SCX(int x)
{
	return (int)(x * vid.fdupx);
}

void SC_HUDInit(void)
{
}

boolean SC_DrawHUD(void)
{
	switch(gamemap)
	{
		case SCM_WaitingZone:
			V_DrawString(16, STRINGY(24), 0, va("Players in waiting room: %d", doomcom->numslots));
			if(server)
				V_DrawString(16, STRINGY(24)+16, 0, "Press lightdash to begin.");
			break;

		case SCM_SACity:
			V_DrawString(4, STRINGY(4), 0, va("Rings: %d",stplyr->health-1));
			//if(menu)
			{
				V_DrawFill(hudinfo[HUD_LIGHTDASHBOX].x, hudinfo[HUD_LIGHTDASHBOX].y, 72, 12, 119);
				V_DrawCenteredString(hudinfo[HUD_LIGHTDASHSTRING1].x, hudinfo[HUD_LIGHTDASHSTRING1].y, V_TRANSLUCENT, "ACTIVATE");
				V_DrawCenteredString(hudinfo[HUD_LIGHTDASHSTRING2].x, hudinfo[HUD_LIGHTDASHSTRING2].y, V_TRANSLUCENT, "MENU");
			}
			break;

		default:
			return false;
	}
	return true;
}

#endif
