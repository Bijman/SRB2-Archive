// SC - SA City
// By Jason the Echidna
// MINE! NO STEALY!

#include "../doomstat.h"
#ifdef SACITY
#include "../d_player.h"
#include "../z_zone.h"

#ifdef HWRENDER
#include "../hardware/hw_light.h"
#endif

#include "SC_Info.h"
#include "JTEMisc.h"

void SC_Init(void)
{
	fixed_t thing;

	thing = MT_SHOPKEEP;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].doomednum = 6100;
	mobjinfo[thing].spawnstate = S_SHOPKEEP_TAP1;
	mobjinfo[thing].radius = mobjinfo[MT_PLAYER].radius;
	mobjinfo[thing].height = mobjinfo[MT_PLAYER].height;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_JASONDOLL;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].doomednum = 6101;
	mobjinfo[thing].spawnstate = S_JASONDOLL;
	mobjinfo[thing].radius = mobjinfo[MT_PLAYER].radius;
	mobjinfo[thing].height = mobjinfo[MT_PLAYER].height;
	mobjinfo[thing].flags = MF_SOLID|MF_PUSHABLE|MF_SLIDEME|MF_SCENERY;

	thing = MT_REDXVIDOLL;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].doomednum = 6102;
	mobjinfo[thing].spawnstate = S_REDXVI;
	mobjinfo[thing].radius = 24*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].flags = MF_SOLID|MF_PUSHABLE|MF_SLIDEME|MF_SCENERY;

	thing = MT_TPARTICLE;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].spawnstate = S_TPARTICLE1;
	mobjinfo[thing].radius = 4*FRACUNIT;
	mobjinfo[thing].height = 8*FRACUNIT;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOGRAVITY|MF_SCENERY;

	thing = S_SHOPKEEP_TAP1;
	states[thing] = states[S_PLAY_TAP1];
	states[thing].sprite = SPR_JTE1;
	states[thing].frame = 1;
	states[thing].nextstate = S_SHOPKEEP_TAP2;

	thing = S_SHOPKEEP_TAP2;
	states[thing] = states[S_PLAY_TAP2];
	states[thing].sprite = SPR_JTE1;
	states[thing].frame = 2;
	states[thing].nextstate = S_SHOPKEEP_TAP1;

	thing = S_SHOPKEEP_ATK1;
	states[thing] = states[S_PLAY_ATK1];
	states[thing].sprite = SPR_JTE1;
	states[thing].frame = 3;
	states[thing].nextstate = S_SHOPKEEP_ATK2;

	thing = S_SHOPKEEP_ATK2;
	states[thing] = states[S_PLAY_ATK2];
	states[thing].sprite = SPR_JTE1;
	states[thing].frame = 4;
	states[thing].nextstate = S_SHOPKEEP_ATK3;

	thing = S_SHOPKEEP_ATK3;
	states[thing] = states[S_PLAY_ATK3];
	states[thing].sprite = SPR_JTE1;
	states[thing].frame = 5;
	states[thing].nextstate = S_SHOPKEEP_ATK4;

	thing = S_SHOPKEEP_ATK4;
	states[thing] = states[S_PLAY_ATK4];
	states[thing].sprite = SPR_JTE1;
	states[thing].frame = 6;
	states[thing].nextstate = S_SHOPKEEP_ATK1;

	thing = S_SHOPKEEP_FALL1;
	states[thing] = states[S_PLAY_FALL1];
	states[thing].sprite = SPR_JTE1;
	states[thing].frame = 7;
	states[thing].nextstate = S_SHOPKEEP_FALL2;

	thing = S_SHOPKEEP_FALL2;
	states[thing] = states[S_PLAY_FALL2];
	states[thing].sprite = SPR_JTE1;
	states[thing].frame = 8;
	states[thing].nextstate = S_SHOPKEEP_FALL1;

	thing = S_JASONDOLL;
	states[thing] = states[S_DISS];
	states[thing].sprite = SPR_JTE1;
	states[thing].frame = 0;
	states[thing].tics = -1;

	thing = S_TPARTICLE1;
	states[thing] = states[S_DISS];
	states[thing].sprite = SPR_TORN;
	states[thing].frame = 0;
	states[thing].tics = 15;
	states[thing].nextstate = S_TPARTICLE2;
	states[thing].action.acp1 = (actionf_p1)A_TornadoParticle;

	thing = S_TPARTICLE2;
	states[thing] = states[S_DISS];
	states[thing].sprite = SPR_TORN;
	states[thing].frame = 1;
	states[thing].tics = 15;
	states[thing].nextstate = S_TPARTICLE1;
	states[thing].action.acp1 = (actionf_p1)A_TornadoParticle;

	thing = SPR_JTE1;
	strcpy(sprnames[thing],"JTE1");
#ifdef HWRENDER
	t_lspr[thing] = &lspr[NOLIGHT];
#endif

	thing = SPR_TORN;
	strcpy(sprnames[thing],"TORN");
#ifdef HWRENDER
	t_lspr[thing] = &lspr[NOLIGHT];
#endif
}

#endif
