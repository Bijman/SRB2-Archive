// SU - SRB2 Universe
// By Jason the Echidna
// MINE! NO STEALY!

#ifdef UNIVERSE
#ifndef __SU_SAVE__
#define __SU_SAVE__

#define SUSAVESIZE (128*1024)
void SU_SaveChar(int savegameslot);
void SU_LoadChar(byte** savebuffer, int playernum);
void SU_Got_LoadChar(char** savebuffer, int playernum);

#endif
#endif
