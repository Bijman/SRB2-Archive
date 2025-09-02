#include "doomstat.h"
#include "d_think.h"
#include "G_game.h"
#include "Hu_stuff.h"
#include "hardware/hw_light.h"
#include "Info.h"
#include "M_menu.h"
#include "p_local.h"
#include "R_things.h"
#include "s_sound.h"
#include "sounds.h"
#ifdef SPMOVES
#include "spmoves.h"
#endif
#include "z_zone.h"

#ifdef SPMOVES
extern consvar_t cv_homing;
extern consvar_t cv_lightdash;
extern consvar_t cv_objectplace;
extern consvar_t cv_timeattacked;
extern struct menu_s menu_t;

// PSO items! Yay!
psoitem_t psoitems[NUMPSOITEMS] = {
{"emerald",PSOI_SPECIAL,-1,-1,0,0,0,0,sfx_None,0},

{"saber",PSOI_SABER,-1,-1,0,0,0,0,sfx_None,0},
{"brand",PSOI_SABER,-1,-1,0,0,0,0,sfx_None,0},
{"buster",PSOI_SABER,-1,-1,0,0,0,0,sfx_None,0},
{"pallasch",PSOI_SABER,-1,-1,0,0,0,0,sfx_None,0},
{"gladius",PSOI_SABER,-1,-1,0,0,0,0,sfx_None,0},

{"sword",PSOI_SWORD,-1,-1,0,0,0,0,sfx_None,0},
{"gigush",PSOI_SWORD,-1,-1,0,0,0,0,sfx_None,0},
{"breaker",PSOI_SWORD,-1,-1,0,0,0,0,sfx_None,0},
{"claymore",PSOI_SWORD,-1,-1,0,0,0,0,sfx_None,0},
{"calibur",PSOI_SWORD,-1,-1,0,0,0,0,sfx_None,0},

{"dagger",PSOI_DAGGER,-1,-1,0,0,0,0,sfx_None,0},
{"knife",PSOI_DAGGER,-1,-1,0,0,0,0,sfx_None,0},
{"blade",PSOI_DAGGER,-1,-1,0,0,0,0,sfx_None,0},
{"edge",PSOI_DAGGER,-1,-1,0,0,0,0,sfx_None,0},
{"ripper",PSOI_DAGGER,-1,-1,0,0,0,0,sfx_None,0},

{"partisan",PSOI_PARTISAN,-1,-1,0,0,0,0,sfx_None,0},
{"halibert",PSOI_PARTISAN,-1,-1,0,0,0,0,sfx_None,0},
{"glaive",PSOI_PARTISAN,-1,-1,0,0,0,0,sfx_None,0},
{"berdys",PSOI_PARTISAN,-1,-1,0,0,0,0,sfx_None,0},
{"gungnir",PSOI_PARTISAN,-1,-1,0,0,0,0,sfx_None,0},

{"slicer",PSOI_SLICER,-1,-1,0,0,0,0,sfx_None,0},
{"spinner",PSOI_SLICER,-1,-1,0,0,0,0,sfx_None,0},
{"cutter",PSOI_SLICER,-1,-1,0,0,0,0,sfx_None,0},
{"sawcer",PSOI_SLICER,-1,-1,0,0,0,0,sfx_None,0},
{"diska",PSOI_SLICER,-1,-1,0,0,0,0,sfx_None,0},

{"handgun",PSOI_HANDGUN,MT_PSOSHOT,-1,1,10,3,0,sfx_None,0},
{"autogun",PSOI_HANDGUN,MT_PSOSHOT,-1,3,15,6,0,sfx_None,0},
{"lockgun",PSOI_HANDGUN,MT_PSOSHOT,-1,5,25,12,0,sfx_None,0},
{"railgun",PSOI_HANDGUN,MT_PSOSHOT,-1,7,40,24,0,sfx_None,0},

{"rifle",PSOI_RIFLE,MT_PSOSHOT,-1,2,0,0,0,sfx_None,0},
{"sniper",PSOI_RIFLE,MT_PSOSHOT,-1,4,0,0,0,sfx_None,0},
{"blaster",PSOI_RIFLE,MT_PSOSHOT,-1,6,0,0,0,sfx_None,0},
{"beam",PSOI_RIFLE,MT_PSOSHOT,-1,8,0,0,0,sfx_None,0},
{"laser",PSOI_RIFLE,MT_PSOSHOT,-1,10,0,0,0,sfx_None,0},

{"mechgun",PSOI_MECHGUN,-1,-1,0,0,0,0,sfx_None,0},
{"assault",PSOI_MECHGUN,-1,-1,0,0,0,0,sfx_None,0},
{"repeater",PSOI_MECHGUN,-1,-1,0,0,0,0,sfx_None,0},
{"gatling",PSOI_MECHGUN,-1,-1,0,0,0,0,sfx_None,0},
{"vulcan",PSOI_MECHGUN,-1,-1,0,0,0,0,sfx_None,0},

{"shot",PSOI_SHOT,-1,-1,0,0,0,0,sfx_None,0},
{"spread",PSOI_SHOT,-1,-1,0,0,0,0,sfx_None,0},
{"cannon",PSOI_SHOT,-1,-1,0,0,0,0,sfx_None,0},
{"launcher",PSOI_SHOT,-1,-1,0,0,0,0,sfx_None,0},
{"arms",PSOI_SHOT,-1,-1,0,0,0,0,sfx_None,0},

{"cane",PSOI_CANE,-1,-1,0,0,0,0,sfx_None,0},
{"stick",PSOI_CANE,-1,-1,0,0,0,0,sfx_None,0},
{"mace",PSOI_CANE,-1,-1,0,0,0,0,sfx_None,0},
{"club",PSOI_CANE,-1,-1,0,0,0,0,sfx_None,0},

{"rod",PSOI_ROD,-1,-1,0,0,0,0,sfx_None,0},
{"pole",PSOI_ROD,-1,-1,0,0,0,0,sfx_None,0},
{"pillar",PSOI_ROD,-1,-1,0,0,0,0,sfx_None,0},
{"striker",PSOI_ROD,-1,-1,0,0,0,0,sfx_None,0},

{"wand",PSOI_WAND,-1,-1,0,0,0,0,sfx_None,0},
{"staff",PSOI_WAND,-1,-1,0,0,0,0,sfx_None,0},
{"baton",PSOI_WAND,-1,-1,0,0,0,0,sfx_None,0},
{"scepter",PSOI_WAND,-1,-1,0,0,0,0,sfx_None,0},

{"photon claw",PSOI_CLAW,-1,-1,0,0,0,0,sfx_None,0},
{"silence claw",PSOI_CLAW,-1,-1,0,0,0,0,sfx_None,0},
{"nei's claw",PSOI_CLAW,-1,-1,0,0,0,0,sfx_None,0},

{"brave knuckle",PSOI_KNUCKLE,-1,-1,0,0,0,0,sfx_None,0},
{"angry fist",PSOI_KNUCKLE,-1,-1,0,0,0,0,sfx_None,0},
{"god hand",PSOI_KNUCKLE,-1,-1,0,0,0,0,sfx_None,0},
{"sonic knuckle",PSOI_KNUCKLE,-1,-1,0,0,0,0,sfx_None,0},

{"red saber",PSOI_SABER,-1,-1,0,0,0,0,sfx_None,0},
{"red sword",PSOI_SWORD,-1,-1,0,0,0,0,sfx_None,0},
{"red dagger",PSOI_DAGGER,-1,-1,0,0,0,0,sfx_None,0},
{"red partisan",PSOI_PARTISAN,-1,-1,0,0,0,0,sfx_None,0},
{"red slicer",PSOI_SLICER,-1,-1,0,0,0,0,sfx_None,0},
{"red handgun",PSOI_HANDGUN,MT_PSOSHOT,-1,13,0,0,0,sfx_None,0},
{"red mechgun",PSOI_MECHGUN,-1,-1,0,0,0,0,sfx_None,0},

{"double saber",PSOI_DOUBLESABER,-1,-1,0,0,0,0,sfx_None,0},
{"twin brand",PSOI_DOUBLESABER,-1,-1,0,0,0,0,sfx_None,0},
{"spread needle",PSOI_SPREAD,-1,-1,0,0,0,0,sfx_None,0},
{"holy ray",PSOI_HANDGUN,MT_PSOSHOT,-1,0,0,0,0,sfx_None,0},
{"psychogun",PSOI_HANDGUN,MT_PSOSHOT,-1,0,0,0,0,sfx_None,0},
{"heaven punisher",PSOI_HANDGUN,MT_PSOSHOT,-1,0,0,0,0,sfx_None,0},

{"akiko's frying pan",PSOI_SABER,-1,-1,0,0,0,0,sfx_None,0},
{"eggblaster",PSOI_HANDGUN,MT_PSOSHOT,-1,0,0,0,0,sfx_None,0},
{"ancient saber",PSOI_SABER,-1,-1,0,0,0,0,sfx_None,0},
{"battle fan",PSOI_SABER,-1,-1,0,0,0,0,sfx_None,0},
{"toy hammer",PSOI_SABER,-1,-1,0,0,0,0,sfx_None,0},
{"crazy tune",PSOI_SABER,-1,-1,0,0,0,0,sfx_None,0},
{"rocketpunch",PSOI_PUNCH,-1,-1,0,0,0,0,sfx_None,0},
{"samba maracas",PSOI_DOUBLESABER,-1,-1,0,0,0,0,sfx_None,0},
{"drill launcher",PSOI_PUNCH,-1,-1,0,0,0,0,sfx_None,0},
{"striker of chao",PSOI_SWORD,-1,-1,0,0,0,0,sfx_None,0},
{"master beam",PSOI_RIFLE,MT_PSOSHOT,-1,0,0,0,0,sfx_None,0},
{"game magazine",PSOI_SABER,-1,-1,0,0,0,0,sfx_None,0},
{"flower bouquet",PSOI_SABER,-1,-1,0,0,0,0,sfx_None,0},

{"jte's blades",PSOI_DAGGER,-1,-1,0,0,0,0,sfx_None,0},
{"jte's railgun",PSOI_RIFLE,MT_REDRING,-1,8,0,0,1,sfx_None,1},

{"barrier",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"shield",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"core shield",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"giga shield",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"soul barrier",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"hard shield",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"brave barrier",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"solid shield",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"flame barrier",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"plasma barrier",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"freeze barrier",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"psychic barrier",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"general shield",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"protect barrier",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"glorious shield",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"imperial barrier",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"guardian sword",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"divinity barrier",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"ultimate shield",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"spiritual shield",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"celestial shield",PSOI_BARRIER,-1,-1,0,0,0,0,sfx_None,0},

{"invisible guard",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"sacred guard",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},

{"s-parts ver1.16",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"s-parts ver2.01",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},

{"force wall",PSOI_WALL,-1,-1,0,0,0,0,sfx_None,0},
{"ranger wall",PSOI_WALL,-1,-1,0,0,0,0,sfx_None,0},
{"hunter wall",PSOI_WALL,-1,-1,0,0,0,0,sfx_None,0},
{"attribute wall",PSOI_WALL,-1,-1,0,0,0,0,sfx_None,0},

{"secret gear",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"combat gear",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"proto regene gear",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"regenerate gear",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"regene gear adv.",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},

{"custom barrier ver00",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},

{"tripolic shield",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"standstill shield",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"safty heart",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"kasami bracer",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},

{"god's shield SUZAKU",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"god's shield GENBU",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"god's shield BYAKKO",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"god's shield SEIRYU",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},

{"red ring",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"blue ring",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"yellow ring",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"purple ring",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"green ring",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"black ring",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},
{"white ring",PSOI_SPBARRIER,-1,-1,0,0,0,0,sfx_None,0},

{"frame",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"armor",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"psy armor",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"giga frame",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"soul frame",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"cross armor",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"solid frame",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"brave armor",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"hyper frame",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"grand armor",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"shock frame",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"king's frame",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"dragon frame",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"absorb armor",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"protect frame",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"general armor",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"perfect frame",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"valiant frame",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"imperial armor",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"holiness armor",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"guardian armor",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"divinity armor",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"ultimate frame",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},
{"celestial armor",PSOI_FRAME,-1,-1,0,0,0,0,sfx_None,0},

{"hunter field",PSOI_FIELD,-1,-1,0,0,0,0,sfx_None,0},
{"ranger field",PSOI_FIELD,-1,-1,0,0,0,0,sfx_None,0},
{"force field",PSOI_FIELD,-1,-1,0,0,0,0,sfx_None,0},

{"revival garment",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"spirit garment",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"stink frame",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"sense plate",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"graviton plate",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"attribute plate",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"custom frame v00",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"guard wave",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},

{"luminous field",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"chu chu fever",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"love heart",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"flame garment",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"virus armor: lafuteria",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"brightness circle",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"aura field",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"electro frame",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"sacred cloth",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},
{"smoking plate",PSOI_SPFRAME,-1,-1,0,0,0,0,sfx_None,0},

{"omega's atk-dagger",PSOI_DAGGER,-1,-1,80,0,0,0,sfx_None,PSOF_QUESTITEM},

};

// JTE_ChaoAttack
//
// Used by Chao for Chao attacks!
//
boolean JTE_ChaoAttack(mobj_t *mobj)
{
        int count;
        mobj_t *mo;
        thinker_t *think;
		mobj_t* closestmo = NULL;
		angle_t an;

        count = 0;
        for(think = thinkercap.next; think != &thinkercap; think = think->next)
        {
                if(think->function.acp1 != (actionf_p1)P_MobjThinker)
                { // Not a mobj thinker
                        continue;
                }

                mo = (mobj_t *)think;
                if(!(mo->flags&MF_COUNTKILL))
                { // Not a valid monster
                        continue;
                }

		if(mo->health <= 0)
			continue;

		if(mo == mobj)
			continue;

		if(mo->flags2 & MF2_FRET)
			continue;

		if(mo->type == MT_DETON)
			continue;

                if(P_AproxDistance(P_AproxDistance(mobj->x-mo->x, mobj->y-mo->y), mobj->z-mo->z)
                        > RING_DIST*2)
                { // Out of range
                        continue;
                }

				if(mo->type == MT_PLAYER) // Don't chase after other players!
					continue;

				if(closestmo && P_AproxDistance(P_AproxDistance(mobj->x-mo->x, mobj->y-mo->y), mobj->z-mo->z)
					> P_AproxDistance(P_AproxDistance(mobj->x-closestmo->x, mobj->y-closestmo->y), mobj->z-closestmo->z))
					continue;

				an = R_PointToAngle2 (mobj->x,
									  mobj->y,
									  mo->x,
									  mo->y) - mobj->angle;

				if (an > ANG90 && an < ANG270)
					continue;   // behind back

				mobj->angle = R_PointToAngle2(mobj->x, mobj->y, mo->x, mo->y);

                if(!P_CheckSight(mobj, mo))
                { // Out of sight
                        continue;
                }

				closestmo = mo;
        }

		if(closestmo)
		{
        // Found a target monster
		mobj->target = closestmo;
		mobj->flags2 |= MF2_CHAOATTACK;
            return(true);
		}
        return(false);
}

// JTE_CreateItem
//
// Teleports an item in from a linedef executor's front sector using the ceiling for the height!
//
void JTE_CreateItem (sector_t*  sec, sector_t* destsec)
{
    mobj_t   *thing;
    msecnode_t* node;

    if (sec->floordata || sec->ceilingdata) //jff 2/22/98
      return;

	node = NULL;
	thing = NULL;

	node = sec->touching_thinglist; // things touching this sector

	if(node)
	{
		thing = node->m_thing;
		if(thing)
		{
		  P_UnsetThingPosition(thing);
		  thing->x = destsec->soundorg.x;
		  thing->y = destsec->soundorg.y;
		  thing->z = sec->ceilingheight;
		  P_SetThingPosition(thing);
		  thing->floorz = destsec->floorheight;
		  thing->ceilingz = destsec->ceilingheight;
		  thing->momx = thing->momy = thing->momz = 0;
		}
	}
}

