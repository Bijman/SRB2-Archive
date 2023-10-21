// SU - SRB2 Universe
// By Jason the Echidna
// MINE! NO STEALY!

#include "../../doomstat.h"
#ifdef UNIVERSE

// Headers
#include "../../byteptr.h"
#include "../../g_game.h"
#include "../../m_misc.h"
#include "../../p_saveg.h"
#include "../../r_main.h"
#include "../../r_things.h"
#include "SU_Command.h"
#include "SU_Menu.h"
#include "SU_Save.h"

static void SU_ArchivePlayer(void)
{

	player_t* player = &players[consoleplayer];
	int i;

	if(!playeringame[consoleplayer])
		return;

	WRITEBYTE(save_p, player->skin);
	WRITEBYTE(save_p, player->skincolor);

	WRITEBYTE(save_p, player->sulevel);
	WRITEULONG(save_p, player->suexp);

	WRITEUSHORT(save_p, player->sumaxhp);
	WRITEUSHORT(save_p, player->sumaxtp);

	WRITEULONG(save_p, player->sumoney);
	for(i = 0; i < NUMSUITEMS; i++)
		WRITEBYTE(save_p, player->suitems[i]);

	// Nothing equipped
	if(player->equipweapon == 0)
		player->equipweapon = itm_saber - 1;
	if(player->equipbarrier == 0)
		player->equipbarrier = itm_barrier - 1;
	if(player->equiparmor == 0)
		player->equiparmor = itm_frame - 1;
	for(i = 0; i < 4; i++)
		if(player->equipslot[i] == 0)
			player->equipslot[i] = NUMSUITEMS - 1;
	if(player->equipmag == 0)
		player->equipmag = NUMSUITEMS - 1;

	WRITEUSHORT(save_p, player->equipweapon - (itm_saber - 1));
	WRITEUSHORT(save_p, player->equipbarrier - (itm_barrier - 1));
	WRITEUSHORT(save_p, player->equiparmor - (itm_frame - 1));
	for(i = 0; i < 4; i++)
		WRITEUSHORT(save_p, player->equipslot[i] - (NUMSUITEMS - 1));
	WRITEUSHORT(save_p, player->equipmag - (NUMSUITEMS - 1));

	// Nothing equipped
	if(player->equipweapon == itm_saber - 1)
		player->equipweapon = 0;
	if(player->equipbarrier == itm_barrier - 1)
		player->equipbarrier = 0;
	if(player->equiparmor == itm_frame - 1)
		player->equiparmor = 0;
	for(i = 0; i < 4; i++)
		if(player->equipslot[i] == NUMSUITEMS - 1)
			player->equipslot[i] = 0;
	if(player->equipmag == NUMSUITEMS - 1)
		player->equipmag = 0;
}

