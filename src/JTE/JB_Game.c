// JB - JTE Bots
// By Jason the Echidna
// MINE! NO STEALY!

#include "../doomstat.h"
#include "../doomdef.h"

#include "../g_game.h"
#include "../p_local.h"
#include "../r_main.h"
#include "../z_zone.h"

#include "JB_Bot.h"
#include "JB_Command.h"
#include "JB_Game.h"
#include "../m_random.h"

#ifdef JTEBOTS

void A_BubbleSpawn(mobj_t *actor);
void A_BubbleCheck(mobj_t *actor);

#define ANGLE_10 0x071C71C7

void JB_LevelInit(void)
{
	memset(bots,0,sizeof(bots));
}

static boolean JB_AngleMove(player_t *player)
{
	if ((player->rmomx > 0 && player->rmomy > 0) && (/*player->mo->angle >= 0 &&*/ player->mo->angle < ANGLE_90)) // Quadrant 1
		return 1;
	else if ((player->rmomx < 0 && player->rmomy > 0) && (player->mo->angle >= ANGLE_90 && player->mo->angle < ANGLE_180)) // Quadrant 2
		return 1;
	else if ((player->rmomx < 0 && player->rmomy < 0) && (player->mo->angle >= ANGLE_180 && player->mo->angle < ANGLE_270)) // Quadrant 3
		return 1;
	else if ((player->rmomx > 0 && player->rmomy < 0) && (player->mo->angle >= ANGLE_270 || player->mo->angle <= ANGLE_45)) // Quadrant 4
		return 1;
	else if (player->rmomx > 0 && (player->mo->angle >= ANGLE_315 || player->mo->angle <= ANGLE_45))
		return 1;
	else if (player->rmomx < 0 && (player->mo->angle >= ANGLE_135 && player->mo->angle <= ANGLE_225))
		return 1;
	else if (player->rmomy > 0 && (player->mo->angle >= ANGLE_45 && player->mo->angle <= ANGLE_135))
		return 1;
	else if (player->rmomy < 0 && (player->mo->angle >= ANGLE_225 && player->mo->angle <= ANGLE_315))
		return 1;

	return 0;
}

// Decrease all don't look list entry timers and remove old ones
static inline void JB_UpdateLook(bot_t *bot)
{
	botdontlook_t *dontlook;

	if (!bot || gametype == GT_COOP)
		return;

	// Go through the list...
	for(dontlook = bot->targetlist; dontlook; dontlook = dontlook->next)
	{
		// Decrease timer...
		dontlook->timer--;

		// Timer ran out? Remove it!
		if (!dontlook->timer)
		{
			P_SetTarget(&dontlook->data, NULL);
			if (dontlook->next)
				dontlook->next->last = dontlook->last;
			else
				bot->targetlist->last = dontlook->last;
			if (!dontlook->last || dontlook->last == bot->targetlist->last)
				bot->targetlist = dontlook->next;
			else
				dontlook->last->next = dontlook->next;
			Z_Free(dontlook);

			// Only bother to remove one per tic.
			return;
		}
	}
}

#ifdef _X_AI_
///////////////////////
// ??? (Other stuff) //
///////////////////////

static boolean JB_CheckMove(INT32 botnum, fixed_t distance)
{
	mobj_t* mo = players[botnum].mo;

	fixed_t newx, newy, tmcz, tmfz;
	angle_t botangle = mo->angle;
	subsector_t *newsubsec;

	botangle >>= ANGLETOFINESHIFT;

	newx = mo->x + FixedMul(distance*FRACUNIT, finecosine[botangle]);
	newy = mo->y + FixedMul(distance*FRACUNIT, finesine[botangle]);
	newsubsec = R_PointInSubsector(newx, newy);
	tmfz = newsubsec->sector->floorheight;
	tmcz = newsubsec->sector->ceilingheight;
	if (newsubsec->sector->ffloors)
	{
		ffloor_t *rover;
		fixed_t delta1, delta2;
		INT32 thingtop = mo->z + mo->height;

		for (rover = newsubsec->sector->ffloors; rover; rover = rover->next)
		{
			if (!(rover->flags & FF_EXISTS))
				continue;

			if (!(rover->flags & FF_SOLID))
				continue;


			delta1 = mo->z - (*rover->bottomheight
				+ ((*rover->topheight - *rover->bottomheight)/2));
			delta2 = thingtop - (*rover->bottomheight
				+ ((*rover->topheight - *rover->bottomheight)/2));

			if (*rover->topheight > tmfz && abs(delta1) < abs(delta2))
				tmfz = *rover->topheight;

			if (*rover->bottomheight < tmcz && abs(delta1) >= abs(delta2)
				&& (!(rover->flags & FF_PLATFORM)))
				tmcz = *rover->bottomheight;

		}
	}

//	if (&players[displayplayer] == &players[botnum])
//		CONS_Printf("checkmove...");
	mo->flags |= MF_NOCLIPTHING; //oh for the love of God...
	if (!P_CheckPosition(mo, newx, newy))
		return false; // solid wall or thing
	mo->flags &= ~MF_NOCLIPTHING;

	if (tmcz - tmfz < mo->height)
		return false; // doesn't fit

	if (tmfz > mo->z || tmcz < mo->z + mo->height)
		return false;
//	if (&players[displayplayer] == &players[botnum])
//		CONS_Printf("successful checkmove %d %d.%d\n", tmfz/FRACUNIT, tmcz/FRACUNIT, mo->z/FRACUNIT);
	return true; //valid move
}
#endif

