// SPM - SPMoves
// By Jason the Echidna
// MINE! NO STEALY!

#include "../doomstat.h"

#ifdef SPMOVES

#include "../d_clisrv.h"
#include "../d_player.h"
#include "../d_ticcmd.h"
#include "../g_game.h"
#include "../i_video.h"
#include "../m_argv.h"
#include "../m_random.h"
#include "../p_local.h"
#include "../r_main.h"
#include "../r_things.h"
#include "../s_sound.h"
#include "../v_video.h"

#include "JTEMisc.h"
#include "SPM_Player.h"
#include "Universe/SU_Command.h"

void SPM_InitSkin(skin_t *skin)
{
		// Cheat checks relocated.
		if(atoi(skin->normalspeed) > 36)
			strcpy(skin->normalspeed, "36");
		if(atoi(skin->jumpfactor) > 100)
			strcpy(skin->jumpfactor, "100");
		if(atoi(skin->thrustfactor) > 5)
			strcpy(skin->thrustfactor, "5");
		if(atoi(skin->accelstart) > 192)
			strcpy(skin->accelstart, "192");
		if(atoi(skin->acceleration) > 50)
			strcpy(skin->acceleration,"50");

		if(!strncmp(skin->name, "shadow",6)) // If it's Shadow...
		{
			skin->spmoves = SPM_CHAOSCONTROL; // Chaos Control!
			strcpy(skin->ability, "0"); // Thok!
			strcpy(skin->super, "1"); // Turn super!
			strcpy(skin->superanims, "0"); // No super anims, though... :(
		}
		else if(!strncmp(skin->name, "metalsonic",10)) // If it's Metal Sonic...
		{
			skin->spmoves = SPM_METALSONIC|SPM_NODROWN; // Mecha attacks!
			strcpy(skin->ability, "0"); // No ability
			strcpy(skin->normalspeed, "40"); // Run FASTER!
			strcpy(skin->jumpfactor, "135"); // Jump HIGHER!
			strcpy(skin->super, "1"); // Turn super!
			strcpy(skin->superanims, "0"); // No super anims, though... :(
		}
		else if(!strncmp(skin->name, "amy",3)) // If it's Amy...
		{
			skin->spmoves = SPM_AMYDASH|SPM_GRABOBJ; // Amy dash!
			strcpy(skin->ability, "0"); // No ability
		}
		else if(!strncmp(skin->name, "hms123",6)) // If it's HMS123...
		{
			// Uuunliiiimited POOWAAAAAAR!!
			skin->spmoves = SPM_HMS123|SPM_CHAOSCONTROL|SPM_NODROWN|SPM_GRABOBJ;

			// Abilities
			strcpy(skin->ability, "0"); // Thok
			strcpy(skin->multiability, "1"); // Multithok!
			strcpy(skin->normalspeed, "60"); // Run FASTER!
			strcpy(skin->accelstart, "200"); // Accellerate FASTER!
			strcpy(skin->acceleration, "60"); // Accellerate MORE!
			strcpy(skin->thrustfactor, "5"); // Accellerate MORE!
			strcpy(skin->jumpfactor, "200"); // Jump HIGHER!
			strcpy(skin->spin, "1"); // Can spin.
			strcpy(skin->runonwater, "1"); // Runs on water.
			strcpy(skin->lightdash, "1"); // Lightdash!
			strcpy(skin->homing, "1"); // Homing attack!
			strcpy(skin->ringslinger, "1"); // ... Ringslinger?

			strcpy(skin->mindash, "60"); // Spindash min
			strcpy(skin->maxdash, "60"); // Spindash max

			strcpy(skin->super, "1"); // Turn super!
			strcpy(skin->superanims, "0"); // No super anims, though... :(
		}
		else if(!stricmp(skin->name, "senya")) // If it's Senya...
			skin->spmoves = SPM_GRABOBJ; // Pick up other mobjs!
		else if(!stricmp(skin->name, "nitemare")) // If it's Nitemare...
			skin->spmoves = SPM_FLAME; // Have flame attack stuffs!
}

