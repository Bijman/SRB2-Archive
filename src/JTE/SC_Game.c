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

#include "SC_Info.h"
#include "SC_Game.h"

void SC_GameplayStuff(player_t *player)
{
	ticcmd_t* cmd;
	static byte welcome = 0;

	if(!player || !player->mo)
		return;

	cmd = &player->cmd;

	switch(gamemap)
	{
		case SCM_WaitingZone:
			if(player-players == consoleplayer)
			{
				static boolean lightdashdown = false;
				if(leveltime < TICRATE*2)
					welcome = 0;
				else if(welcome == 0)
				{
					if(!netgame)
						CONS_Printf("SA City is only for netgames, you shall not progress from here.\n");
					else
					{
						COM_BufAddText("cechoflags 0\n");
						COM_BufAddText("cechoduration 10\n");
						COM_BufAddText("cecho Welcome to Waiting Zone!\\This is the entrance to SA City.\\You must have at least 2 players to begin.\n");
					}
					welcome = 1;
				}
				if(cmd->buttons & BT_LIGHTDASH && !lightdashdown)
				{
					lightdashdown = true;
					//if((doomcom->numslots > 1 && !cv_splitscreen.value)
					//|| (doomcom->numslots > 2 && cv_splitscreen.value))
					{
						D_MapChange(SCM_SACity, GT_COOP, gameskill, 0, 1, false, false);
						return;
					}
					//else
						//CONS_Printf("Didn't you hear me? I said netgames only! You need more people!\n");
				}
				else if(!(cmd->buttons & BT_LIGHTDASH))
					lightdashdown = false;
			}

			// Disable some controls
			cmd->buttons &= ~BT_USE;
			if(player->mo->z > player->mo->floorz)
				cmd->buttons &= ~BT_JUMP;
			player->jumping = false;

			// Reset your position
			P_UnsetThingPosition(player->mo);
			player->mo->x = player->mo->y = 0;
			P_SetThingPosition(player->mo);

			// Make other players invisible
			if(player-players != displayplayer)
				player->mo->flags2 |= MF2_DONTDRAW;
			else
				player->mo->flags2 &= ~MF2_DONTDRAW;
			break;

		case SCM_SACity:
			break;

		default:
			break;
	}
}

#endif
