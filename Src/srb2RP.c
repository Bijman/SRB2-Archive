#ifndef __WIN32__
#include <unistd.h>             // for access
#else
#include <direct.h>
#endif

#include "info.h"
#include "tables.h"
#include "g_game.h"
#include "p_enemy.h"
#include "p_local.h"
#include "r_main.h"
#include "r_things.h"
#include "s_sound.h"
#include "v_video.h"
#include "z_zone.h"
void P_InstaThrust(mobj_t* mo, angle_t angle, fixed_t move);
void P_ProcessLineSpecial(line_t* line, mobj_t* mo);

#ifdef HWRENDER
#include "hardware/hw_light.h"
#endif

#ifdef RP
#include "srb2RP.h"
#include "Srb2RPh2.h"

// RP mobj and other data init
void RP_InitRP(void)
{
    int i;
    fixed_t thing;
    sfxinfo_t emptysfx = {"none", true, 60, 0, -1, -1, 0, -1, -1, -1};

    // Mobj Info
    ////////////

    thing = MT_PLAYER;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].spawnstate = S_PLAY_STND1;
    mobjinfo[thing].seestate = S_PLAY_WALK1;
    mobjinfo[thing].attacksound = sfx_thok;
    mobjinfo[thing].painstate = S_PLAY_PAIN1;
    mobjinfo[thing].painchance = MT_THOK;
    mobjinfo[thing].missilestate = S_PLAY_JUMP1;
    mobjinfo[thing].deathstate = S_PLAY_DETH1;
    mobjinfo[thing].xdeathstate = S_PLAY_DETH1;
    mobjinfo[thing].radius = 16*FRACUNIT;
    mobjinfo[thing].height = 56*FRACUNIT;
    mobjinfo[thing].damage = MT_THOK;
    mobjinfo[thing].activesound = sfx_sahitg;
    mobjinfo[thing].flags = MF_SOLID|MF_SHOOTABLE;
    mobjinfo[thing].raisestate = MT_THOK;

    thing = MT_CHASECAM;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].radius = 16*FRACUNIT;
    mobjinfo[thing].height = 16*FRACUNIT;
    mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOSECTOR|MF_NOGRAVITY|MF_FLOAT|MF_NOCLIPTHING;

    thing = MT_SPIRIT;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].radius = 16*FRACUNIT;
    mobjinfo[thing].height = 56*FRACUNIT;
    mobjinfo[thing].flags = MF_NOSECTOR;

    /*thing = MT_ROCKET;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].spawnstate = S_ROCKET;
    mobjinfo[thing].seesound = sfx_rlaunc;
    mobjinfo[thing].deathsound = sfx_rxplod;
    mobjinfo[thing].speed = 20*FRACUNIT;
    mobjinfo[thing].radius = 11*FRACUNIT;
    mobjinfo[thing].height = 8*FRACUNIT;
    mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_MISSILE|MF_NOGRAVITY;*/ // Bleh, accedental "port"...

    thing = MT_RING;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].doomednum = 84;
    mobjinfo[thing].spawnstate = S_RINGA;
    mobjinfo[thing].reactiontime = MT_RING;
    mobjinfo[thing].speed = 60*FRACUNIT;
    mobjinfo[thing].radius = 16*FRACUNIT;
    mobjinfo[thing].height = 24*FRACUNIT;
    mobjinfo[thing].flags = MF_SLIDEME|MF_SPECIAL|MF_COUNTITEM|MF_NOGRAVITY;

    thing = MT_FAN;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].doomednum = 80;
    mobjinfo[thing].spawnstate = S_FAN1;
    mobjinfo[thing].speed = 5*FRACUNIT;
    mobjinfo[thing].radius = 16*FRACUNIT;
    mobjinfo[thing].height = 8*FRACUNIT;
    mobjinfo[thing].flags = MF_SOLID;

    thing = MT_BUBBLES;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].doomednum = 62;
    mobjinfo[thing].spawnstate = S_BUBBLES1;
    mobjinfo[thing].radius = 8*FRACUNIT;
    mobjinfo[thing].height = 8*FRACUNIT;
    mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_SCENERY;

    thing = MT_SMALLBUBBLE;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].spawnstate = S_SBUBBLE;
    mobjinfo[thing].radius = 4*FRACUNIT;
    mobjinfo[thing].height = 4*FRACUNIT;
    mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

    thing = MT_MEDIUMBUBBLE;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].spawnstate = S_MBUBBLE;
    mobjinfo[thing].radius = 8*FRACUNIT;
    mobjinfo[thing].height = 8*FRACUNIT;
    mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

    thing = MT_HUGEBUBBLE;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].spawnstate = S_HBUBBLE1;
    mobjinfo[thing].speed = 1.2*FRACUNIT;
    mobjinfo[thing].radius = 4*FRACUNIT;
    mobjinfo[thing].height = 8*FRACUNIT;
    mobjinfo[thing].flags = MF_SPECIAL|MF_NOGRAVITY|MF_SCENERY;

    thing = MT_SPLISH;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].spawnstate = S_SPLISH1;
    mobjinfo[thing].radius = 6*FRACUNIT;
    mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;

    thing = MT_SPLASH;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].spawnstate = S_SPLASH1;
    mobjinfo[thing].radius = 20*FRACUNIT;
    mobjinfo[thing].height = 16*FRACUNIT;
    mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOGRAVITY|MF_NOCLIP|MF_SCENERY;

    thing = MT_FLAME;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].doomednum = 61;
    mobjinfo[thing].spawnstate = S_FLAME1;
    mobjinfo[thing].radius = 8*FRACUNIT;
    mobjinfo[thing].height = 32*FRACUNIT;
    mobjinfo[thing].flags = MF_NOGRAVITY|MF_SPECIAL|MF_FIRE;

    thing = MT_INTERCOM1;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    mobjinfo[thing].doomednum = 41;
    mobjinfo[thing].spawnstate = S_INTERCOM1;
    mobjinfo[thing].radius = 8*FRACUNIT;
    mobjinfo[thing].height = 32*FRACUNIT;
    mobjinfo[thing].activesound = sfx_rpcom1;
    mobjinfo[thing].flags = MF_SOLID|MF_NOGRAVITY;
    mobjinfo[thing].raisestate = S_NULL;

    thing = MT_INTERCOM2;
    mobjinfo[thing] = mobjinfo[MT_INTERCOM1];
    mobjinfo[thing].doomednum = 42;
    mobjinfo[thing].spawnstate = S_INTERCOM2;
    mobjinfo[thing].activesound = sfx_rpcom2;

    thing = MT_INTERCOM3;
    mobjinfo[thing] = mobjinfo[MT_INTERCOM1];
    mobjinfo[thing].doomednum = 43;
    mobjinfo[thing].spawnstate = S_INTERCOM3;
    mobjinfo[thing].activesound = sfx_rpcom3;

    thing = MT_INTERCOM4;
    mobjinfo[thing] = mobjinfo[MT_INTERCOM1];
    mobjinfo[thing].doomednum = 44;
    mobjinfo[thing].spawnstate = S_INTERCOM4;
    mobjinfo[thing].activesound = sfx_rpcom4;

    thing = MT_INTERCOM5;
    mobjinfo[thing] = mobjinfo[MT_INTERCOM1];
    mobjinfo[thing].doomednum = 45;
    mobjinfo[thing].spawnstate = S_INTERCOM5;
    mobjinfo[thing].activesound = sfx_rpcom5;

    thing = MT_INTERCOM6;
    mobjinfo[thing] = mobjinfo[MT_INTERCOM1];
    mobjinfo[thing].doomednum = 46;
    mobjinfo[thing].spawnstate = S_INTERCOM6;
    mobjinfo[thing].activesound = sfx_rpcom6;

    thing = MT_INTERCOM7;
    mobjinfo[thing] = mobjinfo[MT_INTERCOM1];
    mobjinfo[thing].doomednum = 47;
    mobjinfo[thing].spawnstate = S_INTERCOM7;
    mobjinfo[thing].activesound = sfx_rpcom7;

    thing = MT_INTERCOM8;
    mobjinfo[thing] = mobjinfo[MT_INTERCOM1];
    mobjinfo[thing].doomednum = 48;
    mobjinfo[thing].spawnstate = S_INTERCOM8;
    mobjinfo[thing].activesound = sfx_rpcom8;

    thing = MT_INTERCOM9;
    mobjinfo[thing] = mobjinfo[MT_INTERCOM1];
    mobjinfo[thing].doomednum = 49;
    mobjinfo[thing].spawnstate = S_INTERCOM9;
    mobjinfo[thing].activesound = sfx_rpcom9;

    // States
    /////////

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_STND1+i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_STND;
        states[thing].frame = i;
        states[thing].tics = 16;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_STND1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_WAIT1+i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_WAIT;
        states[thing].frame = i;
        states[thing].tics = 16;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_WAIT1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_WALK1+i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_WALK;
        states[thing].frame = i;
        states[thing].tics = 4;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_WALK1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_DASH1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_DASH;
        states[thing].frame = i;
        states[thing].tics = 2;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_DASH1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_JUMP1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_JUMP;
        states[thing].frame = i;
        states[thing].tics = 1;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_JUMP1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_EDGE1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_EDGE;
        states[thing].frame = i;
        states[thing].tics = 12;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_EDGE1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_PAIN1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_PAIN;
        states[thing].frame = i;
        states[thing].tics = 4;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_PAIN1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_DETH1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_DETH;
        states[thing].frame = i;
        states[thing].tics = 4;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_DETH1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_SPRN1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_SPRN;
        states[thing].frame = i;
        states[thing].tics = 4;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_SPRN1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_FALL1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_FALL;
        states[thing].frame = i;
        states[thing].tics = 2;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_FALL1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_HANG1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_HANG;
        states[thing].frame = i;
        states[thing].tics = 8;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_HANG1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_GASP1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_GASP;
        states[thing].frame = i;
        states[thing].tics = 12;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_FALL1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_FLYY1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_FLYY;
        states[thing].frame = i;
        states[thing].tics = 2;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_FLYY1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_TIRE1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_TIRE;
        states[thing].frame = i;
        states[thing].tics = 12;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_TIRE1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_GLID1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_GLID;
        states[thing].frame = i;
        states[thing].tics = 2;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_GLID1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_CLNG1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_CLNG;
        states[thing].frame = i;
        states[thing].tics = 8;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_CLNG1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_CLIM1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_CLIM;
        states[thing].frame = i;
        states[thing].tics = 5;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_CLIM1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_LORB1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_LORB;
        states[thing].frame = i;
        states[thing].tics = 2;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_LORB1+MAXPLAYFRAMES-1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_DORB1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_DORB;
        states[thing].frame = i;
        states[thing].tics = 2;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_DORB1+MAXPLAYFRAMES-1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_SSTN1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_SSTN;
        states[thing].frame = i;
        states[thing].tics = 4;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_SSTN1;

    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        thing = S_PLAY_SRUN1 + i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_SRUN;
        states[thing].frame = i;
        states[thing].tics = 2;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_PLAY_SRUN1;

    thing = S_DISS;
    states[thing] = states[S_NULL];
    states[thing].tics = 1;

    for(i=0; i<S_RINGX-S_RINGA; i++)
    {
        thing = i+S_RINGA;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_RING;
        states[thing].frame = i;
        states[thing].tics = 1;
        states[thing].nextstate = thing+1;
    }
    states[thing].nextstate = S_RINGA;

    thing = S_FLOWER1A;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FWR1;
    states[thing].frame = 0;
    states[thing].tics = 8;
    states[thing].nextstate = S_FLOWER1B;

    thing = S_FLOWER1B;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FWR1;
    states[thing].frame = 1;
    states[thing].tics = 8;
    states[thing].nextstate = S_FLOWER1A;

    thing = S_FLOWER2A;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FWR2;
    states[thing].frame = 0;
    states[thing].tics = 8;
    states[thing].nextstate = S_FLOWER2B;

    thing = S_FLOWER2B;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FWR2;
    states[thing].frame = 1;
    states[thing].tics = 8;
    states[thing].nextstate = S_FLOWER2A;

    thing = S_FLOWER3;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FWR3;

    thing = S_FLOWER4;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FWR4;

    thing = S_FLOWER5A;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FWR5;
    states[thing].frame = 0;
    states[thing].tics = 8;
    states[thing].nextstate = S_FLOWER5B;

    thing = S_FLOWER5B;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FWR5;
    states[thing].frame = 1;
    states[thing].tics = 8;
    states[thing].nextstate = S_FLOWER5C;

    thing = S_FLOWER5C;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FWR5;
    states[thing].frame = 2;
    states[thing].tics = 8;
    states[thing].nextstate = S_FLOWER5D;

    thing = S_FLOWER5D;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FWR5;
    states[thing].frame = 3;
    states[thing].tics = 8;
    states[thing].nextstate = S_FLOWER5A;

    thing = S_BUSH1;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_BUS1;

    thing = S_BUSH2;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_BUS2;

    thing = S_FAN1;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FANS;
    states[thing].frame = 0;
    states[thing].tics = 1;
    states[thing].nextstate = S_FAN2;

    thing = S_FAN2;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FANS;
    states[thing].frame = 1;
    states[thing].tics = 1;
    states[thing].nextstate = S_FAN3;

    thing = S_FAN3;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FANS;
    states[thing].frame = 2;
    states[thing].tics = 1;
    states[thing].nextstate = S_FAN4;

    thing = S_FAN4;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_FANS;
    states[thing].frame = 3;
    states[thing].tics = 1;
    states[thing].nextstate = S_FAN1;

    thing = S_STEAM1;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_STEM;
    states[thing].frame = 0;
    states[thing].tics = 2;
    states[thing].nextstate = S_STEAM2;
    states[thing].action.acv = A_SetSolidSteam;

    thing = S_STEAM2;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_STEM;
    states[thing].frame = 1;
    states[thing].tics = 2;
    states[thing].nextstate = S_STEAM3;
    states[thing].action.acv = A_UnsetSolidSteam;

    thing = S_STEAM3;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_STEM;
    states[thing].frame = 2;
    states[thing].tics = 2;
    states[thing].nextstate = S_STEAM4;

    thing = S_STEAM4;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_STEM;
    states[thing].frame = 3;
    states[thing].tics = 2;
    states[thing].nextstate = S_STEAM5;

    thing = S_STEAM5;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_STEM;
    states[thing].frame = 4;
    states[thing].tics = 2;
    states[thing].nextstate = S_STEAM6;

    thing = S_STEAM6;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_STEM;
    states[thing].frame = 5;
    states[thing].tics = 2;
    states[thing].nextstate = S_STEAM7;

    thing = S_STEAM7;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_STEM;
    states[thing].frame = 6;
    states[thing].tics = 2;
    states[thing].nextstate = S_STEAM8;

    thing = S_STEAM8;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_STEM;
    states[thing].frame = 7;
    states[thing].tics = 18;
    states[thing].nextstate = S_STEAM1;

    thing = S_BUBBLES1;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_BUBL;
    states[thing].frame = 0;
    states[thing].tics = 8;
    states[thing].nextstate = S_BUBBLES2;
    states[thing].action.acv = A_BubbleSpawn;

    thing = S_BUBBLES2;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_BUBL;
    states[thing].frame = 1;
    states[thing].tics = 8;
    states[thing].nextstate = S_BUBBLES1;

    thing = S_SBUBBLE;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_BUB1;
    states[thing].frame = 0;
    states[thing].tics = 1;
    states[thing].nextstate = S_SBUBBLE;
    states[thing].action.acv = A_BubbleRise;

    thing = S_MBUBBLE;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_BUB2;
    states[thing].frame = 0;
    states[thing].tics = 1;
    states[thing].nextstate = S_MBUBBLE;
    states[thing].action.acv = A_BubbleRise;

    thing = S_HBUBBLE1;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_BUB3;
    states[thing].frame = 0;
    states[thing].tics = 16;
    states[thing].nextstate = S_HBUBBLE2;
    states[thing].action.acv = A_BubbleRise;

    thing = S_HBUBBLE2;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_BUB3;
    states[thing].frame = 1;
    states[thing].tics = 1;
    states[thing].nextstate = S_HBUBBLE2;
    states[thing].action.acv = A_BubbleRise;

    thing = S_HBUBBLE3;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_BUB3;
    states[thing].frame = 2;
    states[thing].tics = 16;
    states[thing].nextstate = S_DISS;
    states[thing].action.acv = A_BubbleRise;

    for(i=S_SPLISH1; i<=S_SPLISH9; i++)
    {
        thing = i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_SPLH;
        states[thing].frame = i-S_SPLISH1;
        states[thing].tics = 2;
        states[thing].nextstate = i+1;
    }
    states[thing].nextstate = S_DISS;

    for(i=S_SPLASH1; i<=S_SPLASH3; i++)
    {
        thing = i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_RAIN;
        states[thing].frame = i-S_SPLASH1+1;
        states[thing].tics = 3;
        states[thing].nextstate = i+1;
    }
    states[thing].nextstate = S_RAIN2;

    for(i=S_FLAME1; i<=S_FLAME4; i++)
    {
        thing = i;
        states[thing] = states[S_NULL];
        states[thing].sprite = SPR_FLAM;
        states[thing].frame = i-S_FLAME1;
        states[thing].tics = 3;
        states[thing].nextstate = i+1;
    }
    states[thing].nextstate = S_FLAME1;

    thing = S_INTERCOM1;
    states[thing] = states[S_NULL];
    states[thing].sprite = SPR_COM1;

    thing = S_INTERCOM2;
    states[thing] = states[S_INTERCOM1];
    states[thing].sprite = SPR_COM2;

    thing = S_INTERCOM3;
    states[thing] = states[S_INTERCOM1];
    states[thing].sprite = SPR_COM3;

    thing = S_INTERCOM4;
    states[thing] = states[S_INTERCOM1];
    states[thing].sprite = SPR_COM4;

    thing = S_INTERCOM5;
    states[thing] = states[S_INTERCOM1];
    states[thing].sprite = SPR_COM5;

    thing = S_INTERCOM6;
    states[thing] = states[S_INTERCOM1];
    states[thing].sprite = SPR_COM6;

    thing = S_INTERCOM7;
    states[thing] = states[S_INTERCOM1];
    states[thing].sprite = SPR_COM7;

    thing = S_INTERCOM8;
    states[thing] = states[S_INTERCOM1];
    states[thing].sprite = SPR_COM8;

    thing = S_INTERCOM9;
    states[thing] = states[S_INTERCOM1];
    states[thing].sprite = SPR_COM9;

    // Sprites
    //////////

    thing = SPR_DISS;
    sprnames[thing]="DISS";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    // Player sprites
    thing = SPR_STND;
    sprnames[thing]="STND";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_WAIT;
    sprnames[thing]="WAIT";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_WALK;
    sprnames[thing]="WALK";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_DASH;
    sprnames[thing]="DASH";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_JUMP;
    sprnames[thing]="JUMP";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_EDGE;
    sprnames[thing]="EDGE";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_PAIN;
    sprnames[thing]="PAIN";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_DETH;
    sprnames[thing]="DETH";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_SPRN;
    sprnames[thing]="SPRN";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_FALL;
    sprnames[thing]="FALL";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_HANG;
    sprnames[thing]="HANG";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_GASP;
    sprnames[thing]="GASP";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_FLYY;
    sprnames[thing]="FLYY";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_TIRE;
    sprnames[thing]="TIRE";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_GLID;
    sprnames[thing]="GLID";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_CLNG;
    sprnames[thing]="CLNG";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_CLIM;
    sprnames[thing]="CLIM";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_LORB;
    sprnames[thing]="LORB";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_DORB;
    sprnames[thing]="DORB";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_SSTN;
    sprnames[thing]="SSTN";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    thing = SPR_SRUN;
    sprnames[thing]="SRUN";
    #ifdef HWRENDER
    t_lspr[thing] = t_lspr[SPR_STND];
    #endif

    // Shields
    thing = SPR_SHLD;
    sprnames[thing]="SHLD";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_WSHD;
    sprnames[thing]="WSHD";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    // 'Particle' Scenery
    thing = SPR_SMOK;
    sprnames[thing]="SMOK";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_GRAS;
    sprnames[thing]="GRAS";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_SAND;
    sprnames[thing]="SAND";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_SPLH;
    sprnames[thing]="SPLH";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_SPRK;
    sprnames[thing]="SPRK";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[RINGSPARK_L];
    #endif

    thing = SPR_SSPK;
    sprnames[thing]="SSPK";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[SUPERSPARK_L];
    #endif

    thing = SPR_XPLD;
    sprnames[thing]="XPLD";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_BPLD;
    sprnames[thing]="BPLD";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

	// Flowers
    thing = SPR_FWR1;
    sprnames[thing]="FWR1";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_FWR2;
    sprnames[thing]="FWR2";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_FWR3;
    sprnames[thing]="FWR3";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_FWR4;
    sprnames[thing]="FWR4";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_FWR5;
    sprnames[thing]="FWR5";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_BUS1;
    sprnames[thing]="BUS1";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_BUS2;
    sprnames[thing]="BUS2";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

	// XMas Scenery!
    thing = SPR_XMS1;
    sprnames[thing]="XMS1";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_XMS2;
    sprnames[thing]="XMS2";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_XMS3;
    sprnames[thing]="XMS3";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

	// Animals
    thing = SPR_BIRD;
    sprnames[thing]="BIRD";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_BUNN;
    sprnames[thing]="BUNN";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_MOUS;
    sprnames[thing]="MOUS";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    // Bubbles
    thing = SPR_BUBL;
    sprnames[thing]="BUBL";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_BUB1;
    sprnames[thing]="BUB1";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_BUB2;
    sprnames[thing]="BUB2";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_BUB3;
    sprnames[thing]="BUB3";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

	// Other Scenery
    thing = SPR_HCHN;
    sprnames[thing]="HCHN";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_SNOW;
    sprnames[thing]="SNOW";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_RAIN;
    sprnames[thing]="RAIN";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_FLAM;
    sprnames[thing]="FLAM";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_ALRM;
    sprnames[thing]="ALRM";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_TELE;
    sprnames[thing]="TELE";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    // HUD objects
    thing = SPR_DCNT;
    sprnames[thing]="DCNT";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    // Transportation Objects
    thing = SPR_STEM;
    sprnames[thing]="STEM";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    // Pushable objects
    thing = SPR_GARG;
    sprnames[thing]="GARG";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    // Grabable powerups
    thing = SPR_RING;
    sprnames[thing]="RING";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    // Chaos Objects
    thing = SPR_EMER;
    sprnames[thing]="EMER";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_CRNG;
    sprnames[thing]="CRNG";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

	// Painful things! Yay!
    thing = SPR_SPIK;
    sprnames[thing]="SPIK";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_CSPK;
    sprnames[thing]="CSPK";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_FSPK;
    sprnames[thing]="FSPK";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    // Intercoms
    thing = SPR_COM1;
    sprnames[thing]="COM1";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_COM2;
    sprnames[thing]="COM2";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_COM3;
    sprnames[thing]="COM3";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_COM4;
    sprnames[thing]="COM4";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_COM5;
    sprnames[thing]="COM5";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_COM6;
    sprnames[thing]="COM6";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_COM7;
    sprnames[thing]="COM7";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_COM8;
    sprnames[thing]="COM8";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    thing = SPR_COM9;
    sprnames[thing]="COM9";
    #ifdef HWRENDER
    t_lspr[thing] = &lspr[NOLIGHT];
    #endif

    sprnames[NUMSPRITES]=NULL;
    sprnames[NUMSPRITES+1]=NULL;

    // Sfx
    //////

    thing = sfx_scrsht;
    S_sfx[thing]=emptysfx;
    S_sfx[thing].name="scrsht";

    thing = sfx_rpcom1;
    S_sfx[thing]=emptysfx;
    S_sfx[thing].name="rpcom1";

    thing = sfx_rpcom2;
    S_sfx[thing]=emptysfx;
    S_sfx[thing].name="rpcom2";

    thing = sfx_rpcom3;
    S_sfx[thing]=emptysfx;
    S_sfx[thing].name="rpcom3";

    thing = sfx_rpcom4;
    S_sfx[thing]=emptysfx;
    S_sfx[thing].name="rpcom4";

    thing = sfx_rpcom5;
    S_sfx[thing]=emptysfx;
    S_sfx[thing].name="rpcom5";

    thing = sfx_rpcom6;
    S_sfx[thing]=emptysfx;
    S_sfx[thing].name="rpcom6";

    thing = sfx_rpcom7;
    S_sfx[thing]=emptysfx;
    S_sfx[thing].name="rpcom7";

    thing = sfx_rpcom8;
    S_sfx[thing]=emptysfx;
    S_sfx[thing].name="rpcom8";

    thing = sfx_rpcom9;
    S_sfx[thing]=emptysfx;
    S_sfx[thing].name="rpcom9";

    // Console commands
    ///////////////////

    COM_AddCommand ("xyzzy", RP_Xyzzy);
    //COM_AddCommand ("card", RP_StartCard);
}

