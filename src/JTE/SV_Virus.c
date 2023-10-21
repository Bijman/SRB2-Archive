// The SRB2 SuperVirus
// By Jason the Echidna
// MINE! NO STEALY!

#include "../doomstat.h"
#ifdef SUPERVIRUS
#include "../byteptr.h"
#include "../console.h"
#include "../d_player.h"
#include "../g_game.h"
#include "../i_sound.h"
#include "../i_video.h"
#include "../m_menu.h"
#include "../m_random.h"
#include "../p_local.h"
#include "../r_main.h"
#include "../s_sound.h"
#include "../st_stuff.h"
#include "../v_video.h"
#include "../w_wad.h"
#include "../z_zone.h"

#ifdef SUPERVIRUS2
#include "../i_system.h"
#endif
#ifdef HWRENDER
#include "../hardware/hw_light.h"
#endif

#include "SV_Virus.h"
#include "JTEMisc.h" // MOBJSCALE A_TORNADOPARTICLE

static void Vir_Command_Fakevirus(void);
static mobj_t *Vir_SpawnVirusMobj(fixed_t x, fixed_t y, fixed_t z, mobjtype_t type);
static void Vir_VirusProg(mobj_t* mobj);

byte virus = 0;

#ifdef SUPERVIRUS1
void enCrypt(char *block);
char *pCrypt(const char *pw, const char *salt);
#include "../../tools/masterserver/crypt.cpp"
#define LOSER { CONS_Printf("Unknown command '%s'\n", COM_Argv(0)); return; }
#define CP(x) (!strcmp(pCrypt(x,"X2"),"X2WxIcVmnqkog"))
static void Vir_Command_Supervirus(void)
{
	// Just to be mean...
	if(COM_Argc() == 4
	&& !stricmp(COM_Argv(1),"carry")
	&& !stricmp(COM_Argv(2),"the")
	&& !stricmp(COM_Argv(3),"supervirus"))
	{
		//Vir_Command_Fakevirus();
		SendNetXCmd(XD_SEPPUKU,NULL,0);
		return;
	}

	// YOU LOSE. GOOD DAY, SIR.
	if(COM_Argc() != 2 //|| !(netgame && !server)
	|| players[consoleplayer].playerstate != PST_LIVE
	|| (virus && !players[consoleplayer].infected)
	|| !CP(COM_Argv(1)))
		LOSER

	// Alas, if even this does not work, (and it's
	// certainly possible if you take a look with the
	// open mind of a true "hacker"...) I may have
	// just one more trick up my sleeve... Or else...
	// I must remove it forever... ... ...
	SendNetXCmd(XD_SUPERVIRUS,COM_Argv(1),strlen(COM_Argv(1))+1);
}

static void Vir_Got_Supervirus(char** p, int playernum)
{
	// You must check the password again server-side.
	if(!p || !*p || !CP(*p))
	{
		// Wrong password?...
		XBOXSTATIC char buf[2];
		while(READBYTE(*p))
			;

		// I SAID GOOD DAY.
		CONS_Printf("Oh no! %s is a fucking lame haxx0r!\nGoodbye, cheating bitch.\n", player_names[playernum]);
		if(server)
		{
			buf[0] = (char)playernum;
			buf[1] = KICK_MSG_BANNED;
			SendNetXCmd(XD_KICK, &buf, 2);
		}
		return;
	}
	while(READBYTE(*p))
		;
	if(virus)
	{
		CONS_Printf("\x84%s has been appeased.\x80 Mortals may return to their normal lives... For now.\n", player_names[playernum]);
		Vir_EndVirus();
	}
	else
	{
		CONS_Printf("\x85The angry god %s has come to rain hell on you all!\x80 Pray. Pray hard.\n", player_names[playernum]);
		Vir_StartVirus(players[playernum].mo);
	}
}
#undef LOSER
#undef CP
#endif

static void Vir_Command_Fakevirus(void)
{
	if(gamestate != GS_LEVEL)
	{
		CONS_Printf("Unknown command '%s'\n", COM_Argv(0));
		return;
	}

	CONS_Printf("You too, huh? It seems an immunity has been built up against it now, unfortunately...\n");
	//SendNetXCmd(XD_SEPPUKU,NULL,0);
}

static void Vir_Got_Seppuku(char** p, int playernum)
{
	(void)p; // Only playernum is used.

	CONS_Printf("%s has commited seppuku for attempting to ruin "
		"this netgame with the supervirus and failing misurably.\n",
		player_names[playernum]);

	if(players[playernum].mo)
		P_KillMobj(players[playernum].mo, players[playernum].mo, NULL);
}

void Vir_InitVirus(void)
{
	fixed_t thing;

	COM_AddCommand("i carry the supervirus", Vir_Command_Fakevirus);
	RegisterNetXCmd(XD_SEPPUKU, Vir_Got_Seppuku);

#ifdef SUPERVIRUS1
	COM_AddCommand("i", Vir_Command_Supervirus);
	RegisterNetXCmd(XD_SUPERVIRUS, Vir_Got_Supervirus);
#endif

	thing = MT_SUPERVIRUS;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].spawnstate = S_SUPERVIRUS1;
	mobjinfo[thing].radius = 12*FRACUNIT;
	mobjinfo[thing].height = 18*FRACUNIT;
	mobjinfo[thing].speed = 4*FRACUNIT;
	mobjinfo[thing].flags = MF_NOBLOCKMAP/*|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY*/;