//
// JTE_ReadAction
//
// Should be able to create new enemy actions via a simple language.
//
#ifdef SPMBETA
void JTE_ReadAction(MYFILE *f, int num)
{
  char s[MAXLINELEN];
  char *command,*word1,*word2,*word3;
  char* p;
  int i,read=0;
  actionf_p1 action;

  do{
    if(myfgets(s,sizeof(s),f))
    {
      if(s[0]=='\n') break;

      p = strchr(s, '\n');
      if(p) *p = '\0';
      command = strupr(s);

      word1=strupr(strtok(s," "));
      word2=strupr(strtok(NULL," "));
      word3=strupr(strtok(NULL," "));

	  if(!strcmp(command, "LOOK")) action = A_Look;
	  else if(!strcmp(command, "CHASE")) action = A_Chase;
	  else if(!strcmp(command, "FACE TARGET")) action = A_FaceTarget;
	  else if(!strcmp(command, "SHOOT")) action = A_JetgShoot;
	  //else if(!strcmp(word2, "IF") && !strcmp(word3, "MELEE")) action = A_IfMelee;
      else CONS_Printf("Custom Action %d: unknown word '%s' (line %d)\n",num,command,read);
	  actions[num][read] = action;
      read++;
    }
  } while(s[0]!='\n' && !myfeof(f));
}
#endif

int multieggs;
emblem_t multiegglocations[NUMMULTIEGGS] = {
	//   X      Y     Z  Color  Num Map
	{ 1840,   300,  752, 15,     1, 136},
	{ 1153, -1537,  280,  1,     2, 137},
	{   80,  1090,  616,  8,     4, 138},
	{-5300, -4130,  500, 12,     8, 139},
	{    0,     0,    0,  7,    16, 140},
	{ 2046,  2128,  748, 10,    32, 90},
	{-2367, -1567, -128,  5,    64, 91},
	{-1663,  -247,  234, 14,   128, 92},
	{  256,   256, 1048,  8,   256, 93},
	{  964,  3264,  512,  4,   512, 94},
	{    0,  5864, -320,  5,  1024, 95},
	{  224, -1776, 3712,  6,  2048, 96},
	{ 2255,  1214, 1110,  8,  4096, 97},
	{  140, -3378, 1576, 12,  8192, 98},
	{ -880,  1135,  640, 12, 16384, 99}
};

int jtetokens;
emblem_t jtetokenlocations[NUMJTETOKENS] = {
	//   X      Y     Z  Color  Num Map
	{-2208,  2336,  -33,    1,   1, 1},
	{-3776,  1344,    0,    2,   2, 2},
	{ 1184,   992, 2048,    3,   4, 3}, // To be placed.
	{ 4224, -5888, 2144,    4,   8, 4},
	{    0,     0,    0,    5,  16, 5}, // To be placed.
	{ 2048,  5056,   24,    6,  32, 6},
	{-6400,  2752,    0,    7,  64, 7}, // To be placed.
	{  224,  6496,    0,    8, 128, 8}, // To be placed.
	{  -64,   -64,    0,    9, 256, 9},
};

//
// JTE_PlayerSPMove
//
// Checks for player double jump abilitys.
//
void JTE_PlayerSPMove(player_t *player)
{
    ticcmd_t* cmd = &player->cmd;
    switch (player->charability)
    {
					case 3:
						// Now it's Chaos Control time!
						if (player->mfjumped)
						{
							if(!player->thokked)
							{
								if (cmd->buttons & BT_USE && !player->powers[pw_super] && ((cv_gametype.value == GT_COOP && emeralds & EMERALD1) || (!(cv_gametype.value == GT_COOP) && player->health > 10)))  // Player has at least 10 rings or an emerald
								{
									// Chaos Control thok!
									P_InstaThrust (player->mo, player->mo->angle, MAXMOVE); // Catapult the player across the screen!
									S_StartSound (player->mo, sfx_zoom); // Play the ZOOM sound
									player->bonuscount += 10; // Flash the palette

									// Now check the player's color so the right THOK object is displayed.
									if(player->skincolor != 0)
									{
										mobj_t* mobj;
										mobj = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z	- (player->mo->info->height - player->mo->height)/3, player->mo->info->painchance);
										mobj->flags = (mobj->flags & ~MF_TRANSLATION) | ((player->skincolor)<<MF_TRANSSHIFT); // Tails 08-20-2002
									}
									player->mo->flags2 |= MF2_DONTDRAW; // I am invisible!
									player->mo->flags |= MF_NOCLIP; // No clip through walls and things
									if(!(cv_gametype.value == GT_COOP) && (player->health > 10))
									{
										player->health -= 10; // Take your payment for the god move ;P
										player->mo->health -= 10; // Same as above, I guess
									}
								}
								else if(player->superready && !player->powers[pw_super] && !(cmd->buttons & BT_USE) && (player->skin == 0 || (player->skin == 5 && !M_CheckParm("-noshadow")))) // If you can turn into Super
								{								 // and aren't trying to chaos control, do it!
									if(!(player->powers[pw_yellowshield] || player->powers[pw_blackshield] || player->powers[pw_greenshield] || player->powers[pw_redshield] || player->powers[pw_blueshield])) // But don't turn super with a sheild!
									{
										player->powers[pw_super] = true;
										if(player==&players[consoleplayer])
										{
											S_StopMusic();
											S_ChangeMusic(mus_supers, true);
										}
										S_StartSound(player->mo, sfx_supert);
										player->mo->tracer = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, MT_SUPERTRANS);
										player->mo->flags2 |= MF2_DONTDRAW;
										player->mo->momx = player->mo->momy = player->mo->momz = 0;
										player->mfjumped = 0;
										P_SetMobjState(player->mo, S_PLAY_RUN1);
									}
								}
								else if (!(cmd->buttons & BT_USE) || (cv_gametype.value == GT_COOP && !(emeralds & EMERALD1)) || (!(cv_gametype.value == GT_COOP) && !(player->health > 10))) //If you don't want it or don't have the stuff you need for it, I'll just give you a normal thok instead.
								{
									// Normal thok
									P_InstaThrust (player->mo, player->mo->angle, MAXMOVE); // Catapult the player
									S_StartSound (player->mo, player->mo->info->attacksound); // Play the THOK sound

									// Now check the player's color so the right THOK object is displayed.
									if(player->skincolor != 0)
									{
										mobj_t* mobj;
										mobj = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z	- (player->mo->info->height - player->mo->height)/3, player->mo->info->painchance);
										mobj->flags = (mobj->flags & ~MF_TRANSLATION) | ((player->skincolor)<<MF_TRANSSHIFT); // Tails 08-20-2002
									}

									//Only homing with normal thok; don't homing attack with a chaos control thok, darn it!
									if(cv_homing.value && !player->homing && player->mfjumped)
									{
										if(P_LookForEnemies(player))
											if(player->mo->tracer)
												player->homing = 3*TICRATE;
									}
								}
								player->thokked = true;
							}
						}
						break;

					case 4:
						// Supa Thok! Homing attack and black BOOM!
						if (player->mfjumped)
						{
							if(!player->thokked)
							{
								P_InstaThrust (player->mo, player->mo->angle, MAXMOVE); // Catapult the player
								S_StartSound (player->mo, player->mo->info->attacksound); // Play the THOK sound
										
								// Now check the player's color so the right THOK object is displayed.
								if(player->skincolor != 0)
								{
									mobj_t* mobj;
									mobj = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z	- (player->mo->info->height - player->mo->height)/3, player->mo->info->painchance);
									mobj->flags = (mobj->flags & ~MF_TRANSLATION) | ((player->skincolor)<<MF_TRANSSHIFT); // Tails 08-20-2002
								}

								if(P_LookForEnemies(player))
									if(player->mo->tracer)
										player->homing = 3*TICRATE;

								if((emeralds & EMERALD3) && (cmd->buttons & BT_USE))
									player->blackow = 1;

								player->thokked = true;
							}
						}
						break;

					case 5:
						// Now it's time for Multi-Thok. This is the only ability you get, and if you try to homing attack or lightdash or anything else that sonic can do, too bad.
						if (player->mfjumped)
						{
							P_InstaThrust (player->mo, player->mo->angle, MAXMOVE); // Catapult the player
							S_StartSound (player->mo, player->mo->info->attacksound); // Play the THOK sound

							// Now check the player's color so the right THOK object is displayed.
							if(player->skincolor != 0)
							{
								mobj_t* mobj;
								mobj = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z	- (player->mo->info->height - player->mo->height)/3, player->mo->info->painchance);
								mobj->flags = (mobj->flags & ~MF_TRANSLATION) | ((player->skincolor)<<MF_TRANSSHIFT); // Tails 08-20-2002
							}
							if(player->thokked && player->health > 0)
							{
								player->health--;
								player->mo->health--;
							}
							else
								S_StartSound (player->mo, sfx_zoom);
							player->thokked = true;
						}
						break;

					case 6:
						// Lets Glide-Blast or Thok-Glide or whatever you want to call it!
						if (player->mfjumped)
						{
							if(!player->thokked)
								S_StartSound (player->mo, player->mo->info->attacksound); // Play the THOK sound
							player->gliding = 1;
							player->glidetime = 0;
							P_SetMobjState(player->mo, S_PLAY_ABL1);
							player->mfspinning = 0;
							player->mfstartdash = 0;
						}
						break;

					case 7:
						// fox's shooting ability -- Homing attack in SA mode
						if(cv_homing.value && !player->homing && player->mfjumped)
						{
							if(P_LookForEnemies(player))
								if(player->mo->tracer)
								{
									P_InstaThrust (player->mo, player->mo->angle, MAXMOVE); // Catapult the player
									S_StartSound (player->mo, player->mo->info->attacksound); // Play the THOK sound
										
									// Now check the player's color so the right THOK object is displayed.
									if(player->skincolor != 0)
									{
										mobj_t* mobj;
										mobj = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z	- (player->mo->info->height - player->mo->height)/3, player->mo->info->painchance);
										mobj->flags = (mobj->flags & ~MF_TRANSLATION) | ((player->skincolor)<<MF_TRANSSHIFT); // Tails 08-20-2002
									}

									player->homing = 3*TICRATE;
								}
						}
						break;

					case 8:
						// Cyan's climb-but-don't-glide ability 
						break;

					case 9:
						// Salena's flying ability
						if(!player->powers[pw_tailsfly] && player->mfjumped)
						{
							P_SetMobjState (player->mo, S_PLAY_ABL1); // Change to the flying animation
							player->powers[pw_tailsfly] = tailsflytics * 2 + 1;
							player->mfjumped = player->mfspinning = player->mfstartdash = 0;
						}
						// If currently flying, give an ascend boost.
						/*else if (player->powers[pw_tailsfly])
						{
							if(player->fly1 == 0 && player->mo->health > 1)
							{
								player->fly1 = 80;
								player->mo->health--;
								player->health--;
							}
							else
								player->fly1 = 4;
						}*/
						break;
					case 10:
						// RPG-style leveling up Sonic abilitys.
						if (player->rpLevel < 3)
							break;
						else if (player->rpLevel >= 3 && player->rpLevel < 5) // Mini-Thok
						{
							if(!player->thokked && player->mfjumped)
							{
								P_InstaThrust (player->mo, player->mo->angle, MAXMOVE/2); // Catapult the player
								S_StartSound (player->mo, player->mo->info->attacksound); // Play the THOK sound

								// Now check the player's color so the right THOK object is displayed.
								if(player->skincolor != 0)
								{
									mobj_t* mobj;
									mobj = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z	- (player->mo->info->height - player->mo->height)/3, player->mo->info->painchance);
									mobj->flags = (mobj->flags & ~MF_TRANSLATION) | ((player->skincolor)<<MF_TRANSSHIFT); // Tails 08-20-2002
								}

								player->thokked = true;
							}
						}
						else if (player->rpLevel >= 5 && player->rpLevel < 8) // Thok
						{
							if(!player->thokked && player->mfjumped)
							{
									P_InstaThrust (player->mo, player->mo->angle, MAXMOVE); // Catapult the player
									S_StartSound (player->mo, player->mo->info->attacksound); // Play the THOK sound

									// Now check the player's color so the right THOK object is displayed.
									if(player->skincolor != 0)
									{
										mobj_t* mobj;
										mobj = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z	- (player->mo->info->height - player->mo->height)/3, player->mo->info->painchance);
										mobj->flags = (mobj->flags & ~MF_TRANSLATION) | ((player->skincolor)<<MF_TRANSSHIFT); // Tails 08-20-2002
									}

									if(player->rpLevel >= 6 && !player->homing && cmd->buttons & BT_USE) // Homing attack
									{
										if(P_LookForEnemies(player))
											if(player->mo->tracer)
												player->homing = 3*TICRATE;
									}
									player->thokked = true;
							}
						}
						else if (player->rpLevel >= 8) // Multi-Thok
						{
							if(player->mfjumped)
							{
								if(player->thokked == true && player->mo->health > 1)
								{
									player->mo->health--;
									player->health--;
								}
								player->thokked = true;
								P_InstaThrust (player->mo, player->mo->angle, MAXMOVE); // Catapult the player
								S_StartSound (player->mo, player->mo->info->attacksound); // Play the THOK sound

								// Now check the player's color so the right THOK object is displayed.
								if(player->skincolor != 0)
								{
									mobj_t* mobj;
									mobj = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z	- (player->mo->info->height - player->mo->height)/3, player->mo->info->painchance);
									mobj->flags = (mobj->flags & ~MF_TRANSLATION) | ((player->skincolor)<<MF_TRANSSHIFT); // Tails 08-20-2002
								}
							}

						}
						break;
					case 11:
						// RPG-style leveling up Tails abilitys.
						if (player->rpLevel < 3)
							break;
						//else if (player->rpLevel >= 3 && player->rpLevel < 5) // Mini-Fly
						//else if (player->rpLevel >= 5 && player->rpLevel < 8) // Fly
						//else if (player->rpLevel >= 8) // Salena Fly
						break;
					case 12:
						// RPG-style leveling up Knux abilitys.
						if (player->rpLevel < 3)
							break;
						else if (player->rpLevel >= 3 && player->rpLevel < 5) // Climb
							break;
						else if (player->rpLevel >= 5 && player->rpLevel < 8 && player->mfjumped) // Glide n Climb
						{
							player->gliding = 1;
							player->glidetime = 0;
							P_SetMobjState(player->mo, S_PLAY_ABL1);
							P_InstaThrust(player->mo, player->mo->angle, 30*FRACUNIT/NEWTICRATERATIO);
							player->mfspinning = player->mfstartdash = 0;
						}
						else if (player->rpLevel >= 8 && player->mfjumped) // Glide Blast
						{
							player->gliding = player->thokked = 1;
							player->glidetime = 0;
							P_SetMobjState(player->mo, S_PLAY_ABL1);
							P_InstaThrust(player->mo, player->mo->angle, 30*FRACUNIT/NEWTICRATERATIO);
							player->mfspinning = player->mfstartdash = 0;
						}
						break;
					case 13: // Teh floating ability! Fly around n stuff!
						break;

					case 14:
						// You have a chao! Use it!
						if(mapheaderinfo[gamemap-1].typeoflevel & TOL_CHATROOM)
							break;
						if(player->mfjumped)
							if(player->chao && !(player->chao->flags2 & MF2_CHAOATTACK))
								JTE_ChaoAttack(player->chao);
						break;

					case 15: // Mighty's wall kick ability!
					    break;

					case 16: // Mecha Knuckles' Jet-glide
						if (player->mfjumped)
						{
							player->gliding = 1;
							player->glidetime = 0;
							P_SetMobjState(player->mo, S_PLAY_ABL1);
							P_InstaThrust(player->mo, player->mo->angle, 30*FRACUNIT/NEWTICRATERATIO);
							player->mfspinning = 0;
							player->mfstartdash = 0;

							if(player->skin == spm_mechaknux)
							{
								mobj_t* mobj;
								fixed_t jetx, jety;

								jetx = player->mo->x + P_ReturnThrustX(player->mo, player->mo->angle, -56*FRACUNIT);
								jety = player->mo->y + P_ReturnThrustY(player->mo, player->mo->angle, -56*FRACUNIT);

								mobj = P_SpawnMobj(jetx, jety, player->mo->z + 8*FRACUNIT, MT_JETFUME1);
								mobj->target = player->mo;
								mobj->fuse = 56;
							}
						}
						break;

					case 17: // Bat flying!
						if(player->mfjumped)
						{
							player->mo->momz *= (player->jumpfactor/130.0); // Custom height
							S_StartSound(player->mo, sfx_stnmov);
						}
						break;

					default:
					    break;
    }
    return;
}

