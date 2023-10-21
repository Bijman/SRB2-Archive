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
#include "SR_Info.h"

#ifdef JTEBOTS

void A_BubbleSpawn(mobj_t *actor);
void A_BubbleCheck(mobj_t *actor);

#ifdef SUCKYBOTWAYPOINTS
static botwaypoint_t *waypoints;
static botwaypoint_t *lastpoint;
#endif

void JB_LevelInit(void)
{
#ifdef SUCKYBOTWAYPOINTS
	waypoints = lastpoint = NULL;
#endif
	memset(bots,0,sizeof(bots));
}

static boolean JB_AngleMove(player_t *player)
{
	// This determines if the player is facing the direction they are travelling or not.
	// Didn't your teacher say to pay attention in Geometry/Trigonometry class? ;)
	if ((player->rmomx > 0 && player->rmomy > 0) && (/*player->mo->angle >= 0 &&*/ player->mo->angle < ANG90)) // Quadrant 1
		return 1;
	else if ((player->rmomx < 0 && player->rmomy > 0) && (player->mo->angle >= ANG90 && player->mo->angle < ANG180)) // Quadrant 2
		return 1;
	else if ((player->rmomx < 0 && player->rmomy < 0) && (player->mo->angle >= ANG180 && player->mo->angle < ANG270)) // Quadrant 3
		return 1;
	else if ((player->rmomx > 0 && player->rmomy < 0) && ((player->mo->angle >= ANG270 && (player->mo->angle <= ANGLE_MAX)) || (/*player->mo->angle >= 0 &&*/ player->mo->angle <= ANG45))) // Quadrant 4
		return 1;
	else if (player->rmomx > 0 && ((player->mo->angle >= ANG270+ANG45 && player->mo->angle <= ANGLE_MAX)))
		return 1;
	else if (player->rmomx < 0 && (player->mo->angle >= ANG90+ANG45 && player->mo->angle <= ANG180+ANG45))
		return 1;
	else if (player->rmomy > 0 && (player->mo->angle >= ANG45 && player->mo->angle <= ANG90+ANG45))
		return 1;
	else if (player->rmomy < 0 && (player->mo->angle >= ANG180+ANG45 && player->mo->angle <= ANG270+ANG45))
		return 1;
	else
		return 0;
}

////////////////////////////
// DON'T TARGET FUNCTIONS //
////////////////////////////

// Add a new entry to the bot's don't target list.
static void JB_DontTarget(mobj_t *mo, bot_t *bot)
{
	botdontlook_t *dontlook;
	if(!mo || !bot)
		return;

	// Not needed?...
	bot->targettimer = 0;

	// Malloc and link entry
	dontlook = Z_Malloc(sizeof(*dontlook), PU_LEVEL, NULL);
	if(!bot->targetlist)
		bot->targetlist = dontlook;
	else if(bot->targetlist->last)
		bot->targetlist->last->next = dontlook;
	bot->targetlist->last = dontlook;

	// Set entry data
	dontlook->data = mo;
	dontlook->timer = 5*TICRATE;
}

// Find if mobj on don't target list
static boolean JB_SearchTarget(mobj_t *mo, bot_t *bot)
{
	botdontlook_t *dontlook;
	if(!mo || !bot)
		return false;

	// Search the list...
	for(dontlook = bot->targetlist; dontlook; dontlook = dontlook->next)
	{
		// Found it?
		if(dontlook->data == mo)
			return true;
	}
	return false;
}

#ifdef SUCKYBOTWAYPOINTS
void JB_CreateWaypoint(fixed_t x, fixed_t y, fixed_t z, boolean spring)
{
	if(!lastpoint)
		waypoints = lastpoint = Z_Malloc(sizeof(*lastpoint), PU_LEVEL, NULL);
	else
	{
		lastpoint->next = Z_Malloc(sizeof(*lastpoint), PU_LEVEL, NULL);
		lastpoint = lastpoint->next;
	}
	lastpoint->x = x;
	lastpoint->y = y;
	lastpoint->z = z;
	lastpoint->sec = R_PointInSubsector(x,y)->sector;
	lastpoint->springpoint = spring; //&& (z > lastpoint->z);
	lastpoint->next = NULL;
}

void JB_UpdateWaypoints(void)
{
	unsigned short i;
	mobj_t *mo;

	if(gametype != GT_RACE)
		return;

	// Update waypoint list for all bots!
	for(i = 0; i < MAXPLAYERS; i++)
		if(playeringame[i] && !players[i].bot)
		{
			if(players[i].exiting && lastpoint)
				return;

			mo = players[i].mo;
			// Player has gone farther?
			// Add a new waypoint!
			if(!lastpoint //|| mo->subsector->sector != lastpoint->sec
			|| ((players[i].cmd.angleturn != (short)(players[i].mo->angle>>16) || players[i].cmd.sidemove)
				&& P_AproxDistance(lastpoint->x - mo->x, lastpoint->y - mo->y) > 128*FRACUNIT)
			|| (abs(mo->z-lastpoint->z) > MAXSTEPMOVE && mo->player->mfjumped))
			{
				JB_CreateWaypoint(mo->x, mo->y, mo->z, false);
				//JB_CreateWaypoint(mo->x+mo->momx, mo->y+mo->momy, mo->z+mo->momz, false);
			}
			return;
		}
}
#endif

// Decrease all don't look list entry timers and remove old ones
static inline void JB_UpdateLook(bot_t *bot)
{
#ifdef SUCKYBOTWAYPOINTS
	mobj_t *mo;
	botwaypoint_t *point;
#endif
	botdontlook_t *dontlook;

	if(!bot || gametype == GT_COOP)
		return;

#ifdef SUCKYBOTWAYPOINTS
	// Change waypoint?
	mo = bot->player->mo;
	if(!waypoints) // No waypoints at all?
		; // I guess you dun need 'em for this gametype, then!
	else if(!bot->waypoint) // No waypoint?
	{
		bot->waypoint = waypoints; // Start at the beginning.
		bot->waydist = P_AproxDistance(P_AproxDistance(
						mo->x - bot->waypoint->x,
						mo->y - bot->waypoint->y),
						mo->z - bot->waypoint->z);
	}
	else
	{
		if(P_AproxDistance(P_AproxDistance(
			mo->x - bot->waypoint->x,
			mo->y - bot->waypoint->y),
			mo->z - bot->waypoint->z) > bot->waydist+32*FRACUNIT)
		{
			// This loop finds the closest waypoint
			bot->waypoint = waypoints;
			for(point = waypoints->next; point; point = point->next)
			{
				if(P_AproxDistance(P_AproxDistance(
					mo->x - point->x,
					mo->y - point->y),
					mo->z - point->z)
					< P_AproxDistance(P_AproxDistance(
						mo->x - bot->waypoint->x,
						mo->y - bot->waypoint->y),
						mo->z - bot->waypoint->z))
							bot->waypoint = point; // Switch to it, then!
			}
		}

		// This loop goes to next waypoint and accounts for skipping
		// waypoints, both at once.
		for(point = bot->waypoint; point; point = point->next)
		{
			if(mo->subsector->sector == point->sec // In same sector as waypoint?
			// And close enough to it to switch to the next one?
			&& P_AproxDistance(P_AproxDistance(mo->x - point->x, mo->y - point->y),mo->z - point->z) < 128*FRACUNIT
			&& point->next) // And it has a next one?
			{
				bot->waypoint = point->next; // Switch to it, then!
				bot->waydist = P_AproxDistance(P_AproxDistance(
								mo->x - bot->waypoint->x,
								mo->y - bot->waypoint->y),
								mo->z - bot->waypoint->z);
				// No skipping springpoints.
				if(point->next->springpoint)
					break;
			}
		}

		// Mark the location of your waypoint for this tic.
		if(cv_debug || devparm)
		{
			mo = P_SpawnMobj(bot->waypoint->x, bot->waypoint->y, bot->waypoint->z, MT_REDXVI);
			mo->flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;
			mo->angle = bot->player->mo->angle;
			mo->fuse = 2;
		}
	}
#endif

	// Add to list?
	bot->targettimer++;
	if(bot->targettimer >= 3*TICRATE)
		JB_DontTarget(bot->target, bot);

	// Go through the list...
	for(dontlook = bot->targetlist; dontlook; dontlook = dontlook->next)
	{
		// Decrease timer...
		dontlook->timer--;

		// Timer ran out? Remove it!
		if(!dontlook->timer)
		{
			dontlook->data = NULL;
			if(dontlook->next)
				dontlook->next->last = dontlook->last;
			else
				bot->targetlist->last = dontlook->last;
			if(!dontlook->last || dontlook->last == bot->targetlist->last)
				bot->targetlist = dontlook->next;
			else
				dontlook->last->next = dontlook->next;
			Z_Free(dontlook);

			// Only bother to remove one per tic.
			return;
		}
	}
}

