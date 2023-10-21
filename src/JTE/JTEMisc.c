// JTE Misc
// By Jason the Echidna
// MINE! NO STEALY!

#include "JTEDefs.h"
#include "JTEMisc.h"
#include "../doomstat.h"

#include "JB_Bot.h" // JTEBOTS

#if defined(CCPAL) || defined(HOLIDAYS)
#include "../r_main.h"
#define DISTMAP         2
void JTE_ReinitLightTables(void)
{
	int         i;
	int         j;
	int         level;
	int         startmap;
	int         scale;

	// Calculate the light levels to use for each level/scale combination.
	for (i=0 ; i< LIGHTLEVELS ; i++)
	{
		startmap = ((LIGHTLEVELS-1-i)*2)*NUMCOLORMAPS/LIGHTLEVELS;
		for (j=0 ; j<MAXLIGHTZ ; j++)
		{
			// zlight table
			scale = FixedDiv ((BASEVIDWIDTH/2*FRACUNIT), (j+1)<<LIGHTZSHIFT);
			scale >>= LIGHTSCALESHIFT;
			level = startmap - scale/DISTMAP;

			if(level < 0)
				level = 0;

			if(level >= NUMCOLORMAPS)
				level = NUMCOLORMAPS-1;

			zlight[i][j] = colormaps + level*256;
		}

		for(j = 0; j < MAXLIGHTSCALE; j++)
		{
			// scalelight table
			level = startmap - j*vid.width/(viewwidth)/DISTMAP;

			if(level < 0)
				level = 0;

			if(level >= NUMCOLORMAPS)
				level = NUMCOLORMAPS - 1;

			scalelight[i][j] = colormaps + level*256;
		}
	}
}
#endif

#ifdef SPRITESHADOWS
// Finds a floor through which light does not pass.
fixed_t JTE_OpaqueFloorAtPos(fixed_t x, fixed_t y, fixed_t z, fixed_t height)
{
	sector_t *sec;
	fixed_t floorz;

	sec = R_PointInSubsector(x, y)->sector;

	floorz = sec->floorheight;

	if (sec->ffloors)
	{
		ffloor_t *rover;
		fixed_t delta1, delta2;
		int thingtop = z + height;

		for (rover = sec->ffloors; rover; rover = rover->next)
		{
			if (!(rover->flags & FF_EXISTS)
			|| !(rover->flags & FF_RENDERPLANES)
			|| rover->flags & FF_TRANSLUCENT
			|| rover->flags & FF_FOG
			|| rover->flags & FF_INVERTPLANES)
				continue;

			delta1 = z - (*rover->bottomheight + ((*rover->topheight - *rover->bottomheight)/2));
			delta2 = thingtop - (*rover->bottomheight + ((*rover->topheight - *rover->bottomheight)/2));
			if (*rover->topheight > floorz && abs(delta1) < abs(delta2))
				floorz = *rover->topheight;
		}
	}

	return floorz;
}
#endif

#ifdef A_TORNADOPARTICLE
#include "../p_local.h"
#include "../m_random.h"
// Function: A_TornadoParticle
//
// Description: Spins the mobj around it's target
// in a random outward path rising upwards until
// it hits the ceiling. It's angle is unaffected.
//
// var1 = unused
// var2 = unused
//
void A_TornadoParticle(mobj_t* mobj)
{
	angle_t angle;

	// No particle or nothing to spin around? For shame!
	if(!mobj || !mobj->target)
		return;

	// Get this tic's random direction!
	angle = R_PointToAngle2(mobj->x, mobj->y, mobj->target->x, mobj->target->y);
	angle += (ANG90+ANG45) - (P_Random()%9)*ANGLE_10;

	// Spin! Spin!! Faster!
	P_InstaThrust(mobj, angle, 16*FRACUNIT);
	mobj->momz = 8*FRACUNIT;

	// Hit the ceiling? Byebye!
	if(mobj->z + mobj->height >= mobj->ceilingz)
		P_SetMobjState(mobj,S_DISS);
}
#endif

#ifdef JTEMISC
void JTE_DeSuper(player_t* player)
{
	player->powers[pw_super] = false;

	if (player->mo->health > 0)
	{
		if (player->mfjumped || player->mfspinning)
			P_SetPlayerMobjState(player->mo, S_PLAY_ATK1);
		else if (player->running)
			P_SetPlayerMobjState(player->mo, S_PLAY_SPD1);
		else if (player->walking)
			P_SetPlayerMobjState(player->mo, S_PLAY_RUN1);
		else
			P_SetPlayerMobjState(player->mo, S_PLAY_STND);
	}

	// Resume normal music if you're the console player
	if (P_IsLocalPlayer(player))
		P_RestoreMusic(player);

	// If you had a shield, restore its visual significance.
	P_SpawnShieldOrb(player);
}

