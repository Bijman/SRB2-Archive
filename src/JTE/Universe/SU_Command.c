// SU - SRB2 Universe
// By Jason the Echidna
// MINE! NO STEALY!

#include "../../doomstat.h"
#ifdef UNIVERSE

#include "../../byteptr.h"
#include "../../g_game.h"
#include "../../m_misc.h"
#include "../../r_main.h"
#include "../../z_zone.h"

#include "SU_Command.h"
#include "SU_Menu.h"
#include "SU_Save.h"
#include "SU_Type.h"

boolean universe = false;
static int lastslot = 0;

static void SU_Command_Equip(void)
{
	const char* itemname;
	suitemtype_t i, itemnum = 0;

	char buf[128];
	char* p;

	if(!Playing() || !universe)
	{
		CONS_Printf("This command is only for SRB2 Universe, sorry.\n");
		return;
	}

	if(COM_Argc() < 2)
	{
		CONS_Printf("Equipped items:\n");
		if(players[consoleplayer].equipweapon)
			CONS_Printf("Weapon: %s\n",suitems[players[consoleplayer].equipweapon].name);
		if(players[consoleplayer].equipbarrier)
			CONS_Printf("Barrier: %s\n",suitems[players[consoleplayer].equipbarrier].name);
		if(players[consoleplayer].equiparmor)
			CONS_Printf("Armor: %s\n",suitems[players[consoleplayer].equiparmor].name);
		if(players[consoleplayer].equipmag)
			CONS_Printf("Mag: %d\n",suitems[players[consoleplayer].equipmag].name);
		if(!players[consoleplayer].equipweapon
			&& !players[consoleplayer].equipbarrier
			&& !players[consoleplayer].equiparmor
			&& !players[consoleplayer].equipmag)
			CONS_Printf("None!\n");
		return;
	}
	itemname = COM_Argv(1);

	for(i = 0; i < NUMSUITEMS; i++)
		if(suitems[i].name && !stricmp(suitems[i].name, COM_Args()))
		{
			itemnum = i;
			break;
		}

	if(i == NUMSUITEMS)
	{
		CONS_Printf("You don't have that item!\n");
		return;
	}

	if(!players[consoleplayer].suitems[itemnum] && !cv_debug)
	{
		CONS_Printf("You don't have that item!\n");
		return;
	}

	p = buf;
	WRITEUSHORT(p, itemnum);

	if(suitems[itemnum].type & SUT_WEAPON)
	{
		CONS_Printf("Weapon equipped.\n");
		SendNetXCmd(XD_EQUIP, buf, p-buf);
	}
	else if(suitems[itemnum].type & SUT_BARRIER)
	{
		CONS_Printf("Barrier equipped.\n");
		SendNetXCmd(XD_EQUIP, buf, p-buf);
	}
	else if(suitems[itemnum].type & SUT_FRAME)
	{
		CONS_Printf("Armor equipped.\n");
		SendNetXCmd(XD_EQUIP, buf, p-buf);
	}
	else if(suitems[itemnum].type & SUT_SLOT)
	{
		for(i = 0; i < 4; i++)
			if(!players[consoleplayer].equipslot[i])
				break;
		if(i == 4)
		{
			CONS_Printf("All slots are full!\n");
			return;
		}
		CONS_Printf("Slot equipped.\n");
		SendNetXCmd(XD_EQUIP, buf, p-buf);
	}
	else if(suitems[itemnum].type & SUT_MAG)
	{
		CONS_Printf("Mag equipped.\n");
		SendNetXCmd(XD_EQUIP, buf, p-buf);
	}
	else
		CONS_Printf("Error: You can't equip that!\n");
}

static void SU_Got_Equip(char** p, int playernum)
{
	int i;
	unsigned short itemnum = READUSHORT(*p);

	if(suitems[itemnum].type & SUT_WEAPON)
		players[playernum].equipweapon = itemnum;
	else if(suitems[itemnum].type & SUT_BARRIER)
		players[playernum].equipbarrier = itemnum;
	else if(suitems[itemnum].type & SUT_FRAME)
		players[playernum].equiparmor = itemnum;
	else if(suitems[itemnum].type & SUT_SLOT)
	{
		for(i = 0; i < 4; i++)
			if(!players[playernum].equipslot[i])
			{
				players[playernum].equipslot[i] = itemnum;
				break;
			}
	}
	else if(suitems[itemnum].type & SUT_MAG)
		players[playernum].equipmag = itemnum;
}

static void SU_Command_Inventory(void)
{
	int i;

	if(!Playing() || !universe)
	{
		CONS_Printf("This command is only for SRB2 Universe, sorry.\n");
		return;
	}

	CONS_Printf("Inventory:\n");

	if(players[consoleplayer].sumoney)
		CONS_Printf("Meseta: %d\n",players[consoleplayer].sumoney);

	for(i = 0; i < NUMSUITEMS; i++)
		if(players[consoleplayer].suitems[i] && suitems[i].name)
			CONS_Printf("%s: %d\n",suitems[i].name,players[consoleplayer].suitems[i]);
}

static void SU_Command_SaveChar(void)
{
	if(!Playing() || !universe)
	{
		CONS_Printf("This command is only for SRB2 Universe, sorry.\n");
		return;
	}
	else
	{
		int slot;
		if(COM_Argc() != 2)
			slot = lastslot;
		else
			slot = lastslot = atoi(COM_Argv(1));
	
		SU_SaveChar(slot);
	}
}

static void SU_Command_LoadChar(void)
{
	if(!Playing() || !universe)
	{
		CONS_Printf("This command is only for SRB2 Universe, sorry.\n");
		return;
	}
	else
	{
		int slot;
		byte *buf;
		int len;
		char name[256];

		if(COM_Argc() != 2)
			slot = lastslot;
		else
			slot = lastslot = atoi(COM_Argv(1));

		sprintf(name, "SRB2Univ%d.chr", slot);
		len = FIL_ReadFile(name, &buf);
		if(!len)
		{
			CONS_Printf("Character file not found\n");
			return;
		}
		if(netgame)
			SendNetXCmd(XD_LOADCHAR, buf, len);
		else
			SU_LoadChar(&buf, consoleplayer);
	}
}

void SU_InitCons(void)
{
	COM_AddCommand("equip", SU_Command_Equip);
	COM_AddCommand("inventory", SU_Command_Inventory);
	COM_AddCommand("savechar", SU_Command_SaveChar);
	COM_AddCommand("loadchar", SU_Command_LoadChar);

	RegisterNetXCmd(XD_EQUIP, SU_Got_Equip);
	RegisterNetXCmd(XD_LOADCHAR, SU_Got_LoadChar);
}
#endif
