#ifndef __RPH__
#define __RPH__

//////////////
// Includes //
//////////////
#include "doomdef.h"
#include "p_mobj.h"

/////////////////
// Definitions //
/////////////////
#define SUBVERSION 1
#define EARSHOT 1024*FRACUNIT // How far away you can hear other people.
#define INTERCOM_RANGE 480*FRACUNIT // How far away you can hear the intercom.
#define INTERCOM_RANGE2 100*FRACUNIT // How far away you can hear the intercom.
#define NUMABILITYS 7
#define NUMELEMENTS 3

/////////////
// Structs //
/////////////

typedef enum
{
    AS_STAND,
    AS_WAIT,
    AS_WALK,
    AS_DASH,
    AS_JUMP,
    AS_EDGE,
    AS_PAIN,
    AS_DEATH,
    AS_SPRING,
    AS_FALL,
    AS_HANG,
    AS_GASP,
    AS_FLY,
    AS_TIRED,
    AS_GLIDE,
    AS_CLING,
    AS_CLIMB,
    AS_LIGHTORB,
    AS_DARKORB,
    AS_SUPER_STAND,
    AS_SUPER_RUN,

    NUMASTATES
} astate_t;

////////////
// Protos //
////////////

void RP_InitRP(void);
int RP_Earshot(int playernum);
void RP_Hud(void);
boolean RP_TouchSpecialThing(mobj_t* special, mobj_t* toucher);
void RP_MobjThinker(mobj_t* mobj);
void RP_MobjLinedefExec(mobj_t* mo);
void RP_Xyzzy(void);
void RP_AddWads(void);
void RP_StartCard(void);

#endif