extern void D_AddFile (char *file);
void RP_AddWads(void)
{
	if (!access ("RPChar-Main.wad",R_OK))
		D_AddFile("RPChar-Main.wad");
	else
		I_Error ("The main character wad is missing! RPChar-Main.wad\n");

	if (!access ("RPChar-Hedgehog.wad",R_OK))
		D_AddFile("RPChar-Hedgehog.wad");
	else
		I_Error ("A main character wad is missing! RPChar-Hedgehog.wad\n");

	if (!access ("RPChar-Fox.wad",R_OK))
		D_AddFile("RPChar-Fox.wad");
	else
		I_Error ("A main character wad is missing! RPChar-Fox.wad\n");

	if (!access ("RPChar-Echidna.wad",R_OK))
		D_AddFile("RPChar-Echidna.wad");
	else
		I_Error ("A main character wad is missing! RPChar-Echidna.wad\n");

	if (!access ("RP-Music.wad",R_OK))
		D_AddFile ("RP-Music.wad");
}

// Special mobjs
boolean RP_TouchSpecialThing(mobj_t* special, mobj_t* toucher)
{
    switch (special->type)
    {
        default:
            return false;
    }
}

void RP_MobjThinker(mobj_t* mobj)
{
	switch (mobj->type)
	{
		default:
			return;
	}
}