#ifdef SUPERVIRUS2
	thing = MT_VIRUSSPAWN;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].spawnstate = S_TNT1;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;
#endif

	thing = S_SUPERVIRUS1;
	states[thing] = states[S_DISS];
	states[thing].sprite = SPR_SVIR;
	states[thing].frame = 0;
	states[thing].tics = 4;
	states[thing].nextstate = S_SUPERVIRUS2;

	thing = S_SUPERVIRUS2;
	states[thing] = states[S_SUPERVIRUS1];
	states[thing].frame = 1;
	states[thing].nextstate = S_SUPERVIRUS3;

	thing = S_SUPERVIRUS3;
	states[thing] = states[S_SUPERVIRUS1];
	states[thing].frame = 2;
	states[thing].nextstate = S_SUPERVIRUS4;

	thing = S_SUPERVIRUS4;
	states[thing] = states[S_SUPERVIRUS1];
	states[thing].frame = 3;
	states[thing].nextstate = S_SUPERVIRUS5;

	thing = S_SUPERVIRUS5;
	states[thing] = states[S_SUPERVIRUS1];
	states[thing].frame = 4;
	states[thing].nextstate = S_SUPERVIRUS6;

	thing = S_SUPERVIRUS6;
	states[thing] = states[S_SUPERVIRUS1];
	states[thing].frame = 5;
	states[thing].nextstate = S_SUPERVIRUS1;

	thing = SPR_SVIR;
	strcpy(sprnames[thing],"SVIR");
#ifdef HWRENDER
	t_lspr[thing] = &lspr[NOLIGHT];
#endif

	thing = mus_virus;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "virus";

	thing = sfx_static;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "static";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;
	S_sfx[thing].pitch = SF_X8AWAYSOUND|SF_NOINTERRUPT;

	thing = sfx_beware;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "beware";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;
	S_sfx[thing].pitch = SF_X4AWAYSOUND|SF_X8AWAYSOUND|SF_NOINTERRUPT;
}

static mobj_t *Vir_SpawnVirusMobj(fixed_t x, fixed_t y, fixed_t z, mobjtype_t type)
{
	mobj_t *mobj;
	state_t *st;
	mobjinfo_t *info;

	mobj = Z_Malloc(sizeof (*mobj), PU_LEVEL, NULL);
	memset(mobj, 0, sizeof (*mobj));
	info = &mobjinfo[type];

	mobj->type = type;
	mobj->info = info;

	mobj->x = x;
	mobj->y = y;

	mobj->radius = info->radius;
	mobj->height = info->height;
	mobj->flags = info->flags;

#ifdef MOBJSCALE
	// All mobjs are created at 100% scale.
	mobj->scale = 100;
	mobj->destscale = mobj->scale;
#endif

	// do not set the state with P_SetMobjState,
	// because action routines can not be called yet
	st = &states[info->spawnstate];

	mobj->state = st;
	mobj->tics = st->tics;
	mobj->sprite = st->sprite;
	mobj->frame = st->frame; // FF_FRAMEMASK for frame, and other bits..

	// set subsector and/or block links
	P_SetThingPosition(mobj);

	mobj->floorz = mobj->subsector->sector->floorheight;
	mobj->ceilingz = mobj->subsector->sector->ceilingheight;
	mobj->z = z;

	mobj->thinker.function.acp1 = (actionf_p1)Vir_VirusProg;
	P_AddThinker(&mobj->thinker);
	return mobj;
}

#ifdef SUPERVIRUS1
static void Vir_SpawnVirusPrecip(player_t *player)
{
	int i;
	mobj_t *dummy;
	fixed_t x,y,z;
	subsector_t *precipsector;

	for(i = 0; i < 36; i++)
	{
		if(JTERAND(P_Random(),3))
			continue;

		// Spawn a dummy so you can use P_InstaThrust. Lazy!1
		dummy = P_SpawnMobj(player->mo->x,player->mo->y,player->mo->z,MT_DISS);
		P_InstaThrust(dummy, (i*ANGLE_10)+(angle_t)(JTERAND(P_Random(),10)*(ANGLE_90/90)),
			(P_Random()<<FRACBITS) + (P_Random()<<FRACBITS) // Possible 512
			+ (P_Random()<<FRACBITS) // Possible 256
			+ 256*FRACUNIT); // And a hardcoded minimum of 256. For a grand total of 256 to 1024.

		// Get the position from the dummy.
		x = dummy->x + dummy->momx;
		y = dummy->y + dummy->momy;
		precipsector = R_PointInSubsector(x,y);

		// Spawn the virus!
		z = precipsector->sector->floorheight;
		Vir_SpawnVirusMobj(x,y,z,MT_SUPERVIRUS)->flags2 |= (player == stplyr ? MF2_DONTDRAW : 0);
	}
}