static mobj_t *SPM_Scan4Pickup(mobj_t *me)
{
	thinker_t* think;
	mobj_t *mo, *lastmo;

	if(!me || !me->player)
		return NULL;

	for(think = thinkercap.next, mo = NULL, lastmo = NULL; think != &thinkercap; think = think->next)
	{
		if(think->function.acp1 != (actionf_p1)P_MobjThinker) // Not a mobj thinker
			continue;

		mo = (mobj_t*)think;

		// Only pick up certain types of mobjs...
		if(!(mo->flags & MF_PUSHABLE)
		&& !(mo->flags & MF_MONITOR)
		&& !(mo->flags & MF_ENEMY)
		&& !(mo->flags & MF_MISSILE))
			continue;

		// Already dead? Aww...
		if(!mo->health)
			continue;

		// Too far away to pick up normally?
		if(//!(me->player->charflags & SF_SILVER) &&
		(P_AproxDistance(me->x - mo->x, me->y - mo->y) > me->radius*2 + mo->radius*2
		|| me->z + me->height < mo->z || me->z > mo->z + mo->height))
			continue;

		if(lastmo && // Last mo is closer to me?
		P_AproxDistance(P_AproxDistance(me->x - mo->x, me->y - mo->y), me->z - mo->z) >
		P_AproxDistance(P_AproxDistance(me->x - lastmo->x, me->y - lastmo->y), me->z - lastmo->z))
			continue;

		// Found a target
		lastmo = mo;
	}
	return lastmo;
}

// SPM_PlayerAbility
// Checks for player double jump abilities.
//
void SPM_PlayerAbility(player_t *player)
{
	const ticcmd_t* cmd = &player->cmd;

	if(!player || !player->mo || !player->mo->health)
		return;

	if(player->charflags & SF_CHAOSCONTROL)
	{
		// Now it's Chaos Control time!
		if(player->mfjumped
		&& (player->charflags & SF_HMS123
			|| ((!player->thokked || player->charflags & SF_MULTIABILITY)
				&& cmd->buttons & BT_USE // Holding spindash so it's not a normal thok
				&& ((gametype == GT_COOP && emeralds) // Player has a Chaos Emerald
					|| gametype != GT_COOP) // or can't get one
				&& player->health > 20))) // Player has at least 20 rings, regardless
		{
			if(!(player->charflags & SF_HMS123))
			{
				// Take ten rings from the start.
				player->health -= 10;
				player->mo->health -= 10;
			}

			// Chaos Control thok!
			P_InstaThrust (player->mo, player->mo->angle, MAXMOVE); // Catapult the player across the screen!
			if(!(player->charflags & SF_HMS123))
				S_StartSound (player->mo, sfx_zoom); // Play the ZOOM sound
			player->bonuscount = 10; // Flash the palette
			// Now check the player's color so the right THOK object is displayed.
#ifndef THOKTARGET
			if(player->skincolor != 0)
#endif
			{
				mobj_t* mobj;
				mobjtype_t type;

				if (player->thokitem > 0)
					type = player->thokitem;
				else
					type = player->mo->info->painchance;

				mobj = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z	- (player->mo->info->height - player->mo->height)/3, type);
				mobj->flags = (mobj->flags & ~MF_TRANSLATION) | (player->mo->flags & MF_TRANSLATION);
				mobj->target = player->mo;
				mobj->floorz = mobj->z;
				mobj->ceilingz = mobj->z+mobj->height;
			}
			player->mo->flags |= MF_NOCLIP; // No clip through walls and things
			if(player-players != displayplayer)
				player->mo->flags2 |= MF2_DONTDRAW; // I am invisible!
			player->chaoscontrol = true;
			player->thokked = true; // Dur? :/
		}
	}
	if(player->charflags & SF_AMYDASH)
	{
		// Amy's triple jump thing!
		if(player->mfjumped // For a jumping Amy only!
		&& player->thokked < 2 // Only 3 jumps in a row!
		&& player->mo->momz < 0 // Only when you're going down!
		&& player->mo->z + player->mo->momz*2 <= player->mo->floorz + 56*FRACUNIT // And you must be CLOSE TO the ground, not ON it.
		&& (player->speed > player->normalspeed/2 // And you're going faster then one half of your (normal) max speed!
		|| (player->mo->eflags & MF_UNDERWATER && player->speed))) // Or you're underwater and going at ANY speed...
		{ // Then do the double jump!
			player->mfjumped = 0;
			P_DoJump(player, true);
			player->jumping = 0; // You cannot cut the jump height!
			player->thokked++;
			player->mo->momz = FixedMul(player->mo->momz, FixedMul(3*player->thokked*FRACUNIT/2, FRACUNIT));
		}
		else if(player->mfjumped
		&& player->mo->momz < 0) // Jumped too quickly or not going fast enough?
			player->thokked = 3; // Too bad, you start all over! ^_^
	}
	if(player->charflags & SF_FLAME)
	{
		const fixed_t ns = 4<<FRACBITS;
		int i;
		mobj_t *mo;
		angle_t fa;

		// "Ring of Fire"
		if(player->mfjumped && cmd->buttons & BT_USE
		&& (!player->thokked || player->charflags & SF_MULTIABILITY)
		&& player->health > 20)
		{
			player->health = player->mo->health -= 20;
			S_StartSound (player->mo, sfx_zoom);
			for (i = 0; i < 16; i++)
			{
				fa = (i*(FINEANGLES/16));
				mo = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, MT_FLAME);
				mo->flags = MF_MISSILE|MF_FIRE;
				mo->flags2 = MF2_DEBRIS;
				mo->momx = FixedMul(finesine[fa],ns)/NEWTICRATERATIO;
				mo->momy = FixedMul(finecosine[fa],ns)/NEWTICRATERATIO;
				mo->momz = 4*FRACUNIT;
				mo->target = player->mo;
				mo->fuse = TICRATE*6;
			}
			player->thokked = true;
		}
	}
}