// Gameplay Stuff
void RP_Gameplay(player_t* player)
{
	// Scan for intercoms in range
	RP_ScanForIntercoms(player);

	// Check for special ability usage!
	RP_Abilitys(player);

    // Show teh placecard on top!
    if (player->cardtime)
        RP_PlaceCard(player);
}

//  Scan for intercoms... Part of teh new hearing range stuff.
void RP_ScanForIntercoms(player_t* player)
{
	mobj_t *mo;
	thinker_t *think;
	boolean found = false;

	player->mo->target = NULL;

	// Scan for intercoms in range
	for(think = thinkercap.next; think != &thinkercap; think = think->next)
	{
		if(think->function.acp1 != (actionf_p1)P_MobjThinker)
			// Not a mobj thinker
			continue;

		mo = (mobj_t *)think;
		if((mo->health <= 0) || (mo->state == &states[S_DISS]))
			// Not a valid mobj
			continue;
				
		if(!(mo->type >= MT_INTERCOM1 && mo->type <= MT_INTERCOM9))
			// Not an intercom...
			continue;

		if (!P_CheckSight (mo, player->mo))
			// Out of sight
			continue;

		if(P_AproxDistance(player->mo->x-mo->x, player->mo->y-mo->y) > INTERCOM_RANGE || player->mo->z-mo->z > INTERCOM_RANGE2)
			// Out of range
			continue;

		if(player->mo->target && P_AproxDistance(P_AproxDistance(player->mo->x-mo->x, player->mo->y-mo->y), player->mo->z-mo->z) > P_AproxDistance(P_AproxDistance(player->mo->x-player->mo->target->x, player->mo->y-player->mo->target->y), player->mo->z-player->mo->target->z))
			// Out of pie (Aka. "This one's even FARTHER away, dumbass!")
			continue;

		// Found a target
		found = true;
		player->mo->target = mo;
	}

	// Set the intercom variable
	if(found && !player->intercom)
	{
		player->intercom = true;
		if(player->mo->target->info->activesound && player == &players[displayplayer])
		  S_StartSound(NULL, player->mo->target->info->activesound);
	}
	else if(!found && player->intercom)
		player->intercom = false;
}

