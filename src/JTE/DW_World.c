// DW - Dark World
// By Jason the Echidna
// MINE! NO STEALY!

#include "../doomstat.h"
#include "../r_main.h"
#include "../r_things.h"
#include "../info.h"
#include "../p_local.h"
#include "../p_mobj.h"
#include "DW_World.h"

#ifdef DARKWORLD
void DW_Init(void)
{
	const mobjtype_t thing = MT_DARKSHADOW;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_NOGRAVITY|MF_SCENERY;
}

// Dark World shadows
void DW_Shadow(mobj_t* mo)
{
	mobj_t* shdw;
	int num;

	// Don't do Dark World stuff in a normal map!
	if(!(mapheaderinfo[gamemap-1].darkworld))
		return;

	// Create a dark world shadow if you don't have one
	if(!mo->darkshadow)
	{
		mo->darkshadow = P_SpawnMobj(-mo->x, -mo->y, mo->z, MT_DARKSHADOW);
		mo->darkshadow->darkshadow = mo;
	}

	// Convinience
	shdw = mo->darkshadow;

	if(mo->player)
		shdw->player = mo->player;

	// Put the shadow in the same position as the mobj
	P_UnsetThingPosition(shdw);
	shdw->x = -mo->x;
	shdw->y = -mo->y;
	shdw->z = mo->z;
	shdw->floorz = mo->floorz;
	shdw->ceilingz = mo->ceilingz;
	shdw->angle = mo->angle + ANG180;
	P_SetThingPosition(shdw);
	shdw->momx = mo->momx;
	shdw->momy = mo->momy;
	shdw->momz = mo->momz;

	// Make the shadow look exactly like the mobj
	shdw->skin = mo->skin;
	shdw->sprite = mo->sprite;
	shdw->frame = mo->frame;

	// Set the shdow's color to that of the mobj's
	num = ((mo->flags & MF_TRANSLATION)>>MF_TRANSSHIFT);
	shdw->flags = (shdw->flags & ~MF_TRANSLATION) | ((num)<<MF_TRANSSHIFT);

	// Make the shadow semi-transparent so they know it's not the real mobj
	shdw->frame &= ~FF_TRANSMASK;
	//shdw->frame |= (tr_transfir<<FF_TRANSSHIFT);
	shdw->flags2 |= MF2_SHADOW;
}

#endif