// Why isn't ROCKET_SPR used anywhere?! It seriously rocks!!
#ifdef HWRENDER
#include "../hardware/hw_light.h"
void JTE_HwLights(void)
{
	lightspritenum_t thing;

	lspr[RINGSPARK_L].type = 0x13;
	lspr[SUPERSONIC_L].type = 0x13;
	lspr[INVINCIBLE_L].type = 0x13;
	lspr[GREENSHIELD_L].type = 0x13;
	lspr[BLUESHIELD_L].type = 0x13;
	lspr[YELLOWSHIELD_L].type = 0x13;
	lspr[REDSHIELD_L].type = 0x13;
	lspr[BLACKSHIELD_L].type = 0x13;
	lspr[WHITESHIELD_L].type = 0x13;

	thing = SUPERSPARK_L;
	lspr[thing].type = 0x13;
	lspr[thing].light_yoffset = 16.0f;
	lspr[thing].dynamic_radius = 16.0f;
}
#endif
#endif

#ifdef MOBJSCALE
#include "../g_game.h"
#include "../p_mobj.h"
#include "../r_things.h"
#ifdef SCALESIZE
#include "../console.h"
#endif

void JTE_SetScale(mobj_t *mobj, unsigned short newscale)
{
	player_t *player;

	if(!mobj || newscale == mobj->scale)
		return;

	mobj->scale = newscale;
#ifdef SCALESIZE
	mobj->radius = FIXEDSCALE(mobj->info->radius,newscale);
	mobj->height = FIXEDSCALE(mobj->info->height,newscale);
#endif
	if((player = mobj->player))
	{
#ifdef SCALEGRAVITY
		player->jumpfactor = FIXEDSCALE(atoi(skins[player->skin].jumpfactor),newscale);
#endif
#ifdef SCALESPEEDS
		if(FIXEDSCALE(atoi(skins[player->skin].normalspeed),newscale) < MAXMOVE)
			player->normalspeed = FIXEDSCALE(atoi(skins[player->skin].normalspeed),newscale);
		else
			player->normalspeed = MAXMOVE;
		//player->thrustfactor = FIXEDUNSCALE(atoi(skins[player->skin].thrustfactor),newscale);
		//player->accelstart = FIXEDUNSCALE(atoi(skins[player->skin].accelstart),newscale);
		//player->acceleration = FIXEDUNSCALE(atoi(skins[player->skin].acceleration),newscale);
		player->runspeed = FIXEDSCALE(atoi(skins[player->skin].runspeed),newscale);
#endif
#ifdef SCALESIZE
		if(player-players == displayplayer && !twodlevel)
		{
			if(FIXEDSCALE(atoi(cv_viewheight.defaultvalue),newscale) < 0)
				cv_viewheight.value = 0;
			else
				cv_viewheight.value = FIXEDSCALE(atoi(cv_viewheight.defaultvalue),newscale);

			if(FIXEDSCALE(atoi(cv_cam_dist.defaultvalue),newscale) < 5)
				CV_SetValue(&cv_cam_dist,5);
			else
				CV_SetValue(&cv_cam_dist,FIXEDSCALE(atoi(cv_cam_dist.defaultvalue),newscale));

			if(FIXEDSCALE(atoi(cv_cam_height.defaultvalue),newscale) < 0)
				CV_SetValue(&cv_cam_height,0);
			else
				CV_SetValue(&cv_cam_height,FIXEDSCALE(atoi(cv_cam_height.defaultvalue),newscale));

			CV_Set(&cv_cam_speed,va("%f",FLOATSCALE(atof(cv_cam_speed.defaultvalue),(float)(abs(100-newscale)+100))));
		}
#endif
	}
}

#ifdef SCALEVAR
#include "../byteptr.h"

static void MyScale_OnChange(void);
static CV_PossibleValue_t mobjscale_cons_t[] = {{10, "MIN"}, {400, "MAX"}, {0, NULL}};
consvar_t cv_myscale = {"scale", "100", CV_CALL|CV_NOINIT, mobjscale_cons_t, MyScale_OnChange, 0, NULL, NULL, 0, 0, NULL};

void JTE_SendMyScale(void)
{
	unsigned short buf;

	// Outside of netgames, just set the destscale.
	if(!netgame)
	{
		players[consoleplayer].mo->destscale = cv_myscale.value;
		return;
	}

	// In netgames, send net cmd.
	buf = cv_myscale.value;
	SendNetXCmd(XD_MYSCALE,(byte*)&buf,sizeof(short));
}

static void MyScale_OnChange(void)
{
	if(!Playing()
	|| !playeringame[consoleplayer]
	|| players[consoleplayer].playerstate != PST_LIVE
	|| !players[consoleplayer].mo)
		return;

	if(!stricmp(cv_myscale.string, "MAX"))
		cv_myscale.value = 400;
	JTE_SendMyScale();
}

