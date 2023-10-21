// JB - JTE Bots
// By Jason the Echidna
// MINE! NO STEALY!

#ifdef JTEBOTS
#ifndef __JB_GAME__
#define __JB_GAME__

void JB_LevelInit(void);
#ifdef SUCKYBOTWAYPOINTS
void JB_CreateWaypoint(fixed_t x, fixed_t y, fixed_t z, boolean spring);
void JB_UpdateWaypoints(void);
#endif
void JB_BotThink(int botnum);

#endif
#endif