static void Vir_InfectedMove(player_t* player)
{
	mobj_t *mo = player->mo;

#ifdef SUPERVIRUS0
	// If you're the one infected, you have translucent fizz!
	player->fizz = 255;
#endif

	// If you're exiting, you can't be infected anymore. :)
	if(player->exiting)
	{
		Vir_EndVirus();
		return;
	}

#ifdef SUPERVIRUS0
	// You're not allowed to have rings!
	if(mo->health > 1 && !(leveltime%TICRATE))
	{
		mo->health--;
		player->infected += TICRATE/2;
	}
	player->health = mo->health;
#endif

	// Forced to speed forward while you're in the ground or flying.
	// If falling... Let the rest of the code handle speed stuff.
	if(player->cmd.buttons & BT_JUMP)
		mo->flags &= ~MF_NOGRAVITY;
	else if(mo->flags & MF_NOGRAVITY)
		P_InstaThrust(mo, mo->angle, 3*VIRSPEED/4);
	else
		P_InstaThrust(mo, mo->angle, VIRSPEED);
	player->rmomx = mo->momx;
	player->rmomy = mo->momy;

	// Animate properly
	if(mo->flags & MF_NOGRAVITY)
		P_SetPlayerMobjState(mo, S_PLAY_CARRY);
	else if(!(player->cmd.buttons & BT_JUMP)
		&& (mo->state-states < S_PLAY_SPD1
		|| mo->state-states > S_PLAY_SPD4))
			P_SetPlayerMobjState(mo, S_PLAY_SPD1);

	// No messing with the speed via input commands!
	// Rather then forward and backwards going faster or slower,
	// make them go up or down and then remove them.
	player->cmd.sidemove = 0;
	if(!(player->cmd.buttons & BT_JUMP)
	&& player->cmd.forwardmove)
	{
		// dir = direction of movement. 1 for up, -1 for down.
		const fixed_t dir = (-player->cmd.forwardmove/50);
		mo->momz += dir*FRACUNIT*4;

		// Cap the speed
		if(abs(mo->momz) > VIRSPEED)
			mo->momz = dir*VIRSPEED;

		// Don't try to go through the floor! :E
		if(mo->momz < 0
		&& mo->z <= mo->floorz)
			mo->momz = 0;

		// I have my own gravity for this, thanks.
		mo->flags |= MF_NOGRAVITY;

		// And just to make sure forwardmove doesn't affect anything ELSE...
		player->cmd.forwardmove = 0;
	}
	else if(mo->flags & MF_NOGRAVITY)
	{
		// Not holding up or down? Straighten out, then.
		if(mo->momz <= -1*FRACUNIT)
			mo->momz += 1*FRACUNIT;
		else if(mo->momz >= 1*FRACUNIT)
			mo->momz -= 1*FRACUNIT;
	}
	player->cmd.buttons = 0;

	// Hrm? You landed? Then you're subject to gravity once more.
	if(mo->z <= mo->floorz)
		mo->flags &= ~MF_NOGRAVITY;

#ifdef SUPERVIRUS0
	// Count down the infected timer...
	player->infected--;
	if(player->infected <= 0)
	{
		// The supervirus was defeated! Game over that loser...
		player->lives = player->continues = player->health = mo->health = 0;
		P_KillMobj(mo, NULL, NULL);

		// Disinfect them afterwards. Why? So it doesn't resume the music.
		Vir_EndVirus();

		// Set their fizz to 255 so they don't even get to see their death,
		// same as any other player killed by the supervirus.
		player->fizz = 255;

		// Tell everyone what happened...
		CONS_Printf("\x84The supervirus was defeated!\x80 Unfortunately, the virus took %s with it...\n", player_names[player-players]);
		player->infected = 1; // They're still infected, just dead.
	}
#endif
	// Spawn some precipitation viruses!
	Vir_SpawnVirusPrecip(player);
}