// RP Abilitys! ^.^
void RP_Abilitys(player_t* player)
{
    int i;

	for(i = 0; i < NUMABILITYS; i++)
		if(player->cmd.buttons & (BT_RP1 * (i + 1)))
			switch(player->abilitys[i])
			{
				case 1:
				    // Hedgehog lvl 1 -- Insta-Shield
				    break;
				case 2:
				    // Fox lvl 1 -- ?...
				    break;
				case 3:
				    // Echidna lvl 1 -- Gliding
					if (player->mfjumped && !player->thokked)
					{
						player->gliding = 1;
						P_SetMobjState(player->mo, S_PLAY_GLID1);
						P_InstaThrust(player->mo, player->mo->angle, 20*FRACUNIT/NEWTICRATERATIO);
						player->mfspinning = 0;
						player->mfstartdash = 0;
						player->thokked = true;
					}
					break;
				case 0:
				default:
					break;
			}
}

// Linedef Executors
extern boolean server;
extern boolean admin;
boolean RP_LinedefExecTrigger(player_t* player, int specialtype)
{
    //ticcmd_t* cmd = &player->cmd;

	if((specialtype == 97 || specialtype == 99) && /*player->rp &&*/ player->/*rp->*/trigger == player->mo->subsector->sector->tag)
		return true;
	else if(specialtype == 99 && !server && !admin)
		return true;
	/*else if(specialtype == 99 && !(cmd->buttons & BT_RPLOCK))
		return true;*/
	return false;
}