static void Got_MyScale(char** p, int playernum)
{
	if(!playeringame[playernum]
	|| players[playernum].playerstate != PST_LIVE
	|| !players[playernum].mo)
	{
		READUSHORT(*p);
		return;
	}
	players[playernum].mo->destscale = READUSHORT(*p);
}

void JTE_InitScale(void)
{
	CV_RegisterVar(&cv_myscale);
	RegisterNetXCmd(XD_MYSCALE, Got_MyScale);
}
#endif
#endif

#ifdef SPRITEOFFS
#include "../info.h"

spriteoffs_t *spriteoffs[NUMSPRITES];
#define AllocOffs(x)\
		spriteoffs[x] = ZZ_Alloc(sizeof(spriteoffs_t));\
		memset(spriteoffs[x],0,sizeof(spriteoffs_t))
void JTE_InitSpriteOffs(void)
{
	fixed_t thing;
	memset(spriteoffs,0,sizeof(spriteoffs));

	// Mobjs

	thing = MT_EGGMOBILE_FG;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].spawnstate = S_EGGMFG;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_EGGMOBILE_BG;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].spawnstate = S_EGGMBG;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	// States

	thing = S_EGGMFG;
	states[thing] = states[S_NULL];
	states[thing].sprite = SPR_EMFG;

	thing = S_EGGMBG;
	states[thing] = states[S_NULL];
	states[thing].sprite = SPR_EMBG;

	// Sprites

	// Things to remember...
	//
	// The sprite vertexes go:
	//  3--2
	//  | /|
	//  |/ |
	//  0--1
	//
	// Negative z means closer to the camera,
	// while positive means farther away.
	//

	thing = SPR_EMFG;
	strcpy(sprnames[thing],"EMFG");
#ifdef HWRENDER
	t_lspr[thing] = &lspr[NOLIGHT];
#endif
	AllocOffs(thing);
	spriteoffs[thing]->z[3] = spriteoffs[thing]->z[2] = -32;
	spriteoffs[thing]->z[0] = spriteoffs[thing]->z[1] = -1;

	thing = SPR_EMBG;
	strcpy(sprnames[thing],"EMBG");
#ifdef HWRENDER
	t_lspr[thing] = &lspr[NOLIGHT];
#endif
	AllocOffs(thing);
	spriteoffs[thing]->z[3] = spriteoffs[thing]->z[2] = 32;
	spriteoffs[thing]->z[0] = spriteoffs[thing]->z[1] = 1;
}
#undef AllocOffs
#endif

#ifdef HOLIDAYS
#include "../dehacked.h"
void JTE_ReadReplace(MYFILE *f, byte type);
#include "../info.h"
#include "../i_video.h"
#include "../v_video.h"
#include "../w_wad.h"
#include "../z_zone.h"
holidayreplace_t *holidayreplace = NULL;
const char *holidaynicks[NUMHOLIDAYS] = {
	NULL,
	"VALN","PTRK","FOOL","ESTR",
	"WEEN","THXG","XMAS","NEWY"
};

void JTE_InitHolidaySprites(void)
{
	spritenum_t thing;

	thing = SPR_JACK;
	strcpy(sprnames[thing], "JACK");
#ifdef HWRENDER
	t_lspr[thing] = &lspr[NOLIGHT];
#endif

	thing = SPR_TREE;
	strcpy(sprnames[thing], "TREE");
#ifdef HWRENDER
	t_lspr[thing] = &lspr[NOLIGHT];
#endif
}

void JTE_LoadHolidayCfg(void)
{
	int clump;
	const char *holidaystr;
	holidayreplace_t *replace, *next;

	// Clear the previous replace list.
	for(replace = holidayreplace; replace; replace = next)
	{
		next = replace->next;
		Z_Free(replace);
	}
	holidayreplace = NULL;

	// Find out what holiday it is!
	if(holidaynicks[holiday])
		holidaystr = va("%sCFG",holidaynicks[holiday]);
	else
		return;

	// Get the lump... No lump? No special stuffs!
	clump = W_CheckNumForName(holidaystr);
	if (clump == -1)
		return;

	// And load it up. :3
	CONS_Printf("Refreshing holiday config...\n");
	DEH_LoadDehackedLump(clump);
}

#if !defined (__MINGW32__) && !defined (__MINGW64__) && !defined (__CYGWIN__) && !defined (_MSC_VER)
/** Converts a string to uppercase in-place.
  * Replaces strupr() from the Microsoft C runtime.
  *
  * \param n String to uppercase.
  * \return Pointer to the same string passed in, now in all caps.
  * \author Alam Arias
  */