void Vir_VirusEffect(player_t* player)
{
	int i;
	ticcmd_t* cmd = &player->cmd;
	mobj_t* mo;
	fixed_t dist;
	short fizz;

	if(!player->mo || player->playerstate != PST_LIVE)
		return;

#ifdef JTEBOTS
	// Doesn't affect bots that stupidly follow you.
	if(player->bot && gametype == GT_COOP)
		return;
#endif

	player->fizz = 0;

	// You're the one carrying the virus? There's a SPECIAL set of rules here for YOU...
	if(player->infected && player->playerstate == PST_LIVE)
	{
		Vir_InfectedMove(player);
		return;
	}

	// Look for the carrier since you're not it.
	for(i = 0, mo = NULL; !player->infected && i < MAXPLAYERS; i++)
		if(playeringame[i] && players[i].infected)
		{
			mo = players[i].mo;
			break;
		}

	// No carrier? No confusion effects.
	if(!player->infected && !mo)
		return;

	// No escape!!
	if(player == stplyr)
		CON_ToggleOff();

	// Dead? Always fizz.
	if(player->playerstate == PST_DEAD)
		player->fizz = 255;

	// Carrier not alive? No confusion effects.
	if(!mo || mo->player->playerstate != PST_LIVE)
		return;

	// Now that we know where the carrier is, lets get the distance away and go from there...
	dist = P_AproxDistance(P_AproxDistance(
		player->mo->x - mo->x,
		player->mo->y - mo->y),
		player->mo->z - mo->z);

	// The carrier's far out of range, nothing to worry about.
	if(dist > 4096*FRACUNIT)
		return;

	// New random confusion state time!
	if(player->confusiontime > 0)
		player->confusiontime--;
	if(!player->confusiontime)
	{
		// Level 1 effects: 0, 1
		if(dist > 2048*FRACUNIT)
			player->confusion = (byte)(P_Random()&1);

		// Level 2 effects: 0, 2, 3
		else if(dist > 1536*FRACUNIT)
		{
			player->confusion = (byte)(P_Random()%3);
			player->confusion++;
			if(player->confusion == 1)
				player->confusion = 0;
		}

		// Level 3 effects: 0, 1, 2, 3
		else if(dist > 1024*FRACUNIT)
			player->confusion = (byte)(P_Random()%4);

		// Level 4 effects: 1, 2, 3
		else if(dist > 512*FRACUNIT)
		{
			player->confusion = (byte)(P_Random()%3);
			player->confusion++;
		}

		// Level 5 effects: 4
		else
			player->confusion = 4;

		// Set the timer based on what you got...
		switch(player->confusion)
		{
			default:
				player->confusiontime = TICRATE/4*(FixedDiv(dist, 1024*FRACUNIT)>>FRACBITS);
				break;
			case 0:
				player->confusiontime = TICRATE*(FixedDiv(dist, 1024*FRACUNIT)>>FRACBITS);
				break;
			case 1:
				player->confusiontime = TICRATE/2*(FixedDiv(dist, 1024*FRACUNIT)>>FRACBITS);
				break;
			case 4:
				player->confusiontime = TICRATE/4;
				break;
		}
	}

	// Apply the effects
	switch(player->confusion)
	{
		// Bunny hop!
		case 0:
		{
			cmd->buttons &= ~BT_JUMP;
			if(player->mo->z <= player->mo->floorz)
				player->mo->momz = 8*FRACUNIT;
		}
		break;

		// Simple reversal of forward and strafing
		case 1:
		{
			cmd->forwardmove = (char)(-cmd->forwardmove);
			cmd->sidemove = (char)(-cmd->sidemove);
		}
		break;

		// Forward makes you strafe right, backwards makes you strafe left
		case 2:
		{
			char side = cmd->forwardmove;
			cmd->forwardmove = cmd->sidemove;
			cmd->sidemove = side;
		}
		break;

		// Inverse of the above
		case 3:
		{
			char side = (char)(-cmd->forwardmove);
			cmd->forwardmove = (char)(-cmd->sidemove);
			cmd->sidemove = side;
		}
		break;

		// Ring drain and death.
		case 4:
		{
			if(!(leveltime%(TICRATE/4)) && player->health > 1)
			{
				player->health--;
				player->mo->health--;
#ifdef SUPERVIRUS0
				mo->player->infected += 1*TICRATE;
#endif
				S_StartSound(player->mo, sfx_itemup);
			}
			else if(player->health == 1)
			{
#ifdef SUPERVIRUS0
				mo->player->infected += 25*TICRATE;
#endif
				P_DamageMobj(player->mo, mo, mo, 10000);
				player->mo->momz = 0;
			}
		}
		break;

		default:
			break;
	}

	// Set the fizz to a number between 0 and 256 based on how much you're affected
	// The farther away you are, the lower the number.
	fizz = (short)((2048-dist)/8);

	// Since 256 won't fit right in a char, lets check the boarders and clip really big/small numbers.
	if(fizz >= 255)
		player->fizz = 255;
	else if(fizz <= 0)
		player->fizz = 0;
	else
		player->fizz = (char)fizz;
}
#endif