//////////////////////
// SEARCH FUNCTIONS //
//////////////////////

static mobj_t *JB_Look4Collect(int botnum)
{
	thinker_t* think;
	mobj_t *mo, *lastmo, *botmo = players[botnum].mo;
	bot_t *bot = &bots[botnum];
	fixed_t dist,lastdist;

	for(think = thinkercap.next, mo = lastmo = NULL, lastdist = 0;
		think != &thinkercap; think = think->next)
	{
		if(think->function.acp1 != (actionf_p1)P_MobjThinker) // Not a mobj thinker
			continue;

		mo = (mobj_t*)think;

		if(botmo == mo
		|| JB_SearchTarget(mo,bot))
			continue;
		switch(mo->type)
		{
			case MT_PLAYER:
				// If it's not REALLY a player or if it's not alive
				// just skip it... No point in worrying.
				if(!mo->player
				|| mo->player->playerstate != PST_LIVE
				|| mo->player->powers[pw_flashing]
				|| mo->player->powers[pw_invulnerability])
					continue;
				// Spectator
				if(gametype == GT_CTF && !mo->player->ctfteam)
					continue;
				// Same team
				if(gametype == GT_CTF && mo->player->ctfteam == botmo->player->ctfteam)
					continue;
				if(gametype == GT_MATCH && cv_teamplay.value == 1
				&& mo->player->skincolor == botmo->player->skincolor)
					continue;
				if(gametype == GT_MATCH && cv_teamplay.value == 2
				&& mo->player->skin == botmo->player->skin)
					continue;
				// If player is not close, I cannot see them,
				// or I don't have the rings to do anything about it anyway...
				// I do not worry.
				dist = P_AproxDistance(P_AproxDistance(botmo->x - mo->x, botmo->y - mo->y), botmo->z - mo->z);
				if(botmo->health <= 2
				|| mo->z > botmo->z + (128<<FRACBITS)
				// Don't go after them if you're not flashing...
				|| (!botmo->player->powers[pw_flashing]
					// And they're farther then 1024 units from you.
					&& dist > (1024<<FRACBITS))
				|| !P_CheckSight(botmo,mo))
							continue;
				// Otherwise... I worry.
				// I can't look for rings if I'm being watched.
				// I must fight, ready or not!
				return mo;

			case MT_RING:
			case MT_COIN:
			case MT_FLINGRING:
			case MT_FLINGCOIN:
			case MT_HOMINGRING:
			case MT_RAILRING:
			case MT_INFINITYRING:
			case MT_AUTOMATICRING:
			case MT_EXPLOSIONRING:
			case MT_EXTRALARGEBUBBLE: // Take bubbles too!
				// Yes! It's a ring! Score! :D
				if(mo->z < botmo->z + (128<<FRACBITS)
				&& P_CheckSight(botmo,mo))
					break;
				// Can't see it or don't think you can jump to it? Too bad...
				continue;

			default:
				// Monitor? Go for it.
				if(mo->flags & MF_MONITOR
				&& !(mo->flags & MF_NOCLIP)
				&& mo->health
				&& mo->z < botmo->z + (128<<FRACBITS)
				&& P_CheckSight(botmo,mo))
					break;
				// Check if a spring is the closest thing to you.
				// Only use it if you're within stepping distance
				// as well as closer to you then anything else
				// that you find... Otherwise, forget it.
				if(mo->flags & MF_SPRING
				&& botmo->state-states != S_PLAY_PLG1
				&& abs(botmo->z - mo->z) < 128<<FRACBITS
				&& P_CheckSight(botmo,mo))
					break;
				// Not anything I need to look at.
				continue;
		}
		dist = P_AproxDistance(P_AproxDistance(botmo->x - mo->x, botmo->y - mo->y), botmo->z - mo->z);
		if(lastmo && dist > lastdist) // Last one is closer to you?
			continue;

		// Found a target
		lastmo = mo;
		lastdist = dist;
	}
	return lastmo;
}

static mobj_t *JB_Look4Poppable(int botnum)
{
	thinker_t* think;
	mobj_t *mo, *lastmo, *botmo = players[botnum].mo;
	bot_t *bot = &bots[botnum];
	fixed_t dist,lastdist;

	// Simple co-op hack: Check things against your owner, rather then yourself.
	if(gametype == GT_COOP)
		botmo = players[bot->ownernum].mo;

	for(think = thinkercap.next, mo = lastmo = NULL, lastdist = 0;
		think != &thinkercap; think = think->next)
	{
		if(think->function.acp1 != (actionf_p1)P_MobjThinker) // Not a mobj thinker
			continue;

		mo = (mobj_t*)think;

		if(botmo == mo
		|| JB_SearchTarget(mo,bot))
			continue;
		switch(mo->type)
		{
			case MT_RING:
			case MT_COIN:
			case MT_FLINGRING:
			case MT_FLINGCOIN:
			case MT_HOMINGRING:
			case MT_RAILRING:
			case MT_INFINITYRING:
			case MT_AUTOMATICRING:
			case MT_EXPLOSIONRING:
			case MT_EXTRALARGEBUBBLE: // Take bubbles too!
				// Yes! It's a ring! Score! :D
				if(mo->z < botmo->z + (128<<FRACBITS)
				&& P_CheckSight(botmo,mo))
					break;
				// Can't see it or don't think you can jump to it? Too bad...
				continue;

			default:
				// Enemy? Boss? Monitor? Kill it!
				if((mo->flags & MF_ENEMY
				|| mo->flags & MF_BOSS
				|| (mo->flags & MF_MONITOR
					&& !(mo->flags & MF_NOCLIP)
					&& gametype != GT_COOP))
				&& mo->health
				&& mo->z < botmo->z + (128<<FRACBITS)
				&& P_CheckSight(botmo,mo))
					break;
				// Not anything I need to look at.
				continue;
		}

		// Ah, this makes things easier... Only calculate the distance once per mobj.
		dist = P_AproxDistance(P_AproxDistance(botmo->x - mo->x, botmo->y - mo->y), botmo->z - mo->z);

		// In co-op, never go after anything farther then 512 fracunits away.
		if(gametype == GT_COOP && dist > 512<<FRACBITS)
			continue;

        // Last one is closer to you?
		if(lastmo && dist > lastdist)
			continue;

		// Found a target
		lastmo = mo;
		lastdist = dist;
	}
	return lastmo;
}