//
// JTE_PlayerLevelUp
//
// Adds to a players "experience" and calculates "levels"
// For the RPG System SPMoves as well as PSO maps.
//
JTE_PlayerLevelUp(player_t *player, int amount)
{
	if(player->charability >= 10 && player->charability <= 12)
		player->rpScore += amount;
	if(player->rpScore >= (10000 * player->rpLevel + 1) * .05)
	{
			    player->rpScore -= (10000 * player->rpLevel + 1) * .05;
			    player->rpLevel++;
			    if(player == &players[consoleplayer])
			    {
				CONS_Printf("Level up! New Level: %d\n",player->rpLevel);
				if(player->charability == 10)
					switch(player->rpLevel)
					{
					    #ifdef SPMBETA
						case 1:
							CONS_Printf("New ability: Spindash\n");
							break;
						case 2:
							CONS_Printf("New ability: Full Speed\n");
							break;
						#endif
						case 3:
							CONS_Printf("New ability: Mini-Thok\n");
							break;
						#ifdef SPMBETA
						case 4:
							CONS_Printf("New ability: Full Jumpheight\n");
							break;
						#endif
						case 5:
							CONS_Printf("New ability: Speed Thok\n");
							break;
						case 6:
							CONS_Printf("New ability: Homing Attack (Use by holding SPIN when you Thok)\n");
							break;
						case 7:
							CONS_Printf("New ability: Light Dash\n");
							break;
						case 8:
							CONS_Printf("New ability: Multi-Thok\n");
							break;
						case 9:
							CONS_Printf("New ability: Black boom with any shield\n");
							break;
						default:
							CONS_Printf("No new abilitys.\n");
					}
			    }
			    else
				CONS_Printf("%s is now level: %d\n",player_names[player-players],player->rpLevel);
			    S_StartSound(player->mo, sfx_shield);
			    P_ResetPlayer(player);
	}
	if(mapheaderinfo[gamemap-1].typeoflevel & TOL_PSO)
		player->psoscore += amount;
	if(player->psoscore >= (10000 * player->psolevel) * .08 && mapheaderinfo[gamemap-1].typeoflevel & TOL_PSO)
	{
			    player->psoscore -= (10000 * player->psolevel) * .08;
			    player->psolevel++;
			    if(player == &players[consoleplayer])
			           CONS_Printf("* Level up! New Level: %d\n",player->psolevel);
			    else
		    	       CONS_Printf("* Level up! %s is now level: %d\n",player_names[player-players],player->psolevel);
			    S_StartSound(player->mo, sfx_psolvl);
			    player->health = player->mo->health = 9999;
			    player->psomagic = 9999;
			    //P_ResetPlayer(player);
    }
}

//
// JTE_EnemyDropStuff
//
// Spawns items from a dead enemy. Returns true if an item was spawned, false if the game should spawn an animal or something instead
//
boolean JTE_EnemyDropStuff(mobj_t* target)
{
    mobj_t* mo;
    if (mapheaderinfo[gamemap-1].typeoflevel & TOL_PSO && target->flags & MF_ENEMY)
    {
    	mo = P_SpawnMobj (target->x,target->y,target->floorz, MT_PSODROP);
   		mo->fuse = TICRATE/3;
   		return true;
    }
    else if (mapheaderinfo[gamemap-1].typeoflevel & TOL_PSO && target->flags & MF_BOSS)
    {
    	mo = P_SpawnMobj (target->x, target->y, target->floorz, MT_PSOBOSSDROP);
    	mo->fuse = TICRATE/3;
    	return true;
   	}
   	return false;
}

//
// JTE_PlayerGameplayStuff
//
// Does a bunch of calculations for players, some of which involve abilitys, others are part of map types.
//
void JTE_PlayerGameplayStuff(player_t *player)
{
    ticcmd_t* cmd = &player->cmd;

    mobj_t *mo;
    thinker_t *think;
    boolean found = false;

	// Some stupid gameplay hacks to make forced skins and colors work correctly ;P
	if(player->skin != mapheaderinfo[gamemap-1].forcecharacter && mapheaderinfo[gamemap-1].forcecharacter != 255 && !nomonsters)
	{
		char skincmd[33];
		if(cv_splitscreen.value)
		{
			sprintf(skincmd, "skin2 %s\n", skins[mapheaderinfo[gamemap-1].forcecharacter].name);
			CV_Set(&cv_skin2, skins[mapheaderinfo[gamemap-1].forcecharacter].name);
		}

		sprintf(skincmd, "skin %s\n", skins[mapheaderinfo[gamemap-1].forcecharacter].name);

		COM_BufAddText(skincmd);

		if(!netgame)
		{
			if(cv_splitscreen.value)
				SetPlayerSkinByNum(secondarydisplayplayer, mapheaderinfo[gamemap-1].forcecharacter);

			SetPlayerSkinByNum(consoleplayer, mapheaderinfo[gamemap-1].forcecharacter);
		}
	}

	if(player->skincolor != player->prefcolor && player == &players[consoleplayer] && !multiplayer && !netgame && !nomonsters)
	{
		player->skincolor = player->prefcolor;
		CV_SetValue(&cv_playercolor, player->prefcolor);
		player->mo->flags = (player->mo->flags & ~MF_TRANSLATION) | ((player->prefcolor)<<MF_TRANSSHIFT);
	}

	if(player->charability != 14 && mapheaderinfo[gamemap-1].typeoflevel & TOL_CHATROOM)
		player->charability = 14;

    // PSO level stuff
    if(mapheaderinfo[gamemap-1].typeoflevel & TOL_PSO)
    {
        if(player->mo->health > ((player->psolevel) * 100 / 30))
            player->health = player->mo->health = (player->psolevel) * 100 / 30;
        if(player->psomagic > ((player->psolevel) * 100 / 50))
            player->psomagic = (player->psolevel) * 100 / 50;
        if(player->psolevel < 1)
        {
            player->psolevel = 1;
            player->psoscore = 0;
            player->health = player->mo->health = (player->psolevel) * 100 / 30;
            player->psomagic = (player->psolevel) * 100 / 50;
        }
    }
    // End PSO level stuff

    // PSO teller stuff
	player->psoteller = NULL;

	// Scan for PSO tellers in range
	for(think = thinkercap.next; think != &thinkercap; think = think->next)
	{
		if(think->function.acp1 != (actionf_p1)P_MobjThinker)
			// Not a mobj thinker
			continue;

		mo = (mobj_t *)think;
		if((mo->health <= 0) || (mo->state == &states[S_DISS]))
			// Not a valid mobj
			continue;
				
		if(mo->type != MT_PSOTELLER)
			// Not a teller...
			continue;

		if (!P_CheckSight (mo, player->mo))
			// Out of sight
			continue;

		if(P_AproxDistance(P_AproxDistance(player->mo->x-mo->x, player->mo->y-mo->y), player->mo->z-mo->z) > 256*FRACUNIT)
			// Out of range
			continue;

		if(player->mo->target && P_AproxDistance(P_AproxDistance(player->mo->x-mo->x, player->mo->y-mo->y), player->mo->z-mo->z) > P_AproxDistance(P_AproxDistance(player->mo->x-player->psoteller->x, player->mo->y-player->psoteller->y), player->mo->z-player->psoteller->z))
			// Out of pie (Aka. "This one's EVEN FARTHER AWAY!")
			continue;

		// Found a target
		found = true;
		player->psoteller = mo;
	}

	// Set the intercom variable
	if(found && !player->psotellerfound)
	{
		player->psotellerfound = true;
		if(player->psoteller->info->activesound && player == &players[displayplayer])
		  S_StartSound(NULL, player->psoteller->info->activesound);
	}
	else if(!found && player->psotellerfound)
	{
		player->psotellerfound = false;
		player->psoteller = NULL;
	}

	if(player->psoteller && mapheaderinfo[gamemap-1].typeoflevel & TOL_PSO && player->cmd.buttons & BT_LIGHTDASH)
	{
		if(player->psoteller->fuse == 2)
			M_PSOQuestMenu();
		else if(player->psoteller->fuse == 3)
			M_PSOBankMenu();
		else if(player->psoteller->fuse == 4)
			M_PSOShopMenu();
	}
    // End PSO teller stuff

    // Start held items
    if(player->helditem && mapheaderinfo[gamemap-1].typeoflevel & TOL_ADVENTURE)
    {
        mobj_t* item;
        fixed_t itemx;
        fixed_t itemy;

        item = player->helditem;
        player->pickupitem = NULL;
        if(player->ride || player->mo->health <= 0 || item->state == &states[S_DISS]
		|| item->health <= 0 || (cmd->buttons & BT_LIGHTDASH && !player->lightdashdown))
        {
            item->flags &= ~MF_NOGRAVITY;
            item->flags &= ~MF_SCENERY;
            item->flags |= MF_SOLID;
            item->flags |= MF_PUSHABLE;
            item->flags |= MF_SLIDEME;
            item->flags &= ~MF_NOBLOCKMAP;
            item->momx = player->mo->momx;
            item->momy = player->mo->momy;
            if(cmd->buttons & BT_LIGHTDASH && player->mfjumped)
            {
                        item->momz = player->mo->momz + item->info->speed/2;
                        P_InstaThrust (item, item->angle, item->info->speed);
            }
            player->helditem = item = NULL;
            player->lightdashdown = true;
            return;
        }
	itemx = P_ReturnThrustX(player->mo, player->mo->angle, 32*FRACUNIT + player->speed);
	itemy = P_ReturnThrustY(player->mo, player->mo->angle, 32*FRACUNIT + player->speed);
	item->momx = item->momy = item->momz = 0;
	P_UnsetThingPosition (item);
	item->x = player->mo->x + itemx;
	item->y = player->mo->y + itemy;
	item->z = player->mo->z + player->mo->height/2 - item->height/2;
	item->angle = player->mo->angle;
	item->flags |= MF_NOBLOCKMAP;
	P_SetThingPosition (item);
	if(player->mo->momx > item->info->speed)
		player->mo->momx = item->info->speed;
	if(player->mo->momy > item->info->speed)
		player->mo->momy = item->info->speed;
	if(player->mo->momx < -item->info->speed)
		player->mo->momx = -item->info->speed;
	if(player->mo->momy < -item->info->speed)
		player->mo->momy = -item->info->speed;
	if(player->speed > item->info->speed)
		player->speed = item->info->speed;
	item->flags |= MF_NOGRAVITY;
	item->flags |= MF_SCENERY;
	item->flags &= ~MF_SOLID;
	item->flags &= ~MF_PUSHABLE;
	item->flags &= ~MF_SLIDEME;
	item->ceilingz = player->mo->ceilingz;
	item->floorz = player->mo->floorz;
    }
    if(player->pickupitem)
    {
        if(P_AproxDistance (P_AproxDistance (player->mo->x-player->pickupitem->x, player->mo->y-player->pickupitem->y),player->mo->z-player->pickupitem->z) > 128 * FRACUNIT
                || player->ride || player->helditem || player->lightdashallowed || !(player->pickupitem->flags & MF_PUSHABLE))
                player->pickupitem = NULL;
        else if(cmd->buttons & BT_LIGHTDASH && !player->lightdashdown)
        {
                player->helditem = player->pickupitem;
                player->pickupitem = NULL;
                player->lightdashdown = true;
        }
    }
    if(cmd->buttons & BT_LIGHTDASH)
        player->lightdashdown = true;
    if(!(cmd->buttons & BT_LIGHTDASH))
        player->lightdashdown = false;
    // End held items

	// Start riding stuff
	if(player->ride && !(cmd->buttons & BT_JUMP))
	{
		player->ride->angle = player->mo->angle;
		player->ride->momx = player->mo->momx;
		player->ride->momy = player->mo->momy;
		player->mo->momz = player->ride->momz;
		P_UnsetThingPosition (player->ride);
		player->ride->x = player->mo->x;
		player->ride->y = player->mo->y;
		player->ride->z = player->mo->z - player->ride->height;
		P_SetThingPosition (player->ride);
		player->ride->flags |= MF_NOCLIP;
		P_SetMobjState(player->mo, S_PLAY_STND);
		player->mfjumped = player->mfspinning = player->mfstartdash = 0;
	}
	if(player->ride && (cmd->buttons & BT_JUMP || player->exiting))
	{
	    player->ride->momz = -10*FRACUNIT;
		player->ride->flags &= ~MF_NOCLIP;
		player->ride = NULL;
		player->mo->momz = 10*FRACUNIT;
	}
	if(player->ride && (cmd->buttons & BT_USE) && !(player->ride->flags2 & MF2_NOZCTRL))
	{
		if(/*player->ride->type != MT_BLUECRAWLA && player->ride->type != MT_REDCRAWLA && player->ride->type != MT_GFZFISH &&*/ player->ride->momz < player->ride->info->speed * FRACUNIT)
			player->ride->momz += FRACUNIT;
	}
	else if(player->ride && !(cmd->buttons & BT_USE) && player->ride->z > player->ride->floorz /*&& player->ride->type != MT_GFZFISH*/ && !(player->ride->flags2 & MF2_NOZCTRL))
	{
		player->ride->momz -= FRACUNIT;
		if(player->ride->momz < player->ride->info->speed * -FRACUNIT)
			player->ride->momz = player->ride->info->speed * -FRACUNIT;
	}
	// end riding stuff

	// start hikaru fly
	if(cmd->buttons & BT_JUMP && player->powers[pw_tailsfly] && player->mo->health > 1 && player->charability == 9)
	{
		//player->powers[pw_tailsfly]++;
		if(player->mo->momz < 5*FRACUNIT/NEWTICRATERATIO)
			player->mo->momz += FRACUNIT/2/NEWTICRATERATIO;
		if(leveltime % 10 == 1)
		{
			player->mo->health--;
			player->health--;
		}
	}
	// end hikaru fly

	// start "has chao"
	if(player->charability == 14 && (!player->chao || player->chao->health <= 0) && !player->exiting)
	{
		player->chao = P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z + player->mo->height, MT_SPMOVESCHAO);
		player->chao->tracer = player->mo;
	}

	if(player->exiting && player->chao)
	{
		//P_SpawnMobj(player->chao->x, player->chao->y, player->chao->z, MT_CHAODANCE); // TODO: Danceing end-of-level chao!
		P_SetMobjState(player->chao,S_DISS);
		player->chao = NULL;
	}
	// end "has chao"

	// start "runs on air"
	if(cmd->buttons & BT_JUMP && !(cmd->buttons & BT_USE) && player->charability == 13 && !player->powers[pw_flashing] && !player->mfjumped && player->mo->momz < player->normalspeed*FRACUNIT/2)
		player->mo->momz += FRACUNIT*2/NEWTICRATERATIO;
	else if(cmd->buttons & BT_USE && !(cmd->buttons & BT_JUMP) && player->charability == 13 && !player->powers[pw_flashing] && !player->mfjumped && player->mo->z > player->mo->floorz && -player->mo->momz < player->normalspeed*FRACUNIT/2)
		player->mo->momz = -FRACUNIT*2/NEWTICRATERATIO;
	else if(cmd->buttons & BT_USE && cmd->buttons & BT_JUMP && player->charability == 13 && !player->powers[pw_flashing] && !player->mfjumped && player->mo->z > player->mo->floorz)
		player->mo->momz = 0;
	// end "runs on air"

	// start chaos control
	if(player->mo->flags2 & MF2_DONTDRAW
		&& !player->powers[pw_flashing]
		&& player->charability == 3
		&& player->mo->z <= player->mo->floorz
		&& !(player->mo->subsector->sector->ceilingheight - player->mo->subsector->sector->floorheight < 32))
		// If they chaos controlled and are on the ground now but the ceiling isn't below the floor
	{
		player->mo->momx = player->mo->momy = player->mo->momz = 0; // Stop! Stooop! Too fast, too fast!
		player->mo->flags2 &= ~MF2_DONTDRAW; // You can see me?!
		player->mo->flags &= ~MF_NOCLIP; // Your ride's over, get out!
		player->mfspinning = 0;

		//If they had a sheild, make it appear again!
		if(player->powers[pw_blueshield])
			P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, MT_BLUEORB)->target = player->mo;
		else if(player->powers[pw_yellowshield])
			P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, MT_YELLOWORB)->target = player->mo;
		else if(player->powers[pw_greenshield])
			P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, MT_GREENORB)->target = player->mo;
		else if(player->powers[pw_blackshield])
			P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, MT_BLACKORB)->target = player->mo;
		else if(player->powers[pw_redshield])
			P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, MT_REDORB)->target = player->mo;
	}

	if((player->mo->subsector->sector->ceilingheight - player->mo->subsector->sector->floorheight < 32
	|| player->mo->ceilingz - player->mo->floorz < 32)
	&& !(player->mo->flags & MF_NOCLIP)
	&& !player->powers[pw_flashing]
	&& !player->exiting) // If they hit a wall...
	// Be extra careful about this if statement, a logic error here spells death.
	{
	    /*player->mo->angle += 32767.5; // Bounce off it!
	    if(player == &players[consoleplayer])
		localangle = player->mo->angle;
	    else if(player == &players[secondarydisplayplayer] && cv_splitscreen.value)
		localangle2 = player->mo->angle;*/
	    player->lives++; // Give em a life so they dun get a game over just for being stupid...
	    P_DamageMobj(player->mo, NULL, NULL, 10000); // And kill em, there's nothing else you can do for em...
	}
	if(player->mo->flags2 & MF2_DONTDRAW
		&& !player->powers[pw_flashing]
		&& player->charability == 3
		&& cmd->buttons & BT_USE)
		// If they Chaos Controlled and held the button like a good little hedgehog
	{
		P_InstaThrust (player->mo, player->mo->angle, MAXMOVE); // Reward them with another burst for each loop
		player->mo->momz = -1*FRACUNIT; // Down down, nice n slow...
	}

	else if(player->mo->flags2 & MF2_DONTDRAW
		&& !player->powers[pw_flashing]
		&& player->charability == 3
		&& (!(cmd->buttons & BT_USE) || player->exiting)
		&& !(player->mo->subsector->sector->ceilingheight - player->mo->subsector->sector->floorheight < 32))  // If not...
	{
		player->mo->momz = -MAXMOVE; // Drop to the floor and keep ya hands where I can see em!
		player->mo->momx = player->mo->momy = 0; // No move for you!
	}
	// end chaos control

	// start Eggman jets
	if(player->skin == spm_eggman && !player->fly1 && !player->exiting)
	{
		fixed_t jetx, jety;
		mobj_t* filler;

		jetx = player->mo->x + P_ReturnThrustX(player->mo, player->mo->angle, -56*FRACUNIT);
		jety = player->mo->y + P_ReturnThrustY(player->mo, player->mo->angle, -56*FRACUNIT);

		// Spawn the jets here
		filler = P_SpawnMobj(jetx, jety, player->mo->z + 8*FRACUNIT, MT_JETFUME1);
		filler->target = player->mo;
		filler->fuse = 56;
		
		filler = P_SpawnMobj(jetx + P_ReturnThrustX(player->mo, player->mo->angle-ANG90, 24*FRACUNIT), jety + P_ReturnThrustY(player->mo, player->mo->angle-ANG90, 24*FRACUNIT), player->mo->z + 32*FRACUNIT, MT_JETFUME1);
		filler->target = player->mo;
		filler->fuse = 57;

		filler = P_SpawnMobj(jetx + P_ReturnThrustX(player->mo, player->mo->angle+ANG90, 24*FRACUNIT), jety + P_ReturnThrustY(player->mo, player->mo->angle+ANG90, 24*FRACUNIT), player->mo->z + 32*FRACUNIT, MT_JETFUME1);
		filler->target = player->mo;
		filler->fuse = 58;

		player->fly1 = 1;
	}
	if (player->exiting && player->fly1)
	   player->fly1 = 0;
	// end Eggman jets

	if(player->powers[pw_shortsword] == 1)
	{
		player->mo->momx = player->mo->momy = player->mo->momz = 0;
		if(player->mo->z == player->mo->floorz)
			P_SetMobjState(player->mo, S_PLAY_STND);
		else
			P_SetMobjState(player->mo, S_PLAY_ATK1);
	}

	// Decrease teh powas!
	if(player->powers[pw_shortsword])
	{
		player->powers[pw_shortsword]--;
		player->powers[pw_flashing] = player->powers[pw_shortsword];
	}
	if(player->powers[pw_longsword])
		player->powers[pw_longsword]--;
	if(player->powers[pw_supervirusring])
		player->powers[pw_supervirusring]--;
	if(player->powers[pw_confuzed])
		player->powers[pw_confuzed]--;
	if(player->powers[pw_invertedcontrols])
		player->powers[pw_invertedcontrols]--;
	if(player->powers[pw_slow])
		player->powers[pw_slow]--;
}

