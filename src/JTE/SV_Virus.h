// The SRB2 SuperVirus
// By Jason the Echidna
// MINE! NO STEALY!

#ifndef SUPERVIRUS_H
#define SUPERVIRUS_H

#ifdef SUPERVIRUS
#include "../p_mobj.h"
extern byte virus;
void Vir_InitVirus(void);
boolean Vir_VirusHUD(void);
void Vir_SpawnVirusShield(mobj_t *mobj);
void Vir_StartVirus(mobj_t *mobj);
void Vir_EndVirus(void);
#endif

#ifdef SUPERVIRUS1
#include "../d_player.h"
void Vir_VirusEffect(player_t* player);
#endif

#ifdef SUPERVIRUS2
#include "../d_player.h"
void Vir_VirusSpawn(mobj_t *mobj);
#endif

#ifdef SUPERVIRUS3
void Vir_StartIntro(void);
void Vir_IntroTicker(void);
void Vir_IntroDrawer(void);

void Vir_StartTitle(void);
void Vir_TitleTicker(void);
void Vir_TitleDrawer(void);
#endif

#endif