static inline mobj_t *JB_Look4Enemy(int botnum)
{
	thinker_t* think;
	mobj_t *mo, *lastmo, *botmo = players[botnum].mo;
	bot_t *bot = &bots[botnum];
	fixed_t dist,lastdist;

	for(think = thinkercap.next, mo = NULL, lastmo = NULL; think != &thinkercap; think = think->next)
	{
		if(think->function.acp1 != (actionf_p1)P_MobjThinker) // Not a mobj thinker
			continue;

		mo = (mobj_t*)think;

		if(botmo == mo
		|| JB_SearchTarget(mo,bot))
			continue;

		// Not an enemy or boss? Dun need it.
		if(!(mo->flags & MF_ENEMY
			|| mo->flags & MF_BOSS)
		|| !mo->health
		|| mo->z >= botmo->z + (128<<FRACBITS)
		|| !P_CheckSight(botmo,mo))
			continue;

		dist = P_AproxDistance(P_AproxDistance(botmo->x - mo->x, botmo->y - mo->y), botmo->z - mo->z);
		if(lastmo && dist > lastdist) // Last one is closer to you?
			continue;

		// Found a target
		lastmo = mo;
		lastdist = dist;
	}
	return lastmo;
}

static mobj_t *JB_Look4Players(int botnum)
{
	thinker_t* think;
	mobj_t *mo, *lastmo, *botmo = players[botnum].mo;
	bot_t *bot = &bots[botnum];
	fixed_t dist,lastdist;

	for(think = thinkercap.next, mo = lastmo = NULL, lastdist = 0;
		think != &thinkercap; think = think->next)
	{
		if(think->function.acp1 != (actionf_p1)P_MobjThinker) // Not a mobj thinker
			continue;

		mo = (mobj_t*)think;

		if(botmo == mo
		|| JB_SearchTarget(mo,bot))
			continue;
		switch(mo->type)
		{
			case MT_PLAYER:
				// If it's not REALLY a player or if it's not alive
				// just skip it... No point in chasing.
				if(!mo->player
				|| mo->player->playerstate != PST_LIVE
				|| mo->player->powers[pw_flashing]
				|| mo->player->powers[pw_invulnerability])
					continue;
				// Spectator
				if(gametype == GT_CTF && !mo->player->ctfteam)
					continue;
				// Same team
				if(gametype == GT_CTF && mo->player->ctfteam == botmo->player->ctfteam)
					continue;
				if(gametype == GT_MATCH && cv_teamplay.value == 1
				&& mo->player->skincolor == botmo->player->skincolor)
					continue;
				if(gametype == GT_MATCH && cv_teamplay.value == 2
				&& mo->player->skin == botmo->player->skin)
					continue;
				// If the player is visible, go for it.
				if(P_CheckSight(botmo,mo))
					break;
				continue;

			case MT_RING:
			case MT_COIN:
			case MT_FLINGRING:
			case MT_FLINGCOIN:
			case MT_HOMINGRING:
			case MT_RAILRING:
			case MT_INFINITYRING:
			case MT_AUTOMATICRING:
			case MT_EXPLOSIONRING:
			case MT_EXTRALARGEBUBBLE: // Take bubbles too!
				// A ring?... Only take
				// it if noone's around!
				if((!lastmo || lastmo->type != MT_PLAYER)
				&& mo->z < botmo->z + (128<<FRACBITS)
				&& P_CheckSight(botmo,mo))
					break;
				// Can't see it?! Too bad...
				continue;

			default:
				// Not anything I need to look at.
				continue;
		}

		dist = P_AproxDistance(P_AproxDistance(botmo->x - mo->x, botmo->y - mo->y), botmo->z - mo->z);
		if(lastmo && dist > lastdist) // Last one is closer to you?
			continue;

		// Found a target
		lastmo = mo;
		lastdist = dist;
	}
	return lastmo;
}

static inline mobj_t *JB_Look4BotOverlap(int botnum)
{
	thinker_t* think;
	mobj_t *mo, *botmo = players[botnum].mo;

	for(think = thinkercap.next, mo = NULL; think != &thinkercap; think = think->next)
	{
		if(think->function.acp1 != (actionf_p1)P_MobjThinker) // Not a mobj thinker
			continue;

		mo = (mobj_t*)think;

		if(botmo == mo)
			continue;

		// If it's not REALLY a bot or if it's not alive
		// just skip it... No point in chasing.
		if(!mo->player || !mo->player->bot
		|| mo->player->playerstate != PST_LIVE)
			continue;

		// If the bot isn't overlapping me
		// I don't need to avoid it.
		if(botmo->z < mo->z+mo->height
		&& mo->z < botmo->z+botmo->height
		&& P_AproxDistance(botmo->x - mo->x, botmo->y - mo->y)
			< mo->radius + botmo->radius)
				return mo;
	}
	return NULL;
}

static mobj_t *JB_Look4Spring(int botnum)
{
	thinker_t* think;
	mobj_t *mo, *lastmo, *botmo = players[botnum].mo;
	bot_t *bot = &bots[botnum];
	fixed_t dist,lastdist;

	// Already springing? Don't look for another!
	if(botmo->state-states == S_PLAY_PLG1)
		return NULL;

	for(think = thinkercap.next, mo = lastmo = NULL, lastdist = 0;
		think != &thinkercap; think = think->next)
	{
		if(think->function.acp1 != (actionf_p1)P_MobjThinker) // Not a mobj thinker
			continue;

		mo = (mobj_t*)think;

		if(!(mo->flags & MF_SPRING) // Not a spring...
		|| JB_SearchTarget(mo,bot))
			continue;

		dist = P_AproxDistance(P_AproxDistance(botmo->x - mo->x, botmo->y - mo->y), botmo->z - mo->z);
		if(lastmo && dist > lastdist) // Last one is closer to you?
			continue;

		// Found a target
		lastmo = mo;
		lastdist = dist;

		// This is probably the one they used, 'cause it's in it's bounce animation!
		if(mo->state == &states[mo->info->seestate])
			break;
	}
	return lastmo;
}

static mobj_t *JB_Look4Air(int botnum)
{
	thinker_t* think;
	mobj_t *mo, *lastmo, *botmo = players[botnum].mo;
	fixed_t dist,lastdist;

	for(think = thinkercap.next, mo = lastmo = NULL, lastdist = 0;
		think != &thinkercap; think = think->next)
	{
		if(think->function.acp1 != (actionf_p1)P_MobjThinker) // Not a mobj thinker
			continue;

		mo = (mobj_t*)think;

		if(mo->type != MT_EXTRALARGEBUBBLE // Not air bubble...
		&& !(mo->flags & MF_SPRING) // Not spring...
		&& ((mo->state->action.acp1 != (actionf_p1)A_BubbleSpawn // Not a bubble spawn...
				&& mo->state->action.acp1 != (actionf_p1)A_BubbleCheck)
			|| (lastmo && lastmo->type == MT_EXTRALARGEBUBBLE))) // Or already targetting a bubble...
				continue;

		// Can't get it if you can't see it!
		if(!P_CheckSight(botmo,mo))
			continue;

		dist = P_AproxDistance(P_AproxDistance(botmo->x - mo->x, botmo->y - mo->y), botmo->z - mo->z);
		if(lastmo && dist > lastdist) // Last one is closer to you?
			continue;

		// Found a target
		lastmo = mo;
	}
	return lastmo;
}

/////////////////////
// THINK FUNCTIONS //
/////////////////////

static void JB_BotWander(int botnum)
{
	player_t* player = &players[botnum];
	mobj_t* mo = players[botnum].mo;
	ticcmd_t* cmd = &players[botnum].cmd;

	// Bot info
	boolean jumping = player->jumpdown;
	boolean onground = (mo->z <= P_FloorzAtPos(mo->x,mo->y,mo->z,mo->height))
		|| (player->cheats & CF_FLYAROUND)
		|| (mo->flags2&(MF2_ONMOBJ));

	cmd->angleturn = TICCMD_RECEIVED; // Thee halth recieved thy command!
	cmd->forwardmove = 50/NEWTICRATERATIO; // Go full speed. Always.

	// Ability stuff
	switch(player->charability)
	{
		// Thok
		case 0:
#ifdef SPMOVES
			if(player->charflags & SF_METAL
			|| player->charflags & SF_AMYDASH)
				break;
#endif
			if(!onground && !jumping && mo->momz <= 0)
				cmd->buttons |= BT_JUMP;
			else if(onground || (jumping && mo->momz > 0))
				cmd->buttons |= BT_JUMP;
			break;

		default:
			break;
	}
}