//
// JTE_EmblemSpawn
//
// Spawns "emblem" mobjs on map load. Multiplayer Easter Eggs rock.
//
void JTE_EmblemSpawn()
{
    int i;
    mobj_t* emblemmobj;
    showmultiegghud = false;

	if(!(netgame || multiplayer) || (cv_splitscreen.value && !playeringame[2]))
		goto nomulti;

    for(i=1; i<MAXPLAYERS; i++)
        if(playeringame[i])
                break;

    if(i == MAXPLAYERS)
        goto nomulti;

	// MultiEggs
	for(i=0; i<NUMMULTIEGGS; i++)
	{
		if(multiegglocations[i].level != gamemap)
			continue;

		/*if(multieggs & multiegglocations[i].flagnum && !server) // They already found this egg... drat!
			continue;*/

		showmultiegghud = true;

		emblemmobj = P_SpawnMobj(multiegglocations[i].x<<FRACBITS,multiegglocations[i].y<<FRACBITS,multiegglocations[i].z<<FRACBITS, MT_MULTIEGG);
		emblemmobj->health = multiegglocations[i].flagnum;
		emblemmobj->flags =  (emblemmobj->flags & ~MF_TRANSLATION) | ((multiegglocations[i].player)<<MF_TRANSSHIFT);
	}
	return;

	nomulti:

	// JTE Coins
	for(i=0; i<NUMJTETOKENS; i++)
	{
		if(jtetokenlocations[i].level != gamemap)
			continue;

		if(jtetokens & jtetokenlocations[i].flagnum && !server) // They already found this egg... drat!
			continue;

		emblemmobj = P_SpawnMobj(jtetokenlocations[i].x<<FRACBITS,jtetokenlocations[i].y<<FRACBITS,jtetokenlocations[i].z<<FRACBITS, MT_JTETOKEN);
		emblemmobj->health = multiegglocations[i].flagnum;
		emblemmobj->flags =  (emblemmobj->flags & ~MF_TRANSLATION) | ((jtetokenlocations[i].player)<<MF_TRANSSHIFT);
	}
	return;
}

//
// Enemy actions.
//
// They do various things...
//
void A_CheckFlee (mobj_t*   actor)
{
	if (actor->z + actor->info->height + 1 >= actor->ceilingz)
		P_SetMobjState(actor, S_DISS);
	if (actor->z <= actor->floorz)
		P_SetMobjState(actor, S_DISS);
	actor->flags |= MF_NOGRAVITY;
	actor->flags |= MF_NOCLIP;
	if (actor->target && !actor->target->player)
	{
		if(actor->z < actor->floorz + 64*FRACUNIT)
			actor->momz = 2*FRACUNIT;
		actor->angle = R_PointToAngle2(actor->x, actor->y, actor->target->x, actor->target->y);
		actor->flags2 |= MF2_BOSSFLEE;
		actor->momz = FixedMul(FixedDiv(actor->target->z - actor->z, P_AproxDistance(actor->x-actor->target->x,actor->y-actor->target->y)), 2*FRACUNIT);
	}
	else
		actor->momz = 2*FRACUNIT;
}
void A_BirdFlap (mobj_t*   actor)
{
	actor->momz = 1*FRACUNIT;
	P_InstaThrust(actor, actor->angle, actor->info->speed*FRACUNIT);
}
void A_BirdFall (mobj_t* actor)
{
	actor->momz = -1*FRACUNIT;
	if(P_Random() & 1)
		actor->angle += (P_Random() % 37) * (ANG90 / 9);
	P_InstaThrust(actor, actor->angle, actor->info->speed/2*FRACUNIT);
}
void A_SPMovesChao (mobj_t* actor)
{
	fixed_t dist;
	mobj_t* dest;
	fixed_t speed;

	if(!actor->tracer || !actor->tracer->health || !actor->tracer->player || actor->tracer->player->charability != 14)
	{
		P_SetMobjState(actor, S_DISS);
		return;
	}

	if(actor->flags2 & MF2_CHAOATTACK)
	{
		dest = actor->tracer;
		dist = P_AproxDistance(P_AproxDistance(dest->x - actor->x, dest->y - actor->y), dest->z + dest->info->height - actor->z);
		if(!actor->target || !actor->target->health || actor->target->flags2 & MF2_FRET || !(actor->target->flags & MF_SHOOTABLE) || dist > 3200*FRACUNIT)
		{
			actor->flags2 &= ~MF2_CHAOATTACK;
			actor->flags |= MF_NOCLIP;
		}
		else
		{
			dest = actor->target;
			actor->angle = R_PointToAngle2(actor->x, actor->y, dest->x, dest->y);
			dist = P_AproxDistance(P_AproxDistance(dest->x - actor->x, dest->y - actor->y), dest->z + dest->height - actor->z);

			if (dist < 1)
				dist = 1;

			if (dist < dest->radius + dest->height)
				actor->flags &= ~MF_NOCLIP;

			if (dist < dest->radius + dest->height + 24*FRACUNIT)
				speed = actor->info->speed/5;
			else
				speed = actor->info->speed;

			actor->momx = FixedMul(FixedDiv(dest->x - actor->x, dist), speed);
			actor->momy = FixedMul(FixedDiv(dest->y - actor->y, dist), speed);
			actor->momz = FixedMul(FixedDiv(dest->z + dest->info->height - actor->z, dist), speed);
		}
		return;
	}

	// adjust direction
	dest = actor->tracer;

	if (!dest || dest->health <= 0)
	{
		P_SetMobjState(actor, S_DISS);
		return;
	}

	// change angle
	actor->angle = R_PointToAngle2(actor->x, actor->y, dest->x, dest->y);

	// change slope
	dist = P_AproxDistance(P_AproxDistance(dest->x - actor->x, dest->y - actor->y), dest->z + dest->info->height - actor->z);

	if (dist < 1)
		dist = 1;

	if(dist > 3200*FRACUNIT) // If the player is out of range, cheat ;P
		speed = MAXMOVE;
	else if(dist <= actor->info->speed) // If you're in position next to them, stop.
		speed = 0;
	else if(actor->tracer->player->speed*FRACUNIT > actor->info->speed && (actor->tracer->momx || actor->tracer->momy)) // Otherwise, if the player is moving faster then you can, move at your max speed.
		speed = actor->info->speed;
	else if(actor->tracer->player->speed*FRACUNIT > actor->info->speed/3 && (actor->tracer->momx || actor->tracer->momy)) // If they're not moving faster then you can but not stopped, move at their speed.
		speed = actor->tracer->player->speed*FRACUNIT;
	else
		speed = actor->info->speed/3; // Otherwise, you must be fairly close to them, but they're not moving, so go nice and slow up to them :)

	actor->momx = FixedMul(FixedDiv(dest->x - actor->x, dist), speed);
	actor->momy = FixedMul(FixedDiv(dest->y - actor->y, dist), speed);
	actor->momz = FixedMul(FixedDiv(dest->z + dest->info->height - actor->z, dist), speed);
	if(actor->tracer->player->customchao)
	{
	    actor->skin = actor->tracer->skin;
	    actor->frame = actor->state->frame + (actor->tracer->player->customchao-1);
	}
	else
	{
	    actor->skin = NULL;
	    actor->frame = actor->state->frame;
	}
}

//
// JTE_SPMovesFiles
//
// Adds the SPMoves files! ^.^
//
void JTE_SPMovesFiles()
{
    spm_shadow = spm_mechaknux = spm_eggman = spm_jte = spm_jte2 = spm_ss005 = spm_cyan = spm_fox = spm_hikaru = spm_chompy = spm_zim = -1;
	D_AddFile("SPMoves.wad");
	if(!M_CheckParm("-nochars"))
	{
		if(!M_CheckParm("-noshadow") && !access("SPMoves-Shadow.wad",R_OK))
			D_AddFile("SPMoves-Shadow.wad");
		if(!M_CheckParm("-nomechaknux") && !access("SPMoves-MechaKnux.wad",R_OK))
			D_AddFile("SPMoves-MechaKnux.wad");
		if(!M_CheckParm("-noeggman") && !access("SPMoves-Eggman.wad",R_OK))
			D_AddFile("SPMoves-Eggman.wad");
		if(!M_CheckParm("-nojte") && !access("SPMoves-JTE.wad",R_OK))
			D_AddFile("SPMoves-JTE.wad");
		if(!M_CheckParm("-nojte2") && !access("SPMoves-JTE2.wad",R_OK))
			D_AddFile("SPMoves-JTE2.wad");
		if(!M_CheckParm("-noss005") && !access("SPMoves-SS005.wad",R_OK))
			D_AddFile("SPMoves-SS005.wad");
		if(!M_CheckParm("-nocyan") && !access("SPMoves-Cyan.wad",R_OK))
			D_AddFile("SPMoves-Cyan.wad");
		if(!M_CheckParm("-nofox") && !access("SPMoves-fox.wad",R_OK))
			D_AddFile("SPMoves-fox.wad");
		if(!M_CheckParm("-nohikaru") && !access("SPMoves-Hikaru.wad",R_OK))
			D_AddFile("SPMoves-Hikaru.wad");
		if(!M_CheckParm("-nochompy") && !access("SPMoves-Chompy.wad",R_OK))
			D_AddFile("SPMoves-Chompy.wad");
		if(!M_CheckParm("-nozim") && !access("SPMoves-Zim.wad",R_OK))
			D_AddFile("SPMoves-Zim.wad");
	}
	if (!access("SPMoves-Music.wad",R_OK))
		D_AddFile("SPMoves-Music.wad");
}