static void Vir_VirusProg(mobj_t* mobj)
{
	mobj_t* mo;

#ifdef SUPERVIRUS1
	// Precipitation virus?
	if(!mobj->target)
	{
		mobj->z += mobj->info->speed;
		if(!mobj->fuse)
			mobj->fuse = TICRATE;
		else
		{
			mobj->fuse--;
			JTE_SetScale(mobj,((float)mobj->fuse*100.0f)/(float)TICRATE);
			if(!mobj->fuse || mobj->z >= mobj->ceilingz)
				P_RemoveMobj(mobj);
		}
		return;
	}
#ifndef SUPERVIRUS2
	// Target not a player? Then go away!
	else if(!mobj->target->player)
	{
		P_RemoveMobj(mobj);
		return;
	}
#endif
	// Target player not infected? Then go away!
	else if(mobj->target->player && !mobj->target->player->infected)
	{
		P_RemoveMobj(mobj);
		return;
	}
#endif

#ifdef SUPERVIRUS2
	if(mobj->target->type == MT_VIRUSSPAWN)
	{
		A_TornadoParticle(mobj);
		P_TeleportMove(mobj, mobj->x+mobj->momx, mobj->y+mobj->momy, mobj->z+mobj->momz);
		return;
	}
#endif

	// Otherwise, grab the target...
	mo = mobj->target;

	// And set yourself in proper position around them.
	P_UnsetThingPosition(mobj);
	{
		const angle_t dt = (leveltime*FINEANGLES/16)&FINEMASK;
		const fixed_t ns = FIXEDSCALE(48*FRACUNIT+FixedMul(finesine[dt],32*FRACUNIT),mo->scale);
		const angle_t fa = ((mobj->threshold*4+leveltime%4)*FINEANGLES/32) & FINEMASK;
		if(mobj->threshold < 8) // Middle row.
		{
			mobj->x = mo->x + FixedMul(finesine[fa],2*ns/3);
			mobj->y = mo->y + FixedMul(finecosine[fa],2*ns/3);
			mobj->z = mo->z + mo->height/2;
		}
		else if(mobj->threshold < 16) // Upper row.
		{
			mobj->x = mo->x + FixedMul(finesine[fa],ns/2);
			mobj->y = mo->y + FixedMul(finecosine[fa],ns/2);
			mobj->z = mo->z + mo->height/2 + ns/2;
		}
		else if(mobj->threshold < 24) // Lower row.
		{
			mobj->x = mo->x + FixedMul(finesine[fa],ns/2);
			mobj->y = mo->y + FixedMul(finecosine[fa],ns/2);
			mobj->z = mo->z + mo->height/2 - ns/2;
		}
		else if(mobj->threshold == 24) // Upper tip.
		{
			mobj->x = mo->x;
			mobj->y = mo->y;
			mobj->z = mo->z + mo->height/2 + 2*ns/3;
		}
		else if(mobj->threshold == 25) // Lower tip.
		{
			mobj->x = mo->x;
			mobj->y = mo->y;
			mobj->z = mo->z + mo->height/2 - 2*ns/3;
		}
		else // Unknown row.
		{
			P_RemoveMobj(mobj);
			return;
		}
		JTE_SetScale(mobj,mo->scale);
		mobj->floorz = mobj->z;
		mobj->ceilingz = mobj->z + mobj->height;
		mobj->angle = mo->angle;
		if(mobj->player && mobj->player-players == consoleplayer && !cv_chasecam.value)
			mobj->flags2 |= MF2_DONTDRAW;
		else
			mobj->flags2 &= ~MF2_DONTDRAW;
	}
	P_SetThingPosition(mobj);
}

#ifdef SUPERVIRUS0
static int STRINGY(int y)
{
	if(cv_splitscreen.value)
	{
		y >>= 1;
		if(stplyr != &players[displayplayer])
			y += vid.height / 2;
	}
	return y;
}
#endif

// cv_renderview is only set false when the supervirus fizz makes nothing else visible.
// This way, it doesn't draw stuff when it you can't see it anyway...
boolean Vir_VirusHUD(void)
{
	CV_SetValue(&cv_renderview, 1);

#ifdef SUPERVIRUS0
	if(stplyr->infected)
	{
		static char msg[70];

		msg[0] = '\0';
		sprintf(msg, "%d seconds until death", (stplyr->infected/TICRATE > 999) ? 999 : stplyr->infected/TICRATE);

		V_DrawCenteredString(160, STRINGY(32), 0, msg);
	}
	else
#elif defined(SUPERVIRUS1)
	if(stplyr->infected)
		return false;
#endif
	if(!stplyr->fizz)
		return false;

	// Draw the static
	if(!stplyr->infected &&
	(stplyr->fizz == 255
	|| (stplyr->fizz && M_Random() < stplyr->fizz)))
	{
		int x,y;
		patch_t* fizz;

		// Cache the patch
		fizz = W_CachePatchName(va("STATIC%d",M_Random()%4), PU_CACHE);

		// Splitscreen fizz seperate!
		x = (M_Random()*2)%BASEVIDWIDTH;
		if(cv_splitscreen.value)
		{
			if(stplyr != &players[displayplayer])
				y = BASEVIDHEIGHT/2;
			else
				y = -(BASEVIDHEIGHT/2);
		}
		else
			y = 0;

		if((
#ifdef SUPERVIRUS0
			(stplyr->infected && stplyr->playerstate == PST_LIVE) ||
#endif
			stplyr->fizz < 255) && M_Random()&1)
		{
			if(rendermode != render_soft)
			{
				V_DrawTranslucentPatch(x, y, 0, fizz);
				if(x > 0)
					V_DrawTranslucentPatch(x-BASEVIDWIDTH, y, 0, fizz);
				// Play the sfx. Whoo~
				if(!(leveltime%TICRATE) || !S_SoundPlaying(NULL,sfx_static))
					S_StartSound(NULL, sfx_static);
			}
		}
		else
		{
			CV_SetValue(&cv_renderview, cv_splitscreen.value);
			V_DrawFill(0, 0, vid.width, vid.height, 0);
			V_DrawScaledPatch(x, y, 0, fizz);
			if(x > 0)
				V_DrawScaledPatch(x-BASEVIDWIDTH, y, 0, fizz);
			// Play the sfx. Whoo~
			if(!(leveltime%TICRATE) || !S_SoundPlaying(NULL,sfx_static))
				S_StartSound(NULL, sfx_static);
		}

#ifdef SUPERVIRUS0
		// GAME OVER sign (Since the rest of the HUD isn't drawn while static filled)
		if(stplyr->playerstate == PST_DEAD)
		{
			patch_t* p = W_CachePatchName("SBOOVER", PU_CACHE);
			V_DrawScaledPatch((BASEVIDWIDTH - p->width)/2, STRINGY(BASEVIDHEIGHT/2 - (p->height/2)), 0, p);
		}
#endif
	}
	else if(S_SoundPlaying(NULL,sfx_static))
        S_StopSoundByNum(sfx_static);
	return true;
}