static void SPM_ChaosControl(player_t *player, byte buttons)
{
	sector_t *nextsec;

	/////////////////////////
	// CHAOS CONTROL STUFF //
	/////////////////////////

	// If they are gonna crash into a wall...
	nextsec = R_PointInSubsector(player->mo->x + player->mo->momx, player->mo->y + player->mo->momy)->sector;
	if(!nextsec || nextsec->ceilingheight - nextsec->floorheight <= player->mo->height)
	{
		P_BounceMove(player->mo);
		player->mo->angle = R_PointToAngle2(0, 0, player->mo->momx, player->mo->momy);
		if(player-players == consoleplayer)
			localangle = player->mo->angle;
		else if(cv_splitscreen.value
		&& player-players == secondarydisplayplayer)
			localangle2 = player->mo->angle;
		P_InstaThrust(player->mo, player->mo->angle, MAXMOVE); // Recharge the speed, bouncing takes a lot outta ya.
	}

	if(!(player->charflags & SF_HMS123)
	&& !(leveltime%(TICRATE/4)))
	{
		player->health--;
		player->mo->health--;
		if(player->health <= 1)
			player->mo->z = player->mo->floorz;
	}
	if((player->mo->z <= player->mo->floorz
		&& player->mo->ceilingz - player->mo->floorz > player->mo->height)
		|| (player->charflags & SF_HMS123 && !(buttons & BT_JUMP)))
		// If they chaos controlled and are on the ground now but the ceiling isn't below the floor
	{
		if(!(player->charflags & SF_HMS123))
		{
			player->mo->momx = player->mo->momy = player->mo->momz = 0; // Stop! Stooop! Too fast, too fast!
			player->mfspinning = 0;
			player->mfjumped = 0;
		}
		if(player->mo->z <= player->mo->floorz)
			P_SetPlayerMobjState(player->mo, S_PLAY_STND);
		player->mo->flags2 &= ~MF2_DONTDRAW; // You can see me?!
		player->mo->flags &= ~MF_NOCLIP; // Your ride's over, get out!
		player->chaoscontrol = false;

		//If they had a sheild, make it appear again!
		P_SpawnShieldOrb(player);
	}
	else if(!player->exiting && (buttons & BT_USE
		|| (player->charflags & SF_HMS123 && buttons & BT_JUMP)))
	// If they Chaos Controlled and held the button like a good little hedgehog
	{
		P_InstaThrust(player->mo, player->mo->angle, MAXMOVE); // Reward them with another burst for each loop
		if(player->charflags & SF_HMS123)
			player->mo->momz = 0; // HMS123 NEVAR FALLZ! >:E
		else
			player->mo->momz = -FRACUNIT; // Down down, nice n slow...
	}
	else if(!(player->charflags & SF_HMS123))
	{ // If not...
		player->mo->z = player->mo->floorz; // Drop to the floor and keep ya hands where I can see em!
		player->mo->momx = player->mo->momy =
		player->rmomx = player->rmomy = 0; // No move for you!
	}

	// And of course, animate properly while in the air...
	if(player->chaoscontrol
	&& (player->mo->state-states < S_PLAY_ATK1
	|| player->mo->state-states > S_PLAY_ATK4))
		P_SetPlayerMobjState(player->mo, S_PLAY_ATK1);
}