//
// JTE_PlayerFire
//
// Special player fireing code for cases like Eggman and Fox.
//
boolean JTE_PlayerFire(player_t *player)
{
    int i;
    #define HOMING player->powers[pw_homingring]
    #define RAIL player->powers[pw_railring]
    #define AUTOMATIC player->powers[pw_automaticring]
    #define EXPLOSION player->powers[pw_explosionring]


		if(mapheaderinfo[gamemap-1].typeoflevel & TOL_PSO
			&& !player->attackdown && !player->weapondelay
			&& !player->exiting && !((player->charability == 7 || player->skin == spm_eggman) && !player->equipweapon))
		{
			mobj_t* mo;

			player->attackdown = true;

			if(psoitems[player->equipweapon].type == PSOI_HANDGUN || psoitems[player->equipweapon].type == PSOI_RIFLE)
			{
				if(psoitems[player->equipweapon].type == PSOI_HANDGUN)
					player->weapondelay = TICRATE/4;
				if(psoitems[player->equipweapon].type == PSOI_RIFLE)
					player->weapondelay = TICRATE/2;
				player->mo->reactiontime += TICRATE;
				mo = P_SpawnPlayerMissile (player->mo, psoitems[player->equipweapon].firetype);
				if(psoitems[player->equipweapon].usesound)
				    S_StartSound (player->mo, psoitems[player->equipweapon].usesound);
				if(mo && psoitems[player->equipweapon].flags & PSOF_RAILSHOTS)
				{
					player->weapondelay = TICRATE;
					mo->flags2 |= MF2_RAILRING;
					mo->flags2 |= MF2_DONTDRAW;
					for(i=0; i<256; i++)
					{
					     if(mo && mo->flags & MF_MISSILE)
					     {
							if(!(mo->flags & MF_NOBLOCKMAP))
							{
								P_UnsetThingPosition(mo);
								mo->flags |= MF_NOBLOCKMAP;
								P_SetThingPosition(mo);
							}
							if(i&1)
								P_SpawnMobj(mo->x, mo->y, mo->z, MT_SPARK);
							P_RailThinker(mo);
						}
						else
							return true;
					}
				}
			}
			else if(psoitems[player->equipweapon].type == PSOI_SABER || psoitems[player->equipweapon].type == PSOI_CLAW || psoitems[player->equipweapon].type == PSOI_CANE)
			{
				if(psoitems[player->equipweapon].type == PSOI_CANE)
					player->weapondelay = TICRATE;
				if(psoitems[player->equipweapon].type == PSOI_SABER)
					player->weapondelay = TICRATE/2;
				if(psoitems[player->equipweapon].type == PSOI_CLAW)
					player->weapondelay = TICRATE/3;
				player->powers[pw_shortsword] = TICRATE/6;

				player->mo->z++;
				P_ResetPlayer(player);
				P_InstaThrust(player->mo, player->mo->angle, MAXMOVE);
				P_SetMobjState(player->mo, S_PLAY_SPD1);
			}
			return true;
		}
		else if (!player->attackdown && !player->weapondelay
			&& (player->charability == 7 || player->skin == spm_eggman) && !player->exiting) // fox's ability! Shoot! Jason - 6/25/04
		{
			player->attackdown = true;
			if (player->skin == spm_eggman)
			{
				mobj_t* mo;
				int      i;

				player->weapondelay = 2*TICRATE;
				mo = P_SpawnPlayerMissile (player->mo, MT_ROCKET);
			}
			else if (HOMING && RAIL && AUTOMATIC && EXPLOSION)
			{
				mobj_t* mo;
				int      i;

				player->weapondelay = 2;
				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_RAILRING;
					mo->flags2 |= MF2_HOMING;
					mo->flags2 |= MF2_EXPLOSION;
					mo->flags2 |= MF2_AUTOMATIC;
					mo->flags2 |= MF2_DONTDRAW;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);

				for(i=0; i<256; i++)
				{
					if(mo && mo->flags & MF_MISSILE)
					{
						if(!(mo->flags & MF_NOBLOCKMAP))
						{
							P_UnsetThingPosition(mo);
							mo->flags |= MF_NOBLOCKMAP;
							P_SetThingPosition(mo);
						}

						if(i&1)
							P_SpawnMobj(mo->x, mo->y, mo->z, MT_SPARK);

						P_RailThinker(mo);
					}
					else
						return true;
				}
			}
			else if (HOMING && RAIL && AUTOMATIC)
			{
				mobj_t* mo;
				int      i;

				player->weapondelay = 2;

				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_RAILRING;
					mo->flags2 |= MF2_HOMING;
					mo->flags2 |= MF2_AUTOMATIC;
					mo->flags2 |= MF2_DONTDRAW;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);

				for(i=0; i<256; i++)
				{
					if(mo && mo->flags & MF_MISSILE)
					{
						if(!(mo->flags & MF_NOBLOCKMAP))
						{
							P_UnsetThingPosition(mo);
							mo->flags |= MF_NOBLOCKMAP;
							P_SetThingPosition(mo);
						}

						if(i&1)
							P_SpawnMobj(mo->x, mo->y, mo->z, MT_SPARK);

						P_RailThinker(mo);
					}
					else
						return true;
				}
			}
			else if (RAIL && AUTOMATIC && EXPLOSION)
			{
				// Automatic exploding rail
				mobj_t* mo;
				int      i;

				player->weapondelay = 2;

				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_RAILRING;
					mo->flags2 |= MF2_EXPLOSION;
					mo->flags2 |= MF2_AUTOMATIC;
					mo->flags2 |= MF2_DONTDRAW;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);

				for(i=0; i<256; i++)
				{
					if(mo && mo->flags & MF_MISSILE)
					{
						if(!(mo->flags & MF_NOBLOCKMAP))
						{
							P_UnsetThingPosition(mo);
							mo->flags |= MF_NOBLOCKMAP;
							P_SetThingPosition(mo);
						}

						if(i&1)
							P_SpawnMobj(mo->x, mo->y, mo->z, MT_SPARK);

						P_RailThinker(mo);
					}
					else
						return true;
				}
			}
			else if (AUTOMATIC && EXPLOSION && HOMING)
			{
				// Automatic exploding homing
				mobj_t* mo;

				player->weapondelay = 2;

				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_HOMING;
					mo->flags2 |= MF2_EXPLOSION;
					mo->flags2 |= MF2_AUTOMATIC;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);
			}
			else if (EXPLOSION && HOMING && RAIL)
			{
				// Exploding homing rail
				mobj_t* mo;
				int      i;

				player->weapondelay = 1.5*TICRATE;
				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_RAILRING;
					mo->flags2 |= MF2_EXPLOSION;
					mo->flags2 |= MF2_HOMING;
					mo->flags2 |= MF2_DONTDRAW;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);

				for(i=0; i<256; i++)
				{
					if(mo && mo->flags & MF_MISSILE)
					{
						if(!(mo->flags & MF_NOBLOCKMAP))
						{
							P_UnsetThingPosition(mo);
							mo->flags |= MF_NOBLOCKMAP;
							P_SetThingPosition(mo);
						}

						if(i&1)
							P_SpawnMobj(mo->x, mo->y, mo->z, MT_SPARK);

						P_RailThinker(mo);
					}
					else
						return true;
				}
			}
			else if (HOMING && RAIL)
			{
				// Homing rail
				mobj_t* mo;
				int      i;

				player->weapondelay = 1.5*TICRATE;
				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_RAILRING;
					mo->flags2 |= MF2_HOMING;
					mo->flags2 |= MF2_DONTDRAW;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);

				for(i=0; i<256; i++)
				{
					if(mo && mo->flags & MF_MISSILE)
					{
						if(!(mo->flags & MF_NOBLOCKMAP))
						{
							P_UnsetThingPosition(mo);
							mo->flags |= MF_NOBLOCKMAP;
							P_SetThingPosition(mo);
						}

						if(i&1)
							P_SpawnMobj(mo->x, mo->y, mo->z, MT_SPARK);

						P_RailThinker(mo);
					}
					else
						return true;
				}
			}
			else if (EXPLOSION && RAIL)
			{
				// Explosion rail (Graue 11-04-2003 desc fixed)
				mobj_t* mo;
				int      i;

				player->weapondelay = 1.5*TICRATE;
				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_RAILRING;
					mo->flags2 |= MF2_EXPLOSION;
					mo->flags2 |= MF2_DONTDRAW;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);

				for(i=0; i<256; i++)
				{
					if(mo && mo->flags & MF_MISSILE)
					{
						if(!(mo->flags & MF_NOBLOCKMAP))
						{
							P_UnsetThingPosition(mo);
							mo->flags |= MF_NOBLOCKMAP;
							P_SetThingPosition(mo);
						}

						if(i&1)
							P_SpawnMobj(mo->x, mo->y, mo->z, MT_SPARK);

						P_RailThinker(mo);
					}
					else
						return true;
				}
			}
			else if (RAIL && AUTOMATIC)
			{
				// Automatic rail
				mobj_t* mo;
				int      i;

				player->weapondelay = 2;

				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_RAILRING;
					mo->flags2 |= MF2_AUTOMATIC;
					mo->flags2 |= MF2_DONTDRAW;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);

				for(i=0; i<256; i++)
				{
					if(mo && mo->flags & MF_MISSILE)
					{
						if(!(mo->flags & MF_NOBLOCKMAP))
						{
							P_UnsetThingPosition(mo);
							mo->flags |= MF_NOBLOCKMAP;
							P_SetThingPosition(mo);
						}

						if(i&1)
							P_SpawnMobj(mo->x, mo->y, mo->z, MT_SPARK);

						P_RailThinker(mo);
					}
					else
						return true;
				}
			}
			else if (AUTOMATIC && EXPLOSION)
			{
				// Automatic exploding
				mobj_t* mo;

				player->weapondelay = 5;

				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_AUTOMATIC;
					mo->flags2 |= MF2_EXPLOSION;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);
			}
			else if (EXPLOSION && HOMING)
			{
				// Homing exploding
				mobj_t* mo;

				player->weapondelay = TICRATE;
				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_EXPLOSION;
					mo->flags2 |= MF2_HOMING;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);
			}
			else if (AUTOMATIC && HOMING)
			{
				// Automatic homing
				mobj_t* mo;

				player->weapondelay = 2;

				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_AUTOMATIC;
					mo->flags2 |= MF2_HOMING;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);
			}
			else if (HOMING)
			{
				// Homing ring
				mobj_t* mo;

				player->weapondelay = TICRATE/4;
				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_HOMING;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);
			}
			else if (RAIL)
			{
				// Rail ring
				mobj_t* mo;
				int      i;
				int      z;

				z = 0;

				player->weapondelay = 1.5*TICRATE;
				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_RAILRING;
					mo->flags2 |= MF2_DONTDRAW;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);

				for(i=0; i<256; i++)
				{
					if(mo && mo->flags & MF_MISSILE)
					{
						if(!(mo->flags & MF_NOBLOCKMAP))
						{
							P_UnsetThingPosition(mo);
							mo->flags |= MF_NOBLOCKMAP;
							P_SetThingPosition(mo);
						}

						if(i&1)
							P_SpawnMobj(mo->x, mo->y, mo->z, MT_SPARK);

						P_RailThinker(mo);
					}
					else
						return true;
				}
				
			}
			else if (AUTOMATIC)
			{
				// Automatic
				mobj_t* mo;

				player->weapondelay = 2;

				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
					mo->flags2 |= MF2_AUTOMATIC;
			}
			else if (EXPLOSION)
			{
				// Exploding
				mobj_t* mo;

				player->weapondelay = TICRATE;
				mo = P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				if(mo)
				{
					mo->flags2 |= MF2_EXPLOSION;
				}
				S_StartSound (player->mo, player->mo->info->attacksound);
			}
			else // No powers, just a regular shot.
			{
				player->weapondelay = TICRATE/4;
				P_SpawnPlayerMissile (player->mo, MT_FOXBULLET);
				S_StartSound (player->mo, player->mo->info->attacksound);
			}
			return true;
		}
		return false;
}

//
// JTE_InitCharData
//
// Whoo hoo, dynamic character select menus! ^.^
//
void JTE_InitCharData()
{
	if(spm_shadow != -1)
	{
		PlayerMenu[spm_shadow].text = "SHADOW";
		PlayerMenu[spm_shadow].status = 32; // IT_CALL | IT_STRING
		PlayerMenu[spm_shadow].alphaKey = 24;
		strcpy(description[spm_shadow].info, "                 ChaosControl\n             The ultimate   \nlife form, Shadow,     \nis a weapon of mass    \ndestruction, capable  \nof world domination!  ");
		strcpy(description[spm_shadow].picname, "SHADCHAR");
 	}
	if(spm_mechaknux != -1)
	{
		PlayerMenu[spm_mechaknux].text = "MECHA KNUCKLES";
		PlayerMenu[spm_mechaknux].status = 32;
		PlayerMenu[spm_mechaknux].alphaKey = 32;
		strcpy(description[spm_mechaknux].info, "                 Jet-Glide   \n             Unavailable.   \n                       \n                       \n                      \n                      ");
		strcpy(description[spm_mechaknux].picname, "UNKNCHAR");
 	}
	if(spm_eggman != -1)
	{
		PlayerMenu[spm_eggman].text = "EGGMAN";
		PlayerMenu[spm_eggman].status = 32;
		PlayerMenu[spm_eggman].alphaKey = 40;
		strcpy(description[spm_eggman].info, "                 Unknown.    \n             Unavailable.   \n                       \n                       \n                      \n                      ");
		strcpy(description[spm_eggman].picname, "UNKNCHAR");
 	}
	if(spm_jte != -1)
	{
		PlayerMenu[spm_jte].text = "JASON THE ECHIDNA";
		if(grade & 4096)
		  PlayerMenu[spm_jte].status = 32;
		PlayerMenu[spm_jte].alphaKey = 48;
		strcpy(description[spm_jte].info, "                 Glide-Blast \n             With the       \npowerful Glide-Blast   \nmastered through much  \ntraining over the net,\nJTE is hard to catch! ");
		strcpy(description[spm_jte].picname, "JTECHAR");
 	}
	if(spm_ss005 != -1)
	{
		PlayerMenu[spm_ss005].text = "SONIC SHADOW 005";
		//PlayerMenu[spm_ss005].status = 32;
		PlayerMenu[spm_ss005].alphaKey = 56;
		strcpy(description[spm_ss005].info, "                 Unknown.    \n             Unavailable.   \n                       \n                       \n                      \n                      ");
		strcpy(description[spm_ss005].picname, "UNKNCHAR");
 	}
	if(spm_cyan != -1)
	{
		PlayerMenu[spm_cyan].text = "CYAN";
		//PlayerMenu[spm_cyan].status = 32;
		PlayerMenu[spm_cyan].alphaKey = 64;
		strcpy(description[spm_cyan].info, "                  Speed Climb\n             The agility    \nfighter, Cyan, has     \nagile legs and swift   \narms!                 \n                      ");
		strcpy(description[spm_cyan].picname, "UNKNCHAR");
 	}
	if(spm_fox != -1)
	{
		PlayerMenu[spm_fox].text = "'FOX' UNIT 01";
		//PlayerMenu[spm_fox].status = 32;
		PlayerMenu[spm_fox].alphaKey = 72;
		strcpy(description[spm_fox].info, "                 Guns & Ammo \n             Armed to the   \nteeth, fox is a one-man\nwar machine! Instead of\nspeed rely on your big\nguns for kicking butt!");
		strcpy(description[spm_fox].picname, "UNKNCHAR");
 	}
	if(spm_hikaru != -1)
	{
		PlayerMenu[spm_hikaru].text = "HIKARU";
		//PlayerMenu[spm_hikaru].status = 32;
		PlayerMenu[spm_hikaru].alphaKey = 80;
		strcpy(description[spm_hikaru].info, "                 Speed Fly   \n             This young     \nkitsune may lack skill \nbut makes up for it    \nwith amazing flying   \nabilities.            ");
		strcpy(description[spm_hikaru].picname, "UNKNCHAR");
 	}
	if(spm_chompy != -1)
	{
		PlayerMenu[spm_chompy].text = "CHOMPY";
		//PlayerMenu[spm_chompy].status = 32;
		PlayerMenu[spm_chompy].alphaKey = 88;
		strcpy(description[spm_chompy].info, "                 Unknown.    \n             Unavailable.   \n                       \n                       \n                      \n                      ");
		strcpy(description[spm_chompy].picname, "UNKNCHAR");
 	}
	if(spm_zim != -1)
	{
		PlayerMenu[spm_zim].text = "INVADER ZIM";
		//PlayerMenu[spm_zim].status = 32;
		PlayerMenu[spm_zim].alphaKey = 96;
		strcpy(description[spm_zim].info, "                 Speed Climb \n             With his       \nbackpack, he can climb \nwalls with remarkable  \nspeed, but he travels \nslow on foot.         ");
		strcpy(description[spm_zim].picname, "ZIMCHAR");
 	}
}

