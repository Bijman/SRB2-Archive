#include "info.h"
#include "p_local.h"

#ifdef MARIO
#include "mgb2.h"
void D_AddFile (char *file);

// MGB2 new player animation set
void MGB_InitMGB2(void)
{
    fixed_t thing;

    thing = MT_PLAYER;
    mobjinfo[thing].height = 32*FRACUNIT;
    mobjinfo[thing].radius = 14*FRACUNIT;

    thing = S_PLAY_STND;
    states[thing].nextstate = S_NULL;
    states[thing].frame = 0;
    states[thing].tics = -1;

    thing = S_PLAY_RUN1;
    states[thing].frame = 1;

    thing = S_PLAY_RUN2;
    states[thing].frame = 2;

    thing = S_PLAY_RUN3;
    states[thing].nextstate = S_PLAY_RUN1;
    states[thing].frame = 3;

    thing = S_PLAY_ATK1;
    states[thing].nextstate = S_NULL;
    states[thing].frame = 4;
    states[thing].tics = -1;

    thing = S_PLAY_PAIN;
    states[thing].nextstate = S_PLAY_PAIN2;
    states[thing].frame = 7;
    states[thing].tics = 1;

    thing = S_PLAY_PAIN2;
    states[thing].nextstate = S_PLAY_PAIN3;
    states[thing].frame = 5;
    states[thing].tics = 1;

    thing = S_PLAY_PAIN3;
    states[thing].nextstate = S_PLAY_PAIN4;
    states[thing].frame = 0;
    states[thing].tics = 1;

    thing = S_PLAY_PAIN4;
    states[thing].nextstate = S_PLAY_PAIN5;
    states[thing].frame = 7;
    states[thing].tics = 1;

    thing = S_PLAY_PAIN5;
    states[thing].nextstate = S_PLAY_PAIN6;
    states[thing].frame = 5;
    states[thing].tics = 1;

    thing = S_PLAY_PAIN6;
    states[thing].nextstate = S_PLAY_PAIN7;
    states[thing].frame = 0;
    states[thing].tics = 1;

    thing = S_PLAY_PAIN7;
    states[thing].nextstate = S_PLAY_PAIN8;
    states[thing].frame = 7;
    states[thing].tics = 1;

    thing = S_PLAY_PAIN8;
    states[thing].nextstate = S_PLAY_PAIN9;
    states[thing].frame = 5;
    states[thing].tics = 1;

    thing = S_PLAY_PAIN9;
    states[thing].nextstate = S_PLAY_STND;
    states[thing].frame = 0;
    states[thing].tics = 30;

    thing = S_PLAY_DIE1;
    states[thing].frame = 6;

    thing = S_PLAY_DIE2;
    states[thing].frame = 6;

    thing = S_PLAY_DIE3;
    states[thing].frame = 6;

    thing = S_PLAY_ABL1;
    states[thing].frame = 8;

    thing = S_PLAY_ABL2;
    states[thing].frame = 9;

    thing = S_PLAY_SPC1;
    states[thing].frame = 10;

    thing = S_PLAY_SPC2;
    states[thing].frame = 11;

    thing = S_PLAY_SPC3;
    states[thing].frame = 12;

    thing = S_PLAY_SPC4;
    states[thing].frame = 13;

    thing = S_PLAY_CLIMB1;
    states[thing].frame = 14;

    thing = S_PLAY_CLIMB2;
    states[thing].frame = 15;

    thing = S_PLAY_CLIMB3;
    states[thing].frame = 16;

    thing = S_PLAY_CLIMB4;
    states[thing].frame = 17;

    thing = S_PLAY_CLIMB5;
    states[thing].frame = 18;
}

void MGB_AddWads(void)
{
	D_AddFile("MGB-Mario.wad");
	D_AddFile("MGB-Luigi.wad");
}

void MGB_DefaultSkinValues(skin_t *skin)
{
    memset (skin, 0, sizeof(skin_t));
    strcpy (skin->name, "MARIO");
    #ifndef RP
    strcpy (skin->faceprefix, "MARIOF");
    strcpy (skin->nameprefix, "MARION");
    #endif
	strcpy (skin->spin, "0");
	strcpy (skin->starttranscolor, "179");
	strcpy (skin->endtranscolor, "179");
	strcpy (skin->prefcolor, "6");
	strcpy (skin->normalspeed, "5");
	strcpy (skin->thrustfactor, "5");
	strcpy (skin->accelstart, "100");
	strcpy (skin->acceleration, "10");
	strcpy (skin->jumpfactor, "100");
	#ifndef RP
	strcpy (skin->ability, "99");
	#endif
}
#endif
