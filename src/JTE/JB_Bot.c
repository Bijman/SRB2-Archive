// JB - JTE Bots
// By Jason the Echidna
// MINE! NO STEALY!

#include "../doomstat.h"
#include "../doomdef.h"

#include "../d_net.h"
#include "../g_game.h"
#include "../i_net.h"
#include "../p_local.h"
#include "../r_main.h"
#include "../r_things.h"
#include "../z_zone.h"

#include "JB_Bot.h"
#include "JB_Command.h"

#ifdef JTEBOTS

bot_t bots[MAXPLAYERS];
char charselbots[15][16];

static INT32 botline[MAXPLAYERS];
static INT32 botlinesize = 0;

void JB_BotWaitAdd(INT32 skin)
{
	if (skin == -1)
		return; //skin not found
	botline[botlinesize] = skin;
	botlinesize++;
}

void JB_AddWaitingBots(INT32 playernum)
{
	if (netgame)
		botlinesize = 0;
	while(botlinesize > 0)
	{
		botlinesize--;
		JB_BotAdd((UINT8)botline[botlinesize], playernum,
			(UINT8)atoi(skins[botline[botlinesize]].prefcolor),
			skins[botline[botlinesize]].name);
	}
}

boolean JB_BotAdd(UINT8 skin, INT32 playernum, UINT8 color, char *name)
{
	INT32 i;

	// Basic data validation...
	if (!playeringame[playernum]
	|| color > MAXSKINCOLORS
	|| !name || *name == '\0'
	|| (gametype == GT_CTF
		&& (color > 2 || color < 1)))
			return false;

	// Player num for new bot...
	for(i = 0; i < MAXPLAYERS && i < MAXNETNODES; i++)
		if (!nodeingame[i] && !playeringame[i])
			break;
	if (i >= MAXPLAYERS || i >= MAXNETNODES)
		return false;

	// Reset and link the structs...
	memset(&bots[i], 0, sizeof(bot_t));
	memset(&players[i], 0, sizeof(player_t));
	bots[i].player = &players[i];
	players[i].bot = &bots[i];

	// Add in the info...
	players[i].skin = skin;
	players[i].skincolor = color;
	bots[i].ownernum = (UINT8)playernum;

	// Special CTF color/team handling...
	if (gametype == GT_CTF)
	{
		players[i].ctfteam = color;
		switch(players[i].ctfteam)
		{
			case 1:
				players[i].skincolor = 6;
				break;
			case 2:
				players[i].skincolor = 7;
				break;
			default:
				break;
		}
	}

	// Copy in the player name...
	strncpy(player_names[i], name, MAXPLAYERNAME);
	player_names[i][MAXPLAYERNAME] = '\0';

	// Update the doomcom...
	if (i+1 > doomcom->numslots)
		doomcom->numslots = (INT16)(i+1);

	// Occupy the node...
	nodeingame[i] = true;
	playernode[i] = (UINT8)i;
	nodetoplayer[i] = (SINT8)i;
	playerpernode[i] = 1;

	// And... Play ball! Whoo!
	playeringame[i] = true;
	players[i].playerstate = PST_REBORN;
	CONS_Printf("Bot added\n");
	return true;
}