//
// JTE_SPMovesFuse
//
// Some special stuff for fuse-using mobjs in SPMoves. Returns true if the mobj is in it's range.
//
boolean JTE_SPMovesFuse (mobj_t* mobj)
{
    int random;
    mobj_t* mo;
	switch(mobj->type)
	{
		case MT_PAYMACHINE:
		case MT_PSOTELLER:
			mobj->fuse = 1;
			return true;
		case MT_PSODROP:
			mobj->health = ((P_Random() + P_Random() + P_Random() + P_Random() + P_Random()) % (NUMPSOITEMS - 1)) + 1;
			random = P_Random() % 70;
			if(random == 0)
			{
				P_SpawnMobj(mobj->x,mobj->y,mobj->z + 16*FRACUNIT, MT_EMMY);
				P_SpawnMobj(mobj->x,mobj->y,mobj->z + 16*FRACUNIT, MT_TOKEN);
			}
			else if(random > 0 && random < 20)
				P_SpawnMobj(mobj->x,mobj->y,mobj->z, MT_PSOMONEY)->health = (P_Random() % 90) + 10;
			else if(random >= 20 && random < 40)
			{
				switch(psoitems[mobj->health].type)
				{
					case PSOI_SABER:
					case PSOI_SWORD:
					case PSOI_DAGGER:
					case PSOI_PARTISAN:
					case PSOI_SLICER:
					case PSOI_HANDGUN:
					case PSOI_RIFLE:
					case PSOI_MECHGUN:
					case PSOI_SHOT:
					case PSOI_CANE:
					case PSOI_ROD:
					case PSOI_WAND:
					case PSOI_CLAW:
					case PSOI_KNUCKLE:
					case PSOI_DOUBLESABER:
					case PSOI_SPREAD:
					case PSOI_PUNCH:
						mo = P_SpawnMobj(mobj->x, mobj->y, mobj->floorz, MT_PSOWEAPON);
						mo->health = mobj->health;
						mo->flags = (mo->flags & ~MF_TRANSLATION) | ((7)<<MF_TRANSSHIFT); // Temporary - To be removed when proper sprites are available.
						break;
					case PSOI_BARRIER:
					case PSOI_WALL:
					case PSOI_SPBARRIER:
					case PSOI_FRAME:
					case PSOI_FIELD:
					case PSOI_SPFRAME:
						mo = P_SpawnMobj(mobj->x, mobj->y, mobj->floorz, MT_PSOSHIELD);
						mo->health = mobj->health;
						mo->flags = (mo->flags & ~MF_TRANSLATION) | ((5)<<MF_TRANSSHIFT); // Temporary - To be removed when proper sprites are available.
						break;
					case PSOI_INVALID:
					default:
						if(cv_debug)
							CONS_Printf("Invalid PSO item type %d from item number %d", psoitems[mobj->health].type,mobj->health);
						break;
				}
			}
			P_SetMobjState(mobj,S_DISS);
			return true;
		case MT_PSOBOSSDROP:
			mobj->health = ((P_Random() + P_Random() + P_Random() + P_Random() + P_Random()) % (NUMPSOITEMS - 1)) + 1;
			random = P_Random() % 40;
			if(random == 0)
			{
				P_SpawnMobj(mobj->x,mobj->y,mobj->z + 16*FRACUNIT, MT_EMMY);
				P_SpawnMobj(mobj->x,mobj->y,mobj->z + 16*FRACUNIT, MT_TOKEN);
			}
			else if(random > 0 && random < 20)
				P_SpawnMobj(mobj->x,mobj->y,mobj->z, MT_PSOMONEY)->health = (P_Random() % 90) + 10;
			else
			{
				switch(psoitems[mobj->health].type)
				{
					case PSOI_SABER:
					case PSOI_SWORD:
					case PSOI_DAGGER:
					case PSOI_PARTISAN:
					case PSOI_SLICER:
					case PSOI_HANDGUN:
					case PSOI_RIFLE:
					case PSOI_MECHGUN:
					case PSOI_SHOT:
					case PSOI_CANE:
					case PSOI_ROD:
					case PSOI_WAND:
					case PSOI_CLAW:
					case PSOI_KNUCKLE:
					case PSOI_DOUBLESABER:
					case PSOI_SPREAD:
					case PSOI_PUNCH:
						mo = P_SpawnMobj(mobj->x, mobj->y, mobj->floorz, MT_PSOWEAPON);
						mo->health = mobj->health;
						mo->flags = (mo->flags & ~MF_TRANSLATION) | ((7)<<MF_TRANSSHIFT); // Temporary - To be removed when proper sprites are available.
						break;
					case PSOI_BARRIER:
					case PSOI_WALL:
					case PSOI_SPBARRIER:
					case PSOI_FRAME:
					case PSOI_FIELD:
					case PSOI_SPFRAME:
						mo = P_SpawnMobj(mobj->x, mobj->y, mobj->floorz, MT_PSOSHIELD);
						mo->health = mobj->health;
						mo->flags = (mo->flags & ~MF_TRANSLATION) | ((5)<<MF_TRANSSHIFT); // Temporary - To be removed when proper sprites are available.
						break;
					case PSOI_INVALID:
					default:
						if(cv_debug)
							CONS_Printf("Invalid PSO item type %d from item number %d", psoitems[mobj->health].type,mobj->health);
						break;
				}
			}
			P_SetMobjState(mobj,S_DISS);
			return true;
		default:
			return false;
	}
	return false;
}

//
// JTE_SPMovesMobjProg
//
// Some programming for specific mobjs. Non-SOC-able though, so using this for all mobj programming is frowned upon. :(
//
boolean JTE_SPMovesMobjProg (mobj_t* mobj)
{
	switch(mobj->type)
	{
		case MT_PAYMACHINE:
			mobj->fuse++;
			return true;
		case MT_PSOMONEY:
		case MT_PSOWEAPON:
		case MT_PSOSHIELD:
		case MT_PSOITEM:
			mobj->angle -= ANGLE_1 * 10;
			mobj->z = mobj->floorz;
			return true;
		case MT_PSOTELLER:
			mobj->fuse++;
			mobj->flags = (mobj->flags & ~MF_TRANSLATION) | ((4)<<MF_TRANSSHIFT);
			mobj->skin = &skins[1];
			return true;
		case MT_THZPLANT:
			if(eastermode)
				mobj->flags = (mobj->flags & ~MF_TRANSLATION) | ((P_Random() % MAXSKINCOLORS)<<MF_TRANSSHIFT);
			return true;
		case MT_DARKFAIRYCHAOSHOT:
			P_MobjCheckWater (mobj);
			if(mobj->z < mobj->watertop && mobj->z > mobj->waterbottom)
			   P_SetMobjState(mobj, S_DISS);
			return true;
	}
}

