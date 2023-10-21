// SR - Soviet Russia Blast 2
// By Jason the Echidna
// MINE! NO STEALY!

#ifdef RUSSIA
#ifndef __SR_INFO__
#define __SR_INFO__

extern boolean russia;
void SR_Init(void);
void SR_PlayerPop(player_t* player);
void SR_Collect(mobj_t* mo, mobj_t* ring);
void SR_RingSpill(mobj_t* ring);
void SR_CollectThink(mobj_t* mo);

#endif
#endif