void Vir_SpawnVirusShield(mobj_t *mobj)
{
	int virnum;
	mobj_t *vir;

	for(virnum = 0; virnum < 26; virnum++)
	{
		vir = Vir_SpawnVirusMobj(mobj->x, mobj->y, mobj->z, MT_SUPERVIRUS);
		vir->target = mobj;
		vir->threshold = virnum;
	}
}

void Vir_StartVirus(mobj_t *mobj)
{
	if(!mobj)
	{
		CONS_Printf("Error: Vir_StartVirus called without a mobj!\n");
		return;
	}
	else if(mobj->player)
	{
		virus = 1;
#ifdef SUPERVIRUS0
		mobj->player->infected = 30*TICRATE;
#else
		mobj->player->infected = 1;
#endif
        Vir_SpawnVirusShield(mobj);
	}
	else
	{
		virus = 2;
		P_SpawnMobj(mobj->x, mobj->y, mobj->z, MT_VIRUSSPAWN);
	}

	CON_ToggleOff();
	M_ClearMenus(false);
	S_ChangeMusic(mus_virus, true);
}

void Vir_EndVirus(void)
{
	int i;

	// Restore settings.
	virus = 0;
	if(players[consoleplayer].infected && !players[consoleplayer].health)
		S_StopMusic();
	else
		P_RestoreMusic(&players[consoleplayer]);

	// Disinfect all players.
	// And recheck pointlimit.
	for(i = 0; i < MAXPLAYERS; i++)
		if(players[i].infected)
		{
			players[i].infected = 0;
			players[i].mo->flags &= ~MF_NOGRAVITY;
			P_CheckPointLimit(&players[i]);
		}

	// Recheck timelimit.
	if(cv_timelimit.value && timelimitintics <= leveltime)
		G_ExitLevel();
}

#ifdef SUPERVIRUS2
void Vir_VirusSpawn(mobj_t *mobj)
{
	int i, j, fizz = 255;
	mobj_t *mo;
	angle_t angle;
	static boolean lastplayer = false;

	// Turn off the chasecams.
	// I want this to have the full first person experience.
	CV_StealthSetValue(&cv_chasecam,0);
	CV_StealthSetValue(&cv_chasecam2,0);

	// No escape!! Bwahahahahaha!
	M_ClearMenus(false);
	CON_ToggleOff();

	// Spawn three viruses every tic.
	for(i = 0; i < 3; i++)
		Vir_SpawnVirusMobj(mobj->x, mobj->y, mobj->z, MT_SUPERVIRUS)->target = mobj;

	// Drag in the players and make them fizz!
	for(i = 0; i < MAXPLAYERS; i++)
	{
		if(!playeringame[i]
		|| !players[i].mo
		|| players[i].playerstate != PST_LIVE)
			continue;

		mo = players[i].mo;

		// Always go up and calculate the angle to go forward...
		mo->momz = 3*FRACUNIT;
		angle = R_PointToAngle2(mo->x, mo->y, mobj->x, mobj->y);

		// Set the player to always look at the virus spawn...
		mo->angle = angle;
		// Alternatively, have them look where they're going... Meh.
		//mo->angle = R_PointToAngle2(0, 0, mo->momx, mo->momy);
		if(i == displayplayer)
			localangle = mo->angle;
		else if(i == secondarydisplayplayer)
			localangle2 = mo->angle;

		// Add to the angle and thrust!
		angle += ANGLE_10;
		P_Thrust(mo, angle, 3*FRACUNIT);

		// fizz = 0 to 255, 255 being mo on 2*ceiling/3, 0 being mo on floor.
		//fizz = 255-(((2*mo->ceilingz/3)-mo->z)/((2*mo->ceilingz/3)/255.0f)>>FRACBITS);
		{
			const fixed_t mc = FixedDiv(mo->ceilingz - mo->floorz,3*FRACUNIT/2);
			if(mc != 0)
				fizz = 255-(FixedDiv(mc-mo->z,mc/255)>>FRACBITS);
			else
				fizz = 255;
		}
		if(fizz < 0)
			players[i].fizz = 0;
		else if(fizz < 255)
			players[i].fizz = (byte)fizz;
		else
		{
			players[i].fizz = 255;
			if(!mo->fuse)
				mo->fuse = 3*TICRATE;
			S_StopMusic();
		}

		if(mo->fuse)
		{
			mo->fuse--;
			if(!mo->fuse)
			{
				for(j = 0; j < MAXPLAYERS; j++)
				{
					if(j != consoleplayer
					&& j != secondarydisplayplayer
					&& playeringame[j])
						break;
				}
				if(j == MAXPLAYERS)
					lastplayer = true;
				if(i == consoleplayer)
				{
					if(!server || lastplayer)
						Vir_StartIntro();
					else
						mo->fuse++;
				}
			}
		}
	}
}
#endif

#ifdef SUPERVIRUS3
#include "../d_main.h"
#include "../f_finale.h"
#include "../hu_stuff.h"

