// SU - SRB2 Universe
// By Jason the Echidna
// MINE! NO STEALY!

#include "../../doomstat.h"
#ifdef UNIVERSE

#include "../../d_player.h"
#include "../../z_zone.h"

#ifdef HWRENDER
#include "../../hardware/hw_light.h"
#endif

#include "SU_Info.h"
#include "SU_Command.h"
#include "SU_Type.h"

sumobjinfo_t* sumobjinfo[NUMMOBJTYPES];

void SU_InitInfo(void)
{
	fixed_t thing;

	for(thing = 0; thing < NUMMOBJTYPES; thing++)
	{
		if(mobjinfo[thing].flags & MF_ENEMY)
		{
			if(mobjinfo[thing].painstate == S_NULL)
				mobjinfo[thing].painstate = mobjinfo[thing].seestate;
			if(mobjinfo[thing].painsound == sfx_None)
				mobjinfo[thing].painsound = sfx_dmpain;
		}
		switch(thing)
		{
			case MT_BLUECRAWLA:
				sumobjinfo[thing] = Z_Malloc(sizeof(sumobjinfo_t), PU_STATIC, 0);
				memset(sumobjinfo[thing], 0, sizeof(sumobjinfo_t));
				sumobjinfo[thing]->level = 1;
				sumobjinfo[thing]->damage = 5;
				sumobjinfo[thing]->raretype = MT_REDCRAWLA;
				sumobjinfo[thing]->raretyperarity = 25;
				break;

			case MT_REDCRAWLA:
				sumobjinfo[thing] = Z_Malloc(sizeof(sumobjinfo_t), PU_STATIC, 0);
				memset(sumobjinfo[thing], 0, sizeof(sumobjinfo_t));
				sumobjinfo[thing]->level = 2;
				sumobjinfo[thing]->damage = 15;
				sumobjinfo[thing]->raretype = MT_BLUECRAWLA;
				sumobjinfo[thing]->raretyperarity = 255;
				break;

			case MT_GFZFISH:
				sumobjinfo[thing] = Z_Malloc(sizeof(sumobjinfo_t), PU_STATIC, 0);
				memset(sumobjinfo[thing], 0, sizeof(sumobjinfo_t));
				sumobjinfo[thing]->level = 3;
				sumobjinfo[thing]->damage = 20;
				sumobjinfo[thing]->raretype = MT_DISS;
				sumobjinfo[thing]->raretyperarity = 255;
				break;

			case MT_GOLDBUZZ:
				sumobjinfo[thing] = Z_Malloc(sizeof(sumobjinfo_t), PU_STATIC, 0);
				memset(sumobjinfo[thing], 0, sizeof(sumobjinfo_t));
				sumobjinfo[thing]->level = 3;
				sumobjinfo[thing]->damage = 10;
				sumobjinfo[thing]->raretype = MT_REDBUZZ;
				sumobjinfo[thing]->raretyperarity = 25;
				break;

			case MT_REDBUZZ:
				sumobjinfo[thing] = Z_Malloc(sizeof(sumobjinfo_t), PU_STATIC, 0);
				memset(sumobjinfo[thing], 0, sizeof(sumobjinfo_t));
				sumobjinfo[thing]->level = 4;
				sumobjinfo[thing]->damage = 20;
				sumobjinfo[thing]->raretype = MT_GOLDBUZZ;
				sumobjinfo[thing]->raretyperarity = 255;
				break;

			case MT_JETTGUNNER:
				sumobjinfo[thing] = Z_Malloc(sizeof(sumobjinfo_t), PU_STATIC, 0);
				memset(sumobjinfo[thing], 0, sizeof(sumobjinfo_t));
				sumobjinfo[thing]->level = 5;
				sumobjinfo[thing]->damage = 30;
				sumobjinfo[thing]->raretype = MT_JETTBOMBER;
				sumobjinfo[thing]->raretyperarity = 25;
				break;

			case MT_JETTBOMBER:
				sumobjinfo[thing] = Z_Malloc(sizeof(sumobjinfo_t), PU_STATIC, 0);
				memset(sumobjinfo[thing], 0, sizeof(sumobjinfo_t));
				sumobjinfo[thing]->level = 6;
				sumobjinfo[thing]->damage = 35;
				sumobjinfo[thing]->raretype = MT_JETTGUNNER;
				sumobjinfo[thing]->raretyperarity = 255;
				break;

			case MT_CRAWLACOMMANDER:
				sumobjinfo[thing] = Z_Malloc(sizeof(sumobjinfo_t), PU_STATIC, 0);
				memset(sumobjinfo[thing], 0, sizeof(sumobjinfo_t));
				sumobjinfo[thing]->level = 7;
				sumobjinfo[thing]->damage = 25;
				sumobjinfo[thing]->raretype = MT_DISS;
				sumobjinfo[thing]->raretyperarity = 255;
				break;

			case MT_EGGMOBILE:
				sumobjinfo[thing] = Z_Malloc(sizeof(sumobjinfo_t), PU_STATIC, 0);
				memset(sumobjinfo[thing], 0, sizeof(sumobjinfo_t));
				sumobjinfo[thing]->level = 10;
				sumobjinfo[thing]->damage = 50;
				sumobjinfo[thing]->raretype = MT_EGGMOBILE2;
				sumobjinfo[thing]->raretyperarity = 50;
				break;

			case MT_EGGMOBILE2:
				sumobjinfo[thing] = Z_Malloc(sizeof(sumobjinfo_t), PU_STATIC, 0);
				memset(sumobjinfo[thing], 0, sizeof(sumobjinfo_t));
				sumobjinfo[thing]->level = 20;
				sumobjinfo[thing]->damage = 40;
				sumobjinfo[thing]->raretype = MT_EGGMOBILE;
				sumobjinfo[thing]->raretyperarity = 255;
				break;

			default:
				sumobjinfo[thing] = NULL;
				break;
		}
	}

	thing = MT_STARTTELEPORT;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].spawnstate = S_NULL;
	mobjinfo[thing].radius = 32*FRACUNIT;
	mobjinfo[thing].height = 96*FRACUNIT;
	mobjinfo[thing].flags = MF_NOSECTOR|MF_NOGRAVITY|MF_SPECIAL;

	thing = MT_ENDTELEPORT;
	mobjinfo[thing] = mobjinfo[MT_STARTTELEPORT];

	thing = MT_TELEPIPE;
	mobjinfo[thing] = mobjinfo[MT_STARTTELEPORT];

	thing = MT_TELESPARK1;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].spawnstate = S_TELESPARK1;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 32*FRACUNIT;
	mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOGRAVITY;

	thing = MT_TELESPARK2;
	mobjinfo[thing] = mobjinfo[MT_TELESPARK1];
	mobjinfo[thing].spawnstate = S_TELESPARK2;
	mobjinfo[thing].radius = 2*FRACUNIT;
	mobjinfo[thing].height = 56*FRACUNIT;

	thing = MT_PAYMACHINE;
	mobjinfo[thing] = mobjinfo[MT_EGGCAPSULE];
	mobjinfo[thing].doomednum = -1; // Todo: Decide if Paymachine worth keeping
									// Set proper doomednum if yes.

	thing = MT_SUDROP;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].spawnstate = S_NULL;
	mobjinfo[thing].radius = 16*FRACUNIT;
	mobjinfo[thing].height = 24*FRACUNIT;
	mobjinfo[thing].flags = MF_NOBLOCKMAP;

	thing = MT_SUBOSSDROP;
	mobjinfo[thing] = mobjinfo[MT_SUDROP];

	thing = MT_SUITEM;
	mobjinfo[thing] = mobjinfo[MT_SUDROP];
	mobjinfo[thing].spawnstate = S_SUITEM;
	mobjinfo[thing].flags = MF_SPECIAL|MF_NOGRAVITY;

	thing = MT_SUMONEY;
	mobjinfo[thing] = mobjinfo[MT_SUITEM];
	mobjinfo[thing].spawnstate = S_SUMONEY;

	thing = MT_SUSHOT;
	mobjinfo[thing] = mobjinfo[MT_ROCKET];
	mobjinfo[thing].speed = 50*FRACUNIT;

	thing = MT_KINESO;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].doomednum = 6000;
	mobjinfo[thing].spawnstate = S_KINESO;
	mobjinfo[thing].radius = mobjinfo[MT_PLAYER].radius;
	mobjinfo[thing].height = mobjinfo[MT_PLAYER].height;
	mobjinfo[thing].flags = MF_SLIDEME|MF_SOLID|MF_PUSHABLE;

	thing = MT_BARREL;
	mobjinfo[thing] = mobjinfo[MT_DISS];
	mobjinfo[thing].doomednum = 6001;
	mobjinfo[thing].spawnstate = S_BARREL;
	mobjinfo[thing].radius = 25*FRACUNIT;
	mobjinfo[thing].height = 71*FRACUNIT;
	mobjinfo[thing].flags = MF_SLIDEME|MF_SOLID|MF_PUSHABLE;

	thing = S_SUITEM;
	states[thing] = states[S_DISS];
	states[thing].sprite = SPR_UITM;
	states[thing].tics = -1;

	thing = S_SUMONEY;
	states[thing] = states[S_SUITEM];
	states[thing].sprite = SPR_UMON;

	thing = S_TELESPARK1;
	states[thing] = states[S_DISS];
	states[thing].sprite = SPR_UPIP;
	states[thing].tics = -1;

	thing = S_TELESPARK2;
	states[thing] = states[S_TELESPARK1];
	states[thing].frame = 1;

	thing = S_KINESO;
	states[thing] = states[S_DISS];
	states[thing].sprite = SPR_KINE;
	states[thing].tics = -1;

	thing = S_BARREL;
	states[thing] = states[S_DISS];
	states[thing].sprite = SPR_BARL;
	states[thing].tics = -1;

	thing = SPR_UITM;
	strcpy(sprnames[thing],"UITM");
#ifdef HWRENDER
	t_lspr[thing] = &lspr[NOLIGHT];
#endif

	thing = SPR_UMON;
	strcpy(sprnames[thing],"UMON");
#ifdef HWRENDER
	t_lspr[thing] = &lspr[NOLIGHT];
#endif

	thing = SPR_UPIP;
	strcpy(sprnames[thing],"UPIP");
#ifdef HWRENDER
	t_lspr[thing] = &lspr[JETLIGHT_L];
#endif

	thing = SPR_KINE;
	strcpy(sprnames[thing],"KINE");
#ifdef HWRENDER
	t_lspr[thing] = &lspr[NOLIGHT];
#endif

	thing = SPR_BARL;
	strcpy(sprnames[thing],"BARL");
#ifdef HWRENDER
	t_lspr[thing] = &lspr[NOLIGHT];
#endif

	thing = mus_sulvl;
	S_music[thing] = S_music[mus_None];
	S_music[thing].name = "sulvl";
}

#endif
