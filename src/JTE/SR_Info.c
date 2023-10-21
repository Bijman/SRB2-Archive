// Jason the Echidna
// By SR - Soviet Russia Blast 2
// MINE! NO STEALY!

#include "../doomstat.h"

#ifdef RUSSIA
#include "../d_englsh.h"
#include "../g_game.h"
#include "../m_menu.h"
#include "../m_random.h"
#include "../p_local.h"
#include "../p_mobj.h"
#include "../r_main.h"
#include "../s_sound.h"

#ifdef HWRENDER
#include "../hardware/hw_light.h"
#endif

#include "SR_Info.h"

boolean russia = false;

void SR_Init(void)
{
	fixed_t thing;

	thing = MT_EMPTYSHOES;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].spawnstate = S_EMPTYSHOES;
	//mobjinfo[thing].seesound = sfx_pop;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_SCENERY;

	thing = S_EMPTYSHOES;
	states[thing] = states[S_NULL];
	states[thing].sprite = SPR_SHOE;

	thing = SPR_SHOE;
	strcpy(sprnames[thing],"SHOE");
#ifdef HWRENDER
	t_lspr[thing] = &lspr[NOLIGHT];
#endif
}

// Monitor pops YOU
#define BLOODFLAGS (MF_NOBLOCKMAP|MF_SLIDEME|MF_NOCLIPTHING)
#define BLOODFUSE (10*TICRATE)
void SR_PlayerPop(player_t* player)
{
	const fixed_t ns = 10*FRACUNIT;
	int i;
	mobj_t *mo;

	// Blood SPEWS everywhere!
	for (i = 0; i < 32; i++)
	{
		const angle_t fa = (i*FINEANGLES/16) & FINEMASK;
		mo = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z+player->mo->height/2, MT_GOOP);
		mo->momx = FixedMul(finesine[fa],ns);
		mo->momy = FixedMul(finecosine[fa],ns);
		if (i > 15)
			mo->momz = ns;
		else
			mo->momz = player->mo->momz;
		mo->flags = BLOODFLAGS | (6<<MF_TRANSSHIFT);
		mo->fuse = BLOODFUSE;
	}

	// And one that goes straight up and back down, too.
	mo = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, MT_GOOP);
	mo->momz = ns*2;
	mo->flags = BLOODFLAGS | (6<<MF_TRANSSHIFT);
	mo->fuse = BLOODFUSE;

	// Spawn monitor pop
	S_StartSound(player->mo,sfx_pop);
	mo = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, MT_MONITOREXPLOSION);
	P_SetMobjState(mo, S_XPLD1);

	// Spawn shoes
	mo = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, MT_EMPTYSHOES);
	mo->angle = player->mo->angle;
	P_InstaThrust(mo, mo->angle, player->speed*FRACUNIT);
	mo->momz = player->mo->momz+8*FRACUNIT;
	mo->fuse = BLOODFUSE;

	// Pwn player
	P_ResetPlayer(player);
	P_KillMobj(player->mo, NULL, NULL);
	player->mo->health = 0; // So the rings stop following you and MF2_DONTDRAW isn't unset.
	P_UnsetThingPosition(player->mo);
	player->mo->momx = player->mo->momy = player->mo->momz = player->mo->pmomz = 0;
	player->mo->flags = MF_NOBLOCKMAP|MF_NOGRAVITY|MF_NOCLIP;
	player->mo->flags2 |= MF2_DONTDRAW;
	player->ring = mo;
	P_SetThingPosition(player->mo);

}
#undef BLOODFLAGS
#undef BLOODFUSE

// Just got collected
void SR_Collect(mobj_t* mo, mobj_t* ring)
{
	player_t *player;
	if(!mo || !ring || !mo->player
	|| mo->player->ring
	|| mo->player->powers[pw_flashing]
	|| mo->player->powers[pw_invulnerability]
	|| mo->player->exiting)
		return;
	player = mo->player;

	// Play death sound and change state
	P_PlayDeathSound(mo);
	player->ring = ring;

	// Set flags, yer as good as dead now.
	P_ResetPlayer(player);
	P_UnsetThingPosition(mo);
	mo->momx = mo->momy = mo->momz = mo->pmomz = 0;
	mo->flags = MF_NOBLOCKMAP|MF_NOGRAVITY|MF_NOCLIP;
	mo->flags2 |= MF2_DONTDRAW;
	P_SetThingPosition(mo);
}