static void SPM_AmyDash(player_t *player, boolean press)
{
	///////////////////////
	// AMY'S DASH ATTACK //
	///////////////////////
	// If you're Amy and pressing the spindash button...
	if(press && !(player->charflags & SF_SPINALLOWED)
#ifdef UNIVERSE
	&& !universe
#endif
	// And you are not exiting or hurt or anything like that...
	&& !player->exiting && !(player->mo->state-states == S_PLAY_PAIN && player->powers[pw_flashing])
	// And you haven't used the attack already...
	&& !player->usedown && !player->powers[pw_amydash] && !player->weapondelay && !player->thokked
	// And you are jumping...
	&& (player->mfjumped
	// OOOR you are on the floor...
	|| player->mo->z <= player->mo->floorz))
	{ // Then start a dash attack!
		P_ResetScore(player);

		P_InstaThrust(player->mo, player->mo->angle, 40*FRACUNIT);
		player->rmomx = player->mo->momx;
		player->rmomy = player->mo->momy;

		player->powers[pw_amydash] = 5;
		player->jumping = 0;
		player->running = 1;
		player->usedown = true;

		P_SetPlayerMobjState(player->mo, S_PLAY_SPD1);
		S_StartSound(player->mo, sfx_zoom);

		if(player->mfjumped)
			player->thokked = 3;
	}

	if(player->powers[pw_amydash] > 0 && !P_CheckPosition(player->mo, player->mo->x + player->mo->momx, player->mo->y + player->mo->momy))
		player->powers[pw_amydash] = 1;
	else if(player->powers[pw_amydash] > 0)
	{
		P_InstaThrust(player->mo, player->mo->angle, 40*FRACUNIT);
		player->rmomx = player->mo->momx;
		player->rmomy = player->mo->momy;

		// If you backpedal while dashing, you go up!
		if(player->cmd.forwardmove < 0)
		{
			if(player->mo->momz < ((50/NEWTICRATERATIO)*FRACUNIT)/4)
				player->mo->momz = (-player->cmd.forwardmove*FRACUNIT)/4;
			player->mo->z++;

			// But doing so cuts your movement in half! Mwahahahaha!
			player->mo->momx /= 2;
			player->mo->momy /= 2;
		}
		else if(player->mo->momz < 0)
			player->mo->momz = 0;

		P_SetPlayerMobjState(player->mo, S_PLAY_SPD1);
		player->running = 1;
	}

	if(player->powers[pw_amydash] == 1)
	{
		if(player->mo->z > player->mo->floorz)
			P_SetPlayerMobjState(player->mo, S_PLAY_FALL1);

		P_InstaThrust(player->mo, player->mo->angle, player->normalspeed*FRACUNIT);
		player->rmomx = player->mo->momx;
		player->rmomy = player->mo->momy;

		if(!player->powers[pw_flashing])
			player->powers[pw_flashing] = TICRATE/2;

		player->weapondelay = TICRATE;
	}

	if(player->powers[pw_amydash] > 0)
		player->powers[pw_amydash]--;
}