static void JB_Jump4Air(int botnum)
{
	player_t* player = &players[botnum];
	mobj_t* mo = players[botnum].mo;
	ticcmd_t* cmd = &players[botnum].cmd;

	// Bot info
	boolean jumping = player->jumpdown;
	boolean onground = (mo->z <= P_FloorzAtPos(mo->x,mo->y,mo->z,mo->height))
		|| (player->cheats & CF_FLYAROUND)
		|| (mo->flags2&(MF2_ONMOBJ));

	cmd->angleturn = TICCMD_RECEIVED; // Thee halth recieved thy command!
	cmd->forwardmove = 0; // Don't bother moving.

	// Use your ability, whatever it is, at full jump height.
	if(!onground && !jumping && mo->momz <= 0)
		cmd->buttons |= BT_JUMP;
	// Jump, jump, jump, as high as you can. You can't catch me, I'm in need of air, man!
	else if(onground || (jumping && mo->momz > 0))
		cmd->buttons |= BT_JUMP;
}

static void JB_CoopThink(int botnum)
{
	player_t* player = &players[botnum];
	bot_t* bot = &bots[botnum];
	mobj_t* mo = players[botnum].mo;
	ticcmd_t* cmd = &players[botnum].cmd;

	fixed_t dist;
	angle_t angle;

	// Bot info
	boolean jumping = player->jumpdown;
	boolean onground = (mo->z <= P_FloorzAtPos(mo->x,mo->y,mo->z,mo->height))
		|| (player->cheats & CF_FLYAROUND)
		|| (mo->flags2&(MF2_ONMOBJ));

	// Targetting
	bot->target = JB_Look4BotOverlap(botnum);
	if(!bot->target)
	{
		bot->target = players[bot->ownernum].mo;
		if(bot->target && bot->target->state == &states[S_PLAY_PLG1])
		{
			bot->target = JB_Look4Spring(botnum);
			if(!bot->target)
				bot->target = players[bot->ownernum].mo;
		}
	}

	// Should not occur anymore.
	// But just in case...
	if(!bot->target)
	{
		JB_BotWander(botnum);
		return;
	}

	// Target info
	dist = P_AproxDistance(
			bot->target->x - mo->x,
			bot->target->y - mo->y) / FRACUNIT;
	if(player->mfstartdash)
		angle = bot->target->angle; // You facing same direction as spindash target.
	else
		angle = R_PointToAngle2(mo->x, mo->y, bot->target->x, bot->target->y); // You facing target.

#ifdef RUSSIA
	if(russia)
	{
		fixed_t x,y;

		// In Soviet Russia, player follows YOU!
		x = P_ReturnThrustX(bot->target, bot->target->angle, 256*FRACUNIT);
		y = P_ReturnThrustY(bot->target, bot->target->angle, 256*FRACUNIT);
		dist = P_AproxDistance(
				(bot->target->x+x) - mo->x,
				(bot->target->y+y) - mo->y) / FRACUNIT;
		if(player->mfstartdash)
			angle = bot->target->angle; // You facing same direction as spindash target.
		else
			angle = R_PointToAngle2(mo->x, mo->y, // You facing target.
				bot->target->x+x, bot->target->y+y);
	}
#endif

	// Too far away? Disappear and wait to respawn, then...
	if(dist > 512 && !P_CheckSight(mo, bot->target))
	{
		player->playerstate = PST_DEAD;
		P_UnsetThingPosition(mo);
		mo->flags |= MF_NOBLOCKMAP;
		mo->flags |= MF_NOSECTOR;
		mo->flags2 |= MF2_DONTDRAW;
		return;
	}

	// Cheating co-op speeds!
	if(bot->target->player)
	{
		if(bot->target->player->speed > bot->target->player->normalspeed)
			player->normalspeed = bot->target->player->speed;
		else
			player->normalspeed = bot->target->player->normalspeed;
		player->thrustfactor = 5;
		player->accelstart = 200;
		player->acceleration = 50;
#ifdef RUSSIA
		if(russia && dist > 256)
			player->normalspeed = MAXMOVE;
		else if(russia && dist > 128)
			player->normalspeed = FixedMul(player->normalspeed,(FRACUNIT*3)/2);
		else if(russia && dist > 64)
			player->normalspeed = FixedDiv(player->normalspeed,(FRACUNIT*10)/11);
		else
#endif
		if(dist > 340)
			player->normalspeed = MAXMOVE;
		else if(dist > 256)
			player->normalspeed = FixedMul(player->normalspeed,(FRACUNIT*3)/2);
		else if(dist > 132)
			player->normalspeed = FixedDiv(player->normalspeed,(FRACUNIT*10)/11);
	}

	// Turning movement
	cmd->angleturn = 0; // Command pending...
	if(!player->climbing)
	{
		if((mo->angle - ANGLE_10) - angle < angle - (mo->angle - ANGLE_10))
			cmd->angleturn = (short)(cmd->angleturn - (2560/NEWTICRATERATIO)); // Turn right!
		else if((mo->angle + ANGLE_10) - angle > angle - (mo->angle + ANGLE_10))
			cmd->angleturn = (short)(cmd->angleturn + (2560/NEWTICRATERATIO)); // Turn left!
		mo->angle += (cmd->angleturn<<16); // Set the angle of your mobj
		cmd->angleturn = (short)(mo->angle>>16); // And set that to your turning. For some reason.
	}
	cmd->angleturn |= TICCMD_RECEIVED; // Thee halth recieved thy command!

	// Forward movement
	if(player->carried // If you're being carried
	|| (mo->momz > 0 && !jumping // Or you're bouncing on a spring...
		&& bot->springmove)) // And you're already moving in a direction from it...
			cmd->forwardmove = 0; // Do nothing. Moving could ruin it.
	else if(player->spinning) // If you're spinning...
		cmd->forwardmove = 50/NEWTICRATERATIO; // Go ahead and spin!
	else if(bot->target->player && bot->target->player->bot) // If getting out of the way of another bot...
			cmd->forwardmove = -50/NEWTICRATERATIO; // Backstep, please.
#ifdef RUSSIA
	else if((russia && dist > 128) // If you're farther then 256 units away...
		|| !bot->target->player // Or if you're not chasing a player...
		// Or if you're climbing and I'm not...
		//|| (player->climbing && bot->target->player && !bot->target->player->climbing)
		|| mo->floorz < bot->target->floorz) // Or if you're just below me.
			cmd->forwardmove = 50/NEWTICRATERATIO; // Go full speed
	else if((!russia && dist > 256)) // If you're farther then 256 units away...
			cmd->forwardmove = 50/NEWTICRATERATIO; // Go full speed
	// If you're closer then 256 units away, yet farther then 128...
	else if(russia && dist > 64 && bot->target->player && !bot->target->player->bot)
		cmd->forwardmove = 25/NEWTICRATERATIO; // Start slowing down.
	else if(!russia && dist > 128 && bot->target->player && !bot->target->player->bot)
		cmd->forwardmove = 25/NEWTICRATERATIO; // Start slowing down.
#else
	else if(dist > 256 // If you're farther then 256 units away...
		|| !bot->target->player // Or if you're not chasing a player...
		// Or if you're climbing and I'm not...
		//|| (player->climbing && bot->target->player && !bot->target->player->climbing)
		|| mo->floorz < bot->target->floorz) // Or if you're just below me.
			cmd->forwardmove = 50/NEWTICRATERATIO; // Go full speed
	else if(dist > 128 && bot->target->player && !bot->target->player->bot)
		cmd->forwardmove = 25/NEWTICRATERATIO; // Start slowing down.
#endif
	else // If you're closer then 128 units away...
		cmd->forwardmove = 0; // Stop. :|

	// Spindash stuff
	if(bot->target->player)
	{
		// If your player is starting to dash, you should too!
		if(bot->target->player->mfstartdash)
		{
			if(!mo->momx && !mo->momy)
				cmd->buttons |= BT_USE;
		}
		// If your player is spinning, you should too!
		else if(bot->target->player->mfspinning
		&& !player->mfstartdash)
			cmd->buttons |= BT_USE;
		else // Otherwise...
			cmd->buttons &= ~BT_USE; // I guess you shouldn't be spinning, then...
	}

	// Jumping stuff
	if(!onground && !jumping) // In the air but not holding the jump button?
		cmd->buttons &= ~BT_JUMP; // Don't press it again, then.
	else if(bot->target->z > mo->z + mo->height // If your target is above your head...
		&& bot->target->state != &states[S_PLAY_PLG1]) // And they didn't just fly off a spring...
			cmd->buttons |= BT_JUMP; // Then jump!
	else if(bot->target->z > mo->z + MAXSTEPMOVE // If your target is higher up then you can step...
		&& (dist >= 256 // And you're far enough that you're worried about being left behind...
			|| player->speed > player->runspeed) // Or going at running speed...
		&& !jumping // And you're not jumping already...
		&& bot->target->state != &states[S_PLAY_PLG1]) // And they didn't just fly off a spring...
			cmd->buttons |= BT_JUMP; // Then jump!
	else if(bot->target->z > mo->z // If your target's still above you...
		&& jumping && mo->momz > 0) // And you're jumping and still going up...
			cmd->buttons |= BT_JUMP; // Continue to do so!
	else // Otherwise...
		cmd->buttons &= ~BT_JUMP; // I guess you shouldn't be jumping, then...
}