static inline void JB_CoopSpawnBot(INT32 botnum)
{
	player_t* player;
	bot_t* bot;
	mobj_t* mo;
	INT32 ownernum, i = 256;
	fixed_t botx, boty, botz;

	player = &players[botnum];
	bot = player->bot;
	ownernum = bot->ownernum;
	mo = players[ownernum].mo;

	if (!playeringame[ownernum] || !mo)
		return; // D: No owner?... No spawn...

	// Set the starting position of the bot: Try to be 256 units away and 256 units above, but decrease that until you're in the same sector.
	botx = mo->x + P_ReturnThrustX(mo, mo->angle, -i*FRACUNIT);
	boty = mo->y + P_ReturnThrustY(mo, mo->angle, -i*FRACUNIT);

	while(i > 0)
	{
		if (R_PointInSubsector(botx, boty)->sector != R_PointInSubsector(players[ownernum].mo->x, players[ownernum].mo->y)->sector
		|| R_PointInSubsector(botx + mobjinfo[MT_PLAYER].radius, boty + mobjinfo[MT_PLAYER].radius)->sector != R_PointInSubsector(botx, boty)->sector
		|| R_PointInSubsector(botx - mobjinfo[MT_PLAYER].radius, boty - mobjinfo[MT_PLAYER].radius)->sector != R_PointInSubsector(botx, boty)->sector)
		{
			botx = mo->x + P_ReturnThrustX(mo, mo->angle, -i*FRACUNIT);
			boty = mo->y + P_ReturnThrustY(mo, mo->angle, -i*FRACUNIT);
			i--;
		}
		else
			break;
	}

	botz = mo->z + i*FRACUNIT;
	if (botz > R_PointInSubsector(botx, boty)->sector->ceilingheight)
		botz = R_PointInSubsector(botx, boty)->sector->ceilingheight - mobjinfo[MT_PLAYER].height;

	mo = player->mo;

	// Set your position
	P_UnsetThingPosition(mo);
	mo->x = botx;
	mo->y = boty;
	mo->z = botz;
	P_SetThingPosition(mo);

	// If you were super already, restore yourself to your super state.
	/*if (player->powers[pw_super])
		P_SetPlayerMobjState(player->mo, S_PLAY_SUPERSTAND);
	else // If you had a shield, restore its visual significance.
		P_SpawnShieldOrb(player);*/

	// If you have no health, start with your owner's health.
	if (player->health <= 2 && players[bot->ownernum].health > 2)
		player->health = players[bot->ownernum].health;
	else
		player->health = 1;

	mo->health = player->health;
	P_SetTarget(&bot->target, players[ownernum].mo);

	mo->angle = R_PointToAngle2(mo->x, mo->y, players[ownernum].mo->x, players[ownernum].mo->y);

	if (mo->z > mo->floorz+mo->height)
	{
		switch(player->charability)
		{
			case 1:
				// Tails flys his way down
				P_SetPlayerMobjState(mo, S_PLAY_ABL1); // Change to the flying animation
				player->powers[pw_tailsfly] = tailsflytics + 1; // Set the fly timer
				break;

			default:
				// Otherwise, just fall out of the sky. :P
				P_SetPlayerMobjState(mo, S_PLAY_FALL1);
				break;
		}
	}
	else
		P_SetPlayerMobjState(mo, S_PLAY_FALL1);
}

void JB_SpawnBot(INT32 botnum)
{
	bot_t* bot;
//	botdontlook_t *look,*next;

	player_t* player;
	INT32 ownernum;

	if (!playeringame[botnum] || !players[botnum].bot)
		return;

	// Free the botdontlook_t list.
	//for(look = bots[botnum].targetlist; look; look = next)
	//{
	//	next = look->next;
	//	Z_Free(look);
	//}

	// This is G_DoBotRespawn, pretty much. :P
	player = &players[botnum];
	ownernum = bots[botnum].ownernum;

	// Clear the struct...
	memset(&bots[botnum],0,sizeof(bot_t));

	// And set the variables you saved...
	bot = &bots[botnum];
	bot->player = player;
	bot->ownernum = (UINT8)ownernum;

	// Set skin and color, dun matter what gametype.
	// No clue why SetPlayerSkinByNum needs to be
	// called here, but it does. Or else.
	SetPlayerSkinByNum(botnum, player->skin);
	player->mo->flags |= MF_TRANSLATION;
	player->mo->color = player->skincolor;

	// Gametype-specific spawning code.
	switch(gametype)
	{
		case GT_COOP:
			JB_CoopSpawnBot(botnum);
			break;

		default:
			break;
	}
}

#endif