void SR_RingSpill(mobj_t* ring)
{
	int i;
	mobj_t *mo;
	fixed_t ns;
	const angle_t fa = (ring->angle*FINEANGLES/16)&FINEMASK;

	for(i = 0; i < MAXPLAYERS; i++)
		if(playeringame[i]
		&& players[i].playerstate == PST_LIVE
		&& players[i].ring == ring
		&& players[i].health)
		{
			mo = players[i].mo;

			P_UnsetThingPosition(mo);
			mo->flags = mo->info->flags;
			mo->flags2 &= ~MF2_DONTDRAW;
			P_SetThingPosition(mo);

			mo->player->ring = NULL;
			mo->player->powers[pw_flashing] = flashingtics/2;

			ns = 3*FRACUNIT;
			mo->momx = FixedMul(finesine[fa],ns);
			if (!twodlevel)
				mo->momy = FixedMul(finecosine[fa],ns);
			mo->momz = 4*FRACUNIT;
			if ((i&1) && !(maptol & TOL_ADVENTURE))
				mo->momz += 4*FRACUNIT;
		}
}

static void SR_MoveRing(mobj_t* ring)
{
	if(!ring)
		return;

	// Ring runs forward.
	ring->momz = 0;
	P_InstaThrust(ring, ring->angle, 16*FRACUNIT);

	// Ring randomly changes direction.
	if(ring->movecount)
	{
		sector_t *nextsector = R_PointInSubsector(ring->x + ring->momx, ring->y + ring->momy)->sector;
		ring->movecount--;
		if(nextsector->floorheight > ring->z
		|| nextsector->ceilingheight < ring->z + ring->height)
			ring->movedir += ANG180;
		else if(ring->movecount == 0)
		{
			if(P_Random()&1)
				ring->movedir += ANG45;
			else
				ring->movedir -= ANG45;
		}
	}
	if(!ring->movecount)
		ring->movecount = (TICRATE/2)+P_Random()%(TICRATE*2);

	// Ring smoothly trys to turn in the direction it wants.
	if(ring->angle < ring->movedir)
	{
		ring->angle += ANG45/4;
		if(ring->angle > ring->movedir)
			ring->angle = ring->movedir;
	}
	else if(ring->angle > ring->movedir)
	{
		ring->angle -= ANG45/4;
		if(ring->angle < ring->movedir)
			ring->angle = ring->movedir;
	}
}

// Collected thinking
void SR_CollectThink(mobj_t* mo)
{
	int i;
	mobj_t *ring;

	if(!mo || !mo->player || !mo->player->ring)
		return;

	ring = mo->player->ring;

	// Only the lowest number player controls the ring.
	if(!(ring->flags & MF_MONITOR)
	&& !(ring->flags & MF_ENEMY)
	&& !(ring->flags & MF_BOSS))
	{
		for(i = 0; i < mo->player-players; i++)
			if(playeringame[i]
			&& players[i].ring == ring)
				break;
		// If you're controlling the ring, here's what it does...
		if(i == mo->player-players)
			SR_MoveRing(ring);

		// Ring decides to shoot you?
		if(mo->player->exiting
		|| (!(leveltime%TICRATE)
			&& ring->angle == ring->movedir
			&& P_Random() < 128))
		{
			// Reset your flags...
			P_UnsetThingPosition(mo);
			mo->flags = mo->info->flags;
			mo->flags2 &= ~MF2_DONTDRAW;
			P_SetThingPosition(mo);

			// Set your state, no ability use.
			P_SetPlayerMobjState(mo, S_PLAY_ATK1);
			mo->player->jumping = false;
			mo->player->mfjumped = true;
			mo->player->thokked = true;

			// Shooting!
			P_InstaThrust(mo, ring->angle, MAXMOVE);
			S_StartSound(ring, sfx_thok);

			// Pwning!
			mo->player->ring = NULL;
			mo->player->powers[pw_flashing] = flashingtics/2;
			return;
		}
	}

	// Set your position to your carrier's position.
	P_UnsetThingPosition(mo);
	mo->x = ring->x;
	mo->y = ring->y;
	mo->z = ring->z;
	mo->angle = ring->angle;
	mo->player->aiming = 0;
	if(mo->player-players == consoleplayer)
	{
		localangle = mo->angle;
		localaiming = mo->player->aiming;
	}
	else if(mo->player-players == secondarydisplayplayer)
	{
		localangle2 = mo->angle;
		localaiming = mo->player->aiming;
	}
	P_SetThingPosition(mo);
}

#endif