static void JB_SmartCoopThink(int botnum)
{
	player_t* player = &players[botnum];
	bot_t* bot = &bots[botnum];
	mobj_t* mo = players[botnum].mo;
	ticcmd_t* cmd = &players[botnum].cmd;

	fixed_t dist;
	angle_t angle;
	boolean aimed;
	sector_t *nextsector;

	// Bot info
	boolean jumping = player->jumpdown;
	boolean onground = (mo->z <= P_FloorzAtPos(mo->x,mo->y,mo->z,mo->height))
		|| (player->cheats & CF_FLYAROUND)
		|| (mo->flags2&(MF2_ONMOBJ));

	// Targeting
	if(player->powers[pw_underwater] && player->powers[pw_underwater] < 15*TICRATE)
	{
		bot->target = JB_Look4Air(botnum);
		if(!bot->target) // Uh oh... No air?! Try to jump as high as you can, then!
		{
			JB_Jump4Air(botnum);
			return;
		}
	}
	else
		bot->target = JB_Look4Poppable(botnum);
	// Nothing left in the area to do? Move on, then.
	if(!bot->target)
	{
		JB_CoopThink(botnum);
		return;
	}

	// Target info
	dist = P_AproxDistance(P_AproxDistance(
			bot->target->x - player->mo->x,
			bot->target->y - player->mo->y),
			bot->target->z - player->mo->z) / FRACUNIT;
	angle = R_PointToAngle2(mo->x, mo->y, bot->target->x, bot->target->y); // You facing target.
	nextsector = R_PointInSubsector(mo->x + mo->momx*2, mo->y + mo->momy*2)->sector;

	// Turning movement
	aimed = false;
	cmd->angleturn = 0; // Command pending...
	if(!player->climbing)
	{
		if((mo->angle - ANGLE_10) - angle < angle - (mo->angle - ANGLE_10))
			cmd->angleturn = (short)(cmd->angleturn - (2560/NEWTICRATERATIO)); // Turn right!
		else if((mo->angle + ANGLE_10) - angle > angle - (mo->angle + ANGLE_10))
			cmd->angleturn = (short)(cmd->angleturn + (2560/NEWTICRATERATIO)); // Turn left!
		else if(JB_AngleMove(player))
			aimed = true;
		mo->angle += (cmd->angleturn<<16); // Set the angle of your mobj
		cmd->angleturn = (short)(mo->angle>>16); // And set that to your turning. For some reason.
	}
	cmd->angleturn |= TICCMD_RECEIVED; // Thee halth recieved thy command!

	// Forward movement.
	if(mo->momz > 0 && !jumping // If you're bouncing on a spring...
		&& bot->springmove) // And you're already moving in a direction from it...
			cmd->forwardmove = 0; // Do nothing. Moving could ruin it.
	else if(nextsector->special == 4 // If the next sector is HARMFUL to you...
		|| nextsector->special == 5
		|| nextsector->special == 7
		|| nextsector->special == 9
		|| nextsector->special == 11
		|| nextsector->special == 16
		|| nextsector->special == 18
		|| nextsector->special == 519
		|| nextsector->special == 978
		|| nextsector->special == 980
		|| nextsector->special == 983
		|| nextsector->special == 984)
			cmd->forwardmove = -50/NEWTICRATERATIO; // STOP RUNNING TWARDS IT! AGH!
	else if(!aimed) // If you're not aimed properly at something...
		cmd->forwardmove = 25/NEWTICRATERATIO; // Start slowing down.
	else if(dist < 256 // If you're closing in on something
		&& (bot->target->flags & MF_ENEMY // that needs popping...
		|| bot->target->flags & MF_BOSS
		|| bot->target->flags & MF_MONITOR))
			cmd->forwardmove = 25/NEWTICRATERATIO; // Start slowing down.
	else // Otherwise...
		cmd->forwardmove = 50/NEWTICRATERATIO; // Go full speed. Always.

	// Jumping stuff
	if(!onground && !jumping) // In the air but not holding the jump button?
		cmd->buttons &= ~BT_JUMP; // Don't press it again, then.
	else if(nextsector->floorheight > mo->z // If the next sector is above you...
	&& nextsector->floorheight - mo->z < 128*FRACUNIT) // And you can jump up on it...
		cmd->buttons |= BT_JUMP; // Then jump!
	else if(bot->target->z > mo->z // If your target's still above you...
		&& jumping // And you're already holding the jump button...
		&& mo->momz > 0) // And you're still going up...
			cmd->buttons |= BT_JUMP; // Continue to do so!
	else if(bot->target->z > mo->z + mo->height // If it's above your head...
		&& !jumping) // And you're not jumping already...
			cmd->buttons |= BT_JUMP; // Then jump!
	else if((bot->target->flags & MF_ENEMY // If the target
		|| bot->target->flags & MF_BOSS // NEEDS to be popped...
		|| bot->target->flags & MF_MONITOR)
		&& dist < 128 // And you're getting close to it...
		&& !jumping) // And you're not jumping already...
			cmd->buttons |= BT_JUMP; // Then jump!
	else // Otherwise...
		cmd->buttons &= ~BT_JUMP; // I guess you shouldn't be jumping, then...
}