static void SU_UnArchivePlayer(int playernum)
{
	player_t* player = &players[playernum];
	int i;

	player->skin = READBYTE(save_p);
	player->skincolor = READBYTE(save_p);
	SetSavedSkin(playernum, player->skin, player->skincolor);

	player->sulevel = READBYTE(save_p);
	player->suexp = READULONG(save_p);

	player->health = player->sumaxhp = READUSHORT(save_p);
	player->sutp = player->sumaxtp = READUSHORT(save_p);
	if(player->mo)
		player->mo->health = player->health;

	player->sumoney = READULONG(save_p);
	for(i = 0; i < NUMSUITEMS; i++)
		player->suitems[i] = READBYTE(save_p);

	player->equipweapon = READUSHORT(save_p) + (itm_saber - 1);
	player->equipbarrier = READUSHORT(save_p) + (itm_barrier - 1);
	player->equiparmor = READUSHORT(save_p) + (itm_frame - 1);
	for(i = 0; i < 4; i++)
		player->equipslot[i] = READUSHORT(save_p) + (NUMSUITEMS - 1);
	player->equipmag = READUSHORT(save_p) + (NUMSUITEMS - 1);

	// Nothing equipped
	if(player->equipweapon == itm_saber - 1)
		player->equipweapon = 0;
	if(player->equipbarrier == itm_barrier - 1)
		player->equipbarrier = 0;
	if(player->equiparmor == itm_frame - 1)
		player->equiparmor = 0;
	for(i = 0; i < 4; i++)
		if(player->equipslot[i] == NUMSUITEMS - 1)
			player->equipslot[i] = 0;
	if(player->equipmag == NUMSUITEMS - 1)
		player->equipmag = 0;

	// Haxx checking...
	{
		boolean invalid = false;

		// No HP. O_o
		if(player->sumaxhp <= 0)
			invalid = true;

		// Too many of an item...
		for(i = 0; i < NUMSUITEMS; i++)
		{
			if(player->suitems[i] > 99)
			{
				invalid = true;
				break;
			}
		}

		// Too far forward out of range equipment...
		if(player->equipweapon > itm_jterailgun
		|| player->equipbarrier > itm_whitering
		|| player->equiparmor > itm_smokingplate
		|| player->equipmag > 0)
			invalid = true;

		// Don't even HAVE that item?
		else if((player->equipweapon != 0 && player->suitems[player->equipweapon] == 0)
		|| (player->equipbarrier != 0 && player->suitems[player->equipbarrier] == 0)
		|| (player->equiparmor != 0 && player->suitems[player->equiparmor] == 0)
		|| (player->equipmag != 0 && player->suitems[player->equipmag] == 0))
			invalid = true;

		// Out of range equipped slots
		else for(i = 0; i < 4; i++)
			if(player->equipslot[i] != 0)
			{
				invalid = true;
				break;
			}

		// What to do if invalid!
		if(invalid)
		{
			CONS_Printf("Character data is invalid. %s fails at life.\n", player_names[playernum]);

			player->skin = 0;
			player->skincolor = 1;
			SetSavedSkin(playernum, player->skin, player->skincolor);

			player->sulevel = 0;
			player->suexp = 0;

			player->health = player->sumaxhp = 1;
			player->sutp = player->sumaxtp = 0;
			if(player->mo)
				player->mo->health = player->health;

			player->sumoney = 0;
			for(i = 0; i < NUMSUITEMS; i++)
				player->suitems[i] = 0;

			player->equipweapon = itm_invalid;
			player->equipbarrier = itm_invalid;
			player->equiparmor = itm_invalid;
			for(i = 0; i < 4; i++)
				player->equipslot[i] = itm_invalid;
			player->equipmag = itm_invalid;
		}
	}
}

void SU_SaveChar(int savegameslot)
{
	char name[256];
	byte* savebuffer;
	size_t length;

	gameaction = ga_nothing;

	sprintf(name, "SRB2Univ%d.chr", savegameslot);

	save_p = savebuffer = (byte*)malloc(SUSAVESIZE);
	if(!save_p)
	{
		CONS_Printf("No more free memory for character\n");
		return;
	}

	WRITEMEM(save_p, "SUC", 3);
	WRITEBYTE(save_p, VERSION);
	WRITEBYTE(save_p, SUBVERSION);

	SU_ArchivePlayer();

	length = save_p - savebuffer;
	if(length > SUSAVESIZE)
		I_Error("Character buffer overrun");
	FIL_WriteFile(name, savebuffer, length);

	free(savebuffer);
	savebuffer = save_p = NULL;

	CONS_Printf("Character data saved.\n");
}

typedef struct
{
	char name[3];
	byte version;
	byte subversion;
} SUCheader_t;

void SU_LoadChar(byte** savebuffer, int playernum)
{
	SUCheader_t header;

	save_p = *savebuffer;

	if(demoplayback)
		G_StopDemo();

	if(!universe)
		universe = true;

	if(!netgame)
	{
		displayplayer = consoleplayer;
		G_DeferedInitNew(2, G_BuildMapName(SULOBBYMAP), 0, false, false);
	}
	gameaction = ga_nothing;

	header.name[0] = READBYTE(save_p);
	header.name[1] = READBYTE(save_p);
	header.name[2] = READBYTE(save_p);
	header.name[3] = '\0';
	header.version = READBYTE(save_p);
	header.subversion = READBYTE(save_p);

	if(!strcmp(header.name, "SUC"))
		I_Error("Unknown header in character data (Found '%s')", header.name);
	else if(header.version != VERSION)
		I_Error("Version number does not match (Found '%d')", header.version);
	else if(header.subversion != SUBVERSION)
		I_Error("Subversion number does not match (Found '%d')", header.subversion);

	SU_UnArchivePlayer(playernum);
	CONS_Printf("Character data loaded.\n");
}

void SU_Got_LoadChar(char** savebuffer, int playernum)
{
	byte *buf = (byte *)*savebuffer;
	SU_LoadChar(&buf, playernum);
	*savebuffer = (char *)save_p;
}

#endif