//
// JTE_InitSPMoves
//
// Sets up all the data for SPMoves' mobjs and stuff.
//
void JTE_InitSPMoves()
{
    fixed_t thing;

        thing = MT_FOXBULLET;
        mobjinfo[thing].doomednum = -1;
        mobjinfo[thing].spawnstate = S_JETBULLET1;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 32;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 200;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_XPLD1;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 60*FRACUNIT;
        mobjinfo[thing].radius = 4*FRACUNIT;
        mobjinfo[thing].height = 8*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 1;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_MISSILE|MF_NOGRAVITY;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_MULTIEGG;
        mobjinfo[thing].doomednum = -1;
        mobjinfo[thing].spawnstate = S_EEGG;
        mobjinfo[thing].spawnhealth = 1000;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 8;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 200;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 8*FRACUNIT;
        mobjinfo[thing].height = 24*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 1;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SPECIAL|MF_NOGRAVITY;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_JTETOKEN;
        mobjinfo[thing].doomednum = -1;
        mobjinfo[thing].spawnstate = S_JTETOKEN;
        mobjinfo[thing].spawnhealth = 1000;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 8;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 200;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 30*FRACUNIT;
        mobjinfo[thing].height = 51*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 1;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SPECIAL|MF_NOGRAVITY;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_LEAF;
        mobjinfo[thing].doomednum = -1;
        mobjinfo[thing].spawnstate = S_LEAF;
        mobjinfo[thing].spawnhealth = 1000;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 8;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 200;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = -8*FRACUNIT;
        mobjinfo[thing].radius = 4*FRACUNIT;
        mobjinfo[thing].height = 4*FRACUNIT;
        mobjinfo[thing].mass = 4;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_NOBLOCKMAP;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_PAYMACHINE;
        mobjinfo[thing].doomednum = 5000;
        mobjinfo[thing].spawnstate = S_PAYMACHINE;
        mobjinfo[thing].spawnhealth = 20;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 72*FRACUNIT;
        mobjinfo[thing].height = 144*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SPECIAL|MF_SPECIALFLAGS;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_PSODROP;
        mobjinfo[thing].doomednum = -1;
        mobjinfo[thing].spawnstate = S_PSODROP;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 16*FRACUNIT;
        mobjinfo[thing].height = 24*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_NOBLOCKMAP;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_PSOBOSSDROP;
        mobjinfo[thing].doomednum = 4998;
        mobjinfo[thing].spawnstate = S_PSODROP;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 16*FRACUNIT;
        mobjinfo[thing].height = 24*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_NOBLOCKMAP;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_PSOMONEY;
        mobjinfo[thing].doomednum = -1;
        mobjinfo[thing].spawnstate = S_PSOMONEY;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 16*FRACUNIT;
        mobjinfo[thing].height = 24*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SPECIAL|MF_NOGRAVITY;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_PSOWEAPON;
        mobjinfo[thing].doomednum = -1;
        mobjinfo[thing].spawnstate = S_PSOWEAPON;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 16*FRACUNIT;
        mobjinfo[thing].height = 24*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SPECIAL|MF_NOGRAVITY;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_PSOSHIELD;
        mobjinfo[thing].doomednum = -1;
        mobjinfo[thing].spawnstate = S_PSOSHIELD;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 16*FRACUNIT;
        mobjinfo[thing].height = 24*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SPECIAL|MF_NOGRAVITY;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_PSOITEM;
        mobjinfo[thing].doomednum = -1;
        mobjinfo[thing].spawnstate = S_PSOITEM;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 16*FRACUNIT;
        mobjinfo[thing].height = 24*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SPECIAL|MF_NOGRAVITY;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_PSOTELLER;
        mobjinfo[thing].doomednum = 4999;
        mobjinfo[thing].spawnstate = S_PLAY_STND;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 16*FRACUNIT;
        mobjinfo[thing].height = 56*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SPECIAL|MF_SPECIALFLAGS;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_GHZTREE;
        mobjinfo[thing].doomednum = 4997;
        mobjinfo[thing].spawnstate = S_GHZTREE;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 8*FRACUNIT;
        mobjinfo[thing].height = 180*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_SCENERY;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_SPMOVESCHAO;
        mobjinfo[thing].doomednum = -1;
        mobjinfo[thing].spawnstate = S_SPMOVESCHAO1;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 40*FRACUNIT;
        mobjinfo[thing].radius = 10*FRACUNIT;
        mobjinfo[thing].height = 18*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 1;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_NOCLIP|MF_NOGRAVITY;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_CWBALL;
        mobjinfo[thing].doomednum = 4996;
        mobjinfo[thing].spawnstate = S_CWBALL1;
        mobjinfo[thing].spawnhealth = 1000;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 32*FRACUNIT;
        mobjinfo[thing].radius = 10*FRACUNIT;
        mobjinfo[thing].height = 25*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 1;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SLIDEME|MF_SOLID|MF_PUSHABLE;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_CWFLOWER;
        mobjinfo[thing].doomednum = 4995;
        mobjinfo[thing].spawnstate = S_CWFLOWER1;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 11*FRACUNIT;
        mobjinfo[thing].height = 24*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_SCENERY;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_CWBUSH1;
        mobjinfo[thing].doomednum = 4994;
        mobjinfo[thing].spawnstate = S_CWBUSH1;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 32*FRACUNIT;
        mobjinfo[thing].height = 64*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_SCENERY;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_CWBUSH2;
        mobjinfo[thing].doomednum = 4993;
        mobjinfo[thing].spawnstate = S_CWBUSH2;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 0;
        mobjinfo[thing].radius = 24*FRACUNIT;
        mobjinfo[thing].height = 32*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_NOCLIP|MF_SCENERY;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_CWBUMPERCAR;
        mobjinfo[thing].doomednum = 4992;
        mobjinfo[thing].spawnstate = S_CWBUMPERCAR;
        mobjinfo[thing].spawnhealth = 1000;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 12*FRACUNIT;
        mobjinfo[thing].radius = 24*FRACUNIT;
        mobjinfo[thing].height = 48*FRACUNIT;
        mobjinfo[thing].mass = 1;
        mobjinfo[thing].damage = 0;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SOLID|MF_PUSHABLE|MF_BOUNCE|MF_NOGRAVITY|MF_BOSS;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_CWDOLL;
        mobjinfo[thing].doomednum = 4991;
        mobjinfo[thing].spawnstate = S_CWDOLL;
        mobjinfo[thing].spawnhealth = 1000;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 21*FRACUNIT;
        mobjinfo[thing].radius = 15*FRACUNIT;
        mobjinfo[thing].height = 44*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 1;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SLIDEME|MF_SOLID|MF_PUSHABLE;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_SKULL;
        mobjinfo[thing].doomednum = 4990;
        mobjinfo[thing].spawnstate = S_SKULL1;
        mobjinfo[thing].spawnhealth = 1000;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 21*FRACUNIT;
        mobjinfo[thing].radius = 10*FRACUNIT;
        mobjinfo[thing].height = 25*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 1;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SLIDEME|MF_SOLID|MF_PUSHABLE;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_PUMPKIN;
        mobjinfo[thing].doomednum = -1;//4989;
        mobjinfo[thing].spawnstate = S_PUMPKIN1;
        mobjinfo[thing].spawnhealth = 1000;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 21*FRACUNIT;
        mobjinfo[thing].radius = 10*FRACUNIT;
        mobjinfo[thing].height = 25*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 1;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SLIDEME|MF_SOLID|MF_PUSHABLE;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_APPLE;
        mobjinfo[thing].doomednum = -1;//4988;
        mobjinfo[thing].spawnstate = S_APPLE1;
        mobjinfo[thing].spawnhealth = 1000;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_NULL;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_None;
        mobjinfo[thing].speed = 21*FRACUNIT;
        mobjinfo[thing].radius = 10*FRACUNIT;
        mobjinfo[thing].height = 25*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 1;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SLIDEME|MF_SOLID|MF_PUSHABLE;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_DARKFAIRYCHAO;
        mobjinfo[thing].doomednum = 4987;
        mobjinfo[thing].spawnstate = S_DFCHAO1;
        mobjinfo[thing].spawnhealth = 5;
        mobjinfo[thing].seestate = S_DFCHAO2;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 2*TICRATE;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_DFCHAO1;
        mobjinfo[thing].painchance = 200;
        mobjinfo[thing].painsound = sfx_dmpain;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_XPLD1;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_pop;
        mobjinfo[thing].speed = 8;
        mobjinfo[thing].radius = 12*FRACUNIT;
        mobjinfo[thing].height = 24*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 2;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_SLIDEME|MF_ENEMY|MF_SPECIAL|MF_SHOOTABLE|MF_COUNTKILL;
        mobjinfo[thing].raisestate = MT_DARKFAIRYCHAOSHOT;

        thing = MT_DARKFAIRYCHAOSHOT;
        mobjinfo[thing].doomednum = -1;
        mobjinfo[thing].spawnstate = S_DFCHAOSHOT1;
        mobjinfo[thing].spawnhealth = 1;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_XPLD1;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_pop;
        mobjinfo[thing].speed = 40*FRACUNIT;
        mobjinfo[thing].radius = 12*FRACUNIT;
        mobjinfo[thing].height = 24*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 2;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_MISSILE|MF_NOGRAVITY|MF_FIRE;
        mobjinfo[thing].raisestate = S_NULL;

        thing = MT_PSOSHOT;
        mobjinfo[thing].doomednum = -1;
        mobjinfo[thing].spawnstate = S_ROCKET;
        mobjinfo[thing].spawnhealth = 1000;
        mobjinfo[thing].seestate = S_NULL;
        mobjinfo[thing].seesound = sfx_None;
        mobjinfo[thing].reactiontime = 0;
        mobjinfo[thing].attacksound = sfx_None;
        mobjinfo[thing].painstate = S_NULL;
        mobjinfo[thing].painchance = 0;
        mobjinfo[thing].painsound = sfx_None;
        mobjinfo[thing].meleestate = S_NULL;
        mobjinfo[thing].missilestate = S_NULL;
        mobjinfo[thing].deathstate = S_XPLD1;
        mobjinfo[thing].xdeathstate = S_NULL;
        mobjinfo[thing].deathsound = sfx_pop;
        mobjinfo[thing].speed = 50*FRACUNIT;
        mobjinfo[thing].radius = 11*FRACUNIT;
        mobjinfo[thing].height = 8*FRACUNIT;
        mobjinfo[thing].mass = 100;
        mobjinfo[thing].damage = 1;
        mobjinfo[thing].activesound = sfx_None;
        mobjinfo[thing].flags = MF_NOBLOCKMAP|MF_MISSILE|MF_NOGRAVITY;
        mobjinfo[thing].raisestate = S_NULL;

        states[S_EGGMOBILE_FLEE1].action.acv = A_CheckFlee;
        states[S_EGGMOBILE_FLEE2].action.acv = A_CheckFlee;

        states[S_EGGMOBILE2_FLEE1].action.acv = A_CheckFlee;
        states[S_EGGMOBILE2_FLEE2].action.acv = A_CheckFlee;

        states[S_BIRD2].action.acv = A_BirdFall;
        states[S_BIRD3].action.acv = A_BirdFlap;

        states[S_SQRL4].action.acv = A_BunnyHop;
        states[S_SQRL5].action.acv = A_BunnyHop;
        states[S_SQRL6].action.acv = A_BunnyHop;
        states[S_SQRL7].action.acv = A_BunnyHop;
        states[S_SQRL8].action.acv = A_BunnyHop;
        states[S_SQRL9].action.acv = A_BunnyHop;
        states[S_SQRL10].action.acv = A_BunnyHop;

        states[S_SIGN53].frame = 8;
        states[S_SIGN54].frame = 9;
        states[S_SIGN55].frame = 10;

        thing = S_LEAF;
        states[thing].sprite = SPR_LEAF;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_LEAF2;
        states[thing].sprite = SPR_LEAF;
        states[thing].frame = 1;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_PAYMACHINE;
        states[thing].sprite = SPR_CAPS;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_PSODROP;
        states[thing].sprite = SPR_DISS;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_PSOMONEY;
        states[thing].sprite = SPR_PSOM;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_PSOWEAPON;
        states[thing].sprite = SPR_PSOW;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_PSOSHIELD;
        states[thing].sprite = SPR_PSOS;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_PSOITEM;
        states[thing].sprite = SPR_PSOI;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_GHZTREE;
        states[thing].sprite = SPR_GHZT;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_SPMOVESCHAO1;
        states[thing].sprite = SPR_PLAY;
        states[thing].frame = 34;
        states[thing].tics = 3;
        states[thing].action.acv = A_SPMovesChao;
        states[thing].nextstate = S_SPMOVESCHAO2;

        thing = S_SPMOVESCHAO2;
        states[thing].sprite = SPR_PLAY;
        states[thing].frame = 35;
        states[thing].tics = 3;
        states[thing].action.acv = A_SPMovesChao;
        states[thing].nextstate = S_SPMOVESCHAO3;

        thing = S_SPMOVESCHAO3;
        states[thing].sprite = SPR_PLAY;
        states[thing].frame = 34;
        states[thing].tics = 3;
        states[thing].action.acv = A_SPMovesChao;
        states[thing].nextstate = S_SPMOVESCHAO4;

        thing = S_SPMOVESCHAO4;
        states[thing].sprite = SPR_PLAY;
        states[thing].frame = 36;
        states[thing].tics = 3;
        states[thing].action.acv = A_SPMovesChao;
        states[thing].nextstate = S_SPMOVESCHAO1;

        thing = S_CWBALL1;
        states[thing].sprite = SPR_CWGB;
        states[thing].frame = 0;
        states[thing].tics = 8;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_CWBALL2;

        thing = S_CWBALL2;
        states[thing].sprite = SPR_CWGB;
        states[thing].frame = 1;
        states[thing].tics = 8;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_CWBALL1;

        thing = S_CWFLOWER1;
        states[thing].sprite = SPR_CWF1;
        states[thing].frame = 0;
        states[thing].tics = 8;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_CWFLOWER2;

        thing = S_CWFLOWER2;
        states[thing].sprite = SPR_CWF1;
        states[thing].frame = 1;
        states[thing].tics = 8;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_CWFLOWER1;

        thing = S_CWBUSH1;
        states[thing].sprite = SPR_CWB1;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_CWBUSH2;
        states[thing].sprite = SPR_CWB2;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_CWBUMPERCAR;
        states[thing].sprite = SPR_CWBC;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_CWDOLL;
        states[thing].sprite = SPR_CWD1;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_SKULL1;
        states[thing].sprite = SPR_SKL1;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_PUMPKIN1;
        states[thing].sprite = SPR_PMK1;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_APPLE1;
        states[thing].sprite = SPR_APL1;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_DFCHAO1;
        states[thing].sprite = SPR_DFC1;
        states[thing].frame = 0;
        states[thing].tics = 1;
        states[thing].action.acv = A_CrawlaCommanderThink;
        states[thing].nextstate = S_DFCHAO1;

        thing = S_DFCHAO2;
        states[thing].sprite = SPR_DFC1;
        states[thing].frame = 0;
        states[thing].tics = 1;
        states[thing].action.acv = A_CrawlaCommanderThink;
        states[thing].nextstate = S_DFCHAO2;

        thing = S_DFCHAOSHOT1;
        states[thing].sprite = SPR_DFC2;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_SIGN56; // Zim!
        states[thing].sprite = SPR_SIGN;
        states[thing].frame = 7;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        thing = S_JTETOKEN;
        states[thing].sprite = SPR_JTET;
        states[thing].frame = 0;
        states[thing].tics = -1;
        states[thing].action.acv = NULL;
        states[thing].nextstate = S_NULL;

        sprnames[SPR_LEAF] = "LEAF";
        t_lspr[SPR_LEAF] = &lspr[NOLIGHT];

        sprnames[SPR_PSOM] = "PSOM";
        t_lspr[SPR_PSOM] = &lspr[NOLIGHT];
        sprnames[SPR_PSOW] = "PSOW";
        t_lspr[SPR_PSOW] = &lspr[NOLIGHT];
        sprnames[SPR_PSOS] = "PSOS";
        t_lspr[SPR_PSOS] = &lspr[NOLIGHT];
        sprnames[SPR_PSOI] = "PSOI";
        t_lspr[SPR_PSOI] = &lspr[NOLIGHT];

        sprnames[SPR_GHZT] = "GHZT";
        t_lspr[SPR_GHZT] = &lspr[NOLIGHT];

        sprnames[SPR_CWGB] = "CWGB";
        t_lspr[SPR_CWGB] = &lspr[NOLIGHT];
        sprnames[SPR_CWF1] = "CWF1";
        t_lspr[SPR_CWF1] = &lspr[NOLIGHT];
        sprnames[SPR_CWB1] = "CWB1";
        t_lspr[SPR_CWB1] = &lspr[NOLIGHT];
        sprnames[SPR_CWB2] = "CWB2";
        t_lspr[SPR_CWB2] = &lspr[NOLIGHT];
        sprnames[SPR_CWBC] = "CWBC";
        t_lspr[SPR_CWBC] = &lspr[NOLIGHT];
        sprnames[SPR_CWD1] = "CWD1";
        t_lspr[SPR_CWD1] = &lspr[NOLIGHT];

        sprnames[SPR_SKL1] = "SKL1";
        t_lspr[SPR_SKL1] = &lspr[NOLIGHT];
        sprnames[SPR_PMK1] = "PMK1";
        t_lspr[SPR_PMK1] = &lspr[NOLIGHT];
        sprnames[SPR_APL1] = "APL1";
        t_lspr[SPR_APL1] = &lspr[NOLIGHT];

        sprnames[SPR_DFC1] = "DFC1";
        t_lspr[SPR_DFC1] = &lspr[NOLIGHT];
        sprnames[SPR_DFC2] = "DFC2";
        t_lspr[SPR_DFC2] = &lspr[REDBALL_L];

        sprnames[SPR_JTET] = "JTET";
        t_lspr[SPR_JTET] = &lspr[NOLIGHT];

        sprnames[NUMSPRITES] = NULL;
}

