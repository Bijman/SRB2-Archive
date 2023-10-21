// TML - The Missing Link
// By Shuffle
// 'cause Link lost the chain links on his necklace. :)

#include "../doomstat.h"
#ifdef ZELDA

static void Equip_OnChange(byte equip)
{
	if(!(players[consoleplayer].equips & equip<<1))
		return;
	SendNetXCmd(XD_ZELDAEQUIP, equip, 1);
}
static void Got_Equip(char **p, int playernum)
{
	player_t *player = players[playernum];
	player->equipped = READBYTE(*p);
}

static CV_PossibleValue_t zeldaequip_cons_t[] = {{0, "MIN"}, {3, "MAX"}, {0, NULL}};
consvar_t cv_zeldaequip = {NULL,"0",CV_CALL|CV_NOINIT|CV_HIDDEN,zeldaequip_cons_t,Equip_OnChange,0,NULL,NULL,0,0,NULL};
void TML_Init(void)
{
	CV_RegisterVar(&cv_zeldaequip);
	RegisterNetXCmd(XD_ZELDAEQUIP, Got_Equip);
}

void TML_SwordAttack(player_t* player)
{
	mobj_t* mobj;

	// Don't attack if the player is already attacking, dead, reeling from a hit, getting item info,
	// or if he's in the air.
	if(!onground ||player->weapondelay || player->playerstate != PST_LIVE)
		return;

	switch(M_Random()/64)
	{
		case 0:
		default:
			S_StartSound(player->mo, sfx_swipe);
			break;
		case 1:
			S_StartSound(player->mo, sfx_swipe2);
			break;
		case 2:
			S_StartSound(player->mo, sfx_swipe3);
			break;
		case 3:
			S_StartSound(player->mo, sfx_swipe4);
			break;
	}
	P_SetPlayerMobjState(player->mo, S_PLAY_SWORD1);

	P_ResetPlayer(player);
	player->mo->momx = player->mo->momy =
		player->rmomx = player->rmomy = 0;
	player->weapondelay = TICRATE/2;
	P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, MT_SWORD)->target = player->mo;
}

void TML_DropBomb(player_t* player)
{
	mobj_t* mobj;

	if(gametype != GT_MATCH && player->items[itm_bombs] <= 0)
		return;
	if(gametype == GT_MATCH && player->bombtics <= 0)
		return;
	if(player->bombsout >= 3)
		return;

	player->bombsout += 1;
	player->items[itm_bombs]--;
	mobj = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z + 20*FRACUNIT, MT_BOMB);
	mobj->target = player->mo;
	mobj->fuse = 170; // Bombs have fuses, too! :)
}

#endif