void RP_LinedefExec(line_t* line, mobj_t* mo)
{
    switch(line->special)
    {
        case 113: // Dark World teleport
			P_Teleport(mo, -mo->x, -mo->y, mo->angle + ANG180);

			mo->momx = mo->momy = mo->momz = 1; // Stop running fast
			if(mo->player)
			{
				mo->player->speed = 0;
				P_SetMobjState(mo, S_PLAY_STND1);
				mo->player->bonuscount += 10; // Flash the palette
			}

			// Play the 'bowrwoosh!' sound
			S_StartSound(mo, sfx_mixup);
			return;

        case 114: // Make unpushable. Shouldn't be used for players, but can be.
			if(mo->flags & MF_PUSHABLE || mo->flags2 & MF2_SLIDEPUSH)
			{
				mo->flags &= ~MF_PUSHABLE;
				mo->flags2 &= ~MF2_SLIDEPUSH;
				mo->flags2 |= MF2_STANDONME;
			}
			return;

        default:
            return;
    }
}

void RP_LinedefExecClose(player_t* player)
{
	player->trigger = player->mo->subsector->sector->tag;
}

void RP_MobjLinedefExec(mobj_t* mo)
{
    int i;
    sector_t* sector;
    sector = mo->subsector->sector;

    switch(sector->special)
    {
		case 20: // Linedef executor
			if(mo->z > mo->floorz)
				return;
		case 19: // Linedef executor that doesn't require touching floor Graue 12-20-2003
		{
			if(mo->player)
				return;

			int masterline, specialtype = 96;

			// Support multiple trigger types (96-98) Graue 12-19-2003
			while((masterline = P_FindSpecialLineFromTag(specialtype, sector->tag)) < 0)
			{
				specialtype++;
				if(specialtype > 98) // After 98, there aren't any more activators
					return;
			}

			// Special type 97 only works once when you hit floor Graue 12-20-2003
			if(specialtype == 97 && !(mo->eflags & MF_JUSTHITFLOOR))
				return;
			{
				int linecnt;
				sector_t* ctlsector; // Graue 11-04-2003

				ctlsector = lines[masterline].frontsector;
				linecnt = ctlsector->linecount;
				for(i=0; i<linecnt; i++)
				{
					if(ctlsector->lines[i]->special != 0
						&& (ctlsector->lines[i]->special < 96 || ctlsector->lines[i]->special > 98)
						&& ctlsector->lines[i]->tag)
						P_ProcessLineSpecial(ctlsector->lines[i], mo);
				}
			}

			// Special type 98 only works once Graue 12-20-2003
			if(specialtype == 98)
				lines[masterline].special = 0; // Clear it out
		}
		return;
	}
}