//
// JTE_PSODamage
//
// Special damage calculations for PSO mode.
//
boolean JTE_PSODamage (mobj_t*   target,
                       mobj_t*   inflictor,
                       mobj_t*   source,
                       int       damage)
{
    unsigned    ang;
    player_t* player;
    boolean     takedamage;  // false on some case in teamplay

    player = target->player;
		if(player->nightsmode && !player->powers[pw_flashing])
		{
			int radius;
			const double deg2rad = 0.017453293;
			P_UnsetThingPosition(target);
			player->angle_pos = player->old_angle_pos;
			player->speed /= 5;
			player->flyangle += 180;
			player->flyangle %= 360;

			if(source && source->player)
			{
				if(player->nightstime > 20)
					player->nightstime -= 20;
				else
					player->nightstime = 1;
			}
			else
			{
				if(player->nightstime > 5)
					player->nightstime -= 5;
				else
					player->nightstime = 1;
			}

			radius = target->target->info->radius;

			target->x = target->target->x + cos(player->old_angle_pos * deg2rad) * radius;
			target->y = target->target->y + sin(player->old_angle_pos * deg2rad) * radius;

			target->momx = target->momy = 0;
			P_SetThingPosition(target);
			player->powers[pw_flashing] = flashingtics;
			P_SetMobjState(target->tracer, S_NIGHTSHURT1);
			S_StartSound(target, sfx_nghurt);			
		}

		if(inflictor && (inflictor->flags & MF_FIRE) && player->powers[pw_redshield])
			return false; // Invincible to fire objects

		if(source && source->player) // Player hits another player
		{
			extern consvar_t cv_friendlyfire; // Graue 12-28-2003

			if(source == target) // You can't kill yourself, idiot...
				return false;

			// Graue 12-28-2003
			if(!cv_friendlyfire.value && (cv_gametype.value == GT_COOP ||
				cv_gametype.value == GT_RACE || cv_gametype.value == GT_CHAOS
				#ifdef CIRCUITMODE
				|| cv_gametype.value == GT_CIRCUIT
				#endif
				))
				return false;

			if((cv_gametype.value == GT_MATCH || cv_gametype.value == GT_TAG || cv_gametype.value == GT_CTF) && cv_suddendeath.value
				&& !player->powers[pw_flashing] && !player->powers[pw_invulnerability])
				damage = 10000; // Instant-death!

			if(cv_gametype.value == GT_TAG) // Tag Mode!
			{
				int i;

				// If flashing, or in the tag zone, or invulnerable, ignore the tag.
				if(target->player->powers[pw_flashing] || target->player->tagzone || target->player->powers[pw_invulnerability])
					return false;

				if(player->health > 0)
				{
					CONS_Printf("* %s was tagged by %s!\n",player_names[target->player-players],
                            player_names[source->player-players]);
					S_StartSound(players[consoleplayer].mo, sfx_radio);
				}

				// Make the player IT!
				if(source->player->tagit < 298*TICRATE && source->player->tagit > 0)
				{
					target->player->tagit = 300*TICRATE + 1;
					source->player->tagit = 0;
				}

				// Award points to all those who didn't get tagged.
				for(i=0;i<MAXPLAYERS;i++)
				{
					if(playeringame[i])
					{
						if(&players[i] != source->player
							&& &players[i] != target->player)
							players[i].tagcount++;
					}
				}

				target->z++;

				if(target->eflags & MF_UNDERWATER)
					target->momz = 4.04269230769230769230769230769231*FRACUNIT;
				else
					target->momz = 6.9*FRACUNIT;

				if(source->player->mfjumped && source->momz < 0) // Bounce tagger off target
					source->momz = -source->momz;

				ang = R_PointToAngle2 ( inflictor->x,
										inflictor->y,
									    target->x,
								        target->y);

				P_InstaThrust (target, ang, 4*FRACUNIT);
				P_ResetPlayer(target->player);
				target->player->powers[pw_flashing] = flashingtics;
				P_SetMobjState(target, S_PLAY_PAIN);

				// Check for a shield
				if(target->player->powers[pw_redshield] || target->player->powers[pw_blueshield] || target->player->powers[pw_yellowshield] || target->player->powers[pw_blackshield] || target->player->powers[pw_greenshield])
				{
					target->player->powers[pw_redshield] = false;
					target->player->powers[pw_blueshield] = false;      //Get rid of shield
					target->player->powers[pw_yellowshield] = false;
					target->player->powers[pw_blackshield] = false;
					target->player->powers[pw_greenshield] = false;
					S_StartSound (target, sfx_shldls);
					return true;
				}

				if(target->health <= 1) // Death
				{
					P_PlayDeathSound(target);
					CONS_Printf("* %s was tagged to death by %s!\n",player_names[target->player-players],
                            player_names[source->player-players]);
					P_PlayVictorySound(source); // Killer laughs at you! LAUGHS! BWAHAHAHHAHAA!!
					P_PlayerRingBurst(target->player, target->player->psomoney); // Release your moneys!
				}
				else if(target->health > 1) // Ring loss
				{
					P_PlayRinglossSound(target);
					//P_PlayerRingBurst(target->player, target->player->mo->health - 1);
				}

				target->health -= damage;
				target->player->health = target->health;
				return true;
			}
			else if(cv_gametype.value == GT_CTF) // CTF
			{
				// Don't allow players on the same team to hurt one another.
				if(target->player->ctfteam == source->player->ctfteam)
				{
					target->health++;
					target->player->health++;
					return false;
				}
			}

			if(player->health > 0 && !(player->powers[pw_flashing] || player->powers[pw_invulnerability]))
			{
				CONS_Printf("* %s was hit by %s!\n",player_names[target->player-players],
					player_names[source->player-players]);
					S_StartSound(players[consoleplayer].mo, sfx_radio);
			}
		}

		if(damage == 10000)
		{
			player->powers[pw_redshield] = false;
			player->powers[pw_blueshield] = false;      //Get rid of shield
			player->powers[pw_yellowshield] = false;
			player->powers[pw_blackshield] = false;
			player->powers[pw_greenshield] = false;
			player->mo->momx = player->mo->momy = player->mo->momz = 0;

			player->powers[pw_fireflower] = false;
			player->mo->flags =  (player->mo->flags & ~MF_TRANSLATION)
                     | ((player->skincolor)<<MF_TRANSSHIFT);

			if(player->powers[pw_underwater] != 1) // Don't jump up when drowning
				player->mo->momz = 18*FRACUNIT;
			else
				player->mo->momz++;

			if(source && source->type == MT_DISS && source->threshold == 42) // drowned
				S_StartSound(target, sfx_drown);
			else
				P_PlayDeathSound(target);

			P_SetMobjState(target, S_PLAY_DIE1);
			//P_PlayerRingBurst(target->player, target->player->psomoney); // Release your moneys!
			if(cv_gametype.value == GT_CTF && (player->gotflag & MF_REDFLAG || player->gotflag & MF_BLUEFLAG))
				P_PlayerFlagBurst(player);
			if(source && source->player) // Tails 03-13-2001
			{
				P_AddPlayerScore(source->player, 100); // Tails 03-13-2001
			}
		}
		else if ( damage < 10000 // start ignore bouncing & such in invulnerability Tails 02-26-2000
			&& ( (player->cheats&CF_GODMODE)
			|| player->powers[pw_invulnerability] || player->powers[pw_flashing] || player->powers[pw_super]) )
		{
			return false;
		} // end ignore bouncing & such in invulnerability Tails 02-26-2000

		else if ( damage < 10000 && (player->powers[pw_redshield] || player->powers[pw_blueshield] || player->powers[pw_yellowshield] || player->powers[pw_blackshield] || player->powers[pw_greenshield]))  //If One-Hit Shield
		{
			player->powers[pw_blueshield]   = false;      //Get rid of shield
			player->powers[pw_greenshield]  = false;
			player->powers[pw_redshield] = false;
			if(player->powers[pw_yellowshield] >= 3)
				player->powers[pw_yellowshield] = false;
			else if(player->powers[pw_yellowshield])
				player->powers[pw_yellowshield]++;

			if(player->powers[pw_blackshield]) // Give them what's coming to them!
			{
				player->blackow = 1; // BAM!
				player->powers[pw_blackshield] = false;
				player->jumpdown = true;
			}
			damage = 0;                 //Don't take rings away
			player->mo->z++;

			if(player->mo->eflags & MF_UNDERWATER)
					player->mo->momz = 4.04269230769230769230769230769231*FRACUNIT;
			else
					player->mo->momz = 6.9*FRACUNIT;

			if(inflictor == NULL)
				P_InstaThrust (player->mo, -player->mo->angle, 4*FRACUNIT);
			else
			{
				ang = R_PointToAngle2 ( inflictor->x,
										inflictor->y,
										target->x,
										target->y);
				P_InstaThrust (target, ang, 4*FRACUNIT);
			}

			P_SetMobjState(target, target->info->painstate);
			target->player->powers[pw_flashing] = flashingtics; // Tails

			player->powers[pw_fireflower] = false;
			player->mo->flags =  (player->mo->flags & ~MF_TRANSLATION)
                     | ((player->skincolor)<<MF_TRANSSHIFT);

			P_ResetPlayer(player);

			if(source && (source->type == MT_DISS || source->type == MT_FLOORSPIKE || source->type == MT_CEILINGSPIKE) && source->threshold == 43) // spikes
				S_StartSound(target, sfx_spkdth);
			else
				S_StartSound (target, sfx_shldls); // Ba-Dum! Shield loss.

			if(cv_gametype.value == GT_CTF && (player->gotflag & MF_REDFLAG || player->gotflag & MF_BLUEFLAG))
				P_PlayerFlagBurst(player);
			if(source && source->player) // Tails 03-13-2001
			{
				extern consvar_t cv_match_scoring;
				P_AddPlayerScore(source->player, cv_match_scoring.value == 1 ? 25 : 50); // Graue 12-13-2003
			}
			return true;
		}
		else if(player->mo->health > 1) // No shield but have rings.
		{
			// Special stage style damage for circuit mode Graue 12-06-2003
			#ifdef CIRCUITMODE
			if(cv_gametype.value == GT_CIRCUIT && player->mo->health > 10) // You idiot, Graue! The health is number of rings PLUS ONE! Nasty bug fixed by Graue 12-22-2003
				damage = 10;
			else
			#endif
				damage = source->info->spawnhealth * 2 + (P_Random() % 3) - (P_Random() % 5);

			if(damage < 1)
				damage = 1;

			player->mo->z++;

			player->powers[pw_fireflower] = false;
			player->mo->flags =  (player->mo->flags & ~MF_TRANSLATION)
                     | ((player->skincolor)<<MF_TRANSSHIFT);

			if(player->mo->eflags & MF_UNDERWATER)
				player->mo->momz = 4.04269230769230769230769230769231*FRACUNIT;
			else
				player->mo->momz = 6.9*FRACUNIT;

			if(inflictor == NULL)
				P_InstaThrust (player->mo, -player->mo->angle, 4*FRACUNIT);
			else
			{
				ang = R_PointToAngle2 ( inflictor->x,
										inflictor->y,
										target->x,
										target->y);
				P_InstaThrust (target, ang, 4*FRACUNIT);
			}

			P_ResetPlayer(player);

			if(source && (source->type == MT_DISS || source->type == MT_FLOORSPIKE || source->type == MT_CEILINGSPIKE) && source->threshold == 43) // spikes
				S_StartSound(target, sfx_spkdth);

			// Ring loss sound plays despite hitting spikes
			if(player->mo->health > damage)
				P_PlayRinglossSound(target); // Ringledingle!
			else
			{
				P_PlayDeathSound(target);
				if(source && source->player) // Tails 03-13-2001
				{
					P_AddPlayerScore(source->player, 50); // Tails 03-13-2001
				}
			}

			if(cv_gametype.value == GT_CTF && (player->gotflag & MF_REDFLAG || player->gotflag & MF_BLUEFLAG))
				P_PlayerFlagBurst(player);
			if(source && source->player) // Tails 03-13-2001
			{
				P_AddPlayerScore(source->player, 50); // Tails 03-13-2001
			}
		}
		else // No shield, no rings, no invincibility.
		{
			damage = source->info->spawnhealth * 2 + (P_Random() % 3) - (P_Random() % 5);

			if(damage < 1)
				damage = 1;

			player->mo->momz = 18*FRACUNIT;
			player->mo->momx = player->mo->momy = 0;

			player->powers[pw_fireflower] = false;
			player->mo->flags =  (player->mo->flags & ~MF_TRANSLATION)
                     | ((player->skincolor)<<MF_TRANSSHIFT);

			P_ResetPlayer(player);

			if(source && (source->type == MT_DISS || source->type == MT_FLOORSPIKE || source->type == MT_CEILINGSPIKE) && source->threshold == 43) // Spikes
				S_StartSound(target, sfx_spkdth);
			else
				P_PlayDeathSound(target);

			//P_PlayerRingBurst(target->player, target->player->psomoney); // Release your moneys!
			if(cv_gametype.value == GT_CTF && (player->gotflag & MF_REDFLAG || player->gotflag & MF_BLUEFLAG))
				P_PlayerFlagBurst(player);
			if(source && source->player) // Tails 03-13-2001
			{
				P_AddPlayerScore(source->player, 100); // Tails 03-13-2001
			}
		}
        player->health -= damage;   // mirror mobj health here for Dave

        if (player->health < 0)
            player->health = 0;

        takedamage = true;

	if(player->health <= 0)
		P_PlayerRingBurst(player, player->psomoney); // SoM

	target->player->powers[pw_flashing] = flashingtics; // Tails

        //added:22-02-98: force feedback ??? electro-shock???
        if (player == &players[consoleplayer])
            I_Tactile (40,10,40+min(damage, 100)*2);

        // do the damage
        target->health -= damage;
        if (target->health <= 0)
        {
            P_KillMobj ( target, inflictor, source );
            return true;
        }

        P_SetMobjState (target, target->info->painstate);

		if(target->player)
			P_ResetPlayer(target->player);

		target->reactiontime = 0;           // we're awake now...
		return true;
}

//
// JTE_SpecialItems
//
// Special item handling! Called when the item is touched, returns true if the item has SPMoves programming.
//
boolean JTE_SpecialItems(mobj_t* toucher, mobj_t* special)
{
	player_t* player = toucher->player;
	mobj_t* dummymo;

	switch(special->type)
	{
		case MT_MULTIEGG: // Multiplayer easter egg thing
			if(!player || player != &players[consoleplayer] || &players[displayplayer] != &players[consoleplayer])
				return true;
			if(!(multieggs & special->health))
			{
				/*if(!server)
					P_SetMobjState(special, S_DISS);*/
				P_SpawnMobj(special->x, special->y, special->z, MT_SPARK);
				S_StartSound(toucher, sfx_ncitem);
				multieggs |= special->health;
			}
			return true;
		case MT_PAYMACHINE: // Pay Machine: Exchange rings for items!
			if(!player)
				return true;

			if(!(mapheaderinfo[gamemap-1].typeoflevel & TOL_PSO))
			{
				if(!(toucher->player->health >= special->health))
					return true;
	
				// Mark the player as 'pull into the capsule'
				toucher->player->capsule = special;
				toucher->player->capsule->reactiontime = 1;
				return true;
			}
			if(!(toucher->player->psomoney >= special->health))
				return true;

			// Mark the player as 'pull into the capsule'
		   	toucher->player->capsule = special;
			toucher->player->capsule->reactiontime = 1;
			return true;
		case MT_PSOMONEY:
			if(player && toucher->player->powers[pw_flashing] > flashingtics/2)
				return true;
			if(mariomode)
				P_SpawnMobj (special->x,special->y,special->z, MT_SPARK)->momz = special->momz;
			else
				P_SpawnMobj (special->x,special->y,special->z, MT_SPARK);

			if(player)
			{
				if(!(mapheaderinfo[gamemap-1].typeoflevel & TOL_PSO))
				{
					player->health += special->health;
					player->mo->health = player->health;
					player->totalring += special->health;
				}
				else
				{
					player->psomoney += special->health;
		       			if(player == &players[consoleplayer])
						CONS_Printf("* Collected %d Meseta!\n",special->health);
				}
			}
			if(!player)
				toucher->health += special->health;

			if((mapheaderinfo[gamemap-1].typeoflevel & TOL_NIGHTS) || cv_timeattacked.value) // Nights and PSO mode? Why not?
			{
				if(player)
				{
					player->linkcount++;
					player->linktimer = 2*TICRATE;
				}

				dummymo = P_SpawnMobj(special->x, special->y, special->z, MT_NIGHTSCORE);

				if(!player)
					P_SetMobjState(dummymo, dummymo->info->spawnstate+9);

				if(player)
				{
					if(player->linkcount < 10)
					{
						if(player->bonustime)
						{
							P_AddPlayerScore(player, player->linkcount*20);
							P_SetMobjState(dummymo, dummymo->info->xdeathstate+player->linkcount-1);
						}
						else
						{
							P_AddPlayerScore(player, player->linkcount*10);
							P_SetMobjState(dummymo, dummymo->info->spawnstate+player->linkcount-1);
						}
					}
					else
					{
						if(player->bonustime)
						{
							P_AddPlayerScore(player, 200);
							P_SetMobjState(dummymo, dummymo->info->xdeathstate+9);
						}
						else
						{
							P_AddPlayerScore(player, 100);
							P_SetMobjState(dummymo, dummymo->info->spawnstate+9);
						}
					}
				}
				dummymo->momz = FRACUNIT;
				dummymo->fuse = 3*TICRATE;
				if(toucher->target)
				{
					if(toucher->target->flags & MF_AMBUSH)
						P_InstaThrust(dummymo, R_PointToAngle2(dummymo->x, dummymo->y, toucher->target->x, toucher->target->y), 3*FRACUNIT);
					else
						P_InstaThrust(dummymo, R_PointToAngle2(toucher->target->x, toucher->target->y, dummymo->x, dummymo->y), 3*FRACUNIT);
				}
			}
			break;
		case MT_PSOWEAPON:
		case MT_PSOSHIELD:
		case MT_PSOITEM:
			if(!player)
				return true;
			if(player && toucher->player->powers[pw_flashing] > flashingtics/2)
				return true;
			if(mariomode)
				P_SpawnMobj (special->x,special->y,special->z, MT_SPARK)->momz = special->momz;
			else
				P_SpawnMobj (special->x,special->y,special->z, MT_SPARK);

		        player->psoitems[special->health]++;
			if(player == &players[consoleplayer])
				CONS_Printf("* Collected a %s!\n",psoitems[special->health].name);


			if((mapheaderinfo[gamemap-1].typeoflevel & TOL_NIGHTS) || cv_timeattacked.value) // Nights and PSO mode? Why not?
			{
				if(player)
				{
					player->linkcount++;
					player->linktimer = 2*TICRATE;
				}

				dummymo = P_SpawnMobj(special->x, special->y, special->z, MT_NIGHTSCORE);

				if(!player)
					P_SetMobjState(dummymo, dummymo->info->spawnstate+9);

				if(player)
				{
					if(player->linkcount < 10)
					{
						if(player->bonustime)
						{
							P_AddPlayerScore(player, player->linkcount*20);
							P_SetMobjState(dummymo, dummymo->info->xdeathstate+player->linkcount-1);
						}
						else
						{
							P_AddPlayerScore(player, player->linkcount*10);
							P_SetMobjState(dummymo, dummymo->info->spawnstate+player->linkcount-1);
						}
					}
					else
					{
						if(player->bonustime)
						{
							P_AddPlayerScore(player, 200);
							P_SetMobjState(dummymo, dummymo->info->xdeathstate+9);
						}
						else
						{
							P_AddPlayerScore(player, 100);
							P_SetMobjState(dummymo, dummymo->info->spawnstate+9);
						}
					}
				}
				dummymo->momz = FRACUNIT;
				dummymo->fuse = 3*TICRATE;
				if(toucher->target)
				{
					if(toucher->target->flags & MF_AMBUSH)
						P_InstaThrust(dummymo, R_PointToAngle2(dummymo->x, dummymo->y, toucher->target->x, toucher->target->y), 3*FRACUNIT);
					else
						P_InstaThrust(dummymo, R_PointToAngle2(toucher->target->x, toucher->target->y, dummymo->x, dummymo->y), 3*FRACUNIT);
				}
			}
			break;

		default:
			break;
	}
	return false;
}

void JTE_PSOQuest1()
{
    players[consoleplayer].lightdashdown = false;
    return;
}
void JTE_PSOQuest2()
{
    players[consoleplayer].lightdashdown = false;
    return;
}
void JTE_PSOQuest3()
{
    players[consoleplayer].lightdashdown = false;
    return;
}
void JTE_PSOQuest4()
{
    players[consoleplayer].lightdashdown = false;
    return;
}
void JTE_PSOQuest5()
{
    players[consoleplayer].lightdashdown = false;
    return;
}
void JTE_PSOQuest6()
{
    players[consoleplayer].lightdashdown = false;
    return;
}
void JTE_PSOQuest7()
{
    players[consoleplayer].lightdashdown = false;
    return;
}
void JTE_PSOQuest8()
{
    players[consoleplayer].lightdashdown = false;
    return;
}
void JTE_PSOQuest9()
{
    players[consoleplayer].lightdashdown = false;
    return;
}
void JTE_PSOQuest10()
{
    players[consoleplayer].lightdashdown = false;
    return;
}
void JTE_PSOQuest11()
{
    return;
}
void JTE_PSOQuest12()
{
    return;
}
void JTE_PSOQuest13()
{
    return;
}
void JTE_PSOQuest14()
{
    return;
}
void JTE_PSOQuest15()
{
    return;
}
void JTE_PSOQuest16()
{
    return;
}
void JTE_PSOQuest17()
{
    return;
}
void JTE_PSOQuest18()
{
    return;
}
void JTE_PSOQuest19()
{
    return;
}
void JTE_PSOQuest20()
{
    I_Quit ();
    return;
}
#endif

