// SU - SRB2 Universe
// By Jason the Echidna
// MINE! NO STEALY!

#ifdef UNIVERSE
#ifndef __SU_INFO__
#define __SU_INFO__

void SU_InitInfo(void);

typedef struct sumobjinfo_s
{
	byte level;
	byte damage;
	mobjtype_t raretype;
	byte raretyperarity;
} sumobjinfo_t;
extern sumobjinfo_t* sumobjinfo[NUMMOBJTYPES];

#endif
#endif