static void SPM_PickupStuff(player_t *player, boolean press)
{
	mobj_t *mo,*pickup;

	mo = player->mo;
	pickup = mo->tracer;

	// If you've picked up an object...
	if(pickup && pickup->flags & MF_SCENERY)
	{
		// Set it's position to yours.
		P_UnsetThingPosition(pickup);
		pickup->momx = pickup->momy = pickup->momz = pickup->pmomz = 0;
		pickup->x = mo->x + mo->momx + P_ReturnThrustX(mo, mo->angle, HOLDDIST);
		pickup->y = mo->y + mo->momy + P_ReturnThrustY(mo, mo->angle, HOLDDIST);
		pickup->z = mo->z + mo->height/2 + mo->momz;
		pickup->ceilingz = pickup->z + pickup->height;
		pickup->floorz = pickup->z;
		pickup->angle = mo->angle;
		P_SetThingPosition(pickup);
	}

	if(pickup && pickup->flags & MF_SCENERY
	&& !player->weapondelay && press)
	{
		// Toss the pickup!
		fixed_t thrustx, thrusty;

		mo->tracer = NULL;
		player->weapondelay = TICRATE/4;

		P_UnsetThingPosition(pickup);
		if(TOSSMAX - TOSSMOD <= TOSSMIN)
		{
			thrustx = P_ReturnThrustX(mo, mo->angle, TOSSMIN);
			thrusty = P_ReturnThrustY(mo, mo->angle, TOSSMIN);
		}
		else
		{
			thrustx = P_ReturnThrustX(mo, mo->angle, TOSSMAX - TOSSMOD);
			thrusty = P_ReturnThrustY(mo, mo->angle, TOSSMAX - TOSSMOD);
		}
		pickup->momx = mo->momx + thrustx;
		pickup->momy = mo->momy + thrusty;
		pickup->momz = mo->momz + TOSSMIN*2;
		//pickup->flags = pickup->info->flags;
		pickup->flags = MF_NOBLOCKMAP|MF_MISSILE;
		if(pickup->info->flags & MF_ENEMY)
			pickup->flags |= MF_ENEMY;
		if(pickup->info->flags & MF_MONITOR)
			pickup->flags |= MF_MONITOR;
		pickup->target = mo;
		P_SetThingPosition(pickup);
	}
	else if(!pickup && !player->weapondelay && press)
	{
		// Check for pickup
		player->weapondelay = TICRATE/4;
		pickup = mo->tracer = SPM_Scan4Pickup(mo);
		if(pickup)
		{
			P_UnsetThingPosition(pickup);
			pickup->flags = MF_SOLID|MF_NOGRAVITY|MF_NOTHINK|MF_SCENERY;
			P_SetThingPosition(pickup);
		}
	}
}

