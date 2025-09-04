#ifndef __RPH2__
#define __RPH2__

#include "d_player.h"
#include "r_defs.h"

////////////
// Protos //
////////////

boolean RP_LinedefExecTrigger(player_t* player, int specialtype);
void RP_LinedefExec(line_t* line, mobj_t* mo);
void RP_LinedefExecClose(player_t* player);
void RP_PlaceCard(player_t* player);
void RP_Gameplay(player_t* player);
void RP_ScanForIntercoms(player_t* player);
void RP_Abilitys(player_t* player);
spritedef_t* RP_Skin(mobj_t* thing);
void RP_SetAstate(player_t* player);

#endif