static void JB_RaceThink(INT32 botnum)
{
	player_t* player = &players[botnum];
	bot_t* bot = &bots[botnum];
	mobj_t* mo = players[botnum].mo;
	ticcmd_t* cmd = &players[botnum].cmd;

	boolean aimed;
	boolean onground = P_IsObjectOnGround(mo);

	// Turning movement
	aimed = false;
	cmd->angleturn = 0; // Command pending...
	if(!player->climbing)
	{
		if(JB_AngleMove(player))
			aimed = true;
		mo->angle += (cmd->angleturn<<16); // Set the angle of your mobj
		cmd->angleturn = (short)(mo->angle>>16); // And set that to your turning. For some reason.
	}
	cmd->angleturn |= TICCMD_RECEIVED; // Thee halth recieved thy command!

	// Forward movement.
#ifdef _X_AI_
	if((mo->state >= &states[S_PLAY_ATK1]) && (mo->state <= &states[S_PLAY_ATK4]))
		cmd->forwardmove = 0; // Don't do a damn thing if s/he's a fly type and s/he's flying.
	else if(mo->momz != 0 // If you're bouncing on a spring...
#else
	if(mo->momz > 0 // If you're bouncing on a spring...
#endif
		&& bot->springmove) // And you're already moving in a direction from it...
#ifdef _X_AI_
	{
		if (player->speed > 12 && !JB_CheckMove(botnum, 512+(player->speed*12)))
			cmd->forwardmove = -50/NEWTICRATERATIO; // WATCH OUT YOU'RE GONNA CRASH! AAAH!
		else if (mo->momz < 0 || JB_CheckMove(botnum, 1280+(player->speed*16)))
			cmd->forwardmove = 50/NEWTICRATERATIO; // Go full speed anyway.
		else
			cmd->forwardmove = 0; // Do nothing.
	}

#else
			cmd->forwardmove = 0; // Do nothing. Moving could ruin it.
#endif
	else // Otherwise...
		cmd->forwardmove = 50/NEWTICRATERATIO; // Go full speed. Always.

	// To make things interesting, make bots use their boosting power at random.
	if (onground // If you're on the ground...
		&& riderslevel // You're playing on a Riders level...
		&& player->powers[pw_boost] == 0 // Your timer is back to normal...
		&& player->powers[pw_exrun] == 1 // You're riding your Extreme Gear...
		&& player->score > 14 // You have enough air in your tank...
#ifdef _X_AI_
		&& !(P_Random() % (132 - player->score))) // And the dice rolled to a thumbs-up... (more often the more air you have)
#else
		&& !(P_Random() % 96)) // And the dice rolled to a thumbs-up...
#endif
		cmd->buttons |= BT_USE; // Then boost away!
}

void JB_BotThink(INT32 botnum)
{
	bot_t* bot = &bots[botnum];
	player_t* player = &players[botnum];
	ticcmd_t* cmd = &player->cmd;

	if (!playeringame[botnum] || !playeringame[player->bot->ownernum])
		return;

	if (player->spectator)
		player->spectator = 0;

	// If dead, try to respawn as soon as you're under the floor.
	if (player->playerstate == PST_DEAD)
	{
		if (player->mo->z + player->mo->height < player->mo->floorz) // Wait to be completely under the floor
		{
			if (gametype == GT_COOP) // In co-op, wait for your leader to stop, too.
			{
				player_t* owner = &players[player->bot->ownernum];
				if (owner->speed <= owner->normalspeed/2)
					cmd->buttons = BT_JUMP;
			}
			else
				cmd->buttons = BT_JUMP;
		}
		cmd->angleturn = TICCMD_RECEIVED; // Thee halth recieved thy command!
	}
	else if (player->playerstate == PST_LIVE && !cv_objectplace.value)
	{
		mobj_t* lasttarget = bot->target;

		JB_UpdateLook(&bots[botnum]);
		JB_RaceThink(botnum);

		if (bot->target != lasttarget)
			bot->targettimer = 0;
	}
}

#endif