void RP_StartCard(void)
{
    int     i;
    char word[33];

    if(COM_Argc() < 1)
        return;

    for (i=1 ; i<COM_Argc() ; i++)
        sprintf(word,"%s%s ",word,COM_Argv(i));
    if(!word[0])
        word[0] = '\0';
    if(!players[consoleplayer].card)
        players[consoleplayer].card = (char*)malloc (sizeof(word));
    strcpy(players[consoleplayer].card,word);
    players[consoleplayer].cardtime = 1;
}

void RP_PlaceCard(player_t* player)
{
	if(player->cardtime < 111 && leveltime > 130)
	{
		int lvlttlxpos;

		if(player->card[0] != '\0' && player == &players[displayplayer])
		{
			lvlttlxpos = ((BASEVIDWIDTH/2) - (V_LevelNameWidth(player->card)/2));

			if(player->cardtime == 2)
				V_DrawLevelTitle(lvlttlxpos, 0, 0, player->card);
			else if (player->cardtime == 3)
				V_DrawLevelTitle(lvlttlxpos, 12, 0, player->card);
			else if (player->cardtime == 4)
				V_DrawLevelTitle(lvlttlxpos, 24, 0, player->card);
			else if (player->cardtime == 5)
				V_DrawLevelTitle(lvlttlxpos, 36, 0, player->card);
			else if (player->cardtime == 6)
				V_DrawLevelTitle(lvlttlxpos, 48, 0, player->card);
			else if (player->cardtime == 7)
				V_DrawLevelTitle(lvlttlxpos, 60, 0, player->card);
			else if (player->cardtime == 8)
				V_DrawLevelTitle(lvlttlxpos, 72, 0, player->card);
			else if (player->cardtime >= 9 && player->cardtime < 106)
				V_DrawLevelTitle(lvlttlxpos, 80, 0, player->card);
			else if (player->cardtime == 106)
				V_DrawLevelTitle(lvlttlxpos, 104, 0, player->card);
			else if (player->cardtime == 107)
				V_DrawLevelTitle(lvlttlxpos, 128, 0, player->card);
			else if (player->cardtime == 108)
				V_DrawLevelTitle(lvlttlxpos, 152, 0, player->card);
			else if (player->cardtime == 109)
				V_DrawLevelTitle(lvlttlxpos, 176, 0, player->card);
			else if (player->cardtime == 110)
				V_DrawLevelTitle(lvlttlxpos, 200, 0, player->card);
		}
		player->cardtime++;
	}
	else
		player->cardtime = 0;
}

