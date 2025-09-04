#ifdef DARKWORLD
#include "r_main.h"
#include "r_things.h"
#include "info.h"
#include "p_local.h"
#include "DarkWorld.h"

void DarkWorldInit(void)
{
    fixed_t thing;

    thing = MT_DARKSHADOW;
    mobjinfo[thing] = mobjinfo[MT_DISS];
    //mobjinfo[thing].spawnstate = S_RINGA;
    mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOSECTOR|MF_NOGRAVITY|MF_NOCLIP;
}

// Dark World shadows
void DarkWorldShadows(mobj_t* mo)
{
	// Create a dark world shadow if you don't have one
	if(!mo->darkshadow)
	{
	    mo->darkshadow = P_SpawnMobj(-mo->x, -mo->y, mo->z, MT_DARKSHADOW);
	    if(mo->player)
	 	   mo->darkshadow->player = mo->player;

        mo->darkshadow->darkshadow = mo;
	}

	mobj_t* shdw;
	shdw = mo->darkshadow;

	// Put the shadow in the same position as the mobj
    P_UnsetThingPosition (shdw);
	shdw->x = -mo->x;
	shdw->y = -mo->y;
	shdw->z = mo->z;
    shdw->floorz = mo->floorz;
    shdw->ceilingz = mo->ceilingz;
	shdw->angle = mo->angle + ANG180;
    P_SetThingPosition (shdw);

    // Make the shadow look exactly like the mobj
    shdw->skin = mo->skin;
    //shdw->state = mo->state;
    //shdw->tics = mo->tics;
    shdw->sprite = mo->sprite;
    shdw->frame = mo->frame;
    if(mo->player)
        shdw->flags = (shdw->flags & ~MF_TRANSLATION) | ((mo->player->skincolor)<<MF_TRANSSHIFT);

    // Make the shadow semi-transparent so they know it's not really the mobj
	shdw->frame |= (tr_transmed<<FF_TRANSSHIFT);

	// And dun let it move on it's own.
    shdw->momx = shdw->momy = shdw->momz = 0;
}

#endif

