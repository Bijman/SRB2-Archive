// SU - SRB2 Universe
// By Jason the Echidna
// MINE! NO STEALY!

#include "../../doomstat.h"
#ifdef UNIVERSE

#include "../../d_player.h"
#include "../../g_game.h"
#include "../../m_random.h"
#include "../../p_local.h"
#include "../../r_main.h"
#include "../../r_defs.h"
#include "../../s_sound.h"
#include "../../z_zone.h"

#ifdef HWRENDER
#include "../../hardware/hw_light.h"
#endif
#include "../../hardware/hw3sound.h"

#include "../JB_Bot.h"
#include "SU_Info.h"
#include "SU_Game.h"
#include "SU_Menu.h"
#include "SU_Command.h"

void SU_Init(void)
{
	SU_InitCons();
	SU_InitItems();
	SU_InitInfo();
}

// Returns a random number between min and max!
static long SU_Random(long min, long max)
{
	long i;
	long num = 0;

	if(max <= min)
		return min;

	for(num = i = min; i < max; i++)
		num += P_Random()%2;

	return num;
}

void SU_LevelUp(player_t *player, int amount)
{
	int i;

	if(!universe)
		return;

	player->suexp += amount;
	while(player->sulevel < 200
		&& player->suexp >= (unsigned int)(600*player->sulevel))
	{
		player->suexp -= 600*player->sulevel;
		player->sulevel++;

		if(player == &players[consoleplayer])
		   CONS_Printf("* Level up! New Level: %d\n",player->sulevel);
		else
			CONS_Printf("* Level up! %s is now level: %d\n",player_names[player-players], player->sulevel);

		S_ChangeMusic(mus_sulvl, false);

		for(i = 0; i < MAXPLAYERS; i++)
			if(playeringame[i])
				players[i].powers[pw_extralife] = 3*TICRATE;

		player->sumaxhp += SU_Random(3,8);
		player->sumaxtp += SU_Random(3,8);
		player->health = player->mo->health = player->sumaxhp;
		player->sutp  = player->sumaxtp;
	}
}

boolean SU_EnemyDropStuff(mobj_t* target)
{
	mobj_t* mo;

	if(!universe)
		return false;

	if(target->flags & MF_ENEMY)
	{
		mo = P_SpawnMobj(target->x, target->y,
				P_FloorzAtPos(target->x, target->y, target->z, target->height),
				MT_SUDROP);
		mo->fuse = TICRATE/3;
		return true;
	}
	else if(target->flags & MF_BOSS)
	{
		mo = P_SpawnMobj(target->x, target->y,
				P_FloorzAtPos(target->x, target->y, target->z, target->height),
				MT_SUBOSSDROP);
		mo->fuse = TICRATE/3;
		return true;
	}
	return false;
}

void SU_GameplayStuff(player_t *player)
{
	if(!universe)
		return;

#ifdef JTEBOTS
	///////////////////////
	// SU BOT MANAGEMENT //
	///////////////////////
	// Bots always have their owner's stats and items and whatnot :P
	// Well, actually, they don't carry items or money, but they still have the same stuff equipped...
	if(player->bot && gametype == GT_COOP)
	{
		int i;

		player->sulevel = players[player->bot->ownernum].sulevel;
		player->sumaxhp = players[player->bot->ownernum].sumaxhp;
		player->sumaxtp = players[player->bot->ownernum].sumaxtp;
		player->suexp = 0;
		player->sumoney = 0;

		player->equipweapon = players[player->bot->ownernum].equipweapon;
		player->equipbarrier = players[player->bot->ownernum].equipbarrier;
		player->equiparmor = players[player->bot->ownernum].equiparmor;
		player->equipmag = players[player->bot->ownernum].equipmag;
		for(i = 0; i < 4; i++)
			player->equipslot[i] = players[player->bot->ownernum].equipslot[i];
		return;
	}
#endif

	/////////////////////////
	// SU LEVEL MANAGEMENT //
	/////////////////////////

	// Just started? Lets hook you up with a n00b pack!
	if(player->sulevel < 1)
	{
		player->sulevel = 1;
		player->suexp = 0;

		// Random HP from 30 to 100, but only even tens.
		player->sumaxhp = SU_Random(3,10);
		player->sumaxhp *= 10;

		// TP is the inverse of HP + 30, so having 30 HP gives you 100 TP and vice versa.
		player->sumaxtp = 130 - player->sumaxhp;

		player->health = player->mo->health = player->sumaxhp;
		player->sutp = player->sumaxtp;

		player->suitems[itm_saber] = 1;
		player->suitems[itm_frame] = 1;
		//player->suitems[itm_mag] = 1;
		player->equipweapon = itm_saber;
		player->equiparmor = itm_frame;
		//player->equipmag = itm_mag;
	}

	// Exceeding the ABSOLUTE MAXIMUM hp or tp? Set it to the absolute max, then...
	if(player->sulevel > 200)
		player->sulevel = 200;
	if(player->sumaxhp > 9999)
		player->sumaxhp = 9999;
	if(player->sumaxtp > 9999)
		player->sumaxtp = 9999;

	// Exceeding your level's max hp or tp? Set it to the current max, then...
	if((unsigned)player->health > player->sumaxhp && player->health > 0)
		player->health = player->mo->health = player->sumaxhp;
	if(player->sutp > player->sumaxtp)
		player->sutp = player->sumaxtp;

	//////////////////////
	// SWORD MANAGEMENT //
	//////////////////////
	// While you have your sword out, dash around swinging it like a madman!
	if(player->powers[pw_sword] > 0)
	{
		// Fly around, whee!!
		P_InstaThrust(player->mo, player->mo->angle, 40*FRACUNIT);
		player->rmomx = player->mo->momx;
		player->rmomy = player->mo->momy;
		if(player->mo->momz < 0)
			player->mo->momz = 0;

		// Hrm? Not running for some reason? No problem, just start now!
		if(!player->running)
		{
			P_SetPlayerMobjState(player->mo, S_PLAY_SPD1);
			player->running = 1;
		}
	}

	// Didn't hit anything? Oh well...
	if(player->powers[pw_sword] == 1)
	{
		// In the air? Fall!
		if(player->mo->z > player->mo->floorz)
		{
			P_SetPlayerMobjState(player->mo, S_PLAY_FALL1);
			P_InstaThrust(player->mo, player->mo->angle, player->normalspeed*FRACUNIT);
			player->rmomx = player->mo->momx;
			player->rmomy = player->mo->momy;
		}
		// On the floor? Go backwards a bit...
		else if(!player->dashspeed)
		{
			P_InstaThrust(player->mo, player->mo->angle - ANG180, 10*FRACUNIT);
			player->rmomx = player->mo->momx;
			player->rmomy = player->mo->momy;
		}
		// Hrm? Not using a one-hit weapon? Just go normal speed I guess...
		// Wouldn't want to go backwards into any enemys you just hit!
		else
		{
			P_InstaThrust(player->mo, player->mo->angle, player->normalspeed*FRACUNIT);
			player->rmomx = player->mo->momx;
			player->rmomy = player->mo->momy;
		}

		player->powers[pw_flashing] = TICRATE/2; // Just in case you were only a liiiiittle too far away...
		player->weapondelay = TICRATE; // And no using it lots of times at once, mister!
		player->dashspeed = 0; // Reset the dashspeed thingy... o.o Just in case, I guess.
	}

	// Decrease each tic.
	if(player->powers[pw_sword] > 0)
		player->powers[pw_sword]--;
}

