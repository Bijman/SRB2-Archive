//#ifdef RIDERS
// Riders Info
// Original by Jason the Echidna

#include "../doomstat.h"
#include "../d_player.h"
#include "../z_zone.h"
#include "../s_sound.h"
#include "../sounds.h"
#include "../info.h"

#include "Rider_Info.h"

void Rider_Init(void)
{
	size_t thing;

	//////////////
	////THINGS////
	//////////////

	thing = MT_SPEEDMONITOR;
	mobjinfo[thing].doomednum = 1465;
	mobjinfo[thing].spawnstate = S_SPEEDMONITOR;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 40*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_SCENERY;

	thing = MT_FLYMONITOR;
	mobjinfo[thing].doomednum = 1466;
	mobjinfo[thing].spawnstate = S_FLYMONITOR;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 40*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_SCENERY;

	thing = MT_POWERMONITOR;
	mobjinfo[thing].doomednum = 1467;
	mobjinfo[thing].spawnstate = S_POWERMONITOR;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 40*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_SCENERY;

	thing = MT_AIRTANKBOX;
	mobjinfo[thing].doomednum = 1468;
	mobjinfo[thing].spawnstate = S_AIRTANKBOX;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painstate = S_AIRTANKBOX;
	mobjinfo[thing].deathstate = S_AIRTANKBOX2;
	mobjinfo[thing].deathsound = sfx_pop;
	mobjinfo[thing].speed = MT_CAPSULEEXPLOSION;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = MT_AIRICO;
	mobjinfo[thing].flags = MF_SOLID|MF_SHOOTABLE|MF_MONITOR;

	thing = MT_AIRICO;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_AIRTANKBOX2;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].seesound = sfx_wdjump;
	mobjinfo[thing].reactiontime = 10;
	mobjinfo[thing].speed = 2*FRACUNIT;
	mobjinfo[thing].radius = 8*FRACUNIT;
	mobjinfo[thing].height = 14*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 62*FRACUNIT;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_SCENERY|MF_NOGRAVITY|MF_BOXICON;

	thing = MT_RSUPERRINGBOX10;
	mobjinfo[thing].doomednum = 1469;
	mobjinfo[thing].spawnstate = S_SUPERRINGBOX4;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painstate = S_SUPERRINGBOX4;
	mobjinfo[thing].deathstate = S_SUPERRINGBOX6;
	mobjinfo[thing].deathsound = sfx_pop;
	mobjinfo[thing].speed = MT_CAPSULEEXPLOSION;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = MT_RSUPERICO10;
	mobjinfo[thing].flags = MF_SOLID|MF_SHOOTABLE|MF_MONITOR;

	thing = MT_RSUPERICO10;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SUPERRINGBOX6;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].seesound = sfx_itemup;
	mobjinfo[thing].reactiontime = 10;
	mobjinfo[thing].speed = 2*FRACUNIT;
	mobjinfo[thing].radius = 8*FRACUNIT;
	mobjinfo[thing].height = 14*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 62*FRACUNIT;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_SCENERY|MF_NOGRAVITY|MF_BOXICON;

	thing = MT_RSUPERRINGBOX20;
	mobjinfo[thing].doomednum = 1470;
	mobjinfo[thing].spawnstate = S_SUPERRINGBOX8;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painstate = S_SUPERRINGBOX8;
	mobjinfo[thing].deathstate = S_SUPERRINGBOX10;
	mobjinfo[thing].deathsound = sfx_pop;
	mobjinfo[thing].speed = MT_CAPSULEEXPLOSION;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = MT_RSUPERICO20;
	mobjinfo[thing].flags = MF_SOLID|MF_SHOOTABLE|MF_MONITOR;

	thing = MT_RSUPERICO20;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SUPERRINGBOX10;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].seesound = sfx_itemup;
	mobjinfo[thing].reactiontime = 20;
	mobjinfo[thing].speed = 2*FRACUNIT;
	mobjinfo[thing].radius = 8*FRACUNIT;
	mobjinfo[thing].height = 14*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 62*FRACUNIT;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_SCENERY|MF_NOGRAVITY|MF_BOXICON;

	thing = MT_RSUPERRINGBOX30;
	mobjinfo[thing].doomednum = 1471;
	mobjinfo[thing].spawnstate = S_SUPERRINGBOX12;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painstate = S_SUPERRINGBOX12;
	mobjinfo[thing].deathstate = S_SUPERRINGBOX14;
	mobjinfo[thing].deathsound = sfx_pop;
	mobjinfo[thing].speed = MT_CAPSULEEXPLOSION;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = MT_RSUPERICO30;
	mobjinfo[thing].flags = MF_SOLID|MF_SHOOTABLE|MF_MONITOR;

	thing = MT_RSUPERICO30;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SUPERRINGBOX14;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].seesound = sfx_itemup;
	mobjinfo[thing].reactiontime = 30;
	mobjinfo[thing].speed = 2*FRACUNIT;
	mobjinfo[thing].radius = 8*FRACUNIT;
	mobjinfo[thing].height = 14*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 62*FRACUNIT;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_SCENERY|MF_NOGRAVITY|MF_BOXICON;

	thing = MT_RINV;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_PINV5;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painstate = S_PINV5;
	mobjinfo[thing].deathstate = S_PINV7;
	mobjinfo[thing].deathsound = sfx_pop;
	mobjinfo[thing].speed = MT_CAPSULEEXPLOSION;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = MT_RINVCICO;
	mobjinfo[thing].flags = MF_SOLID|MF_SHOOTABLE|MF_MONITOR;

	thing = MT_RINVCICO;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_PINV7;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].seesound = sfx_None;
	mobjinfo[thing].reactiontime = 25;
	mobjinfo[thing].speed = 2*FRACUNIT;
	mobjinfo[thing].radius = 8*FRACUNIT;
	mobjinfo[thing].height = 14*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 62*FRACUNIT;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_SCENERY|MF_NOGRAVITY|MF_BOXICON;

	thing = MT_YELLOWCAPSULE;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SHBX;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painstate = S_SHBX;
	mobjinfo[thing].deathstate = S_SHBX2;
	mobjinfo[thing].deathsound = sfx_pop;
	mobjinfo[thing].speed = MT_CAPSULEEXPLOSION;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = MT_YSHIELDCAPICO;
	mobjinfo[thing].flags = MF_SOLID|MF_SHOOTABLE|MF_MONITOR;

	thing = MT_YSHIELDCAPICO;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SHBX2;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].seesound = sfx_shield;
	mobjinfo[thing].reactiontime = 25;
	mobjinfo[thing].speed = 2*FRACUNIT;
	mobjinfo[thing].radius = 8*FRACUNIT;
	mobjinfo[thing].height = 14*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 62*FRACUNIT;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_SCENERY|MF_NOGRAVITY|MF_BOXICON;

	thing = MT_QUESTIONBOX3;
	mobjinfo[thing].doomednum = 1472;
	mobjinfo[thing].spawnstate = S_RANDOMCAPSULE1;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painstate = S_RANDOMCAPSULE1;
	mobjinfo[thing].deathstate = S_RANDOMCAPSULE3;
	mobjinfo[thing].deathsound = sfx_pop;
	mobjinfo[thing].speed = MT_CAPSULEEXPLOSION;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 0;
	mobjinfo[thing].flags = MF_SOLID|MF_SHOOTABLE|MF_MONITOR;

	thing = MT_CAPSULEEXPLOSION;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_MONITOREXPLOSION6;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painstate = S_MONITOREXPLOSION6;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 0;
	mobjinfo[thing].flags = MF_NOCLIP;

	thing = MT_BLUEDIAG;
	mobjinfo[thing].doomednum = 1473;
	mobjinfo[thing].spawnstate = S_BDIAG1;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].seestate = S_BDIAG2;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painsound = sfx_spring;
	mobjinfo[thing].speed = 12*FRACUNIT;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 16*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 16*FRACUNIT;
	mobjinfo[thing].flags = MF_SOLID|MF_SPRING;

	thing = MT_SIDESPRING;
	mobjinfo[thing].doomednum = 1474;
	mobjinfo[thing].spawnstate = S_SIDESPRING1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].seestate = S_SIDESPRING2;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painsound = sfx_spring;
	mobjinfo[thing].speed = 0;
	mobjinfo[thing].radius = 20*FRACUNIT;
	mobjinfo[thing].height = 16*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 20*FRACUNIT;
	mobjinfo[thing].flags = MF_SOLID|MF_NOGRAVITY|MF_SPRING;

	thing = MT_SHELLSHIELD;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SHELLSHIELD1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = pw_shell;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_SHELLITEM;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SHELLITEM1;
	mobjinfo[thing].spawnhealth = 6;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].attacksound = sfx_tink;
	mobjinfo[thing].deathsound = sfx_shbrk;
	mobjinfo[thing].speed = 64*FRACUNIT;
	mobjinfo[thing].radius = 8*FRACUNIT;
	mobjinfo[thing].height = 16*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].activesound = sfx_grnshl;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_BOUNCE;

	thing = MT_REDSHELLSHIELD;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_REDSHELLSHIELD1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = pw_redshell;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_REDSHELLITEM;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_REDSHELLITEM1;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].deathsound = sfx_shbrk;
	mobjinfo[thing].speed = 56*FRACUNIT;
	mobjinfo[thing].radius = 8*FRACUNIT;
	mobjinfo[thing].height = 16*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].activesound = sfx_redshl;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_TRANSLATION|MF_BOUNCE;

	thing = MT_REDSHELLITEM2;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SHELLITEM1;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].deathsound = sfx_shbrk;
	mobjinfo[thing].speed = 56*FRACUNIT;
	mobjinfo[thing].radius = 12*FRACUNIT;
	mobjinfo[thing].height = 16*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].activesound = sfx_redshl;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_TRANSLATION|MF_BOUNCE;

	thing = MT_BANANASHIELD;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BANANASHIELD;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = pw_banana;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_BANANAITEM;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BANANAITEM;
	mobjinfo[thing].spawnhealth = 2;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 1*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].activesound = sfx_peel;
	mobjinfo[thing].flags = MF_SHOOTABLE;

	thing = MT_FAKESHIELD;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_FAKESHIELD;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = pw_fakeitem;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_FAKEITEM;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_FAKEITEM;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].activesound = sfx_fake;
	mobjinfo[thing].flags = MF_SHOOTABLE;

	thing = MT_BOMBSHIELD;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BOMBSHIELD;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 256*FRACUNIT;
	mobjinfo[thing].deathstate = S_RINGEXPLODE;
	mobjinfo[thing].speed = pw_bomb;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_BOMBITEM;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BOMBITEM1;
	mobjinfo[thing].spawnhealth = 105;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 256*FRACUNIT;
	mobjinfo[thing].deathstate = S_RINGEXPLODE;
	mobjinfo[thing].speed = 0;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 24*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].activesound = sfx_bomb;
	mobjinfo[thing].flags = MF_BOUNCE|MF_FLOAT|MF_NOCLIPTHING|MF_MISSILE|MF_SHOOTABLE;

	thing = MT_EXPLOSION;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BPLD1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 32*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_SPECIAL|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_THUNDERSHIELD;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_THUNDERSHIELD1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 64*FRACUNIT;
	mobjinfo[thing].height = 64*FRACUNIT;
	mobjinfo[thing].mass = 16;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_LAKITU;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_LAKITUSL1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 64*FRACUNIT;
	mobjinfo[thing].height = 57*FRACUNIT;
	mobjinfo[thing].mass = 16;
	mobjinfo[thing].flags = MF_NOGRAVITY;

	thing = MT_DRIFT;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_DRIFT1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 12;
	mobjinfo[thing].radius = 64*FRACUNIT;
	mobjinfo[thing].height = 64*FRACUNIT;
	mobjinfo[thing].mass = 16;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_POKEY;
	mobjinfo[thing].doomednum = 1475;
	mobjinfo[thing].spawnstate = S_POKEY1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].seestate = S_POKEY1;
	mobjinfo[thing].reactiontime = 32;
	mobjinfo[thing].painchance = 100;
	mobjinfo[thing].speed = 3;
	mobjinfo[thing].radius = 21*FRACUNIT;
	mobjinfo[thing].height = 69*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_SPECIAL|MF_SHOOTABLE;

	thing = MT_FIREPOINT;
	mobjinfo[thing].doomednum = 1477;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = 10*FRACUNIT;
	mobjinfo[thing].radius = 128*FRACUNIT;
	mobjinfo[thing].height = 1*FRACUNIT;
	mobjinfo[thing].mass = 10000;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOSECTOR|MF_NOCLIP|MF_NOGRAVITY|MF_NOCLIPHEIGHT;

	thing = MT_FIRECHAIN;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_FIRECHAIN1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = 24*FRACUNIT;
	mobjinfo[thing].radius = 17*FRACUNIT;
	mobjinfo[thing].height = 34*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SPECIAL|MF_NOGRAVITY|MF_NOCLIPHEIGHT|MF_FIRE;

	thing = MT_QUESTIONBOX2;
	mobjinfo[thing].doomednum = 1478;
	mobjinfo[thing].spawnstate = S_RANDOMITEM1;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painstate = S_RANDOMITEM1;
	mobjinfo[thing].deathstate = S_RANDOMITEM33;
	mobjinfo[thing].deathsound = sfx_pop;
	mobjinfo[thing].speed = 64*FRACUNIT;//MT_ITEMEXPLOSION;
	mobjinfo[thing].radius = 32*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 0;
	mobjinfo[thing].flags = MF_SLIDEME|MF_SHOOTABLE|MF_NOGRAVITY|MF_MONITOR;
	mobjinfo[thing].raisestate = S_RANDOMITEM33;

	thing = MT_ITEMEXPLOSION;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_ITEMEXPLOSION1;
	mobjinfo[thing].spawnhealth = 1;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painstate = S_ITEMEXPLOSION1;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 0;
	mobjinfo[thing].flags = MF_NOCLIP;

	thing = MT_FLAYM;
	mobjinfo[thing].doomednum = 1479;
	mobjinfo[thing].spawnstate = S_FLAYM1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 8*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_NOGRAVITY|MF_SPECIAL|MF_FIRE;

	thing = MT_DEVIL;
	mobjinfo[thing].doomednum = 1480;
	mobjinfo[thing].spawnstate = S_DEVIL;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 40*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_SOLID;

	thing = MT_ANGEL;
	mobjinfo[thing].doomednum = 1481;
	mobjinfo[thing].spawnstate = S_ANGEL;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 40*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_SOLID;

	thing = MT_PALMTREE;
	mobjinfo[thing].doomednum = 1482;
	mobjinfo[thing].spawnstate = S_PALMTREE;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 40*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_NOCLIP|MF_SCENERY;

	thing = MT_FLAG;
	mobjinfo[thing].doomednum = 1483;
	mobjinfo[thing].spawnstate = S_FLAG;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 40*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_NOCLIP|MF_SCENERY;

	thing = MT_HEDGEHOG;
	mobjinfo[thing].doomednum = 1484;
	mobjinfo[thing].spawnstate = S_HEDGEHOG;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 64*FRACUNIT;
	mobjinfo[thing].height = 64*FRACUNIT;
	mobjinfo[thing].mass = 16;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_TALLBUSH;
	mobjinfo[thing].doomednum = 1485;
	mobjinfo[thing].spawnstate = S_BUSH1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = 0;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 0;
	mobjinfo[thing].flags = MF_NOCLIP|MF_SCENERY;

	thing = MT_TWEE;
	mobjinfo[thing].doomednum = 1486;
	mobjinfo[thing].spawnstate = S_TWEE;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 40*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_NOCLIP|MF_SCENERY;

	thing = MT_HYDRANT;
	mobjinfo[thing].doomednum = 1487;
	mobjinfo[thing].spawnstate = S_HYDRANT;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 40*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_SCENERY;

	thing = MT_SMOK2;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SMOK6;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 20*FRACUNIT;
	mobjinfo[thing].height = 16*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOGRAVITY|MF_NOCLIP|MF_SCENERY;

	thing = MT_SMOK3;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SMOK10;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 20*FRACUNIT;
	mobjinfo[thing].height = 16*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOGRAVITY|MF_NOCLIP|MF_SCENERY;

	thing = MT_CHEESE;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_CHEESE1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 64*FRACUNIT;
	mobjinfo[thing].height = 57*FRACUNIT;
	mobjinfo[thing].mass = 16;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_CHIP;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_CHIP1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 64*FRACUNIT;
	mobjinfo[thing].height = 57*FRACUNIT;
	mobjinfo[thing].mass = 16;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_BIRDIE;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BIRDIE1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 64*FRACUNIT;
	mobjinfo[thing].height = 57*FRACUNIT;
	mobjinfo[thing].mass = 16;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_RANDOMAUDIENCE;
	mobjinfo[thing].doomednum = 1488;
	mobjinfo[thing].spawnstate = S_RANDOMAUDIENCE;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 40*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_SCENERY;

	thing = MT_ENEMYFLIP;
	mobjinfo[thing].doomednum = 1489;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 32*FRACUNIT;
	mobjinfo[thing].height = 64*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_SCENERY;

	thing = MT_WAYPOINT;
	mobjinfo[thing].doomednum = 1492;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 0;
	mobjinfo[thing].speed = 0;
	mobjinfo[thing].radius = 1*FRACUNIT;
	mobjinfo[thing].height = 2*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 0;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOSECTOR|MF_NOCLIP|MF_NOGRAVITY;

	thing = MT_SKYANIMATION;
	mobjinfo[thing].doomednum = 1493;
	mobjinfo[thing].spawnstate = S_SKYANIMATION;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 40*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_SCENERY;

	thing = MT_THUNBERSHIELD;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_THUNBERSHIELD1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 64*FRACUNIT;
	mobjinfo[thing].height = 64*FRACUNIT;
	mobjinfo[thing].mass = 16;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_YELLOWORB2;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SORB1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 20;
	mobjinfo[thing].speed = pw_ringshield;
	mobjinfo[thing].radius = 64*FRACUNIT;
	mobjinfo[thing].height = 64*FRACUNIT;
	mobjinfo[thing].mass = 16;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_WHITEORB2;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SORB1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 4;
	mobjinfo[thing].speed = pw_zjumpshield;
	mobjinfo[thing].radius = 64*FRACUNIT;
	mobjinfo[thing].height = 64*FRACUNIT;
	mobjinfo[thing].mass = 16;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_GREENORB2;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SORB1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 17;
	mobjinfo[thing].speed = pw_zwatershield;
	mobjinfo[thing].radius = 64*FRACUNIT;
	mobjinfo[thing].height = 64*FRACUNIT;
	mobjinfo[thing].mass = 16;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_BLACKORB2;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SORB1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 7;
	mobjinfo[thing].speed = pw_zbombshield;
	mobjinfo[thing].radius = 64*FRACUNIT;
	mobjinfo[thing].height = 64*FRACUNIT;
	mobjinfo[thing].mass = 16;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_BOOMERSHIELD;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BOOMERSHIELD1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 64*FRACUNIT;
	mobjinfo[thing].height = 64*FRACUNIT;
	mobjinfo[thing].mass = 16;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_TSHELLSHIELD;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SHELLSHIELD1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = 10*FRACUNIT;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_TSHELLSHIELD2;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SHELLSHIELD4;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = 10*FRACUNIT;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_TSHELLSHIELD3;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_SHELLSHIELD7;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = 10*FRACUNIT;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_TREDSHELLSHIELD;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_REDSHELLSHIELD2;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = 10*FRACUNIT;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_TREDSHELLSHIELD2;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_REDSHELLSHIELD5;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = 10*FRACUNIT;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_TREDSHELLSHIELD3;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_REDSHELLSHIELD8;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = 10*FRACUNIT;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_BANANASHIELD2;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BANANASHIELD;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = pw_banana;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_BANANASHIELD3;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BANANASHIELD;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].speed = pw_banana;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_BOMBSHIELD2;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BOMBSHIELD;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 256*FRACUNIT;
	mobjinfo[thing].deathstate = S_RINGEXPLODE;
	mobjinfo[thing].speed = pw_bomb;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_BOMBSHIELD3;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BOMBSHIELD;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 256*FRACUNIT;
	mobjinfo[thing].deathstate = S_RINGEXPLODE;
	mobjinfo[thing].speed = pw_bomb;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_BLACKSHELLITEM;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BLACKSHELLITEM1;
	mobjinfo[thing].spawnhealth = 105;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].attacksound = sfx_tink;
	mobjinfo[thing].deathsound = sfx_shbrk;
	mobjinfo[thing].painchance = 256*FRACUNIT;
	mobjinfo[thing].deathstate = S_RINGEXPLODE;
	mobjinfo[thing].speed = 64*FRACUNIT;
	mobjinfo[thing].radius = 8*FRACUNIT;
	mobjinfo[thing].height = 16*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].activesound = sfx_bomb;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_BOUNCE;

	thing = MT_BLACKSHELLSHIELD;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BLACKSHELLSHIELD1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 256*FRACUNIT;
	mobjinfo[thing].deathstate = S_RINGEXPLODE;
	mobjinfo[thing].speed = pw_blackshell;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_TBLACKSHELLSHIELD;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_TBLACKSHELLSHIELD2;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 256*FRACUNIT;
	mobjinfo[thing].deathstate = S_RINGEXPLODE;
	mobjinfo[thing].speed = 10*FRACUNIT;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_TBLACKSHELLSHIELD2;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_TBLACKSHELLSHIELD5;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 256*FRACUNIT;
	mobjinfo[thing].deathstate = S_RINGEXPLODE;
	mobjinfo[thing].speed = 10*FRACUNIT;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_TBLACKSHELLSHIELD3;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_TBLACKSHELLSHIELD8;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 256*FRACUNIT;
	mobjinfo[thing].deathstate = S_RINGEXPLODE;
	mobjinfo[thing].speed = 10*FRACUNIT;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].flags = MF_SHOOTABLE|MF_NOGRAVITY|MF_SCENERY;

	thing = MT_KITCHENSINK;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_KITCHENSINK;
	mobjinfo[thing].spawnhealth = 105;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].painchance = 256*FRACUNIT;
	mobjinfo[thing].deathsound = sfx_shbrk;
	mobjinfo[thing].deathstate = S_NULL;
	mobjinfo[thing].speed = 0;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 24*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].damage = 1;
	mobjinfo[thing].activesound = sfx_bomb;
	mobjinfo[thing].flags = MF_BOUNCE|MF_FLOAT|MF_NOCLIPTHING|MF_MISSILE|MF_SHOOTABLE;

	thing = MT_GREENSHELLTRAIL;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_GST1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 20*FRACUNIT;
	mobjinfo[thing].height = 16*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOGRAVITY|MF_NOCLIP|MF_SCENERY;

	thing = MT_REDSHELLTRAIL;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_RST1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 20*FRACUNIT;
	mobjinfo[thing].height = 16*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOGRAVITY|MF_NOCLIP|MF_SCENERY;

	thing = MT_BLACKSHELLTRAIL;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_BST1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 20*FRACUNIT;
	mobjinfo[thing].height = 16*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOGRAVITY|MF_NOCLIP|MF_SCENERY;

	thing = MT_KITCHENSINKTRAIL;
	mobjinfo[thing].doomednum = -1;
	mobjinfo[thing].spawnstate = S_KST1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 20*FRACUNIT;
	mobjinfo[thing].height = 16*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOGRAVITY|MF_NOCLIP|MF_SCENERY;

	thing = MT_BBLAMP;
	mobjinfo[thing].doomednum = 3535;
	mobjinfo[thing].spawnstate = S_BBLAMP1;
	mobjinfo[thing].spawnhealth = 1000;
	mobjinfo[thing].reactiontime = 8;
	mobjinfo[thing].radius = 12*FRACUNIT;
	mobjinfo[thing].height = 27*FRACUNIT;
	mobjinfo[thing].mass = 100;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOGRAVITY|MF_NOCLIP|MF_SCENERY;

	//////////////
	////FRAMES////
	//////////////

	// Actual Player frames. The one on info.c are the old ones.
	thing = S_PLAY_STND;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_STND;

	thing = S_PLAY_RUN1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 1;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RUN2;

	thing = S_PLAY_RUN2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 2;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RUN3;

	thing = S_PLAY_RUN3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 3;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RUN4;

	thing = S_PLAY_RUN4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 4;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RUN1;

	thing = S_PLAY_RUN5;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 5;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RUN6;

	thing = S_PLAY_RUN6;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 6;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RUN7;

	thing = S_PLAY_RUN7;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 7;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RUN8;

	thing = S_PLAY_RUN8;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 8;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RUN1;

	thing = S_PLAY_SPD1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 9;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SPD2;

	thing = S_PLAY_SPD2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 10;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SPD3;

	thing = S_PLAY_SPD3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 11;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SPD4;

	thing = S_PLAY_SPD4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 12;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SPD1;

	thing = S_PLAY_ATK1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 13;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_ATK2;

	thing = S_PLAY_ATK2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 14;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_ATK3;

	thing = S_PLAY_ATK3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 15;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_ATK4;

	thing = S_PLAY_ATK4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 16;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_ATK1;

	thing = S_PLAY_JUMP;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 17;
	states[thing].tics = -1;
	states[thing].nextstate = S_PLAY_RUN1;

	thing = S_PLAY_PLG1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 18;
	states[thing].tics = 12;
	states[thing].nextstate = S_PLAY_ABL1;

	thing = S_PLAY_ABL1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 19;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_ABL2;

	thing = S_PLAY_ABL2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 20;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_ABL3;

	thing = S_PLAY_ABL3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 21;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_ABL4;

	thing = S_PLAY_ABL4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 22;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_ABL5;

	thing = S_PLAY_ABL5;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 23;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_ABL6;

	thing = S_PLAY_ABL6;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 24;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_ABL7;

	thing = S_PLAY_ABL7;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 25;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_ABL8;

	thing = S_PLAY_ABL8;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 26;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_JUMP;

	thing = S_PLAY_PAIN;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 27;
	states[thing].tics = -1;
	states[thing].nextstate = S_PLAY_STND;

	thing = S_PLAY_DIE1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 27;
	states[thing].tics = 8;
	states[thing].action.acp1 = (actionf_p1)A_Fall;
	states[thing].nextstate = S_PLAY_DIE2;

	thing = S_PLAY_DIE2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 27;
	states[thing].tics = 7;
	states[thing].nextstate = S_PLAY_DIE3;

	thing = S_PLAY_DIE3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 27;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	///////////////////////////////////////////

	thing = S_PLAY_KARTSTND1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_KARTSTND2;

	thing = S_PLAY_KARTSTND2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_KARTSTND1;

	thing = S_PLAY_KARTWALK1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 2;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_KARTWALK2;

	thing = S_PLAY_KARTWALK2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 3;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_KARTWALK3;

	thing = S_PLAY_KARTWALK3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 4;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_KARTWALK4;

	thing = S_PLAY_KARTWALK4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 5;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_KARTWALK1;

	thing = S_PLAY_KARTRUN1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_KARTRUN2;

	thing = S_PLAY_KARTRUN2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 5;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_KARTRUN1;

	thing = S_PLAY_KARTATK;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	thing = S_PLAY_KARTPLG1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	thing = S_PLAY_KARTPAIN;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 6;
	states[thing].tics = -1;
	states[thing].nextstate = S_PLAY_KARTSTND1;

	thing = S_PLAY_KARTDIE1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 6;
	states[thing].tics = 8;
	states[thing].action.acp1 = (actionf_p1)A_Fall;
	states[thing].nextstate = S_PLAY_KARTDIE2;

	thing = S_PLAY_KARTDIE2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 6;
	states[thing].tics = 7;
	states[thing].nextstate = S_PLAY_KARTDIE3;

	thing = S_PLAY_KARTDIE3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 6;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	thing = S_PLAY_KARTSSTND;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 7;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_KARTSSTND;

	thing = S_PLAY_KARTSWALK1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 7;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_KARTSWALK2;

	thing = S_PLAY_KARTSWALK2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 8;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_KARTSWALK3;

	thing = S_PLAY_KARTSWALK3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 9;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_KARTSWALK4;

	thing = S_PLAY_KARTSWALK4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 10;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_KARTSWALK1;

	thing = S_PLAY_KARTSRUN1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 7;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_KARTSRUN2;

	thing = S_PLAY_KARTSRUN2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 9;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_KARTSRUN1;

	thing = S_PLAY_KARTSATK;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 7;
	states[thing].tics = 10;
	states[thing].nextstate = S_PLAY_KARTSSTND;

	thing = S_PLAY_KARTSPLG1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 9;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	thing = S_PLAY_KARTSPAIN;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 7;
	states[thing].tics = -1;
	states[thing].nextstate = S_PLAY_KARTSSTND;

	thing = S_PLAY_KARTSDIE1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 7;
	states[thing].tics = 8;
	states[thing].action.acp1 = (actionf_p1)A_Fall;
	states[thing].nextstate = S_PLAY_KARTSDIE2;

	thing = S_PLAY_KARTSDIE2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 7;
	states[thing].tics = 7;
	states[thing].nextstate = S_PLAY_KARTSDIE3;

	thing = S_PLAY_KARTSDIE3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 7;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	thing = S_PLAY_RSTND;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 39;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_RSTND;

	thing = S_PLAY_RRUN1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 40;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RRUN2;

	thing = S_PLAY_RRUN2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 41;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RRUN3;

	thing = S_PLAY_RRUN3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 42;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RRUN4;

	thing = S_PLAY_RRUN4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 43;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RRUN5;

	thing = S_PLAY_RRUN5;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 44;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RRUN6;

	thing = S_PLAY_RRUN6;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 45;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RRUN7;

	thing = S_PLAY_RRUN7;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 46;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RRUN8;

	thing = S_PLAY_RRUN8;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 47;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RRUN1;

	thing = S_PLAY_RSPD1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 48;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_RSPD2;

	thing = S_PLAY_RSPD2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 49;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_RSPD3;

	thing = S_PLAY_RSPD3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 50;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_RSPD4;

	thing = S_PLAY_RSPD4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 51;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_RSPD1;

	thing = S_PLAY_SKATE1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 48;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SKATE2;

	thing = S_PLAY_SKATE2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 49;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SKATE3;

	thing = S_PLAY_SKATE3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 50;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SKATE4;

	thing = S_PLAY_SKATE4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 51;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SKATE5;

	thing = S_PLAY_SKATE5;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 57;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SKATE6;

	thing = S_PLAY_SKATE6;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 58;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SKATE7;

	thing = S_PLAY_SKATE7;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 59;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SKATE8;

	thing = S_PLAY_SKATE8;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 60;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SKATE9;

	thing = S_PLAY_SKATE9;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 61;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SKATE10;

	thing = S_PLAY_SKATE10;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 62;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_SKATE1;

	thing = S_PLAY_CRUN1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 48;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_CRUN2;

	thing = S_PLAY_CRUN2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 49;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_CRUN3;

	thing = S_PLAY_CRUN3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 50;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_CRUN4;

	thing = S_PLAY_CRUN4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 51;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_CRUN5;

	thing = S_PLAY_CRUN5;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 57;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_CRUN6;

	thing = S_PLAY_CRUN6;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 58;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_CRUN7;

	thing = S_PLAY_CRUN7;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 59;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_CRUN8;

	thing = S_PLAY_CRUN8;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 60;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_CRUN1;

	thing = S_PLAY_RATK1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 52;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RATK2;

	thing = S_PLAY_RATK2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 53;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RATK3;

	thing = S_PLAY_RATK3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 54;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RATK4;

	thing = S_PLAY_RATK4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 55;
	states[thing].tics = 4;
	states[thing].nextstate = S_PLAY_RATK1;

	thing = S_PLAY_SLIDE;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 54;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_SLIDE;

	thing = S_PLAY_RPLG1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 56;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	thing = S_PLAY_SILV1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 32825;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	thing = S_PLAY_SILV2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 32826;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	thing = S_PLAY_RABL1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 57;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_RABL2;

	thing = S_PLAY_RABL2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 58;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_RABL1;

	thing = S_PLAY_CLIMB1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 59;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	thing = S_PLAY_CLIMB2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 60;
	states[thing].tics = 5;
	states[thing].nextstate = S_PLAY_CLIMB3;

	thing = S_PLAY_CLIMB3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 61;
	states[thing].tics = 5;
	states[thing].nextstate = S_PLAY_CLIMB4;

	thing = S_PLAY_CLIMB4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 62;
	states[thing].tics = 5;
	states[thing].nextstate = S_PLAY_CLIMB5;

	thing = S_PLAY_CLIMB5;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 59;
	states[thing].tics = 5;
	states[thing].nextstate = S_PLAY_CLIMB2;

	thing = S_PLAY_FLY1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 57;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY2;

	thing = S_PLAY_FLY2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 58;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY3;

	thing = S_PLAY_FLY3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 59;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY4;

	thing = S_PLAY_FLY4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 60;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY1;

	thing = S_PLAY_CFLY1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 48;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_CFLY2;

	thing = S_PLAY_CFLY2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 49;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_CFLY3;

	thing = S_PLAY_CFLY3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 50;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_CFLY4;

	thing = S_PLAY_CFLY4;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 51;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_CFLY5;

	thing = S_PLAY_CFLY5;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 57;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_CFLY6;

	thing = S_PLAY_CFLY6;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 58;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_CFLY7;

	thing = S_PLAY_CFLY7;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 59;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_CFLY8;

	thing = S_PLAY_CFLY8;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 60;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_CFLY1;

	thing = S_PLAY_RPAIN;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 63;
	states[thing].tics = -1;
	states[thing].nextstate = S_PLAY_RSTND;

	thing = S_PLAY_RDIE1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 63;
	states[thing].tics = 8;
	states[thing].action.acp1 = (actionf_p1)A_Fall;
	states[thing].nextstate = S_PLAY_RDIE2;

	thing = S_PLAY_RDIE2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 63;
	states[thing].tics = 7;
	states[thing].nextstate = S_PLAY_RDIE3;

	thing = S_PLAY_RDIE3;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 63;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	thing = S_PLAY_FALL1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 64;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_FALL2;

	thing = S_PLAY_FALL2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 65;
	states[thing].tics = 2;
	states[thing].nextstate = S_PLAY_FALL1;

	thing = S_PLAY_CARRY;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 66;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	thing = S_PLAY_DRONE1;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 67;
	states[thing].tics = -1;
	states[thing].nextstate = S_PLAY_DRONE2;

	thing = S_PLAY_DRONE2;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 67;
	states[thing].tics = -1;
	states[thing].nextstate = S_PLAY_DRONE1;

	thing = S_PLAY_FLY1A;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 68;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY1B;

	thing = S_PLAY_FLY1B;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 68;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY1A;

	thing = S_PLAY_FLY2A;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 69;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY2B;

	thing = S_PLAY_FLY2B;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 69;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY2A;

	thing = S_PLAY_FLY3A;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 70;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY3B;

	thing = S_PLAY_FLY3B;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 70;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY3A;

	thing = S_PLAY_FLY4A;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 71;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY4B;

	thing = S_PLAY_FLY4B;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 71;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY4A;

	thing = S_PLAY_FLY5A;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 72;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY5B;

	thing = S_PLAY_FLY5B;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 72;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY5A;

	thing = S_PLAY_FLY6A;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 73;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY6B;

	thing = S_PLAY_FLY6B;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 73;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY6A;

	thing = S_PLAY_FLY7A;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 74;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY7B;

	thing = S_PLAY_FLY7B;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 74;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY7A;

	thing = S_PLAY_FLY8A;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 75;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY8B;

	thing = S_PLAY_FLY8B;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 75;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY8A;

	thing = S_PLAY_FLY9A;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 76;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY9B;

	thing = S_PLAY_FLY9B;
	states[thing].sprite = SPR_RIDR;
	states[thing].frame = 76;
	states[thing].tics = 1;
	states[thing].nextstate = S_PLAY_FLY9A;

	// Speed Monitor
	thing = S_SPEEDMONITOR;
	states[thing].sprite = SPR_SPED;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].action.acp1 = (actionf_p1)A_Hover;
	states[thing].nextstate = S_SPEEDMONITOR;

	// Fly Monitor
	thing = S_FLYMONITOR;
	states[thing].sprite = SPR_FLYM;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].action.acp1 = (actionf_p1)A_Hover;
	states[thing].nextstate = S_FLYMONITOR;

	// Power Monitor
	thing = S_POWERMONITOR;
	states[thing].sprite = SPR_POWR;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].action.acp1 = (actionf_p1)A_Hover;
	states[thing].nextstate = S_POWERMONITOR;

	// Air Tank Box
	thing = S_AIRTANKBOX;
	states[thing].sprite = SPR_ATTV;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_AIRTANKBOX1;

	thing = S_AIRTANKBOX1;
	states[thing].sprite = SPR_RTEX;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_AIRTANKBOX;

	thing = S_AIRTANKBOX2;
	states[thing].sprite = SPR_ATTV;
	states[thing].frame = 1;
	states[thing].tics = 18;
	states[thing].action.acp1 = (actionf_p1)A_MonitorPop;
	states[thing].nextstate = S_AIRTANKBOX3;

	thing = S_AIRTANKBOX3;
	states[thing].sprite = SPR_ATTV;
	states[thing].frame = 1;
	states[thing].tics = 18;
	states[thing].action.acp1 = (actionf_p1)A_AirBox;
	states[thing].nextstate = S_DISS;

	// Riders Super Ring Box 10
	thing = S_SUPERRINGBOX4;
	states[thing].sprite = SPR_SRTV;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_SUPERRINGBOX5;

	thing = S_SUPERRINGBOX5;
	states[thing].sprite = SPR_RTEX;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_SUPERRINGBOX4;

	thing = S_SUPERRINGBOX6;
	states[thing].sprite = SPR_SRTV;
	states[thing].frame = 1;
	states[thing].tics = 18;
	states[thing].action.acp1 = (actionf_p1)A_MonitorPop;
	states[thing].nextstate = S_SUPERRINGBOX7;

	thing = S_SUPERRINGBOX7;
	states[thing].sprite = SPR_SRTV;
	states[thing].frame = 1;
	states[thing].tics = 18;
	states[thing].action.acp1 = (actionf_p1)A_RingBox;
	states[thing].nextstate = S_DISS;

	// Riders Super Ring Box 20
	thing = S_SUPERRINGBOX8;
	states[thing].sprite = SPR_SRTV;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_SUPERRINGBOX9;

	thing = S_SUPERRINGBOX9;
	states[thing].sprite = SPR_RTEX;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_SUPERRINGBOX8;

	thing = S_SUPERRINGBOX10;
	states[thing].sprite = SPR_SRTV;
	states[thing].frame = 3;
	states[thing].tics = 18;
	states[thing].action.acp1 = (actionf_p1)A_MonitorPop;
	states[thing].nextstate = S_SUPERRINGBOX11;

	thing = S_SUPERRINGBOX11;
	states[thing].sprite = SPR_SRTV;
	states[thing].frame = 3;
	states[thing].tics = 18;
	states[thing].action.acp1 = (actionf_p1)A_RingBox;
	states[thing].nextstate = S_DISS;

	// Riders Super Ring Box 30
	thing = S_SUPERRINGBOX12;
	states[thing].sprite = SPR_SRTV;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].nextstate = S_SUPERRINGBOX13;

	thing = S_SUPERRINGBOX13;
	states[thing].sprite = SPR_RTEX;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_SUPERRINGBOX12;

	thing = S_SUPERRINGBOX14;
	states[thing].sprite = SPR_SRTV;
	states[thing].frame = 5;
	states[thing].tics = 18;
	states[thing].action.acp1 = (actionf_p1)A_MonitorPop;
	states[thing].nextstate = S_SUPERRINGBOX15;

	thing = S_SUPERRINGBOX15;
	states[thing].sprite = SPR_SRTV;
	states[thing].frame = 5;
	states[thing].tics = 18;
	states[thing].action.acp1 = (actionf_p1)A_RingBox;
	states[thing].nextstate = S_DISS;

	// Invicibility Capsule
	thing = S_PINV5;
	states[thing].sprite = SPR_RINV;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_PINV6;

	thing = S_PINV6;
	states[thing].sprite = SPR_RTEX;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_PINV5;

	thing = S_PINV7;
	states[thing].sprite = SPR_RINV;
	states[thing].frame = 1;
	states[thing].tics = 18;
	states[thing].action.acp1 = (actionf_p1)A_MonitorPop;
	states[thing].nextstate = S_PINV8;

	thing = S_PINV8;
	states[thing].sprite = SPR_RINV;
	states[thing].frame = 1;
	states[thing].tics = 18;
	states[thing].action.acp1 = (actionf_p1)A_Invincibility;
	states[thing].nextstate = S_DISS;

	// Ring Shield Capsule
	thing = S_SHBX;
	states[thing].sprite = SPR_YLBX;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_SHBX1;

	thing = S_SHBX1;
	states[thing].sprite = SPR_RTEX;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_SHBX;

	thing = S_SHBX2;
	states[thing].sprite = SPR_YLBX;
	states[thing].frame = 1;
	states[thing].tics = 18;
	states[thing].action.acp1 = (actionf_p1)A_MonitorPop;
	states[thing].nextstate = S_SHBX3;

	thing = S_SHBX3;
	states[thing].sprite = SPR_YLBX;
	states[thing].frame = 1;
	states[thing].tics = 18;
	states[thing].action.acp1 = (actionf_p1)A_RingShield;
	states[thing].nextstate = S_DISS;

	// Random Capsule
	thing = S_RANDOMCAPSULE1;
	states[thing].sprite = SPR_TION;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_RANDOMCAPSULE2;

	thing = S_RANDOMCAPSULE2;
	states[thing].sprite = SPR_RTEX;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_RANDOMCAPSULE1;

	thing = S_RANDOMCAPSULE3;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].action.acp1 = (actionf_p1)A_CapsulePop;
	states[thing].nextstate = S_DISS;

	// Capsule Explosion
	thing = S_MONITOREXPLOSION6;
	states[thing].sprite = SPR_RTEX;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_MONITOREXPLOSION7;

	thing = S_MONITOREXPLOSION7;
	states[thing].sprite = SPR_RTEX;
	states[thing].frame = 2;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	// Blue Diagonal Spring
	thing = S_BDIAG1;
	states[thing].sprite = SPR_BSPR;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	thing = S_BDIAG2;
	states[thing].sprite = SPR_BSPR;
	states[thing].frame = 1;
	states[thing].tics = 4;
	states[thing].action.acp1 = (actionf_p1)A_Pain;
	states[thing].nextstate = S_BDIAG3;

	thing = S_BDIAG3;
	states[thing].sprite = SPR_BSPR;
	states[thing].frame = 2;
	states[thing].tics = 1;
	states[thing].nextstate = S_BDIAG4;

	thing = S_BDIAG4;
	states[thing].sprite = SPR_BSPR;
	states[thing].frame = 3;
	states[thing].tics = 1;
	states[thing].nextstate = S_BDIAG5;

	thing = S_BDIAG5;
	states[thing].sprite = SPR_BSPR;
	states[thing].frame = 3;
	states[thing].tics = 1;
	states[thing].nextstate = S_BDIAG1;

	// Sideways Spring
	thing = S_SIDESPRING1;
	states[thing].sprite = SPR_SPRZ;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	thing = S_SIDESPRING2;
	states[thing].sprite = SPR_SPRZ;
	states[thing].frame = 1;
	states[thing].tics = 4;
	states[thing].action.acp1 = (actionf_p1)A_Pain;
	states[thing].nextstate = S_SIDESPRING3;

	thing = S_SIDESPRING3;
	states[thing].sprite = SPR_SPRZ;
	states[thing].frame = 2;
	states[thing].tics = 1;
	states[thing].nextstate = S_SIDESPRING4;

	thing = S_SIDESPRING4;
	states[thing].sprite = SPR_SPRZ;
	states[thing].frame = 3;
	states[thing].tics = 1;
	states[thing].nextstate = S_SIDESPRING5;

	thing = S_SIDESPRING5;
	states[thing].sprite = SPR_SPRZ;
	states[thing].frame = 3;
	states[thing].tics = 1;
	states[thing].nextstate = S_SIDESPRING1;

	// Green Shell Shield
	thing = S_SHELLSHIELD1;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_SHELLSHIELD2;

	thing = S_SHELLSHIELD2;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_SHELLSHIELD3;

	thing = S_SHELLSHIELD3;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_SHELLSHIELD4;

	thing = S_SHELLSHIELD4;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].nextstate = S_SHELLSHIELD5;

	thing = S_SHELLSHIELD5;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].nextstate = S_SHELLSHIELD6;

	thing = S_SHELLSHIELD6;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 5;
	states[thing].tics = 2;
	states[thing].nextstate = S_SHELLSHIELD7;

	thing = S_SHELLSHIELD7;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 6;
	states[thing].tics = 2;
	states[thing].nextstate = S_SHELLSHIELD8;

	thing = S_SHELLSHIELD8;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 7;
	states[thing].tics = 2;
	states[thing].nextstate = S_SHELLSHIELD1;

	// Triple Green Shell Shield
	thing = S_TSHELLSHIELD1;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TSHELLSHIELD2;

	thing = S_TSHELLSHIELD2;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TSHELLSHIELD3;

	thing = S_TSHELLSHIELD3;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TSHELLSHIELD4;

	thing = S_TSHELLSHIELD4;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TSHELLSHIELD5;

	thing = S_TSHELLSHIELD5;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TSHELLSHIELD6;

	thing = S_TSHELLSHIELD6;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 5;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TSHELLSHIELD7;

	thing = S_TSHELLSHIELD7;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 6;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TSHELLSHIELD8;

	thing = S_TSHELLSHIELD8;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 7;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TSHELLSHIELD1;

	// Green Shell Item
	thing = S_SHELLITEM1;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_SHELLITEM2;

	thing = S_SHELLITEM2;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].var1 = MT_GREENSHELLTRAIL;
	states[thing].action.acp1 = (actionf_p1)A_SmokeTrailer;
	states[thing].nextstate = S_SHELLITEM3;

	thing = S_SHELLITEM3;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_SHELLITEM4;

	thing = S_SHELLITEM4;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].var1 = MT_GREENSHELLTRAIL;
	states[thing].action.acp1 = (actionf_p1)A_SmokeTrailer;
	states[thing].nextstate = S_SHELLITEM5;

	thing = S_SHELLITEM5;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].nextstate = S_SHELLITEM6;

	thing = S_SHELLITEM6;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 5;
	states[thing].tics = 2;
	states[thing].var1 = MT_GREENSHELLTRAIL;
	states[thing].action.acp1 = (actionf_p1)A_SmokeTrailer;
	states[thing].nextstate = S_SHELLITEM7;

	thing = S_SHELLITEM7;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 6;
	states[thing].tics = 2;
	states[thing].nextstate = S_SHELLITEM8;

	thing = S_SHELLITEM8;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 7;
	states[thing].tics = 2;
	states[thing].var1 = MT_GREENSHELLTRAIL;
	states[thing].action.acp1 = (actionf_p1)A_SmokeTrailer;
	states[thing].nextstate = S_SHELLITEM1;

	// Red Shell Shield
	thing = S_REDSHELLSHIELD1;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_REDSHELLSHIELD2;

	thing = S_REDSHELLSHIELD2;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_REDSHELLSHIELD3;

	thing = S_REDSHELLSHIELD3;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_REDSHELLSHIELD4;

	thing = S_REDSHELLSHIELD4;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].nextstate = S_REDSHELLSHIELD5;

	thing = S_REDSHELLSHIELD5;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].nextstate = S_REDSHELLSHIELD6;

	thing = S_REDSHELLSHIELD6;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 5;
	states[thing].tics = 2;
	states[thing].nextstate = S_REDSHELLSHIELD7;

	thing = S_REDSHELLSHIELD7;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 6;
	states[thing].tics = 2;
	states[thing].nextstate = S_REDSHELLSHIELD8;

	thing = S_REDSHELLSHIELD8;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 7;
	states[thing].tics = 2;
	states[thing].nextstate = S_REDSHELLSHIELD1;

	// Triple Red Shell Shield
	thing = S_TREDSHELLSHIELD1;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TREDSHELLSHIELD2;

	thing = S_TREDSHELLSHIELD2;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TREDSHELLSHIELD3;

	thing = S_TREDSHELLSHIELD3;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TREDSHELLSHIELD4;

	thing = S_TREDSHELLSHIELD4;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TREDSHELLSHIELD5;

	thing = S_TREDSHELLSHIELD5;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TREDSHELLSHIELD6;

	thing = S_TREDSHELLSHIELD6;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 5;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TREDSHELLSHIELD7;

	thing = S_TREDSHELLSHIELD7;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 6;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TREDSHELLSHIELD8;

	thing = S_TREDSHELLSHIELD8;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 7;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TREDSHELLSHIELD1;

	// Red Shell Item
	thing = S_REDSHELLITEM1;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_ThrownRing;
	states[thing].nextstate = S_REDSHELLITEM2;

	thing = S_REDSHELLITEM2;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].var1 = S_REDSHELLITEM9;
	states[thing].var2 = S_REDSHELLITEM10;
	states[thing].action.acp1 = (actionf_p1)A_DualAction;
	states[thing].nextstate = S_REDSHELLITEM3;

	thing = S_REDSHELLITEM3;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_ThrownRing;
	states[thing].nextstate = S_REDSHELLITEM4;

	thing = S_REDSHELLITEM4;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].var1 = S_REDSHELLITEM9;
	states[thing].var2 = S_REDSHELLITEM10;
	states[thing].action.acp1 = (actionf_p1)A_DualAction;
	states[thing].nextstate = S_REDSHELLITEM5;

	thing = S_REDSHELLITEM5;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_ThrownRing;
	states[thing].nextstate = S_REDSHELLITEM6;

	thing = S_REDSHELLITEM6;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 5;
	states[thing].tics = 2;
	states[thing].var1 = S_REDSHELLITEM9;
	states[thing].var2 = S_REDSHELLITEM10;
	states[thing].action.acp1 = (actionf_p1)A_DualAction;
	states[thing].nextstate = S_REDSHELLITEM7;

	thing = S_REDSHELLITEM7;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 6;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_ThrownRing;
	states[thing].nextstate = S_REDSHELLITEM8;

	thing = S_REDSHELLITEM8;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 7;
	states[thing].tics = 2;
	states[thing].var1 = S_REDSHELLITEM9;
	states[thing].var2 = S_REDSHELLITEM10;
	states[thing].action.acp1 = (actionf_p1)A_DualAction;
	states[thing].nextstate = S_REDSHELLITEM1;

	// Banana Shield
	thing = S_BANANASHIELD;
	states[thing].sprite = SPR_PEEL;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_BANANASHIELD;

	// Banana Item
	thing = S_BANANAITEM;
	states[thing].sprite = SPR_PEEL;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	// Fake Item Shield
	thing = S_FAKESHIELD;
	states[thing].sprite = SPR_FAIK;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_FAKESHIELD;

	// Fake Item
	thing = S_FAKEITEM;
	states[thing].sprite = SPR_FAIK;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	// Bomb Shield
	thing = S_BOMBSHIELD;
	states[thing].sprite = SPR_BOMB;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_BOMBSHIELD;

	// Bomb Item
	thing = S_BOMBITEM1;
	states[thing].sprite = SPR_BOMB;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].action.acp1 = (actionf_p1)A_GrenadeRing;
	states[thing].nextstate = S_BOMBITEM2;

	thing = S_BOMBITEM2;
	states[thing].sprite = SPR_BOMB;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].action.acp1 = (actionf_p1)A_GrenadeRing;
	states[thing].nextstate = S_BOMBITEM1;

	// Thunder Shield
	thing = S_THUNDERSHIELD1;
	states[thing].sprite = SPR_THUN;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_THUNDERSHIELD2;

	thing = S_THUNDERSHIELD2;
	states[thing].sprite = SPR_THUN;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_THUNDERSHIELD3;

	thing = S_THUNDERSHIELD3;
	states[thing].sprite = SPR_THUN;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].nextstate = S_THUNDERSHIELD4;

	thing = S_THUNDERSHIELD4;
	states[thing].sprite = SPR_THUN;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].nextstate = S_DISS;

	// Lakitu
	thing = S_LAKITUSL1;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].var1 = 1;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITUSL1;

	thing = S_LAKITUSL2;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 1;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUSL3;

	thing = S_LAKITUSL3;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 2;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUSL4;

	thing = S_LAKITUSL4;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 3;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUSL5;

	thing = S_LAKITUSL5;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 4;
	states[thing].tics = 27;
	states[thing].nextstate = S_LAKITUSL6;

	thing = S_LAKITUSL6;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 5;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUSL7;

	thing = S_LAKITUSL7;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 6;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUSL8;

	thing = S_LAKITUSL8;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 7;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUSL9;

	thing = S_LAKITUSL9;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 8;
	states[thing].tics = 27;
	states[thing].nextstate = S_LAKITUSL10;

	thing = S_LAKITUSL10;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 9;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUSL11;

	thing = S_LAKITUSL11;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 10;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUSL12;

	thing = S_LAKITUSL12;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 11;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUSL13;

	thing = S_LAKITUSL13;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 12;
	states[thing].tics = 1;
	states[thing].var1 = 2;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITUSL13;

	thing = S_LAKITUFSH1;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 13;
	states[thing].tics = 64;
	states[thing].nextstate = S_LAKITUFSH2;

	thing = S_LAKITUFSH2;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 14;
	states[thing].tics = 35;
	states[thing].nextstate = S_DISS;

	thing = S_LAKITULAP1A;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 15;
	states[thing].tics = 1;
	states[thing].var1 = 4;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP1A;

	thing = S_LAKITULAP1B;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 15;
	states[thing].tics = 1;
	states[thing].var1 = 3;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP1B;

	thing = S_LAKITULAP2A;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 16;
	states[thing].tics = 1;
	states[thing].var1 = 5;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP2A;

	thing = S_LAKITULAP2B;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 16;
	states[thing].tics = 1;
	states[thing].var1 = 3;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP2B;

	thing = S_LAKITULAP3A;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 17;
	states[thing].tics = 1;
	states[thing].var1 = 6;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP3A;

	thing = S_LAKITULAP3B;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 17;
	states[thing].tics = 1;
	states[thing].var1 = 3;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP3B;

	thing = S_LAKITULAP4A;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 18;
	states[thing].tics = 1;
	states[thing].var1 = 7;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP4A;

	thing = S_LAKITULAP4B;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 18;
	states[thing].tics = 1;
	states[thing].var1 = 3;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP4B;

	thing = S_LAKITULAP5A;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 19;
	states[thing].tics = 1;
	states[thing].var1 = 8;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP5A;

	thing = S_LAKITULAP5B;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 19;
	states[thing].tics = 1;
	states[thing].var1 = 3;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP5B;

	thing = S_LAKITULAP6A;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 20;
	states[thing].tics = 1;
	states[thing].var1 = 9;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP6A;

	thing = S_LAKITULAP6B;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 20;
	states[thing].tics = 1;
	states[thing].var1 = 3;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP6B;

	thing = S_LAKITULAP7A;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 21;
	states[thing].tics = 1;
	states[thing].var1 = 10;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP7A;

	thing = S_LAKITULAP7B;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 21;
	states[thing].tics = 1;
	states[thing].var1 = 3;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP7B;

	thing = S_LAKITULAP8A;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 22;
	states[thing].tics = 1;
	states[thing].var1 = 11;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP8A;

	thing = S_LAKITULAP8B;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 22;
	states[thing].tics = 1;
	states[thing].var1 = 3;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAP8B;

	thing = S_LAKITULAPFA;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 23;
	states[thing].tics = 1;
	states[thing].var1 = 12;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAPFA;

	thing = S_LAKITULAPFB;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 23;
	states[thing].tics = 1;
	states[thing].var1 = 3;
	states[thing].action.acp1 = (actionf_p1)A_Lakitu;
	states[thing].nextstate = S_LAKITULAPFB;

	thing = S_LAKITUFLG1;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 24;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUFLG2;

	thing = S_LAKITUFLG2;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 25;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUFLG3;

	thing = S_LAKITUFLG3;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 26;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUFLG4;

	thing = S_LAKITUFLG4;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 27;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUFLG5;

	thing = S_LAKITUFLG5;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 28;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUFLG6;

	thing = S_LAKITUFLG6;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 29;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUFLG7;

	thing = S_LAKITUFLG7;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 30;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUFLG8;

	thing = S_LAKITUFLG8;
	states[thing].sprite = SPR_LAKI;
	states[thing].frame = 31;
	states[thing].tics = 4;
	states[thing].nextstate = S_LAKITUFLG1;

	// Drift
	thing = S_DRIFT1;
	states[thing].sprite = SPR_DRFT;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_DRIFT2;

	thing = S_DRIFT2;
	states[thing].sprite = SPR_DRFT;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_DRIFT3;

	thing = S_DRIFT3;
	states[thing].sprite = SPR_DRFT;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_DRIFT1;

	thing = S_DRIFT4;
	states[thing].sprite = SPR_DRFT;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].nextstate = S_DRIFT5;

	thing = S_DRIFT5;
	states[thing].sprite = SPR_DRFT;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].nextstate = S_DRIFT6;

	thing = S_DRIFT6;
	states[thing].sprite = SPR_DRFT;
	states[thing].frame = 5;
	states[thing].tics = 2;
	states[thing].nextstate = S_DRIFT4;

	// Pokey
	thing = S_POKEY1;
	states[thing].sprite = SPR_POKE;
	states[thing].frame = 0;
	states[thing].tics = 6;
	states[thing].var1 = 0;
	states[thing].var2 = 2;
	states[thing].action.acp1 = (actionf_p1)A_MoveAbsolute;
	states[thing].nextstate = S_POKEY2;

	thing = S_POKEY2;
	states[thing].sprite = SPR_POKE;
	states[thing].frame = 1;
	states[thing].tics = 6;
	states[thing].var1 = 0;
	states[thing].var2 = 2;
	states[thing].action.acp1 = (actionf_p1)A_MoveAbsolute;
	states[thing].nextstate = S_POKEY3;

	thing = S_POKEY3;
	states[thing].sprite = SPR_POKE;
	states[thing].frame = 2;
	states[thing].tics = 6;
	states[thing].var1 = 0;
	states[thing].var2 = 2;
	states[thing].action.acp1 = (actionf_p1)A_MoveAbsolute;
	states[thing].nextstate = S_POKEY4;

	thing = S_POKEY4;
	states[thing].sprite = SPR_POKE;
	states[thing].frame = 3;
	states[thing].tics = 6;
	states[thing].var1 = 0;
	states[thing].var2 = 2;
	states[thing].action.acp1 = (actionf_p1)A_MoveAbsolute;
	states[thing].nextstate = S_POKEY1;

	thing = S_POKEY5;
	states[thing].sprite = SPR_POKE;
	states[thing].frame = 0;
	states[thing].tics = 6;
	states[thing].var1 = 180;
	states[thing].var2 = 2;
	states[thing].action.acp1 = (actionf_p1)A_MoveAbsolute;
	states[thing].nextstate = S_POKEY6;

	thing = S_POKEY6;
	states[thing].sprite = SPR_POKE;
	states[thing].frame = 1;
	states[thing].tics = 6;
	states[thing].var1 = 180;
	states[thing].var2 = 2;
	states[thing].action.acp1 = (actionf_p1)A_MoveAbsolute;
	states[thing].nextstate = S_POKEY7;

	thing = S_POKEY7;
	states[thing].sprite = SPR_POKE;
	states[thing].frame = 2;
	states[thing].tics = 6;
	states[thing].var1 = 180;
	states[thing].var2 = 2;
	states[thing].action.acp1 = (actionf_p1)A_MoveAbsolute;
	states[thing].nextstate = S_POKEY8;

	thing = S_POKEY8;
	states[thing].sprite = SPR_POKE;
	states[thing].frame = 3;
	states[thing].tics = 6;
	states[thing].var1 = 180;
	states[thing].var2 = 2;
	states[thing].action.acp1 = (actionf_p1)A_MoveAbsolute;
	states[thing].nextstate = S_POKEY5;

	thing = S_POKEYIDLE;
	states[thing].sprite = SPR_DISS;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	// Fire Chain
	thing = S_FIRECHAIN1;
	states[thing].sprite = SPR_FBLL;
	states[thing].frame = 32768;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_MaceRotate;
	states[thing].nextstate = S_FIRECHAIN2;

	thing = S_FIRECHAIN2;
	states[thing].sprite = SPR_FBLL;
	states[thing].frame = 32769;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_MaceRotate;
	states[thing].nextstate = S_FIRECHAIN3;

	thing = S_FIRECHAIN3;
	states[thing].sprite = SPR_FBLL;
	states[thing].frame = 32770;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_MaceRotate;
	states[thing].nextstate = S_FIRECHAIN4;

	thing = S_FIRECHAIN4;
	states[thing].sprite = SPR_FBLL;
	states[thing].frame = 32771;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_MaceRotate;
	states[thing].nextstate = S_FIRECHAIN1;

	// Mario Kart Random Item
	thing = S_RANDOMITEM1;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 0;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM2;

	thing = S_RANDOMITEM2;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 1;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM3;

	thing = S_RANDOMITEM3;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 2;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM4;

	thing = S_RANDOMITEM4;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 3;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM5;

	thing = S_RANDOMITEM5;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 4;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM6;

	thing = S_RANDOMITEM6;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 5;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM7;

	thing = S_RANDOMITEM7;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 6;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM8;

	thing = S_RANDOMITEM8;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 7;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM9;

	thing = S_RANDOMITEM9;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 8;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM10;

	thing = S_RANDOMITEM10;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 9;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM11;

	thing = S_RANDOMITEM11;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 10;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM12;

	thing = S_RANDOMITEM12;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 11;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM13;

	thing = S_RANDOMITEM13;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 12;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM14;

	thing = S_RANDOMITEM14;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 13;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM15;

	thing = S_RANDOMITEM15;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 14;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM16;

	thing = S_RANDOMITEM16;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 15;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM17;

	thing = S_RANDOMITEM17;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 16;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM18;

	thing = S_RANDOMITEM18;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 17;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM19;

	thing = S_RANDOMITEM19;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 18;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM20;

	thing = S_RANDOMITEM20;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 19;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM21;

	thing = S_RANDOMITEM21;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 20;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM22;

	thing = S_RANDOMITEM22;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 21;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM23;

	thing = S_RANDOMITEM23;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 22;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM24;

	thing = S_RANDOMITEM24;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 23;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM25;

	thing = S_RANDOMITEM25;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 24;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM26;

	thing = S_RANDOMITEM26;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 25;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM27;

	thing = S_RANDOMITEM27;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 26;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM28;

	thing = S_RANDOMITEM28;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 27;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM29;

	thing = S_RANDOMITEM29;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 28;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM30;

	thing = S_RANDOMITEM30;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 29;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM31;

	thing = S_RANDOMITEM31;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 30;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM32;

	thing = S_RANDOMITEM32;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 31;
	states[thing].tics = 3;
	states[thing].action.acp1 = (actionf_p1)A_AttractChase;
	states[thing].nextstate = S_RANDOMITEM1;

	thing = S_RANDOMITEM33;
	states[thing].sprite = SPR_RNDM;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].action.acp1 = (actionf_p1)A_ItemPop;
	states[thing].nextstate = S_DISS;

	// Mario Kart Item Explosion
	thing = S_ITEMEXPLOSION1;
	states[thing].sprite = SPR_SPRK;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_ITEMEXPLOSION2;

	thing = S_ITEMEXPLOSION2;
	states[thing].sprite = SPR_SPRK;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_ITEMEXPLOSION3;

	thing = S_ITEMEXPLOSION3;
	states[thing].sprite = SPR_SPRK;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_ITEMEXPLOSION4;

	thing = S_ITEMEXPLOSION4;
	states[thing].sprite = SPR_SPRK;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].nextstate = S_ITEMEXPLOSION5;

	thing = S_ITEMEXPLOSION5;
	states[thing].sprite = SPR_DISS;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	// Non-Transparent Flame
	thing = S_FLAYM1;
	states[thing].sprite = SPR_FLAM;
	states[thing].frame = 0;
	states[thing].tics = 3;
	states[thing].nextstate = S_FLAYM2;

	thing = S_FLAYM2;
	states[thing].sprite = SPR_FLAM;
	states[thing].frame = 1;
	states[thing].tics = 3;
	states[thing].nextstate = S_FLAYM3;

	thing = S_FLAYM3;
	states[thing].sprite = SPR_FLAM;
	states[thing].frame = 2;
	states[thing].tics = 3;
	states[thing].nextstate = S_FLAYM4;

	thing = S_FLAYM4;
	states[thing].sprite = SPR_FLAM;
	states[thing].frame = 3;
	states[thing].tics = 3;
	states[thing].nextstate = S_FLAYM1;

	// Devil Gargoyle
	thing = S_DEVIL;
	states[thing].sprite = SPR_DEVL;
	states[thing].frame = 0;
	states[thing].tics = -1;

	// Naked Angel Gargoyle... yeah.
	thing = S_ANGEL;
	states[thing].sprite = SPR_ANGE;
	states[thing].frame = 0;
	states[thing].tics = -1;

	// Palm Tree
	thing = S_PALMTREE;
	states[thing].sprite = SPR_PALM;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	// Flag
	thing = S_FLAG;
	states[thing].sprite = SPR_FLAG;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	// Hedgehog
	thing = S_HEDGEHOG;
	states[thing].sprite = SPR_HEDG;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	// Tall Bush
	thing = S_BUSH1;
	states[thing].sprite = SPR_BUS2;
	states[thing].frame = 1;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	// Twee
	thing = S_TWEE;
	states[thing].sprite = SPR_TWEE;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	// Hydrant
	thing = S_HYDRANT;
	states[thing].sprite = SPR_HYDR;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_NULL;

	// Smoke 2
	thing = S_SMOK6;
	states[thing].sprite = SPR_SMOK;
	states[thing].frame = 5;
	states[thing].tics = 4;
	states[thing].nextstate = S_SMOK7;

	thing = S_SMOK7;
	states[thing].sprite = SPR_SMOK;
	states[thing].frame = 6;
	states[thing].tics = 5;
	states[thing].nextstate = S_SMOK8;

	thing = S_SMOK8;
	states[thing].sprite = SPR_SMOK;
	states[thing].frame = 7;
	states[thing].tics = 6;
	states[thing].nextstate = S_SMOK9;

	thing = S_SMOK9;
	states[thing].sprite = SPR_SMOK;
	states[thing].frame = 8;
	states[thing].tics = 7;
	states[thing].nextstate = S_SMOK10;

	thing = S_SMOK10;
	states[thing].sprite = SPR_SMOK;
	states[thing].frame = 9;
	states[thing].tics = 8;
	states[thing].nextstate = S_NULL;

	// Cheese
	thing = S_CHEESE1;
	states[thing].sprite = SPR_CHES;
	states[thing].frame = 0;
	states[thing].tics = 3;
	states[thing].nextstate = S_CHEESE2;

	thing = S_CHEESE2;
	states[thing].sprite = SPR_CHES;
	states[thing].frame = 1;
	states[thing].tics = 3;
	states[thing].nextstate = S_CHEESE1;

	thing = S_CHEESE3;
	states[thing].sprite = SPR_CHES;
	states[thing].frame = 2;
	states[thing].tics = 3;
	states[thing].nextstate = S_CHEESE4;

	thing = S_CHEESE4;
	states[thing].sprite = SPR_CHES;
	states[thing].frame = 3;
	states[thing].tics = 3;
	states[thing].nextstate = S_CHEESE3;

	// Chip
	thing = S_CHIP1;
	states[thing].sprite = SPR_CHIP;
	states[thing].frame = 0;
	states[thing].tics = 3;
	states[thing].nextstate = S_CHIP2;

	thing = S_CHIP2;
	states[thing].sprite = SPR_CHIP;
	states[thing].frame = 1;
	states[thing].tics = 3;
	states[thing].nextstate = S_CHIP1;

	thing = S_CHIP3;
	states[thing].sprite = SPR_CHIP;
	states[thing].frame = 2;
	states[thing].tics = 3;
	states[thing].nextstate = S_CHIP4;

	thing = S_CHIP4;
	states[thing].sprite = SPR_CHIP;
	states[thing].frame = 3;
	states[thing].tics = 3;
	states[thing].nextstate = S_CHIP3;

	// Birdie
	thing = S_BIRDIE1;
	states[thing].sprite = SPR_BRDY;
	states[thing].frame = 0;
	states[thing].tics = 4;
	states[thing].nextstate = S_BIRDIE2;

	thing = S_BIRDIE2;
	states[thing].sprite = SPR_BRDY;
	states[thing].frame = 1;
	states[thing].tics = 4;
	states[thing].nextstate = S_BIRDIE1;

	thing = S_BIRDIE3;
	states[thing].sprite = SPR_BRDY;
	states[thing].frame = 2;
	states[thing].tics = 4;
	states[thing].nextstate = S_BIRDIE4;

	thing = S_BIRDIE4;
	states[thing].sprite = SPR_BRDY;
	states[thing].frame = 3;
	states[thing].tics = 4;
	states[thing].nextstate = S_BIRDIE3;

	// Random Audience
	thing = S_RANDOMAUDIENCE;
	states[thing].sprite = SPR_DISS;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].var1 = 2947;
	states[thing].var2 = 2951;
	states[thing].action.acp1 = (actionf_p1)A_RandomStateRange;
	states[thing].nextstate = S_RANDOMAUDIENCE;

	thing = S_TOAD1;
	states[thing].sprite = SPR_TOAD;
	states[thing].frame = 1;
	states[thing].tics = 5;
	states[thing].nextstate = S_TOAD2;

	thing = S_BOO1;
	states[thing].sprite = SPR_BBOO;
	states[thing].frame = 0;
	states[thing].tics = 5;
	states[thing].nextstate = S_BOO2;

	thing = S_GMBA1;
	states[thing].sprite = SPR_GMBA;
	states[thing].frame = 0;
	states[thing].tics = 5;
	states[thing].nextstate = S_GMBA2;

	thing = S_SHYG1;
	states[thing].sprite = SPR_SHYG;
	states[thing].frame = 0;
	states[thing].tics = 5;
	states[thing].nextstate = S_SHYG2;

	thing = S_SNIF1;
	states[thing].sprite = SPR_SNIF;
	states[thing].frame = 0;
	states[thing].tics = 5;
	states[thing].nextstate = S_SNIF2;

	thing = S_TOAD2;
	states[thing].sprite = SPR_TOAD;
	states[thing].frame = 2;
	states[thing].tics = 20;
	states[thing].action.acp1 = (actionf_p1)A_BunnyHop;
	states[thing].var1 = 7;
	states[thing].var2 = 0;
	states[thing].nextstate = S_TOAD1;

	thing = S_BOO2;
	states[thing].sprite = SPR_BBOO;
	states[thing].frame = 1;
	states[thing].tics = 20;
	states[thing].action.acp1 = (actionf_p1)A_BunnyHop;
	states[thing].var1 = 7;
	states[thing].var2 = 0;
	states[thing].nextstate = S_BOO1;

	thing = S_GMBA2;
	states[thing].sprite = SPR_GMBA;
	states[thing].frame = 1;
	states[thing].tics = 20;
	states[thing].action.acp1 = (actionf_p1)A_BunnyHop;
	states[thing].var1 = 7;
	states[thing].var2 = 0;
	states[thing].nextstate = S_GMBA1;

	thing = S_SHYG2;
	states[thing].sprite = SPR_SHYG;
	states[thing].frame = 1;
	states[thing].tics = 20;
	states[thing].action.acp1 = (actionf_p1)A_BunnyHop;
	states[thing].var1 = 7;
	states[thing].var2 = 0;
	states[thing].nextstate = S_SHYG1;

	thing = S_SNIF2;
	states[thing].sprite = SPR_SNIF;
	states[thing].frame = 1;
	states[thing].tics = 20;
	states[thing].action.acp1 = (actionf_p1)A_BunnyHop;
	states[thing].var1 = 7;
	states[thing].var2 = 0;
	states[thing].nextstate = S_SNIF1;

	// Sky Animation
	thing = S_SKYANIMATION;
	states[thing].sprite = SPR_DISS;
	states[thing].frame = 0;
	states[thing].tics = -1;
	states[thing].nextstate = S_SKYANIMATION;

	// Blue Thunder Shield
	thing = S_THUNBERSHIELD1;
	states[thing].sprite = SPR_THNB;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_THUNBERSHIELD2;

	thing = S_THUNBERSHIELD2;
	states[thing].sprite = SPR_THNB;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_THUNBERSHIELD3;

	thing = S_THUNBERSHIELD3;
	states[thing].sprite = SPR_THNB;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].nextstate = S_THUNBERSHIELD4;

	thing = S_THUNBERSHIELD4;
	states[thing].sprite = SPR_THNB;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].nextstate = S_DISS;

	//Red Thunder Shield (Armageddon)
	thing = S_BOOMERSHIELD1;
	states[thing].sprite = SPR_THNC;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_BOOMERSHIELD2;

	thing = S_BOOMERSHIELD2;
	states[thing].sprite = SPR_THNC;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_BOOMERSHIELD3;

	thing = S_BOOMERSHIELD3;
	states[thing].sprite = SPR_THNC;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].nextstate = S_BOOMERSHIELD4;

	thing = S_BOOMERSHIELD4;
	states[thing].sprite = SPR_THNC;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].nextstate = S_DISS;

	// Green Shell Item
	thing = S_BLACKSHELLITEM1;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_BLACKSHELLITEM2;

	thing = S_BLACKSHELLITEM2;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].var1 = MT_BLACKSHELLTRAIL;
	states[thing].action.acp1 = (actionf_p1)A_SmokeTrailer;
	states[thing].nextstate = S_BLACKSHELLITEM3;

	thing = S_BLACKSHELLITEM3;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_BLACKSHELLITEM4;

	thing = S_BLACKSHELLITEM4;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].var1 = MT_BLACKSHELLTRAIL;
	states[thing].action.acp1 = (actionf_p1)A_SmokeTrailer;
	states[thing].nextstate = S_BLACKSHELLITEM5;

	thing = S_BLACKSHELLITEM5;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].nextstate = S_BLACKSHELLITEM6;

	thing = S_BLACKSHELLITEM6;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 5;
	states[thing].tics = 2;
	states[thing].var1 = MT_BLACKSHELLTRAIL;
	states[thing].action.acp1 = (actionf_p1)A_SmokeTrailer;
	states[thing].nextstate = S_BLACKSHELLITEM7;

	thing = S_BLACKSHELLITEM7;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 6;
	states[thing].tics = 2;
	states[thing].nextstate = S_BLACKSHELLITEM8;

	thing = S_BLACKSHELLITEM8;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 7;
	states[thing].tics = 2;
	states[thing].var1 = MT_BLACKSHELLTRAIL;
	states[thing].action.acp1 = (actionf_p1)A_SmokeTrailer;
	states[thing].nextstate = S_BLACKSHELLITEM1;

	// Red Shell Shield
	thing = S_BLACKSHELLSHIELD1;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_BLACKSHELLSHIELD2;

	thing = S_BLACKSHELLSHIELD2;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_BLACKSHELLSHIELD3;

	thing = S_BLACKSHELLSHIELD3;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_BLACKSHELLSHIELD4;

	thing = S_BLACKSHELLSHIELD4;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].nextstate = S_BLACKSHELLSHIELD5;

	thing = S_BLACKSHELLSHIELD5;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].nextstate = S_BLACKSHELLSHIELD6;

	thing = S_BLACKSHELLSHIELD6;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 5;
	states[thing].tics = 2;
	states[thing].nextstate = S_BLACKSHELLSHIELD7;

	thing = S_BLACKSHELLSHIELD7;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 6;
	states[thing].tics = 2;
	states[thing].nextstate = S_BLACKSHELLSHIELD8;

	thing = S_BLACKSHELLSHIELD8;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 7;
	states[thing].tics = 2;
	states[thing].nextstate = S_BLACKSHELLSHIELD1;

	// Triple Red Shell Shield
	thing = S_TBLACKSHELLSHIELD1;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TBLACKSHELLSHIELD2;

	thing = S_TBLACKSHELLSHIELD2;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TBLACKSHELLSHIELD3;

	thing = S_TBLACKSHELLSHIELD3;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TBLACKSHELLSHIELD4;

	thing = S_TBLACKSHELLSHIELD4;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 3;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TBLACKSHELLSHIELD5;

	thing = S_TBLACKSHELLSHIELD5;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 4;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TBLACKSHELLSHIELD6;

	thing = S_TBLACKSHELLSHIELD6;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 5;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TBLACKSHELLSHIELD7;

	thing = S_TBLACKSHELLSHIELD7;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 6;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TBLACKSHELLSHIELD8;

	thing = S_TBLACKSHELLSHIELD8;
	states[thing].sprite = SPR_BSHL;
	states[thing].frame = 7;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_RotateSpikeBall;
	states[thing].nextstate = S_TBLACKSHELLSHIELD1;

	// Kitchen Sink
	thing = S_KITCHENSINK;
	states[thing].sprite = SPR_SINK;
	states[thing].frame = 1;
	states[thing].tics = 4;
	states[thing].var1 = MT_KITCHENSINKTRAIL;
	states[thing].action.acp1 = (actionf_p1)A_SmokeTrailer;
	states[thing].nextstate = S_KITCHENSINK;

	// Green Shell Trail
	thing = S_GST1;
	states[thing].sprite = SPR_GRST;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_GST2;

	thing = S_GST2;
	states[thing].sprite = SPR_GRST;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_GST3;

	thing = S_GST3;
	states[thing].sprite = SPR_GRST;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_GST4;

	thing = S_GST4;
	states[thing].sprite = SPR_GRST;
	states[thing].frame = 3;
	states[thing].tics = 5;
	states[thing].nextstate = S_GST5;

	thing = S_GST5;
	states[thing].sprite = SPR_GRST;
	states[thing].frame = 4;
	states[thing].tics = 4;
	states[thing].nextstate = S_GST6;

	thing = S_GST6;
	states[thing].sprite = SPR_GRST;
	states[thing].frame = 5;
	states[thing].tics = 3;
	states[thing].nextstate = S_GST7;

	thing = S_GST7;
	states[thing].sprite = SPR_GRST;
	states[thing].frame = 6;
	states[thing].tics = 2;
	states[thing].nextstate = S_GST8;

	thing = S_GST8;
	states[thing].sprite = SPR_GRST;
	states[thing].frame = 7;
	states[thing].tics = 1;
	states[thing].nextstate = S_GST9;

	thing = S_GST9;
	states[thing].sprite = SPR_GRST;
	states[thing].frame = 8;
	states[thing].tics = 1;
	states[thing].nextstate = S_NULL;

	// Red Shell Trail
	thing = S_RST1;
	states[thing].sprite = SPR_REST;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_RST2;

	thing = S_RST2;
	states[thing].sprite = SPR_REST;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_RST3;

	thing = S_RST3;
	states[thing].sprite = SPR_REST;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_RST4;

	thing = S_RST4;
	states[thing].sprite = SPR_REST;
	states[thing].frame = 3;
	states[thing].tics = 5;
	states[thing].nextstate = S_RST5;

	thing = S_RST5;
	states[thing].sprite = SPR_REST;
	states[thing].frame = 4;
	states[thing].tics = 4;
	states[thing].nextstate = S_RST6;

	thing = S_RST6;
	states[thing].sprite = SPR_REST;
	states[thing].frame = 5;
	states[thing].tics = 3;
	states[thing].nextstate = S_RST7;

	thing = S_RST7;
	states[thing].sprite = SPR_REST;
	states[thing].frame = 6;
	states[thing].tics = 2;
	states[thing].nextstate = S_RST8;

	thing = S_RST8;
	states[thing].sprite = SPR_REST;
	states[thing].frame = 7;
	states[thing].tics = 1;
	states[thing].nextstate = S_RST9;

	thing = S_RST9;
	states[thing].sprite = SPR_REST;
	states[thing].frame = 8;
	states[thing].tics = 1;
	states[thing].nextstate = S_NULL;

	// Black Shell Trail
	thing = S_BST1;
	states[thing].sprite = SPR_BLST;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_BST2;

	thing = S_BST2;
	states[thing].sprite = SPR_BLST;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_BST3;

	thing = S_BST3;
	states[thing].sprite = SPR_BLST;
	states[thing].frame = 2;
	states[thing].tics = 2;
	states[thing].nextstate = S_BST4;

	thing = S_BST4;
	states[thing].sprite = SPR_BLST;
	states[thing].frame = 3;
	states[thing].tics = 5;
	states[thing].nextstate = S_BST5;

	thing = S_BST5;
	states[thing].sprite = SPR_BLST;
	states[thing].frame = 4;
	states[thing].tics = 4;
	states[thing].nextstate = S_BST6;

	thing = S_BST6;
	states[thing].sprite = SPR_BLST;
	states[thing].frame = 5;
	states[thing].tics = 3;
	states[thing].nextstate = S_BST7;

	thing = S_BST7;
	states[thing].sprite = SPR_BLST;
	states[thing].frame = 6;
	states[thing].tics = 2;
	states[thing].nextstate = S_BST8;

	thing = S_BST8;
	states[thing].sprite = SPR_BLST;
	states[thing].frame = 7;
	states[thing].tics = 1;
	states[thing].nextstate = S_BST9;

	thing = S_BST9;
	states[thing].sprite = SPR_BLST;
	states[thing].frame = 8;
	states[thing].tics = 1;
	states[thing].nextstate = S_NULL;

	// Kitchen Sink Trail
	thing = S_KST1;
	states[thing].sprite = SPR_KIST;
	states[thing].frame = 0;
	states[thing].tics = 1;
	states[thing].nextstate = S_KST2;

	thing = S_KST2;
	states[thing].sprite = SPR_KIST;
	states[thing].frame = 1;
	states[thing].tics = 5;
	states[thing].nextstate = S_KST3;

	thing = S_KST3;
	states[thing].sprite = SPR_KIST;
	states[thing].frame = 2;
	states[thing].tics = 3;
	states[thing].nextstate = S_NULL;

	thing = S_REDSHELLITEM9;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].action.acp1 = (actionf_p1)A_ThrownRing;
	states[thing].nextstate = S_REDSHELLITEM2;

	thing = S_REDSHELLITEM10;
	states[thing].sprite = SPR_SHLL;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].var1 = MT_REDSHELLTRAIL;
	states[thing].action.acp1 = (actionf_p1)A_SmokeTrailer;
	states[thing].nextstate = S_REDSHELLITEM3;

	thing = S_BBLAMP1;
	states[thing].sprite = SPR_LAMP;
	states[thing].frame = 0;
	states[thing].tics = 2;
	states[thing].nextstate = S_BBLAMP2;

	thing = S_BBLAMP2;
	states[thing].sprite = SPR_LAMP;
	states[thing].frame = 1;
	states[thing].tics = 2;
	states[thing].nextstate = S_BBLAMP1;

	///////////////
	////SPRITES////
	///////////////

	thing = SPR_SPED;
	strcpy(sprnames[thing],"SPED");

	thing = SPR_FLYM;
	strcpy(sprnames[thing],"FLYM");

	thing = SPR_POWR;
	strcpy(sprnames[thing],"POWR");

	thing = SPR_DEVL;
	strcpy(sprnames[thing],"DEVL");

	thing = SPR_ANGE;
	strcpy(sprnames[thing],"ANGE");

	thing = SPR_ATTV;
	strcpy(sprnames[thing],"ATTV");

	thing = SPR_RNDM;
	strcpy(sprnames[thing],"RNDM");

	thing = SPR_BSPR;
	strcpy(sprnames[thing],"BSPR");

	thing = SPR_MSTR;
	strcpy(sprnames[thing],"MSTR");

	thing = SPR_SHEL;
	strcpy(sprnames[thing],"SHEL");

	thing = SPR_RSHL;
	strcpy(sprnames[thing],"RSHL");

	thing = SPR_NANA;
	strcpy(sprnames[thing],"NANA");

	thing = SPR_PEEL;
	strcpy(sprnames[thing],"PEEL");

	thing = SPR_FAKE;
	strcpy(sprnames[thing],"FAKE");

	thing = SPR_FAIK;
	strcpy(sprnames[thing],"FAIK");

	thing = SPR_BOOM;
	strcpy(sprnames[thing],"BOOM");

	thing = SPR_BOMB;
	strcpy(sprnames[thing],"BOMB");

	thing = SPR_THUN;
	strcpy(sprnames[thing],"THUN");

	thing = SPR_THNB;
	strcpy(sprnames[thing],"THNB");

	thing = SPR_THNC;
	strcpy(sprnames[thing],"THNC");

	thing = SPR_SRTV;
	strcpy(sprnames[thing],"SRTV");

	thing = SPR_GRTV;
	strcpy(sprnames[thing],"GRTV");

	thing = SPR_RINV;
	strcpy(sprnames[thing],"RINV");

	thing = SPR_YLBX;
	strcpy(sprnames[thing],"YLBX");

	thing = SPR_TION;
	strcpy(sprnames[thing],"TION");

	thing = SPR_RTEX;
	strcpy(sprnames[thing],"RTEX");

	thing = SPR_LAKI;
	strcpy(sprnames[thing],"LAKI");

	thing = SPR_DRFT;
	strcpy(sprnames[thing],"DRFT");

	thing = SPR_POKE;
	strcpy(sprnames[thing],"POKE");

	thing = SPR_FLAG;
	strcpy(sprnames[thing],"FLAG");

	thing = SPR_PALM;
	strcpy(sprnames[thing],"PALM");

	thing = SPR_CHES;
	strcpy(sprnames[thing],"CHES");

	thing = SPR_CHIP;
	strcpy(sprnames[thing],"CHIP");

	thing = SPR_BRDY;
	strcpy(sprnames[thing],"BRDY");

	thing = SPR_HEDG;
	strcpy(sprnames[thing],"HEDG");

	thing = SPR_SPRZ;
	strcpy(sprnames[thing],"SPRZ");

	thing = SPR_TWEE;
	strcpy(sprnames[thing],"TWEE");

	thing = SPR_HYDR;
	strcpy(sprnames[thing],"HYDR");

	thing = SPR_BBOO;
	strcpy(sprnames[thing],"BBOO");

	thing = SPR_GMBA;
	strcpy(sprnames[thing],"GMBA");

	thing = SPR_SHYG;
	strcpy(sprnames[thing],"SHYG");

	thing = SPR_SNIF;
	strcpy(sprnames[thing],"SNIF");

	thing = SPR_SINK;
	strcpy(sprnames[thing],"SINK");

	thing = SPR_BSHL;
	strcpy(sprnames[thing],"BSHL");

	thing = SPR_GRST;
	strcpy(sprnames[thing],"GRST");

	thing = SPR_REST;
	strcpy(sprnames[thing],"REST");

	thing = SPR_BLST;
	strcpy(sprnames[thing],"BLST");

	thing = SPR_KIST;
	strcpy(sprnames[thing],"KIST");

	thing = SPR_LAMP;
	strcpy(sprnames[thing],"LAMP");

	//////////////
	////SOUNDS////
	//////////////

	thing = sfx_boop;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "boop";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 127;

	thing = sfx_beep;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "beep";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 127;

	thing = sfx_menu2;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "menu2";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 1;

	thing = sfx_swtchx;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "swtchx";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 1;

	thing = sfx_rgrind;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "rgrind";
	S_sfx[thing].priority = 1;

	thing = sfx_boost;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "boost";
	S_sfx[thing].priority = 1;

	thing = sfx_lvlup;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "lvlup";
	S_sfx[thing].priority = 1;

	thing = sfx_rdrift;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "rdrift";
	S_sfx[thing].priority = 1;

	thing = sfx_land;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "land";
	S_sfx[thing].priority = 1;

	thing = sfx_lkt1;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "lkt1";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 127;

	thing = sfx_lkt2;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "lkt2";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 127;

	thing = sfx_lkt3;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "lkt3";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 127;

	thing = sfx_kart1;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "kart1";
	S_sfx[thing].priority = 48;
	S_sfx[thing].pitch = SF_X8AWAYSOUND;

	thing = sfx_kart2;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "kart2";
	S_sfx[thing].priority = 48;
	S_sfx[thing].pitch = SF_X8AWAYSOUND;

	thing = sfx_kart3;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "kart3";
	S_sfx[thing].priority = 48;
	S_sfx[thing].pitch = SF_X8AWAYSOUND;

	thing = sfx_jump2;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "jump2";
	S_sfx[thing].priority = 60;

	thing = sfx_skate;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "skate";
	S_sfx[thing].priority = 48;
	S_sfx[thing].pitch = SF_X8AWAYSOUND;

	thing = sfx_sfloat;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "sfloat";
	S_sfx[thing].priority = 60;

	thing = sfx_sfall;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "sfall";
	S_sfx[thing].priority = 60;

	thing = sfx_popr;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "popr";
	S_sfx[thing].priority = 127;
	S_sfx[thing].pitch = SF_X4AWAYSOUND;

	thing = sfx_bmper2;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "bmper2";
	S_sfx[thing].priority = 127;

	thing = sfx_pulpul;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "pulpul";
	S_sfx[thing].priority = 64;

	thing = sfx_mlap;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mlap";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;

	thing = sfx_mush;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mush";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;

	thing = sfx_star;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "star";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;

	thing = sfx_mega;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mega";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;

	thing = sfx_bomb;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "bomb";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;

	thing = sfx_bomb2;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "bomb2";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;
	S_sfx[thing].pitch = SF_X4AWAYSOUND;

	thing = sfx_peel;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "peel";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;

	thing = sfx_slip;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "slip";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;

	thing = sfx_fake;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "fake";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;

	thing = sfx_grnshl;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "grnshl";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;
	S_sfx[thing].pitch = SF_X2AWAYSOUND;

	thing = sfx_redshl;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "redshl";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;
	S_sfx[thing].pitch = SF_X4AWAYSOUND;

	thing = sfx_shbrk;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "shbrk";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;
	S_sfx[thing].pitch = SF_X4AWAYSOUND;

	thing = sfx_mkdrft;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mkdrft";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;

	thing = sfx_mkslid;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mkslid";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;

	thing = sfx_mkitem;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mkitem";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 64;

	thing = sfx_dwnind;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "dwnind";
	S_sfx[thing].singularity = false;
	S_sfx[thing].priority = 64;

	thing = sfx_mkitem1;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mkitm1";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 72;

	thing = sfx_mkitem2;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mkitm2";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 72;

	thing = sfx_mkitem3;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mkitm3";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 72;

	thing = sfx_mkitem4;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mkitm4";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 72;

	thing = sfx_mkitem5;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mkitm5";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 72;

	thing = sfx_mkitem6;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mkitm6";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 72;

	thing = sfx_mkitem7;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mkitm7";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 72;

	thing = sfx_mkitem8;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mkitm8";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 72;

	thing = sfx_mkitemF;
	S_sfx[thing] = S_sfx[sfx_None];
	S_sfx[thing].name = "mkitmf";
	S_sfx[thing].singularity = true;
	S_sfx[thing].priority = 72;

	/////////////
	////MUSIC////
	/////////////

	thing = mus_rwin;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "rwin";

	thing = mus_mwin;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "mwin";

	thing = mus_mok;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "mok";

	thing = mus_mlose;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "mlose";

	thing = mus_swin;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "swin";

	thing = mus_slose;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "slose";

	thing = mus_rmenu;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "rmenu";

	thing = mus_mmenu;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "mmenu";

	thing = mus_opmenu;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "opmenu";

	thing = mus_rinvnc;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "rinvnc";

	thing = mus_mega;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "mega";

	thing = mus_wait2j;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "wait2j";
}
//#endif