static void JB_TimeRaceThink(int botnum)
{
#ifdef SUCKYBOTWAYPOINTS
	player_t* player = &players[botnum];
	bot_t* bot = &bots[botnum];
	mobj_t* mo = players[botnum].mo;
	ticcmd_t* cmd = &players[botnum].cmd;

	fixed_t dist, dist2;
	angle_t angle;
	boolean aimed = false;
	boolean spring = false;

	// Bot info
	boolean jumping = player->jumpdown;
	boolean onground = (mo->z <= P_FloorzAtPos(mo->x,mo->y,mo->z,mo->height))
		|| (player->cheats & CF_FLYAROUND)
		|| (mo->flags2&(MF2_ONMOBJ));

	// Targetting
	bot->target = JB_Look4Enemy(botnum);

	// Waypoint info
	if(bot->waypoint)
	{
		dist2 = P_AproxDistance(P_AproxDistance(
					bot->waypoint->x - player->mo->x,
					bot->waypoint->y - player->mo->y),
					bot->waypoint->z - player->mo->z) / FRACUNIT;
		angle = R_PointToAngle2(mo->x, mo->y, bot->waypoint->x, bot->waypoint->y); // You facing waypoint.
		if(bot->waypoint->springpoint && onground)
		{
			mo = bot->target;
			bot->target = JB_Look4Spring(botnum);
			if(bot->target)
				spring = true;
			else
				bot->target = mo;
			mo = players[botnum].mo;
		}
	}
	else
	{
		dist2 = 0;
		angle = mo->angle;
	}

	// Target info
	if(bot->target)
	{
		dist = P_AproxDistance(P_AproxDistance(
				bot->target->x - player->mo->x,
				bot->target->y - player->mo->y),
				bot->target->z - player->mo->z) / FRACUNIT;
		if(spring)
			angle = R_PointToAngle2(mo->x, mo->y, bot->target->x, bot->target->y); // You facing spring.
	}
	else
		dist = 0; // Just for safety...

	// Turning movement
	cmd->angleturn = 0; // Command pending...
	if(!player->climbing)
	{
		if((mo->angle - ANGLE_10) - angle < angle - (mo->angle - ANGLE_10))
			cmd->angleturn = (short)(cmd->angleturn - (2560/NEWTICRATERATIO)); // Turn right!
		else if((mo->angle + ANGLE_10) - angle > angle - (mo->angle + ANGLE_10))
			cmd->angleturn = (short)(cmd->angleturn + (2560/NEWTICRATERATIO)); // Turn left!
		else if(JB_AngleMove(player))
			aimed = true;
		mo->angle += (cmd->angleturn<<16); // Set the angle of your mobj
		cmd->angleturn = (short)(mo->angle>>16); // And set that to your turning. For some reason.
	}
	cmd->angleturn |= TICCMD_RECEIVED; // Thee halth recieved thy command!

	// Forward movement
	if(!bot->waypoint // No waypoint?
	|| (mo->subsector->sector == bot->waypoint->sec // In same sector as waypoint
		&& dist2 < 128)) // And close enough where it should've switched you to the next waypoint already?
			cmd->forwardmove = 0; // Stop...
	else if(mo->momz > 0 && !jumping // If you're bouncing on a spring...
		&& bot->springmove) // And you're already moving in a direction from it...
			cmd->forwardmove = 0; // Do nothing. Moving could ruin it.
	else if(!aimed) // Not aimed properly?
		cmd->forwardmove = 25/NEWTICRATERATIO; // Slow down!
	else // Otherwise...
		cmd->forwardmove = 50/NEWTICRATERATIO; // Go full speed

	// Jumping stuff
	if(!onground && !jumping) // In the air but not holding the jump button?
		cmd->buttons &= ~BT_JUMP; // Don't press it again, then.
	// waypoint jumping
	else if(bot->waypoint //&& aimed // Aimed at a waypoint?
		&& !bot->waypoint->springpoint // And it's not a spring waypoint
		//&& dist2 < 256 // And you're close to it...
		// And it's floor is above you
		&& (mo->z < bot->waypoint->z || mo->momz > 0))
			cmd->buttons |= BT_JUMP; // Then jump!
	// target jumping
	else if(!bot->target) // No target?
		cmd->buttons &= ~BT_JUMP; // No jump!
	else if(spring // If your target's a spring...
		&& bot->target->z > mo->z + MAXSTEPMOVE) // And it's higher then you can step...
			cmd->buttons |= BT_JUMP; // Then jump!
	else if((bot->target->flags & MF_ENEMY // If the target
		|| bot->target->flags & MF_BOSS) // NEEDS to be popped...
		&& dist < 128 // And you're getting close to it...
		&& mo->momz >= 0) // And you're either not jumping or still going up...
			cmd->buttons |= BT_JUMP; // Then jump!
	else // Otherwise...
		cmd->buttons &= ~BT_JUMP; // I guess you shouldn't be jumping, then...
#else
	JB_CoopThink(botnum);
#endif
}

static void JB_RaceThink(int botnum)
{
	player_t* player = &players[botnum];
	bot_t* bot = &bots[botnum];
	mobj_t* mo = players[botnum].mo;
	ticcmd_t* cmd = &players[botnum].cmd;

	fixed_t dist;
	angle_t angle;
	boolean aimed;
	sector_t *nextsector;

	// Bot info
	boolean jumping = player->jumpdown;
	boolean onground = (mo->z <= P_FloorzAtPos(mo->x,mo->y,mo->z,mo->height))
		|| (player->cheats & CF_FLYAROUND)
		|| (mo->flags2&(MF2_ONMOBJ));

	// Targeting
	if(player->powers[pw_underwater] && player->powers[pw_underwater] < 15*TICRATE)
	{
		bot->target = JB_Look4Air(botnum);
		if(!bot->target) // Uh oh... No air?! Try to jump as high as you can, then!
		{
			JB_Jump4Air(botnum);
			return;
		}
	}
	else
		bot->target = JB_Look4Poppable(botnum);
	// Nothing left in the area to do? Move on, then.
	if(!bot->target)
	{
		JB_TimeRaceThink(botnum);
		return;
	}

	// Target info
	dist = P_AproxDistance(P_AproxDistance(
			bot->target->x - player->mo->x,
			bot->target->y - player->mo->y),
			bot->target->z - player->mo->z) / FRACUNIT;
	angle = R_PointToAngle2(mo->x, mo->y, bot->target->x, bot->target->y); // You facing target.
	nextsector = R_PointInSubsector(mo->x + mo->momx*2, mo->y + mo->momy*2)->sector;

	// Turning movement
	aimed = false;
	cmd->angleturn = 0; // Command pending...
	if(!player->climbing)
	{
		if((mo->angle - ANGLE_10) - angle < angle - (mo->angle - ANGLE_10))
			cmd->angleturn = (short)(cmd->angleturn - (2560/NEWTICRATERATIO)); // Turn right!
		else if((mo->angle + ANGLE_10) - angle > angle - (mo->angle + ANGLE_10))
			cmd->angleturn = (short)(cmd->angleturn + (2560/NEWTICRATERATIO)); // Turn left!
		else if(JB_AngleMove(player))
			aimed = true;
		mo->angle += (cmd->angleturn<<16); // Set the angle of your mobj
		cmd->angleturn = (short)(mo->angle>>16); // And set that to your turning. For some reason.
	}
	cmd->angleturn |= TICCMD_RECEIVED; // Thee halth recieved thy command!

	// Forward movement.
	if(mo->momz > 0 && !jumping // If you're bouncing on a spring...
		&& bot->springmove) // And you're already moving in a direction from it...
			cmd->forwardmove = 0; // Do nothing. Moving could ruin it.
	else if(nextsector->special == 4 // If the next sector is HARMFUL to you...
		|| nextsector->special == 5
		|| nextsector->special == 7
		|| nextsector->special == 9
		|| nextsector->special == 11
		|| nextsector->special == 16
		|| nextsector->special == 18
		|| nextsector->special == 519
		|| nextsector->special == 978
		|| nextsector->special == 980
		|| nextsector->special == 983
		|| nextsector->special == 984)
			cmd->forwardmove = -50/NEWTICRATERATIO; // STOP RUNNING TWARDS IT! AGH!
	else if(!aimed) // If you're not aimed properly at something...
		cmd->forwardmove = 25/NEWTICRATERATIO; // Start slowing down.
	else if(dist < 256 // If you're closing in on something
		&& (bot->target->flags & MF_ENEMY // that needs popping...
		|| bot->target->flags & MF_BOSS
		|| bot->target->flags & MF_MONITOR))
			cmd->forwardmove = 25/NEWTICRATERATIO; // Start slowing down.
	else // Otherwise...
		cmd->forwardmove = 50/NEWTICRATERATIO; // Go full speed. Always.

	// Jumping stuff
	if(!onground && !jumping) // In the air but not holding the jump button?
		cmd->buttons &= ~BT_JUMP; // Don't press it again, then.
	else if(nextsector->floorheight > mo->z // If the next sector is above you...
	&& nextsector->floorheight - mo->z < 128*FRACUNIT) // And you can jump up on it...
		cmd->buttons |= BT_JUMP; // Then jump!
	else if(bot->target->z > mo->z // If your target's still above you...
		&& jumping // And you're already holding the jump button...
		&& mo->momz > 0) // And you're still going up...
			cmd->buttons |= BT_JUMP; // Continue to do so!
	else if(bot->target->z > mo->z + mo->height // If it's above your head...
		&& !jumping) // And you're not jumping already...
			cmd->buttons |= BT_JUMP; // Then jump!
	else if((bot->target->flags & MF_ENEMY // If the target
		|| bot->target->flags & MF_BOSS // NEEDS to be popped...
		|| bot->target->flags & MF_MONITOR)
		&& dist < 128 // And you're getting close to it...
		&& !jumping) // And you're not jumping already...
			cmd->buttons |= BT_JUMP; // Then jump!
	else // Otherwise...
		cmd->buttons &= ~BT_JUMP; // I guess you shouldn't be jumping, then...
}