boolean SU_Fuse(mobj_t* mobj)
{
	int prandom;
	angle_t lastangle;
	mobj_t* mo;

	switch(mobj->type)
	{
		case MT_PAYMACHINE:
			mobj->fuse = 1;
			return true;

		case MT_SUDROP:
		case MT_SUBOSSDROP:
		prandom = (int)(P_Random()/2.55f); // 255 / 2.55 = 100
		if((mobj->type == MT_SUBOSSDROP && prandom < 80) // 80% chance of item if a boss.
		|| prandom < 30) // 30% chance of item if not a boss.
			prandom = (int)(P_Random()/2.55f);
			if(prandom < 30)
			{
				mo = P_SpawnMobj(mobj->x,mobj->y,mobj->z + 24*FRACUNIT, MT_SUMONEY);
				mo->health = (int)((P_Random()+1)/2.56f); // 256 / 2.56 = 100
				mo->flags = (mo->flags & ~MF_TRANSLATION) | ((15)<<MF_TRANSSHIFT); // Gold money
			}
			else
			{
				// Lower rarity = More common.
				prandom = 0;
				do {
					mobj->health = SU_Random(itm_invalid+1, NUMSUITEMS); // Pick an item, any item.
					prandom++;
					if(prandom > 100)
					{
						//CONS_Printf("Warning: Random generator timeout (%d,%d,%d,%d)\n",
						//	min, max, mobj->health, suitems[mobj->health].rarity);
						break;
					}
					// Got an item too rare? Respin!
				} while(suitems[mobj->health].rarity == 0
					|| P_Random()/2.55f > suitems[mobj->health].rarity);

				mo = P_SpawnMobj(mobj->x, mobj->y, mobj->floorz, MT_SUITEM);
				mo->health = mobj->health+1;
				if(suitems[mobj->health].type & SUT_SPECIAL
					|| suitems[mobj->health].rarity < 10)
						mo->flags = (mo->flags & ~MF_TRANSLATION) | ((6)<<MF_TRANSSHIFT); // Red rare items!
				else if(suitems[mobj->health].type & SUT_WEAPON)
					mo->flags = (mo->flags & ~MF_TRANSLATION) | ((7)<<MF_TRANSSHIFT); // Blue weapons
				else if(suitems[mobj->health].type & SUT_BARRIER
					|| suitems[mobj->health].type & SUT_FRAME
					|| suitems[mobj->health].type & SUT_SLOT)
						mo->flags = (mo->flags & ~MF_TRANSLATION) | ((5)<<MF_TRANSSHIFT); // Orange barriers and armor
				else
						mo->flags = (mo->flags & ~MF_TRANSLATION) | ((12)<<MF_TRANSSHIFT); // Green other items
			}
			P_SetMobjState(mobj,S_DISS);
			return true;
		case MT_STARTTELEPORT:
		case MT_ENDTELEPORT:
		case MT_TELEPIPE:
			// Create teleport sparkles to mark your location...
			if(mobj->lastlook)
			{
				mo = P_SpawnMobj(mobj->x, mobj->y, mobj->z, MT_TELESPARK1);
				lastangle = (angle_t)(mobj->movedir);
			}
			else
			{
				mo = P_SpawnMobj(mobj->x, mobj->y, mobj->z, MT_TELESPARK2);
				lastangle = (angle_t)(mobj->threshold);
			}

			mo->target = mobj; // Make it circle you...
			mo->fuse = TICRATE + P_Random()%(2*TICRATE); // It will randomly disappear after a second. Or two. Or three.
			mobj->fuse = 2*TICRATE/12; // Make about 12 sparkles at a time. (6 of each type)

			// Make it start at the last point in the circle plus an angle...
			mo->angle = (lastangle + FINEANGLES/6 + (P_Random()%2)*(FINEANGLES/12)) & FINEMASK;

			if(mobj->type == MT_STARTTELEPORT)
				mo->flags = (mo->flags & ~MF_TRANSLATION) | ((7)<<MF_TRANSSHIFT); // Blue
			else if(mobj->type == MT_ENDTELEPORT)
				mo->flags = (mo->flags & ~MF_TRANSLATION) | ((6)<<MF_TRANSSHIFT); // Red
			else
				mo->flags = (mo->flags & ~MF_TRANSLATION) | ((mobj->friction)<<MF_TRANSSHIFT);

			// Set the last angle for this type of sparkle to the new angle.
			if(mobj->lastlook)
				mobj->movedir = mo->angle;
			else
				mobj->threshold = mo->angle;

			mobj->lastlook = !mobj->lastlook; // Toggle what you'll make next.
			return true;

		default:
			break;
	}
	return false;
}