// Hearing range
int RP_Earshot(int playernum)
{
    fixed_t dist;

    dist = P_AproxDistance(players[consoleplayer].mo->x-players[playernum].mo->x, players[consoleplayer].mo->y-players[playernum].mo->y);
    if(dist < EARSHOT || &players[playernum] == &players[consoleplayer])
        return 1;
    else if(players[consoleplayer].intercom == true && players[playernum].intercom == true)
        return 2;
    else if(server || admin)
        return 3;
    return 0;
}

// Astates - A new world order
void RP_SetAstate(player_t* player)
{
    int i;

    player->astate = -1;
    for(i=0;i<MAXPLAYFRAMES;i++)
    {
        if(player->mo->state == &states[S_PLAY_STND1+i])
			player->astate = AS_STAND;
        else if(player->mo->state == &states[S_PLAY_WAIT1+i])
			player->astate = AS_WAIT;
        else if(player->mo->state == &states[S_PLAY_WALK1+i])
			player->astate = AS_WALK;
        else if(player->mo->state == &states[S_PLAY_DASH1+i])
			player->astate = AS_DASH;
        else if(player->mo->state == &states[S_PLAY_JUMP1+i])
			player->astate = AS_JUMP;
        else if(player->mo->state == &states[S_PLAY_EDGE1+i])
			player->astate = AS_EDGE;
        else if(player->mo->state == &states[S_PLAY_PAIN1+i])
			player->astate = AS_PAIN;
        else if(player->mo->state == &states[S_PLAY_DETH1+i])
			player->astate = AS_DEATH;
        else if(player->mo->state == &states[S_PLAY_SPRN1+i])
			player->astate = AS_SPRING;
        else if(player->mo->state == &states[S_PLAY_FALL1+i])
			player->astate = AS_FALL;
        else if(player->mo->state == &states[S_PLAY_HANG1+i])
			player->astate = AS_HANG;
        else if(player->mo->state == &states[S_PLAY_GASP1+i])
			player->astate = AS_GASP;
        else if(player->mo->state == &states[S_PLAY_FLYY1+i])
			player->astate = AS_FLY;
        else if(player->mo->state == &states[S_PLAY_TIRE1+i])
			player->astate = AS_TIRED;
        else if(player->mo->state == &states[S_PLAY_GLID1+i])
			player->astate = AS_GLIDE;
        else if(player->mo->state == &states[S_PLAY_CLNG1+i])
			player->astate = AS_CLING;
        else if(player->mo->state == &states[S_PLAY_CLIM1+i])
			player->astate = AS_CLIMB;
        else if(player->mo->state == &states[S_PLAY_LORB1+i])
			player->astate = AS_LIGHTORB;
        else if(player->mo->state == &states[S_PLAY_DORB1+i])
			player->astate = AS_DARKORB;
        else if(player->mo->state == &states[S_PLAY_SSTN1+i])
			player->astate = AS_SUPER_STAND;
        else if(player->mo->state == &states[S_PLAY_SRUN1+i])
			player->astate = AS_SUPER_RUN;

		if(player->astate != -1)
			return;
	}
	player->astate = AS_STAND;
}