static char *strup1(char *n)
{
	int i;
	char *upr = n;
	if (!n) return NULL;
	for (i = 0; n[i]; i++)
		upr[i] = toupper(n[i]);
	return upr;
}
#undef strupr
#define strupr(n) strup1(n)
#endif

void JTE_ReadReplace(MYFILE *f, byte type)
{
	XBOXSTATIC char s[MAXLINELEN];
	char *word;
	char *word2;
	char *tmp;
	holidayreplace_t *replace, *find/*, *last*/;

	// Remove old replacements of this type.
	/*for(replace = holidayreplace, last = NULL; replace; replace = find)
	{
		find = replace->next;
		if(replace->type == type)
		{
			if(last)
				last->next = find;
			if(replace == holidayreplace)
				holidayreplace = find;
			Z_Free(replace);
		}
		else
			last = replace;
	}*/

	// Load in new ones.
	do
	{
		if (myfgets(s, sizeof (s), f))
		{
			if (s[0] == '\n')
				break;

			// First remove trailing newline, if there is one
			tmp = strchr(s, '\n');
			if (tmp)
				*tmp = '\0';

			tmp = strchr(s, '#');
			if (tmp)
				*tmp = '\0';

			// Get the part before the " = "
			tmp = strchr(s, '=');
			*(tmp-1) = '\0';
			word = strupr(s);

			// Now get the part after
			tmp += 2;
			word2 = strupr(tmp);

			// Malloc and link replace
			replace = ZZ_Alloc(sizeof(*replace));
			if(!holidayreplace)
				holidayreplace = replace;
			else for(find = holidayreplace; find; find = find->next)
				if(find->next == NULL)
				{
					find->next = replace;
					break;
				}

			// Copy in the data
			strncpy(replace->original,word,8);
			replace->original[8] = '\0';

			// SFX replace
			if(type == 3)
			{
				strlwr(replace->original);
				snprintf(replace->replace,8,"%d",S_AddSoundFx(word2,false,16,false));
				replace->replace[8] = '\0';
				replace->type = 5;
				continue;
			}
			// Other replace
			else
			{
				strncpy(replace->replace,word2,8);
				replace->replace[8] = '\0';
				replace->type = type;
			}
		}
	} while (!myfeof(f));
}

typedef struct titleprecip_s
{
	unsigned short x,y;
	char momx,momy;
	patch_t *patch;

	struct titleprecip_s *next;
} titleprecip_t;
static titleprecip_t *titleprecip = NULL;

void JTE_TitlePrecip(void)
{
	titleprecip_t *precip, *last;
	static int preciptimer = 0;

	// Add another one!
	{
		char momx = 0,momy = 0;
		patch_t *patch = NULL;

		preciptimer++;
		switch(holiday)
		{
			case H_WEEN:
				if(preciptimer < 1)
					break;
				patch = W_CachePatchName("RAINA0", PU_CACHE);
				momx = 0;
				momy = JTERAND(M_Random(),4)+6;
				preciptimer = 0;
				break;

			case H_XMAS:
				if(preciptimer < 4)
					break;
				patch = W_CachePatchName(va("SNO1%c0",JTERAND(M_Random(),3)+'A'), PU_CACHE);
				momx = JTERAND(M_Random(),4)-2;
				momy = 1;
				preciptimer = 0;
				break;

			default:
				return;
		}
		if(preciptimer == 0)
		{
			precip = ZZ_Alloc(sizeof(titleprecip_t));
			precip->next = titleprecip;
			titleprecip = precip;

			precip->patch = patch;
			precip->x = JTERAND(M_Random(),BASEVIDWIDTH);
			if(precip->momy >= 0)
				precip->y = (rendermode == render_soft ? precip->patch->topoffset : 0);
			else
				precip->y = BASEVIDWIDTH + precip->patch->topoffset;
			precip->momx = momx;
			precip->momy = momy;
		}
	}

	// Draw and tick the precipitation
	for(precip = titleprecip, last = NULL; precip; precip = precip->next)
	{
		V_DrawScaledPatch(precip->x, precip->y, 0, precip->patch);

		// Move it
		precip->x += precip->momx;
		precip->y += precip->momy;

		// Wrap around the X axis
		if(precip->x > BASEVIDWIDTH)
		{
			if(precip->momx < 0)
				precip->x += BASEVIDWIDTH;
			else
				precip->x -= BASEVIDWIDTH;
		}

		// At the bottom? Free it.
		if(precip->y > BASEVIDHEIGHT + precip->patch->topoffset)
		{
			if(last)
				last->next = precip->next;
			else
				last = precip->next;
			if(titleprecip == precip)
				titleprecip = last;
			Z_Free(precip);
			precip = last;
			continue;
		}

		// Repeat
		last = precip;
	}
}
#endif