boolean SU_MobjProg(mobj_t* mobj)
{
	fixed_t gravityadd = -gravity/4;


	switch(mobj->type)
	{
		case MT_STARTTELEPORT:
		case MT_ENDTELEPORT:
			if(mobj->fuse == 0)
				mobj->fuse = TICRATE/4;
			return true;

		case MT_TELESPARK1:
		{
			if(!mobj->target)
				return true;

			// Make this sparkle bounce!
			if(mobj->z - 16*FRACUNIT <= mobj->target->z)
				mobj->momz = FRACUNIT + (P_Random()%5)*FRACUNIT;
			else if(mobj->momz > -3*FRACUNIT)
				mobj->momz += gravityadd;
		}
		// Leave this fallthrough
		case MT_TELESPARK2:
			if(!mobj->target)
				return true;

			// The TELESPARK mobj angle is already "fine" because it's not used for anything else.
			// Unless someone makes an edit of the sprites that gives them an actual angle, of course.
			// But that'd probably just look cool. o_o

			// 4 seconds to go full circle.
			mobj->angle = (mobj->angle + FINEANGLES/(4*TICRATE)) & FINEMASK;

			// Set it to it's new position
			P_UnsetThingPosition(mobj);
			mobj->x = mobj->target->x + FixedMul(finecosine[mobj->angle], mobj->target->radius);
			mobj->y = mobj->target->y + FixedMul(finesine[mobj->angle], mobj->target->radius);
			if(mobj->z < mobj->target->z)
				mobj->z = mobj->target->z;
			P_SetThingPosition(mobj);

			mobj->momx = mobj->momy = 0;
			return true;

		case MT_PAYMACHINE:
			mobj->fuse++;
			return true;

		case MT_SUMONEY:
		case MT_SUITEM:
			// Gently rotate and stay on the floor
			mobj->angle -= ANGLE_10;
			if(mobj->z > mobj->floorz)
				mobj->z = mobj->floorz;
			return true;

		case MT_KINESO:
			return true;

		case MT_BARREL:
			// Barrel contains water/slime/lava/whatever else
			if(mobj->movecount > 1)
				return true;
			// Barrel contains dead person, so clean it out!
			else if(mobj->movecount == 1 && (!mobj->target || !mobj->target->health))
			{
				mobj->movecount = 0;
				mobj->target = NULL;
			}
			return false;

		default:
			break;
	}
	return false;
}

