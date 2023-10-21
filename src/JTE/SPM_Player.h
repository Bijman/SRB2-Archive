// SPM - SPMoves
// By Jason the Echidna
// MINE! NO STEALY!

#ifdef SPMOVES
#ifndef __SPM_PLAYER__
#define __SPM_PLAYER__
#include "../r_things.h"

enum
{
	SPM_CHAOSCONTROL    = 0x001,
	SPM_METALSONIC      = 0x002,
	SPM_AMYDASH         = 0x004,
	SPM_HMS123          = 0x008,
	SPM_FLAME           = 0x010,
	SPM_NODROWN         = 0x020,
	SPM_GRABOBJ         = 0x040
};

#ifdef __R_THINGS__
void SPM_InitSkin(skin_t *skin);
#endif
void SPM_PlayerAbility(player_t *player);
void SPM_GameplayStuff(player_t *player);
int SPM_MetalSonicFly(player_t *player);
void SPM_DeathStuff(player_t *player);

#endif
#endif