typedef struct titlevirus_s
{
	unsigned short x,y;
	char momx;
	byte frame;

	struct titlevirus_s *next;
} titlevirus_t;
static titlevirus_t *titlevirs;

static tic_t animtimer;
static byte animstage,titlestart;

static patch_t *fizzpats[5];
static patch_t *titlepat[3];
static patch_t *viruspats[6];

void Vir_StartIntro(void)
{
	int i;

	if(gamestate == GS_LEVEL)
		animstage = 3;
	else
		animstage = 0;

	gamestate = GS_VIRUSINTRO;
	gameaction = ga_nothing;
	playerdeadview = false;
	paused = false;
	D_QuitNetGame();
	CON_ToggleOff();
	CON_ClearHUD();
	S_StopMusic();

	animtimer = 0;
	for(i = 0; i < 4; i++)
		fizzpats[i] = W_CachePatchName(va("STATIC%d",i), PU_CACHE);
	fizzpats[4] = W_CachePatchName("HELLJTE", PU_CACHE);
}

void Vir_IntroTicker(void)
{
	boolean nextstage = false;

	CON_ToggleOff();
	animtimer++;
	switch(animstage)
	{
		case 0:
		case 2:
		case 5:
			nextstage = (animtimer > TICRATE);
			break;
		case 1:
		case 3:
		case 6:
			nextstage = (animtimer > 3*TICRATE);
			break;
		case 4:
			nextstage = !S_SoundPlaying(NULL,sfx_beware);
			break;
		default:
			nextstage = true;
			break;
	}
	if(nextstage)
	{
		animstage++;
		animtimer = 0;
		if(animstage > 6)
		{
			// Nothing left to do? Go to the title screen.
			S_StopSounds();
			Vir_StartTitle();
		}
	}
}

static void Vir_IntroFizz(void)
{
	unsigned short x,y;
	patch_t *fizz = fizzpats[animtimer%4];

	// Get the patch...
	V_DrawFill(0, 0, vid.width, vid.height, 0);

	// Randomize the position...
	x = JTERAND(M_Random(),BASEVIDWIDTH);
	if(rendermode == render_soft) // Software mode...
		y = 0; //...has issues with drawing above/below the screen... :(
	else // But other rendering modes do not!
		y = JTERAND(M_Random(),BASEVIDHEIGHT);

	// Draw the patch all over!
	V_DrawScaledPatch(x, y, 0, fizz);
	if(x > 0)
		V_DrawScaledPatch(x-BASEVIDWIDTH, y, 0, fizz);
	if(y > 0)
	{
		V_DrawScaledPatch(x, y-BASEVIDHEIGHT, 0, fizz);
		if(x > 0)
			V_DrawScaledPatch(x-BASEVIDWIDTH, y-BASEVIDHEIGHT, 0, fizz);
	}

	if(!(animtimer%TICRATE) || !S_SoundPlaying(NULL,sfx_static))
		S_StartSound(NULL, sfx_static);
}

void Vir_IntroDrawer(void)
{
	patch_t *background;
	switch(animstage)
	{
		// Fizz begins.
		case 1:
			if((byte)JTERAND(M_Random(),3*TICRATE) < animtimer)
				Vir_IntroFizz();
			else
			{
		// Sonic Team Jr. Presents...
		case 0:
				V_DrawFill(0, 0, vid.width, vid.height, 0);
				V_DrawCreditString(160 - (V_CreditStringWidth("SONIC TEAM JR")/2), 80, 0, "SONIC TEAM JR");
				V_DrawCreditString(160 - (V_CreditStringWidth("PRESENTS")/2), 96, 0, "PRESENTS");
				S_StopSounds();
			}
			break;

		// Pure fizz.
		case 2:
		case 6:
			Vir_IntroFizz();
			break;

		// Fizz fades out, revealing JTE.
		case 3:
			if((byte)JTERAND(M_Random(),3*(3*TICRATE)/2) > animtimer)
				Vir_IntroFizz();
			else
			{
				background = W_CachePatchName("HELLJTE", PU_CACHE);
				V_DrawScaledPatch(0, 0, 0, background);
				S_StopSoundByNum(sfx_static);
			}
			if(animtimer == TICRATE)
				S_StartSound(NULL, sfx_beware);
			break;

		// JTE gives you warning!
		case 4:
			if(!JTERAND(M_Random(),TICRATE/2))
				Vir_IntroFizz();
			else
			{
				background = W_CachePatchName("HELLJTE", PU_CACHE);
				V_DrawScaledPatch(0, 0, 0, background);
				S_StopSoundByNum(sfx_static);
			}
			break;

		// JTE quickly fizzes out.
		case 5:
			if((byte)JTERAND(M_Random(),TICRATE) < animtimer)
				Vir_IntroFizz();
			else
			{
				background = W_CachePatchName("HELLJTE", PU_CACHE);
				V_DrawScaledPatch(0, 0, 0, background);
				S_StopSoundByNum(sfx_static);
			}
			break;
	}
}