boolean SU_SpecialItems(mobj_t* toucher, mobj_t* special)
{
	player_t* player = toucher->player;
	mobj_t* dummymo;

	switch(special->type)
	{
		case MT_PAYMACHINE: // Pay Machine: Exchange rings for items!
			if(!player)
				return true;

			if(toucher->subsector->sector == special->subsector->sector
				|| !toucher->player->mfjumped)
					return true;

			if(!universe)
			{
				if(!(toucher->player->health >= special->health))
					return true;

				// Mark the player as 'pull into the capsule'
				toucher->player->capsule = special;
				toucher->player->capsule->reactiontime = 1;
				return true;
			}
			if(!(toucher->player->sumoney >= (unsigned)special->health))
				return true;

			// Mark the player as 'pull into the capsule'
		   	toucher->player->capsule = special;
			toucher->player->capsule->reactiontime = 1;
			return true;

		case MT_SUMONEY:
			if(player && toucher->player->powers[pw_flashing] > flashingtics/2)
				return true;
			if(mariomode)
				P_SpawnMobj (special->x,special->y,special->z, MT_SPARK)->momz = special->momz;
			else
				P_SpawnMobj (special->x,special->y,special->z, MT_SPARK);

			if(player)
			{
				if(!universe)
				{
					player->health += special->health;
					player->mo->health = player->health;
					player->totalring += special->health;
				}
				else
				{
					player->sumoney += special->health;
#ifdef JTEBOTS
					if(player->bot && gametype == GT_COOP)
						players[player->bot->ownernum].sumoney += special->health;
#endif

					if(player == &players[consoleplayer]
#ifdef JTEBOTS
					|| (player->bot && gametype == GT_COOP && player->bot->ownernum == consoleplayer)
#endif
					)
						CONS_Printf("* Collected %d Meseta!\n",special->health);
				}
			}
			if(!player)
				toucher->health += special->health;

			if((mapheaderinfo[gamemap-1].typeoflevel & TOL_NIGHTS) || cv_timeattacked.value) // Nights and SRB2 Universe? Why not?
			{
				if(player)
				{
					player->linkcount++;
					player->linktimer = 2*TICRATE;
				}

				dummymo = P_SpawnMobj(special->x, special->y, special->z, MT_NIGHTSCORE);

				if(!player)
					P_SetMobjState(dummymo, dummymo->info->spawnstate+9);

				if(player)
				{
					if(player->linkcount < 10)
					{
						if(player->bonustime)
						{
							P_AddPlayerScore(player, player->linkcount*20);
							P_SetMobjState(dummymo, dummymo->info->xdeathstate+player->linkcount-1);
						}
						else
						{
							P_AddPlayerScore(player, player->linkcount*10);
							P_SetMobjState(dummymo, dummymo->info->spawnstate+player->linkcount-1);
						}
					}
					else
					{
						if(player->bonustime)
						{
							P_AddPlayerScore(player, 200);
							P_SetMobjState(dummymo, dummymo->info->xdeathstate+9);
						}
						else
						{
							P_AddPlayerScore(player, 100);
							P_SetMobjState(dummymo, dummymo->info->spawnstate+9);
						}
					}
				}
				dummymo->momz = FRACUNIT;
				dummymo->fuse = 3*TICRATE;
				if(toucher->target)
				{
					if(toucher->target->flags & MF_AMBUSH)
						P_InstaThrust(dummymo, R_PointToAngle2(dummymo->x, dummymo->y, toucher->target->x, toucher->target->y), 3*FRACUNIT);
					else
						P_InstaThrust(dummymo, R_PointToAngle2(toucher->target->x, toucher->target->y, dummymo->x, dummymo->y), 3*FRACUNIT);
				}
			}
			return false;

		case MT_SUITEM:
			if(!player)
				return true;
			if(player && toucher->player->powers[pw_flashing] > flashingtics/2)
				return true;

			special->health--; // Was set up by 1 since items with a health of 0 are un-collectable.

			if(mariomode)
				P_SpawnMobj (special->x,special->y,special->z, MT_SPARK)->momz = special->momz;
			else
				P_SpawnMobj (special->x,special->y,special->z, MT_SPARK);

			player->suitems[special->health]++;
#ifdef JTEBOTS
			if(player->bot && gametype == GT_COOP)
				players[player->bot->ownernum].suitems[special->health]++;
#endif
			if((player == &players[consoleplayer]
			&& suitems[special->health].name)
#ifdef JTEBOTS
			|| (player->bot && gametype == GT_COOP && player->bot->ownernum == consoleplayer)
#endif
			)
				CONS_Printf("* Collected a %s!\n",suitems[special->health].name);


			if((mapheaderinfo[gamemap-1].typeoflevel & TOL_NIGHTS) || cv_timeattacked.value) // Nights and SRB2 Universe? Why not?
			{
				if(player)
				{
					player->linkcount++;
					player->linktimer = 2*TICRATE;
				}

				dummymo = P_SpawnMobj(special->x, special->y, special->z, MT_NIGHTSCORE);

				if(!player)
					P_SetMobjState(dummymo, dummymo->info->spawnstate+9);

				if(player)
				{
					if(player->linkcount < 10)
					{
						if(player->bonustime)
						{
							P_AddPlayerScore(player, player->linkcount*20);
							P_SetMobjState(dummymo, dummymo->info->xdeathstate+player->linkcount-1);
						}
						else
						{
							P_AddPlayerScore(player, player->linkcount*10);
							P_SetMobjState(dummymo, dummymo->info->spawnstate+player->linkcount-1);
						}
					}
					else
					{
						if(player->bonustime)
						{
							P_AddPlayerScore(player, 200);
							P_SetMobjState(dummymo, dummymo->info->xdeathstate+9);
						}
						else
						{
							P_AddPlayerScore(player, 100);
							P_SetMobjState(dummymo, dummymo->info->spawnstate+9);
						}
					}
				}
				dummymo->momz = FRACUNIT;
				dummymo->fuse = 3*TICRATE;
				if(toucher->target)
				{
					if(toucher->target->flags & MF_AMBUSH)
						P_InstaThrust(dummymo, R_PointToAngle2(dummymo->x, dummymo->y, toucher->target->x, toucher->target->y), 3*FRACUNIT);
					else
						P_InstaThrust(dummymo, R_PointToAngle2(toucher->target->x, toucher->target->y, dummymo->x, dummymo->y), 3*FRACUNIT);
				}
			}
			return false;

		case MT_STARTTELEPORT:
			if(player-players == consoleplayer && player->cmd.buttons & BT_LIGHTDASH)
			{
				player->cmd.buttons &= BT_LIGHTDASH;
				SU_Teleport2LastMenu();
			}
			return true;
		case MT_ENDTELEPORT:
			if(player-players == consoleplayer && player->cmd.buttons & BT_LIGHTDASH)
			{
				player->cmd.buttons &= BT_LIGHTDASH;
				SU_Teleport2NextMenu();
			}
			return true;
		case MT_TELEPIPE:
			if(player-players == consoleplayer && player->cmd.buttons & BT_LIGHTDASH)
			{
				player->cmd.buttons &= BT_LIGHTDASH;
				SU_Teleport2HUBMenu();
			}
			return true;

		default:
			return false;
	}
}

void SU_UseSpecialWeapon(player_t* player, suitemtype_t item)
{
	player = NULL;
	item = NUMSUITEMS;
}

void SU_UseSpecialShield(player_t* player, suitemtype_t item)
{
	player = NULL;
	item = NUMSUITEMS;
}

void SU_UseSpecialItem(player_t* player, suitemtype_t item)
{
	player = NULL;
	item = NUMSUITEMS;
}