static void JB_MatchThink(int botnum)
{
	player_t* player = &players[botnum];
	bot_t* bot = &bots[botnum];
	mobj_t* mo = players[botnum].mo;
	ticcmd_t* cmd = &players[botnum].cmd;

	fixed_t dist;
	angle_t angle;
	boolean aimed, abilityjump;
	sector_t *nextsector;

	// Bot info
	boolean jumping = player->jumpdown;
	boolean onground = (mo->z <= P_FloorzAtPos(mo->x,mo->y,mo->z,mo->height))
		|| (player->cheats & CF_FLYAROUND)
		|| (mo->flags2&(MF2_ONMOBJ));

	// Targeting
	if(!bot->target || !bot->target->player || mo->health <= 5)
	{
		bot->target = NULL;
		if(player->powers[pw_underwater] && player->powers[pw_underwater] < 15*TICRATE)
		{
			bot->target = JB_Look4Air(botnum);
			if(!bot->target) // Uh oh... No air?! Try to jump as high as you can, then!
			{
				JB_Jump4Air(botnum);
				return;
			}
		}
		else if(mo->health <= 10)
			bot->target = JB_Look4Collect(botnum);
		else
		{
			bot->target = JB_Look4Players(botnum);
			if(bot->target && bot->target->state == &states[S_PLAY_PLG1])
				bot->target = JB_Look4Spring(botnum);
			else if(!bot->target)
				bot->target = JB_Look4Collect(botnum);
		}
	}

	// No target?
	if(!bot->target)
	{
		JB_BotWander(botnum);
		return;
	}

	// Target info
	dist = P_AproxDistance(P_AproxDistance(
			bot->target->x - player->mo->x,
			bot->target->y - player->mo->y),
			bot->target->z - player->mo->z) / FRACUNIT;
	angle = R_PointToAngle2(mo->x, mo->y, bot->target->x, bot->target->y); // You facing target.
	nextsector = R_PointInSubsector(mo->x + mo->momx*2, mo->y + mo->momy*2)->sector;

	// Turning movement
	aimed = false;
	cmd->angleturn = 0; // Command pending...
	if(!player->climbing)
	{
		if((mo->angle - ANGLE_10) - angle < angle - (mo->angle - ANGLE_10))
			cmd->angleturn = (short)(cmd->angleturn - (2560/NEWTICRATERATIO)); // Turn right!
		else if((mo->angle + ANGLE_10) - angle > angle - (mo->angle + ANGLE_10))
			cmd->angleturn = (short)(cmd->angleturn + (2560/NEWTICRATERATIO)); // Turn left!
		else if(JB_AngleMove(player))
			aimed = true;
		mo->angle += (cmd->angleturn<<16); // Set the angle of your mobj
		cmd->angleturn = (short)(mo->angle>>16); // And set that to your turning. For some reason.
	}
	cmd->angleturn |= TICCMD_RECEIVED; // Thee halth recieved thy command!

	// Ability stuff
	abilityjump = false;
	switch(player->charability)
	{
		case 0: // Thok
#ifdef SPMOVES
			if(player->charflags & SF_METAL
			|| player->charflags & SF_AMYDASH)
				break;
#endif
			if(bot->target->flags & MF_SPRING // No thok over spring!
			|| bot->target->type == MT_EXTRALARGEBUBBLE)
				break;
			// Thok!
			else if(aimed && !onground && !jumping && mo->momz <= 0)
				cmd->buttons |= BT_JUMP;
			// Jump to full height!
			else if((!jumping && onground) || (jumping && mo->momz > 0))
				cmd->buttons |= BT_JUMP;
			// Ready the jump button!
			else
				cmd->buttons &= ~BT_JUMP;
			// Ability is controlling jump button! MWAHAHAHA!
			abilityjump = true;
			break;

		case 1: // Fly
		case 7: // Swim
			if(bot->target->flags & MF_SPRING // No fly over spring!
			|| (player->charability == 7 // No swim out of water!
				&& !(player->mo->eflags & MF_UNDERWATER))
			|| mo->health < 10) // No snipe without ammo!
					break;
			// Fly!
			else if(!onground && !jumping && mo->momz <= 0)
				cmd->buttons |= BT_JUMP;
			// Jump to full height!
			else if((!jumping && onground) || (jumping && mo->momz > 0))
				cmd->buttons |= BT_JUMP;
			// Ready the jump button!
			else
				cmd->buttons &= ~BT_JUMP;
			// Ability is controlling jump button! MWAHAHAHA!
			abilityjump = true;
			break;

		case 2: // Glide and climb
		case 3: // Glide with no climb
			if(bot->target->z > mo->z // Target still above you
			&& !onground && !jumping // You're in the air but not holding the jump button
			&& mo->momz <= 0) // You aren't gonna get high enough
			{ // So what do you do? Glide!... I dunno.
				cmd->buttons |= BT_JUMP;
				abilityjump = true;
			}
			break;

		case 4: // Double-Jump
			// Jump again at top of jump height!
			if(!onground && !jumping && mo->momz <= 0)
				cmd->buttons |= BT_JUMP;
			break;

		default:
			break;
	}

	// Forward movement.
	if(mo->momz > 0 && !jumping // If you're bouncing on a spring...
		&& bot->springmove) // And you're already moving in a direction from it...
			cmd->forwardmove = 0; // Do nothing. Moving could ruin it.
	else if(nextsector->special == 4 // If the next sector is HARMFUL to you...
		|| nextsector->special == 5
		|| nextsector->special == 7
		|| nextsector->special == 9
		|| nextsector->special == 11
		|| nextsector->special == 16
		|| nextsector->special == 18
		|| nextsector->special == 519
		|| nextsector->special == 978
		|| nextsector->special == 980
		|| nextsector->special == 983
		|| nextsector->special == 984)
			cmd->forwardmove = -50/NEWTICRATERATIO; // STOP RUNNING TWARDS IT! AGH!
	else if(!aimed && !bot->target->player) // If you're not aimed properly at something that isn't a person...
		cmd->forwardmove = 25/NEWTICRATERATIO; // Start slowing down.
	else // Otherwise...
		cmd->forwardmove = 50/NEWTICRATERATIO; // Go full speed. Always.

	// Jumping stuff
	if(abilityjump) // Ability has changed the state of your jump button already?
		; // Then don't mess with it!
	else if(!onground && !jumping) // In the air but not holding the jump button?
		cmd->buttons &= ~BT_JUMP; // Don't press it again, then.
	else if(nextsector->floorheight > mo->z // If the next sector is above you...
	&& nextsector->floorheight - mo->z < 128*FRACUNIT) // And you can jump up on it...
		cmd->buttons |= BT_JUMP; // Then jump!
	else if(bot->target->z > mo->z // If your target's still above you...
		&& jumping // And you're already holding the jump button...
		&& mo->momz > 0) // And you're jumping and still going up...
			cmd->buttons |= BT_JUMP; // Continue to do so!
	else if(bot->target->z > mo->z + mo->height // If your target is above your head...
		&& !jumping // And you're not jumping already...
		&& bot->target->state != &states[S_PLAY_PLG1]) // And they didn't just fly off a spring...
			cmd->buttons |= BT_JUMP; // Then jump!
	else if((bot->target->flags & MF_ENEMY // If the target
		|| bot->target->flags & MF_BOSS // NEEDS to be popped...
		|| bot->target->flags & MF_MONITOR)
		&& dist < 128 // And you're getting close to it...
		&& !jumping) // And you're not jumping already...
			cmd->buttons |= BT_JUMP; // Then jump!
	else // Otherwise...
		cmd->buttons &= ~BT_JUMP; // I guess you shouldn't be jumping, then...

	// Shooting stuff
	if(cmd->buttons & BT_ATTACK) // If you're holding the button down...
		cmd->buttons &= ~BT_ATTACK; // DO NOT HOLD THE BUTTON DOWN!
	else if(aimed // If you're properly aimed...
	&& bot->target->player // At a player...
	&& mo->health > 2) // And you have at least one ring to spare...
		cmd->buttons |= BT_ATTACK; // Fire away!
	// PTR_AimTraverse
}