// SPM_GameplayStuff
// Does extended ability things such as being in "Chaos Control mode"
//
void SPM_GameplayStuff(player_t *player)
{
	ticcmd_t* cmd = &player->cmd;

	if(!player || !player->mo || !player->mo->health)
		return;

	////////////////////////
	// SUPER SHADOW STUFF //
	////////////////////////
	if(player->powers[pw_super]
		&& player->charflags & SF_CHAOSCONTROL)
	{
		// Color flashing, yay!
		switch((leveltime/2)%6)
		{
			case 0:
				player->mo->flags = (player->mo->flags & ~MF_TRANSLATION) | ((1)<<MF_TRANSSHIFT); // Grey
				break;
			case 1:
			case 5:
				player->mo->flags = (player->mo->flags & ~MF_TRANSLATION) | ((4)<<MF_TRANSSHIFT); // Silver
				break;
			case 2:
			case 4:
				player->mo->flags = (player->mo->flags & ~MF_TRANSLATION) | ((14)<<MF_TRANSSHIFT); // Gold
				break;
			case 3:
				player->mo->flags = (player->mo->flags & ~MF_TRANSLATION) | ((15)<<MF_TRANSSHIFT); // Yellow
				break;
		}
	}

	// Chaos control.
	if(player->chaoscontrol)
		SPM_ChaosControl(player, cmd->buttons);

	///////////////////////
	// METAL SONIC STUFF //
	///////////////////////
	// If you're Metal Sonic....

	if(player->charflags & SF_NODROWN)
		player->powers[pw_underwater] = player->powers[pw_spacetime] = 0; // First of all you can breathe underwater...
	if(player->charflags & SF_METAL)
	{
		if(player->mo->state-states >= S_PLAY_TAP1 // Secondly, if you're in an animation where you shouldn't have a jet, remove it...
		&& player->mo->state-states <= S_PLAY_TAP2)
		{
			if(player->jet)
				P_SetMobjState(player->jet, S_DISS);
			player->jet = NULL;
		}
		else if(player->mo->state-states == S_PLAY_PAIN)
		{
			if(player->jet)
				P_SetMobjState(player->jet, S_DISS);
			player->jet = NULL;
		}
		else if(player->mo->state-states >= S_PLAY_ATK1
			&& player->mo->state-states <= S_PLAY_ATK4)
		{
			if(player->jet)
				P_SetMobjState(player->jet, S_DISS);
			player->jet = NULL;
		}
		else if(player->powers[pw_flashing])
		{
			if(player->jet)
				P_SetMobjState(player->jet, S_DISS);
			player->jet = NULL;
		}
		// Otherwise, if you don't have a jet already...
		else if(!player->jet)
		{ // Then spawn one.
			fixed_t jetx, jety;
			mobj_t* filler;

			jetx = player->mo->x + P_ReturnThrustX(player->mo, player->mo->angle, -16*FRACUNIT);
			jety = player->mo->y + P_ReturnThrustY(player->mo, player->mo->angle, -16*FRACUNIT);

			filler = P_SpawnMobj(jetx, jety, player->mo->z + 16*FRACUNIT, MT_JETFUME1);
			filler->target = player->mo;
			filler->fuse = 59;
			player->jet = filler;
		}
	}
	// If you're NOT metal sonic and you have a jet...
	else if(player->jet)
	{ // Wtf? How did THAT get there? Remove it!
		P_SetMobjState(player->jet, S_DISS);
		player->jet = NULL;
	}

	/////////////////////////////
	// SUPER METAL SONIC STUFF //
	/////////////////////////////
	if(player->powers[pw_super]
		&& player->charflags & SF_METAL)
	{
		// Color flashing, yay!
		switch(leveltime%3)
		{
			case 0:
				player->mo->flags = (player->mo->flags & ~MF_TRANSLATION) | ((5)<<MF_TRANSSHIFT); // Orange
				break;
			case 1:
				player->mo->flags = (player->mo->flags & ~MF_TRANSLATION) | ((14)<<MF_TRANSSHIFT); // Gold
				break;
			case 2:
				player->mo->flags = (player->mo->flags & ~MF_TRANSLATION) | ((15)<<MF_TRANSSHIFT); // Yellow
				break;
		}

		if(player->mo->momz < 0)
		{
			if(player->mo->state-states >= S_PLAY_FALL1
			&& player->mo->state-states <= S_PLAY_FALL2)
				P_SetPlayerMobjState(player->mo, S_PLAY_STND);
			else if(player->mo->state-states >= S_PLAY_ATK1
			&& player->mo->state-states <= S_PLAY_ATK4)
			{
				P_SetPlayerMobjState(player->mo, S_PLAY_STND);
				player->mfjumped = player->mfspinning = 0;
			}
		}
	}

	// Amy's dash attack.
	if(player->charflags & SF_AMYDASH)
		SPM_AmyDash(player, (cmd->buttons & BT_USE));

	// Pick up objects.
	if(player->charflags & SF_GRABOBJ)
		SPM_PickupStuff(player, (cmd->buttons & BT_LIGHTDASH));

	// HMS123 is always super!
	if(player->charflags & SF_HMS123
	&& player->superready
	&& !player->powers[pw_super]
	&& leveltime > 0)
		P_DoSuperTransformation(player, false);
}

int SPM_MetalSonicFly(player_t *player)
{
	// SUPER METAL SONIC FLYS!
	if(player->charflags & SF_METAL
		&& player->powers[pw_super]
		//&& !(player->mfjumped && player->mo->momz > -(player->normalspeed*FRACUNIT/2))
		&& !player->mfspinning
		&& player->mo->state-states != S_PLAY_PLG1
		&& (player->mo->state-states < S_PLAY_FALL1
		|| player->mo->state-states > S_PLAY_FALL2))
	{
		player->mfjumped = true;
		if(player->cmd.buttons & BT_JUMP
			&& player->mo->momz + gravity/NEWTICRATERATIO
			< (player->normalspeed*FRACUNIT/2))
				return 2;
		else if(player->mo->momz < -(player->normalspeed*FRACUNIT/2))
		{
			player->mo->momz = -(player->normalspeed*FRACUNIT/2);
			return 1;
		}
	}
	return 0;
}

// SPM_DeathStuff
// Does cleanup stuff for when yer dead!
//
void SPM_DeathStuff(player_t *player)
{
	if(player->jet)
	{
		P_SetMobjState(player->jet, S_DISS);
		player->jet = NULL;
	}
}

#endif