static inline int SU_CalcDamage(mobj_t* target, mobj_t* source, int damage)
{
	if(damage > 0)
	{
		// Original damage setting for players
		if(source->player)
		{
			if(source->player->equipweapon
			&& source->player->equipweapon < NUMSUITEMS
			&& suitems[source->player->equipweapon-1].damage > 0)
				damage = suitems[source->player->equipweapon].damage + source->player->sulevel + 1;
			else
				damage = source->player->sulevel + 1;
		}

		// Original damage setting for enemys and other crap
		else
		{
			if(sumobjinfo[source->type] && sumobjinfo[source->type]->damage)
				damage = sumobjinfo[source->type]->damage;
			else if(source->info->spawnhealth)
				damage = source->info->spawnhealth*3;
			else
				return false;
		}
	}

	// Damage randomness and armor.
	if(target->player)
	{
		// Add randomness to the damage...
		damage += 3+(P_Random()%8);

		// Subtract damage for player level...
		damage -= target->player->sulevel*(1+(P_Random()%2));

		// Subtract damage for barrier...
		if(target->player->equipbarrier)
			damage -= suitems[target->player->equipbarrier].damage;

		// Subtract damage for armor...
		if(target->player->equiparmor)
			damage -= suitems[target->player->equiparmor].damage;
	}
	else
	{
		damage += 5+(P_Random()%5);
		if(sumobjinfo[target->type])
			damage -= SU_Random(sumobjinfo[target->type]->level, sumobjinfo[target->type]->level+20);
		else
			damage -= P_Random()%10;
	}

	// Hehe, oops, subtracted too much damage!
	if(damage <= 0)
		damage = 1;

	return damage;
}