static inline void JB_TagThink(int botnum)
{
	player_t* player = &players[botnum];
	bot_t* bot = &bots[botnum];
	mobj_t* mo = players[botnum].mo;
	ticcmd_t* cmd = &players[botnum].cmd;

	angle_t angle;
	boolean aimed;
	sector_t *nextsector;

	// Bot info
	boolean jumping = player->jumpdown;
	boolean onground = (mo->z <= P_FloorzAtPos(mo->x,mo->y,mo->z,mo->height))
		|| (player->cheats & CF_FLYAROUND)
		|| (mo->flags2&(MF2_ONMOBJ));

	// Targeting
	if(player->powers[pw_underwater] && player->powers[pw_underwater] < 15*TICRATE)
	{
		bot->target = JB_Look4Air(botnum);
		if(!bot->target) // Uh oh... No air?! Try to jump as high as you can, then!
		{
			JB_Jump4Air(botnum);
			return;
		}
	}
	else if(mo->health <= 5)
		bot->target = JB_Look4Collect(botnum);
	else
	{
		bot->target = JB_Look4Players(botnum);
		if(!bot->target)
			bot->target = JB_Look4Collect(botnum);
	}

	// No target?
	if(!bot->target)
	{
		JB_BotWander(botnum);
		return;
	}

	// Target info
	angle = R_PointToAngle2(mo->x, mo->y, bot->target->x, bot->target->y); // You facing target.
	nextsector = R_PointInSubsector(mo->x + mo->momx*2, mo->y + mo->momy*2)->sector;

	// Turning movement
	aimed = false;
	cmd->angleturn = 0; // Command pending...
	if(!player->climbing)
	{
		if((mo->angle - ANGLE_10) - angle < angle - (mo->angle - ANGLE_10))
			cmd->angleturn = (short)(cmd->angleturn - (2560/NEWTICRATERATIO)); // Turn right!
		else if((mo->angle + ANGLE_10) - angle > angle - (mo->angle + ANGLE_10))
			cmd->angleturn = (short)(cmd->angleturn + (2560/NEWTICRATERATIO)); // Turn left!
		else if(JB_AngleMove(player))
			aimed = true;
		mo->angle += (cmd->angleturn<<16); // Set the angle of your mobj
		cmd->angleturn = (short)(mo->angle>>16); // And set that to your turning. For some reason.
	}
	cmd->angleturn |= TICCMD_RECEIVED; // Thee halth recieved thy command!

	// Forward movement.
	if(mo->momz > 0 && !jumping // If you're bouncing on a spring...
		&& bot->springmove) // And you're already moving in a direction from it...
			cmd->forwardmove = 0; // Do nothing. Moving could ruin it.
	else if(nextsector->special == 4 // If the next sector is HARMFUL to you...
		|| nextsector->special == 5
		|| nextsector->special == 7
		|| nextsector->special == 9
		|| nextsector->special == 11
		|| nextsector->special == 16
		|| nextsector->special == 18
		|| nextsector->special == 519
		|| nextsector->special == 978
		|| nextsector->special == 980
		|| nextsector->special == 983
		|| nextsector->special == 984)
			cmd->forwardmove = -50/NEWTICRATERATIO; // STOP RUNNING TWARDS IT! AGH!
	else if(bot->target->player) // If it's a player chasing you...
		cmd->forwardmove = -50/NEWTICRATERATIO; // Go backwards!
	else if(!aimed) // If you're not aimed properly at something that isn't a person...
		cmd->forwardmove = 25/NEWTICRATERATIO; // Start slowing down.
	else // Otherwise...
		cmd->forwardmove = 50/NEWTICRATERATIO; // Go full speed. Always.

	// Jumping stuff
	if(!onground && !jumping) // In the air but not holding the jump button?
		cmd->buttons &= ~BT_JUMP; // Don't press it again, then.
	else if(nextsector->floorheight > mo->z // If the next sector is above you...
	&& nextsector->floorheight - mo->z < 128*FRACUNIT) // And you can jump up on it...
		cmd->buttons |= BT_JUMP; // Then do so!
	else if(jumping // If you're already holding the jump button...
		&& mo->momz > 0) // And you're still going up...
			cmd->buttons |= BT_JUMP; // Continue to do so!
	else if(bot->target->player // If your target's a player
		&& bot->target->z <= mo->z + mo->height // And it's below your head...
		&& !jumping) // And you're not jumping already...
			cmd->buttons |= BT_JUMP; // Then jump!
	else // Otherwise...
		cmd->buttons &= ~BT_JUMP; // I guess you shouldn't be jumping, then...
}

static inline void JB_ChaosThink(int botnum)
{
	JB_RaceThink(botnum);
}

void JB_BotThink(int botnum)
{
	bot_t* bot = &bots[botnum];
	player_t* player = &players[botnum];
	ticcmd_t* cmd = &player->cmd;

	if(!playeringame[botnum] || !playeringame[player->bot->ownernum])
		return;

	// If dead, try to respawn as soon as you're under the floor.
	if(player->playerstate == PST_DEAD)
	{
		if(player->mo->z + player->mo->height < player->mo->floorz) // Wait to be completely under the floor
		{
			if(gametype == GT_COOP) // In co-op, wait for your leader to stop, too.
			{
				player_t* owner = &players[player->bot->ownernum];
				if(owner->speed <= owner->normalspeed/2)
					cmd->buttons = BT_JUMP;
			}
			else
				cmd->buttons = BT_JUMP;
		}
		cmd->angleturn = TICCMD_RECEIVED; // Thee halth recieved thy command!
	}
	else if(player->playerstate == PST_LIVE && !cv_objectplace.value)
	{
		mobj_t* lasttarget = bot->target;

		JB_UpdateLook(&bots[botnum]);
		switch(gametype)
		{
			case GT_COOP:
				if(cv_botcoopai.value)
					JB_SmartCoopThink(botnum);
				else
					JB_CoopThink(botnum);
				break;

			case GT_RACE:
				if(cv_racetype.value == 1 || circuitmap)
					JB_TimeRaceThink(botnum);
				else
					JB_RaceThink(botnum);
				break;

			case GT_MATCH:
				JB_MatchThink(botnum);
				break;

			case GT_TAG:
				if(player->tagit)
					JB_MatchThink(botnum);
				else
					JB_TagThink(botnum);
				break;

#ifdef CHAOSISNOTDEADYET
			case GT_CHAOS:
				JB_ChaosThink(botnum);
				break;
#endif

			default:
				JB_MatchThink(botnum);
				break;
		}
		if(bot->target != lasttarget)
			bot->targettimer = 0;
	}
}

#endif