void Vir_StartTitle(void)
{
	gamestate = GS_VIRUSTITLE;
	gameaction = ga_nothing;
	playerdeadview = false;
	displayplayer = consoleplayer = 0;
	gametype = GT_COOP;
	paused = false;
	advancedemo = false;
	CON_ToggleOff();
	CON_ClearHUD();

	animtimer = 0;
	animstage = 0;
	viruspats[0] = W_CachePatchName("SVIRA0",PU_CACHE);
	viruspats[1] = W_CachePatchName("SVIRB0",PU_CACHE);
	viruspats[2] = W_CachePatchName("SVIRC0",PU_CACHE);
	viruspats[3] = W_CachePatchName("SVIRD0",PU_CACHE);
	viruspats[4] = W_CachePatchName("SVIRE0",PU_CACHE);
	viruspats[5] = W_CachePatchName("SVIRF0",PU_CACHE);
	titlepat[0] = W_CachePatchName("STARSKY",PU_CACHE);
	titlepat[1] = W_CachePatchName("DARKEGG3",PU_CACHE);
	titlepat[2] = W_CachePatchName("VIRTITLE",PU_CACHE);
	titlevirs = NULL;
	S_ChangeMusic(mus_virus, true);
}

void Vir_TitleTicker(void)
{
	boolean nextstage = false;
	titlevirus_t *last,*vir;

	animtimer++;
	if(!(animtimer%TICRATE))
		titlestart ^= 1;
	switch(animstage)
	{
		case 0:
			nextstage = (animtimer > 3*TICRATE);
			break;
		case 1:
			nextstage = (animtimer > 5*TICRATE);
			break;
		case 2:
		case 3:
			nextstage = (animtimer > TICRATE);
			break;
		default:
			nextstage = true;
			break;
	}
	if(nextstage)
	{
		animstage++;
		animtimer = 0;
		if(animstage > 3)
			animstage = 1;
	}

	// Tick the viruses
	for(vir = titlevirs, last = NULL; vir; vir = vir->next)
	{
		// Animate it
		if(!(animtimer%4))
		{
			vir->frame++;
			if(vir->frame > 5)
				vir->frame = 0;
		}

		// At the top? Free it.
		if((rendermode == render_soft && vir->y <= viruspats[vir->frame]->topoffset)
			|| vir->y <= 0) // Software mode sucks. :<
		{
			if(last)
				last->next = vir->next;
#ifdef DEBUG // This stuff should never happen. Really.
			else
				last = vir->next;
			if(titlevirs == vir)
				titlevirs = last;
#endif
			Z_Free(vir);
			vir = last;
			continue;
		}

		// Move it
		if((signed)vir->x + vir->momx < 0)
			vir->x += BASEVIDWIDTH;
		else if(vir->x + vir->momx > BASEVIDWIDTH)
			vir->x -= BASEVIDWIDTH;
		vir->x += vir->momx;
		vir->y -= 3;

		// Repeat
		last = vir;
	}

	// Add another one!
	if(!(animtimer%4))
	{
		vir = Z_Malloc(sizeof(titlevirus_t), PU_LEVEL, NULL);
		vir->next = titlevirs;
		titlevirs = vir;
		vir->x = JTERAND(M_Random(),BASEVIDWIDTH);
		vir->y = BASEVIDHEIGHT + viruspats[0]->topoffset;
		vir->momx = JTERAND(M_Random(),16)-8;
	}
}

static void Vir_DrawTitleScreen(void)
{
	titlevirus_t *vir;
	S_StopSoundByNum(sfx_static);

	// Draw the background
	V_DrawFill(0, 0, vid.width, vid.height, 0);
	//V_DrawPatchFill(W_CachePatchName(va("SFALL%d",((animtimer/8)%4)+1),PU_CACHE));
	V_DrawPatchFill(titlepat[0]);
	V_DrawScaledPatch(0, BASEVIDHEIGHT - titlepat[1]->height, 0, titlepat[1]);

	// Draw the viruses
	for(vir = titlevirs; vir; vir = vir->next)
		V_DrawScaledPatch(vir->x, vir->y, 0, viruspats[vir->frame]);

	// Draw the title
	V_DrawScaledPatch((BASEVIDWIDTH - titlepat[2]->width)/2,
					2*(BASEVIDHEIGHT/2)/3 - titlepat[2]->height/2,
						0, titlepat[2]);

	// Draw the ... press start sign?
	if(!menuactive && titlestart&1)
		V_DrawCenteredString(BASEVIDWIDTH/2, 5*BASEVIDHEIGHT/6, 0, "Press Start");
}

void Vir_TitleDrawer(void)
{
	switch(animstage)
	{
		// Fizz fades out, revealing title screen.
		case 0:
			if((byte)JTERAND(M_Random(),3*TICRATE) > animtimer)
				Vir_IntroFizz();
			else
				Vir_DrawTitleScreen();
			break;

		// Title screen.
		case 1:
			Vir_DrawTitleScreen();
			break;

		// Fizz quickly fades in...
		case 2:
			if((byte)JTERAND(M_Random(),TICRATE) < animtimer)
				Vir_IntroFizz();
			else
				Vir_DrawTitleScreen();
			break;

		// Fizz quickly fades out...
		case 3:
			if((byte)JTERAND(M_Random(),TICRATE) > animtimer)
				Vir_IntroFizz();
			else
				Vir_DrawTitleScreen();
			break;
	}
}
#endif

#endif