boolean SU_DamageMobj(mobj_t* target, mobj_t* inflictor, mobj_t* source, int damage)
{
	angle_t ang;
	player_t* player;

	if(!source || !target)
		return false;

	damage = SU_CalcDamage(target, source, damage);

	if(target->flags & MF_ENEMY)
	{
		if(target->fuse) // Invincible
			return false;

		if(target->health - damage > 1)
		{
			if(inflictor)
				ang = R_PointToAngle2(inflictor->x, inflictor->y, target->x, target->y);
			else
				ang = R_PointToAngle2(source->x, source->y, target->x, target->y);

			if(target->info->painsound) S_StartSound(target, target->info->painsound);
			target->fuse = TICRATE;
			target->flags2 |= MF2_FRET;

			target->z++;
			target->momz += FixedDiv(source->momz,27*FRACUNIT/20);
			if(target->momz < 5*FRACUNIT && target->momz >= 0)
				target->momz = 5*FRACUNIT;
			else if(target->momz > -5*FRACUNIT && target->momz < 0)
				target->momz = -5*FRACUNIT;

			P_InstaThrust(target, ang, target->momz);
		}
		else
		{
			target->flags2 &= ~MF2_DONTDRAW;
			target->flags |= MF_NOGRAVITY;
			target->fuse = 0;
			target->momx = target->momy = target->momz = 0;
		}

		if(source->player)
		{
			if(source->player->mfjumped)
			{
				if(source->momz < 0)
					source->momz = -source->momz;
				if(source->momz < 5*FRACUNIT)
					source->momz = FixedMul(5*FRACUNIT,27*FRACUNIT/20);
			}
			if(source->player->powers[pw_sword])
			{
				ang = R_PointToAngle2(target->x, target->y, source->x, source->y);

				// If you have infinitive hits on your sword...
				if(source->player->dashspeed == 1)
				{
					// Don't do anything to stop you yet. Instead just get the flashing from the hit.
					source->player->powers[pw_flashing] = TICRATE;
				}
				// If you have 2 hits using this sword...
				else if(source->player->dashspeed == 2)
				{
					// This is your first hit, so turn the attack into a single-hit now.
					source->player->dashspeed = 0;
				}
				else
				{
					source->player->powers[pw_sword] = 0;
					source->player->powers[pw_flashing] = TICRATE;
					source->z++;
					if(source->eflags & MF_UNDERWATER)
						source->momz = (10511*FRACUNIT)/2600;
					else
						source->momz = (69*FRACUNIT)/10;
					P_InstaThrust(source, ang, 4*FRACUNIT);
					P_SetPlayerMobjState(source, S_PLAY_FALL1);
				}
			}
		}
	}
	else if(target->flags & MF_BOSS)
	{
		if(target->health - damage > 0)
		{
			target->flags2 |= MF2_FRET;
			target->flags |= MF_TRANSLATION;

			if(source->player)
				if(source->player->powers[pw_sword])
				{
					ang = R_PointToAngle2(source->x, source->y, target->x, target->y);

					source->player->powers[pw_sword] = 0;
					source->player->powers[pw_flashing] = TICRATE;
					source->z++;
					if(source->eflags & MF_UNDERWATER)
						source->momz = (10511*FRACUNIT)/2600;
					else
						source->momz = (69*FRACUNIT)/10;
					P_InstaThrust(source, ang-ANG180, 4*FRACUNIT);
					P_SetPlayerMobjState(source, S_PLAY_FALL1);
				}
		}
	}

	player = target->player;

	if(player && player->exiting)
		return false;

	if(player) // Player is the target
	{
		if(player->nightsmode && !player->powers[pw_flashing])
		{
			angle_t fa;

			P_UnsetThingPosition(target);
			player->angle_pos = player->old_angle_pos;
			player->speed /= 5;
			player->flyangle += 180;
			player->flyangle %= 360;

			if(source && source->player)
			{
				if(player->nightstime > 20)
					player->nightstime -= 20;
				else
					player->nightstime = 1;
			}
			else
			{
				if(player->nightstime > 5)
					player->nightstime -= 5;
				else
					player->nightstime = 1;
			}

			fa = FINEANGLE_C(player->flyangle);

			target->x = target->target->x + FixedMul(finecosine[fa],target->target->info->radius);
			target->y = target->target->y + FixedMul(finesine[fa],target->target->info->radius);

			target->momx = target->momy = 0;
			P_SetThingPosition(target);
			//player->powers[pw_flashing] = flashingtics;
			player->powers[pw_flashing] = TICRATE;
			P_SetMobjState(target->tracer, S_NIGHTSHURT1);
			S_StartSound(target, sfx_nghurt);
		}

		if(inflictor && (inflictor->flags & MF_FIRE))
		{
			if(player->powers[pw_fireshield]
#ifdef SPMOVES
			|| player->charflags & SF_FLAME
#endif
			)
				return false; // Invincible to fire objects

			if((gametype == GT_COOP || gametype == GT_RACE) && source && source->player)
				return false; // Don't get hurt by fire generated from friends.
		}

		if(source && source->player) // Player hits another player
		{
			if(source == target) // You can't kill yourself, idiot...
				return false;

			if(!cv_friendlyfire.value && (gametype == GT_COOP ||
				gametype == GT_RACE
#ifdef CHAOSISNOTDEADYET
				 || gametype == GT_CHAOS
#endif
))
				return false;

			if((gametype == GT_MATCH || gametype == GT_TAG || gametype == GT_CTF) && cv_suddendeath.value
				&& !player->powers[pw_flashing] && !player->powers[pw_invulnerability])
				damage = -1; // Instant-death!
		}

		if(player->cheats & CF_GODMODE)
			return false;
		else if(damage == -1)
		{
			player->powers[pw_fireshield] = false;
			player->powers[pw_jumpshield] = false; // Get rid of shield
			player->powers[pw_ringshield] = false;
			player->powers[pw_bombshield] = false;
			player->powers[pw_watershield] = false;
			player->mo->momx = player->mo->momy = player->mo->momz = 0;

			player->powers[pw_fireflower] = false;
			player->mo->flags =  (player->mo->flags & ~MF_TRANSLATION)
			                   | ((player->skincolor)<<MF_TRANSSHIFT);

			if(player->powers[pw_underwater] != 1) // Don't jump up when drowning
				player->mo->momz = 18*FRACUNIT;
			else
				player->mo->momz++;

			P_ForceFeed(player, 40, 10, TICRATE, 40 + min(damage, 100)*2);

			if(source && source->type == MT_DISS && source->threshold == 42) // drowned
				S_StartSound(target, sfx_drown);
			else
				P_PlayDeathSound(target);

			P_SetPlayerMobjState(target, S_PLAY_DIE1);
			if(gametype == GT_CTF && (player->gotflag & MF_REDFLAG || player->gotflag & MF_BLUEFLAG))
				P_PlayerFlagBurst(player, false);
			if(source && source->player)
				P_AddPlayerScore(source->player, 100);
		}
		else if(player->powers[pw_invulnerability] || player->powers[pw_flashing]
			|| player->powers[pw_super]) // ignore bouncing & such in invulnerability
		{
			if(inflictor && (inflictor->flags & MF_MISSILE)
				&& (inflictor->flags2 & MF2_SUPERFIRE)
				&& player->powers[pw_super])
			{
				fixed_t fallbackspeed;

				P_ForceFeed(player, 40, 10, TICRATE, 40 + min(damage, 100)*2);
				damage = 0; // Don't take rings away
				player->mo->z++;

				if(player->mo->eflags & MF_UNDERWATER)
					player->mo->momz = (10511*FRACUNIT)/2600;
				else
					player->mo->momz = (69*FRACUNIT)/10;

				ang = R_PointToAngle2(inflictor->x,	inflictor->y, target->x, target->y);

				// explosion and rail rings send you farther back, making it more difficult
				// to recover
				if(inflictor->flags2 & MF2_EXPLOSION)
				{
					if(inflictor->flags2 & MF2_RAILRING)
						fallbackspeed = 24*FRACUNIT; // 6x
					else
						fallbackspeed = 16*FRACUNIT; // 4x
				}
				else if(inflictor->flags2 & MF2_RAILRING)
					fallbackspeed = 12*FRACUNIT; // 3x
				else
					fallbackspeed = 4*FRACUNIT; // the usual amount of force

				P_InstaThrust(target, ang, fallbackspeed);

#ifdef SPMOVES
				if(player->charability != 0)
					P_SetPlayerMobjState(target, S_PLAY_PAIN);
				else
#endif
				P_SetPlayerMobjState(target, S_PLAY_SUPERHIT);
				//target->player->powers[pw_flashing] = flashingtics;
				target->player->powers[pw_flashing] = TICRATE;

				player->mo->flags = (player->mo->flags & ~MF_TRANSLATION)
									| ((player->skincolor)<<MF_TRANSSHIFT);

				P_ResetPlayer(player);
				return true;
			}
			else
				return false;
		}
		else if(player->powers[pw_fireshield] || player->powers[pw_jumpshield] || player->powers[pw_ringshield] || player->powers[pw_bombshield] || player->powers[pw_watershield])  //If One-Hit Shield
		{
			player->powers[pw_jumpshield] = false; // Get rid of shield
			player->powers[pw_ringshield] = false;
			player->powers[pw_watershield] = false;
			player->powers[pw_fireshield] = false;

			if(player->powers[pw_bombshield]) // Give them what's coming to them!
			{
				player->blackow = 1; // BAM!
				player->powers[pw_bombshield] = false;
				player->jumpdown = true;
			}
			P_ForceFeed(player, 40, 10, TICRATE, 40 + min(damage, 100)*2);
			damage = 0; // Don't take rings away
			player->mo->z++;

			if(player->mo->eflags & MF_UNDERWATER)
				player->mo->momz = (10511*FRACUNIT)/2600;
			else
				player->mo->momz = (69*FRACUNIT)/10;

			if(inflictor == NULL)
				P_InstaThrust (player->mo, player->mo->angle-ANG180, 4*FRACUNIT);
			else
			{
				fixed_t fallbackspeed;

				ang = R_PointToAngle2(inflictor->x,	inflictor->y, target->x, target->y);

				// explosion and rail rings send you farther back, making it more difficult
				// to recover
				if(inflictor->flags2 & MF2_EXPLOSION)
				{
					if(inflictor->flags2 & MF2_RAILRING)
						fallbackspeed = 24*FRACUNIT; // 6x
					else
						fallbackspeed = 16*FRACUNIT; // 4x
				}
				else if(inflictor->flags2 & MF2_RAILRING)
					fallbackspeed = 12*FRACUNIT; // 3x
				else
					fallbackspeed = 4*FRACUNIT; // the usual amount of force

				P_InstaThrust(target, ang, fallbackspeed);
			}

			P_SetPlayerMobjState(target, target->info->painstate);
			//target->player->powers[pw_flashing] = flashingtics;
			target->player->powers[pw_flashing] = TICRATE;

			player->powers[pw_fireflower] = false;
			player->mo->flags = (player->mo->flags & ~MF_TRANSLATION)
								| ((player->skincolor)<<MF_TRANSSHIFT);

			P_ResetPlayer(player);

			if(source && (source->type == MT_DISS || source->type == MT_FLOORSPIKE || source->type == MT_CEILINGSPIKE) && source->threshold == 43) // spikes
				S_StartSound(target, sfx_spkdth);
			else
				S_StartSound (target, sfx_shldls); // Ba-Dum! Shield loss.

#ifdef SCALEMARIO
			if(mariomode)
				target->destscale = MARIOSIZE;
#endif

			if(gametype == GT_CTF && (player->gotflag & MF_REDFLAG || player->gotflag & MF_BLUEFLAG))
				P_PlayerFlagBurst(player, false);
			if(source && source->player)
			{
				P_AddPlayerScore(source->player, cv_match_scoring.value == 1 ? 25 : 50);
			}
			return true;
		}
		else if(player->mo->health > damage) // No shield but have rings.
		{
			//damage = player->mo->health - 1;

			player->mo->z++;

			player->powers[pw_fireflower] = false;
			player->mo->flags =  (player->mo->flags & ~MF_TRANSLATION)
								| ((player->skincolor)<<MF_TRANSSHIFT);

			if(player->mo->eflags & MF_UNDERWATER)
				player->mo->momz = (10511*FRACUNIT)/2600;
			else
				player->mo->momz = (69*FRACUNIT)/10;

			if(inflictor == NULL)
				P_InstaThrust (player->mo, player->mo->angle-ANG180, 4*FRACUNIT);
			else
			{
				fixed_t fallbackspeed;

				ang = R_PointToAngle2(inflictor->x,	inflictor->y, target->x, target->y);

				// explosion and rail rings send you farther back, making it more difficult
				// to recover
				if(inflictor->flags2 & MF2_EXPLOSION)
				{
					if(inflictor->flags2 & MF2_RAILRING)
						fallbackspeed = 24*FRACUNIT; // 6x
					else
						fallbackspeed = 16*FRACUNIT; // 4x
				}
				else if(inflictor->flags2 & MF2_RAILRING)
					fallbackspeed = 12*FRACUNIT; // 3x
				else
					fallbackspeed = 4*FRACUNIT; // the usual amount of force

				P_InstaThrust(target, ang, fallbackspeed);
			}

			P_ForceFeed(player, 40, 10, TICRATE, 40 + min(damage, 100)*2);

			P_ResetPlayer(player);

			if(source && (source->type == MT_DISS || source->type == MT_FLOORSPIKE || source->type == MT_CEILINGSPIKE) && source->threshold == 43) // spikes
				S_StartSound(target, sfx_spkdth);

			// Ring loss sound plays despite hitting spikes
			S_StartSound (target, sfx_shldls); // Ba-Dum! Shield loss.
			//P_PlayRinglossSound(target); // Ringledingle!
#ifdef SCALEMARIO
			if(mariomode)
				target->destscale = MARIOSIZE;
#endif

			if(gametype == GT_CTF && (player->gotflag & MF_REDFLAG || player->gotflag & MF_BLUEFLAG))
				P_PlayerFlagBurst(player, false);
			if(source && source->player)
				P_AddPlayerScore(source->player, 50);
		}
		else // No shield, no rings, no invincibility.
		{
			//damage = 1;
			player->mo->momz = 18*FRACUNIT;
			player->mo->momx = player->mo->momy = 0;

			player->powers[pw_fireflower] = false;
			player->mo->flags =  (player->mo->flags & ~MF_TRANSLATION)
								| ((player->skincolor)<<MF_TRANSSHIFT);

			P_ForceFeed(player, 40, 10, TICRATE, 40 + min(damage, 100)*2);

			P_ResetPlayer(player);

			if(source && (source->type == MT_DISS || source->type == MT_FLOORSPIKE || source->type == MT_CEILINGSPIKE) && source->threshold == 43) // Spikes
				S_StartSound(target, sfx_spkdth);
			else
				P_PlayDeathSound(target);

			if(gametype == GT_CTF && (player->gotflag & MF_REDFLAG || player->gotflag & MF_BLUEFLAG))
				P_PlayerFlagBurst(player, false);
			if(source && source->player)
				P_AddPlayerScore(source->player, 100);
		}
	}

	// player specific
	if(player)
	{
		player->health -= damage; // mirror mobj health here for Dave

		if(player->health < 0)
			player->health = 0;

		if(damage > 0 && player->health > 0)
			//target->player->powers[pw_flashing] = flashingtics;
			player->powers[pw_flashing] = TICRATE;

		P_ForceFeed(player, 40, 10, TICRATE, 40 + min(damage, 100)*2);
	}

	if(cv_killingdead.value && source && source->player && P_Random() < 192)
		P_DamageMobj(source, target, target, 1);

	// do the damage
	target->health -= damage;
	if(target->health <= 0)
	{
		P_KillMobj(target, inflictor, source);
		return true;
	}

	if(target->player)
		P_SetPlayerMobjState(target, target->info->painstate);
	else
		P_SetMobjState(target, target->info->painstate);

	if(target->player)
		P_ResetPlayer(target->player);

	target->reactiontime = 0; // we're awake now...

	if(source && source != target)
	{
		// if not intent on another player,
		// chase after this one
		target->target = source;
		target->threshold = BASETHRESHOLD;
		if(target->state == &states[target->info->spawnstate] && target->info->seestate != S_NULL)
		{
			if(target->player)
				P_SetPlayerMobjState(target, target->info->seestate);
			else
				P_SetMobjState(target, target->info->seestate);
		}
	}

	return true;
}