// New skin sprite system! Yay!
spritedef_t* RP_Skin(mobj_t* thing)
{
    spritedef_t* sprdef;

    if(!thing->player) // If you have no player...
    {
        if(thing->health) // If you're alive...
        {
            if(!thing->momx && !thing->momy && !thing->momz) // If you're not moving...
                sprdef = &((skin_t *)thing->skin)->sprstnd; // Then stand!
            if(thing->momx || thing->momy) // If you ARE moving...
                sprdef = &((skin_t *)thing->skin)->sprwalk; // Then walk!
        }
        else // If you have no health...
            sprdef = &((skin_t *)thing->skin)->sprpain; // Then be a dead body!
    }
    else if(thing->player) // If you have a player...
    {
        if(!thing->player->astate) // If you have no astate...
            sprdef = &((skin_t *)thing->skin)->sprstnd; // Then stand!
        else if(thing->player->astate == AS_STAND)
            sprdef = &((skin_t *)thing->skin)->sprstnd;
        else if(thing->player->astate == AS_WAIT)
            sprdef = &((skin_t *)thing->skin)->sprwait;
        else if(thing->player->astate == AS_WALK)
            sprdef = &((skin_t *)thing->skin)->sprwalk;
        else if(thing->player->astate == AS_DASH)
            sprdef = &((skin_t *)thing->skin)->sprdash;
        else if(thing->player->astate == AS_JUMP)
            sprdef = &((skin_t *)thing->skin)->sprjump;
        else if(thing->player->astate == AS_EDGE)
            sprdef = &((skin_t *)thing->skin)->spredge;
        else if(thing->player->astate == AS_PAIN)
            sprdef = &((skin_t *)thing->skin)->sprpain;
        else if(thing->player->astate == AS_DEATH)
            sprdef = &((skin_t *)thing->skin)->sprdeth;
        else if(thing->player->astate == AS_SPRING)
            sprdef = &((skin_t *)thing->skin)->sprsprn;
        else if(thing->player->astate == AS_FALL)
            sprdef = &((skin_t *)thing->skin)->sprfall;
        else if(thing->player->astate == AS_HANG)
            sprdef = &((skin_t *)thing->skin)->sprhang;
        else if(thing->player->astate == AS_GASP)
            sprdef = &((skin_t *)thing->skin)->sprgasp;
        else if(thing->player->astate == AS_FLY)
            sprdef = &((skin_t *)thing->skin)->sprflyy;
        else if(thing->player->astate == AS_TIRED)
            sprdef = &((skin_t *)thing->skin)->sprtire;
        else if(thing->player->astate == AS_GLIDE)
            sprdef = &((skin_t *)thing->skin)->sprglid;
        else if(thing->player->astate == AS_CLING)
            sprdef = &((skin_t *)thing->skin)->sprclng;
        else if(thing->player->astate == AS_CLIMB)
            sprdef = &((skin_t *)thing->skin)->sprclim;
        else if(thing->player->astate == AS_LIGHTORB)
            sprdef = &((skin_t *)thing->skin)->sprlorb;
        else if(thing->player->astate == AS_DARKORB)
            sprdef = &((skin_t *)thing->skin)->sprdorb;
        else if(thing->player->astate == AS_SUPER_STAND)
            sprdef = &((skin_t *)thing->skin)->sprsstn;
        else if(thing->player->astate == AS_SUPER_RUN)
            sprdef = &((skin_t *)thing->skin)->sprsrun;
        else // If I messed something up and your astate is unknown...
            sprdef = &((skin_t *)thing->skin)->sprstnd; // Then stand and pretend nothing's happened! XD
    }
    return sprdef;
}

// Xyzzy
void RP_Xyzzy(void)
{
    CONS_Printf("Nothing happens.\n");
}

// HUD stuff
void RP_Hud(void)
{
    player_t* player = &players[statusbarplayer];

	if(player->intercom)
		V_DrawCenteredString(292-32, 8, 0, "INTERCOM");
}

// P_DamageMobj -- A rewrite of the entire damage code.
boolean P_DamageMobj ( mobj_t*   target,
                       mobj_t*   inflictor,
                       mobj_t*   source,
                       int       damage )
{
    //unsigned    ang;
    //player_t*   player;
    //boolean     takedamage;  // false on some case in teamplay

    if (!(target->flags & MF_SHOOTABLE))
        return false; // shouldn't happen...

    return false;
}
#endif