void SU_FiringCode(player_t* player)
{
	if(player->equipweapon
		&& suitems[player->equipweapon].type & SUT_WEAPON
		&& gamemap != SULOBBYMAP
		&& gamemap != SUHUBMAP
		&& !player->attackdown
		&& !player->weapondelay
		&& !player->powers[pw_sword]
		&& !player->exiting
		&& !player->thokked
		&& !player->powers[pw_flashing])
	{
		const sutype_t type = suitems[player->equipweapon].type;
		if(type & SUT_SPECIAL)
		{
			// Special-use weapons.
			SU_UseSpecialWeapon(player, player->equipweapon);
		}
		else if(type & SUW_HANDGUN
			|| type & SUW_RIFLE
			|| type & SUW_PUNCH)
		{
			if(type & SUW_HANDGUN)
				player->weapondelay = TICRATE/4;
			else
				player->weapondelay = TICRATE/2;

			if(suitems[player->equipweapon].firetype != MT_DISS)
				P_SpawnPlayerMissile(player->mo, suitems[player->equipweapon].firetype);
		}
		else if(type & SUW_SABER
			|| type & SUW_CANE
			|| type & SUW_CLAW
			|| type & SUW_KNUCKLE
			|| type & SUW_SWORD
			|| type & SUW_ROD
			|| type & SUW_DAGGER
			|| type & SUW_WAND
			|| type & SUW_DOUBLESABER)
		{
			P_InstaThrust(player->mo, player->mo->angle, 40*FRACUNIT);
			player->rmomx = player->mo->momx;
			player->rmomy = player->mo->momy;
			player->running = 1;

			// Short range single target weapons
			if(type & SUW_SABER
			|| type & SUW_CANE
			|| type & SUW_CLAW
			|| type & SUW_KNUCKLE)
			{
				player->powers[pw_sword] = 5;
				player->dashspeed = 0;
			}
			// Medium range multiple target weapons
			else if(type & SUW_SWORD
				|| type & SUW_ROD)
			{
				player->powers[pw_sword] = 7;
				player->dashspeed = 1;
			}
			// Long range two-hit weapons
			else if(type & SUW_DAGGER
				|| type & SUW_WAND
				|| type & SUW_DOUBLESABER)
			{
				player->powers[pw_sword] = 9;
				player->dashspeed = 2;
			}

			P_SetPlayerMobjState(player->mo, S_PLAY_SPD1);
			S_StartAttackSound(player->mo, sfx_zoom);
		}
		player->thokked = (player->mo->z > player->mo->floorz); // If you're not on the floor, set thokked to 1!
	}
	else if(!(suitems[player->equipweapon].type & SUT_WEAPON))
		player->equipweapon = 0;

	player->attackdown = true;
}


void SU_BarrelPop(mobj_t* mobj)
{
	mobj_t* barrel = mobj->tracer;

	// Set flags back to normal
	mobj->flags2 &= ~MF2_DONTDRAW;
	mobj->flags &= ~MF_NOCLIPTHING;

	// De-link them
	barrel->movecount = 0;
	barrel->target = NULL;
	mobj->tracer = NULL;

	// Position for popping...
	P_UnsetThingPosition(mobj);
	mobj->x = barrel->x;
	mobj->y = barrel->y;
	mobj->z = barrel->z + barrel->height + 2*FRACUNIT;
	P_SetThingPosition(mobj);
	mobj->momx = barrel->momx;
	mobj->momy = barrel->momy;

	// Player-specific stuff
	if(mobj->player)
	{
		P_ResetPlayer(mobj->player);
		P_SetPlayerMobjState(mobj, S_PLAY_PLG1);
		P_DoJump(mobj->player, true); // Players JUMP out, rather then popping out.
		mobj->player->jumping = false; // But no cutting the jump height
		mobj->momz -= mobj->momz / 8; // And take off 1/8th plz...
	}
	else
	{
		P_SetMobjState(mobj, mobj->info->spawnstate);

		// Pop!
		if(barrel->momz > 0)
			mobj->momz = barrel->momz + 9*FRACUNIT;
		else
			mobj->momz = 9*FRACUNIT;
	}
}

#endif
