// SU - SRB2 Universe
// By Jason the Echidna
// MINE! NO STEALY!

#include "../../doomstat.h"
#ifdef UNIVERSE

#include "../../info.h"
#include "SU_Type.h"


// SU items! Yay!
suitem_t suitems[NUMSUITEMS];
static suitminfo_t iteminfo[NUMINFOTYPES];

static inline unsigned int SU_CalcInc(unsigned int base, unsigned int max, unsigned int num, unsigned int maxnum)
{
	unsigned int val, inc;
	inc = (max - base) / maxnum+1;
	val = (base + (inc*num));
	if(val >= max)
		val = max;
	return val;
}

static inline unsigned int SU_CalcDec(unsigned int base, unsigned int max, unsigned int num, unsigned int maxnum)
{
	unsigned int val, inc;
	inc = (max - base) / maxnum;
	val = (max - (inc*num));
	if(val <= base)
		val = base;
	return val;
}

static unsigned int SU_CalcItemDamage(suitemtype_t i)
{
	suitminfo_t *info;

	// Damage already set
	if(suitems[i].damage)
		return suitems[i].damage;

	// Find weapon type
	if(suitems[i].type & SUW_SABER)
		info = &iteminfo[info_saber];
	else if(suitems[i].type & SUW_SWORD)
		info = &iteminfo[info_sword];
	else if(suitems[i].type & SUW_DAGGER)
		info = &iteminfo[info_dagger];
	else if(suitems[i].type & SUT_BARRIER)
		info = &iteminfo[info_barrier];
	else if(suitems[i].type & SUT_FRAME)
		info = &iteminfo[info_frame];
	// Not found? No damage.
	else
		return 0;

	// Obviously not in the autogeneration list.
	if(i > info->last || i < info->first)
		return 0;

	// Calculate slope
	return SU_CalcInc(info->damage.base, info->damage.max,
		(unsigned int)(i - info->first), (unsigned int)(info->last - info->first));
}

static byte SU_CalcItemRarity(suitemtype_t i)
{
	suitminfo_t *info;

	// Rarity already set
	if(suitems[i].rarity)
		return suitems[i].rarity;

	// Find weapon type
	if(suitems[i].type & SUW_SABER)
		info = &iteminfo[info_saber];
	else if(suitems[i].type & SUW_SWORD)
		info = &iteminfo[info_sword];
	else if(suitems[i].type & SUW_DAGGER)
		info = &iteminfo[info_dagger];
	else if(suitems[i].type & SUT_BARRIER)
		info = &iteminfo[info_barrier];
	else if(suitems[i].type & SUT_FRAME)
		info = &iteminfo[info_frame];
	// Not found? No damage.
	else
		return 0;

	// Obviously not in the autogeneration list.
	if(i > info->last || i < info->first)
		return 0;

	// Calculate slope
	return (byte)SU_CalcDec(info->rarity.base, info->rarity.max,
		(unsigned int)(i - info->first), (unsigned int)(info->last - info->first));
}

void SU_InitItems(void)
{
	unsigned long i;
	suitminfo_t *info;
	suitem_t *itm;

	memset(iteminfo,0,sizeof(iteminfo));
	memset(suitems,0,sizeof(suitems));

	// Sabers
	info = &iteminfo[info_saber];
	info->first = itm_saber;
	info->last = itm_gladius;
	info->damage.base = 10;
	info->damage.max = 80;
	info->rarity.base = 20;
	info->rarity.max = 99;

	// Swords
	info = &iteminfo[info_sword];
	info->first = itm_sword;
	info->last = itm_calibur;
	info->damage.base = 8;
	info->damage.max = 60;
	info->rarity.base = 40;
	info->rarity.max = 90;

	// Daggers
	info = &iteminfo[info_dagger];
	info->first = itm_dagger;
	info->last = itm_ripper;
	info->damage.base = 6;
	info->damage.max = 50;
	info->rarity.base = 30;
	info->rarity.max = 80;

	// Partisans
	info = &iteminfo[info_partisan];
	info->first = itm_partisan;
	info->last = itm_gungnir;
	info->damage.base = 6;
	info->damage.max = 50;
	info->rarity.base = 40;
	info->rarity.max = 90;

	// Barriers
	info = &iteminfo[info_barrier];
	info->first = itm_barrier;
	info->last = itm_celestialshield;
	info->damage.base = 10;
	info->damage.max = 500;
	info->rarity.base = 1;
	info->rarity.max = 90;

	// Frames
	info = &iteminfo[info_frame];
	info->first = itm_frame;
	info->last = itm_celestialarmor;
	info->damage.base = 10;
	info->damage.max = 500;
	info->rarity.base = 10;
	info->rarity.max = 99;

	// Item-specific details
	for(i = 0; i < NUMSUITEMS; i++)
	{
		itm = &suitems[i];
		itm->name = NULL;
		itm->type = SUT_SPECIAL;
		itm->firetype = MT_DISS;
		itm->playertracer = MT_DISS;
		itm->usesound = sfx_None;

		switch(i)
		{
			// Sabers
			case itm_saber:
				itm->name = "saber";
				itm->type = SUT_WEAPON|SUW_SABER;
				break;
			case itm_brand:
				itm->name = "brand";
				itm->type = SUT_WEAPON|SUW_SABER;
				break;
			case itm_buster:
				itm->name = "buster";
				itm->type = SUT_WEAPON|SUW_SABER;
				break;
			case itm_pallasch:
				itm->name = "pallasch";
				itm->type = SUT_WEAPON|SUW_SABER;
				break;
			case itm_gladius:
				itm->name = "gladius";
				itm->type = SUT_WEAPON|SUW_SABER;
				break;

			// Swords
			case itm_sword:
				itm->name = "sword";
				itm->type = SUT_WEAPON|SUW_SWORD;
				break;
			case itm_gigush:
				itm->name = "gigush";
				itm->type = SUT_WEAPON|SUW_SWORD;
				break;
			case itm_breaker:
				itm->name = "breaker";
				itm->type = SUT_WEAPON|SUW_SWORD;
				break;
			case itm_claymore:
				itm->name = "claymore";
				itm->type = SUT_WEAPON|SUW_SWORD;
				break;
			case itm_calibur:
				itm->name = "calibur";
				itm->type = SUT_WEAPON|SUW_SWORD;
				break;

			// Daggers
			case itm_dagger:
				itm->name = "dagger";
				itm->type = SUT_WEAPON|SUW_DAGGER;
				break;
			case itm_knife:
				itm->name = "knife";
				itm->type = SUT_WEAPON|SUW_DAGGER;
				break;
			case itm_blade:
				itm->name = "blade";
				itm->type = SUT_WEAPON|SUW_DAGGER;
				break;
			case itm_edge:
				itm->name = "edge";
				itm->type = SUT_WEAPON|SUW_DAGGER;
				break;
			case itm_ripper:
				itm->name = "ripper";
				itm->type = SUT_WEAPON|SUW_DAGGER;
				break;

			// Partisans
			case itm_partisan:
				itm->name = "partisan";
				itm->type = SUT_WEAPON|SUW_PARTISAN;
				break;
			case itm_halibert:
				itm->name = "halibert";
				itm->type = SUT_WEAPON|SUW_PARTISAN;
				break;
			case itm_glaive:
				itm->name = "glaive";
				itm->type = SUT_WEAPON|SUW_PARTISAN;
				break;
			case itm_berdys:
				itm->name = "berdys";
				itm->type = SUT_WEAPON|SUW_PARTISAN;
				break;
			case itm_gungnir:
				itm->name = "gungnir";
				itm->type = SUT_WEAPON|SUW_PARTISAN;
				break;

			// Slicers
			case itm_slicer:
				itm->name = "slicer";
				itm->type = SUT_WEAPON|SUW_SLICER;
				break;
			case itm_spinner:
				itm->name = "spinner";
				itm->type = SUT_WEAPON|SUW_SLICER;
				break;
			case itm_cutter:
				itm->name = "cutter";
				itm->type = SUT_WEAPON|SUW_SLICER;
				break;
			case itm_sawcer:
				itm->name = "sawcer";
				itm->type = SUT_WEAPON|SUW_SLICER;
				break;
			case itm_diska:
				itm->name = "diska";
				itm->type = SUT_WEAPON|SUW_SLICER;
				break;

			// Handguns
			case itm_handgun:
				itm->name = "handgun";
				itm->type = SUT_WEAPON|SUW_HANDGUN;
				break;
			case itm_autogun:
				itm->name = "autogun";
				itm->type = SUT_WEAPON|SUW_HANDGUN;
				break;
			case itm_lockgun:
				itm->name = "lockgun";
				itm->type = SUT_WEAPON|SUW_HANDGUN;
				break;
			case itm_railgun:
				itm->name = "railgun";
				itm->type = SUT_WEAPON|SUW_HANDGUN;
				break;

			// Rifles
			case itm_rifle:
				itm->name = "rifle";
				itm->type = SUT_WEAPON|SUW_RIFLE;
				break;
			case itm_sniper:
				itm->name = "sniper";
				itm->type = SUT_WEAPON|SUW_RIFLE;
				break;
			case itm_blaster:
				itm->name = "blaster";
				itm->type = SUT_WEAPON|SUW_RIFLE;
				break;
			case itm_beam:
				itm->name = "beam";
				itm->type = SUT_WEAPON|SUW_RIFLE;
				break;
			case itm_laser:
				itm->name = "laser";
				itm->type = SUT_WEAPON|SUW_RIFLE;
				break;

			// Mechguns
			case itm_mechgun:
				itm->name = "mechgun";
				itm->type = SUT_WEAPON|SUW_MECHGUN;
				break;
			case itm_assault:
				itm->name = "assault";
				itm->type = SUT_WEAPON|SUW_MECHGUN;
				break;
			case itm_repeater:
				itm->name = "assault";
				itm->type = SUT_WEAPON|SUW_MECHGUN;
				break;
			case itm_gatling:
				itm->name = "gatling";
				itm->type = SUT_WEAPON|SUW_MECHGUN;
				break;
			case itm_vulcan:
				itm->name = "vulcan";
				itm->type = SUT_WEAPON|SUW_MECHGUN;
				break;

			// Shots
			case itm_shot:
				itm->name = "shot";
				itm->type = SUT_WEAPON|SUW_SHOT;
				break;
			case itm_spread:
				itm->name = "spread";
				itm->type = SUT_WEAPON|SUW_SHOT;
				break;
			case itm_cannon:
				itm->name = "cannon";
				itm->type = SUT_WEAPON|SUW_SHOT;
				break;
			case itm_launcher:
				itm->name = "launcher";
				itm->type = SUT_WEAPON|SUW_SHOT;
				break;
			case itm_arms:
				itm->name = "arms";
				itm->type = SUT_WEAPON|SUW_SHOT;
				break;

			// Canes
			case itm_cane:
				itm->name = "cane";
				itm->type = SUT_WEAPON|SUW_CANE;
				break;
			case itm_stick:
				itm->name = "stick";
				itm->type = SUT_WEAPON|SUW_CANE;
				break;
			case itm_mace:
				itm->name = "mace";
				itm->type = SUT_WEAPON|SUW_CANE;
				break;
			case itm_club:
				itm->name = "club";
				itm->type = SUT_WEAPON|SUW_CANE;
				break;

			// Rods
			case itm_rod:
				itm->name = "rod";
				itm->type = SUT_WEAPON|SUW_ROD;
				break;
			case itm_pole:
				itm->name = "pole";
				itm->type = SUT_WEAPON|SUW_ROD;
				break;
			case itm_pillar:
				itm->name = "pillar";
				itm->type = SUT_WEAPON|SUW_ROD;
				break;
			case itm_striker:
				itm->name = "striker";
				itm->type = SUT_WEAPON|SUW_ROD;
				break;

			// Wands
			case itm_wand:
				itm->name = "wand";
				itm->type = SUT_WEAPON|SUW_WAND;
				break;
			case itm_staff:
				itm->name = "staff";
				itm->type = SUT_WEAPON|SUW_WAND;
				break;
			case itm_baton:
				itm->name = "baton";
				itm->type = SUT_WEAPON|SUW_WAND;
				break;
			case itm_scepter:
				itm->name = "scepter";
				itm->type = SUT_WEAPON|SUW_WAND;
				break;

			// Claws
			case itm_photonclaw:
				itm->name = "photon claw";
				itm->type = SUT_WEAPON|SUW_CLAW;
				break;
			case itm_silenceclaw:
				itm->name = "silence claw";
				itm->type = SUT_WEAPON|SUW_CLAW;
				break;
			case itm_neisclaw:
				itm->name = "nei's claw";
				itm->type = SUT_WEAPON|SUW_CLAW;
				break;

			// Knuckles
			case itm_braveknuckle:
				itm->name = "brave knuckle";
				itm->type = SUT_WEAPON|SUW_KNUCKLE;
				break;
			case itm_angryfist:
				itm->name = "angry fist";
				itm->type = SUT_WEAPON|SUW_KNUCKLE;
				break;
			case itm_godhand:
				itm->name = "god hand";
				itm->type = SUT_WEAPON|SUW_KNUCKLE;
				break;
			case itm_sonicknuckle:
				itm->name = "sonic knuckle";
				itm->type = SUT_WEAPON|SUW_KNUCKLE;
				break;

			// Red weapons
			case itm_redsaber:
				itm->name = "red saber";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SABER;
				break;
			case itm_redsword: 
				itm->name = "red sword";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SWORD;
				break;
			case itm_reddagger:
				itm->name = "red dagger";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_DAGGER;
				break;
			case itm_redpartisan: 
				itm->name = "red partisan";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_PARTISAN;
				break;
			case itm_redslicer: 
				itm->name = "red slicer";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SLICER;
				break;
			case itm_redhandgun: 
				itm->name = "red handgun";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_HANDGUN;
				break;
			case itm_redmechgun: 
				itm->name = "red mechgun";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_MECHGUN;
				break;

			// Special items, Set 1
			case itm_doublesaber: 
				itm->name = "double saber";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_DOUBLESABER;
				break;
			case itm_twinbrand: 
				itm->name = "twin brand";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_DOUBLESABER;
				break;
			case itm_spreadneedle: 
				itm->name = "spread needle";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SPREAD;
				break;
			case itm_holyray:
				itm->name = "holy ray";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_HANDGUN;
				break;
			case itm_psychogun:
				itm->name = "psychogun";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_HANDGUN;
				break;
			case itm_heavenpunisher:
				itm->name = "heaven punisher";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_HANDGUN;
				break;

			// Special items, Set 2 (Saber types)
			case itm_akikosfryingpan:
				itm->name = "akiko's frying pan";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SABER;
				break;
			case itm_ancientsaber:
				itm->name = "ancient saber";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SABER;
				break;
			case itm_battlefan:
				itm->name = "battle fan";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SABER;
				break;
			case itm_toyhammer:
				itm->name = "toy hammer";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SABER;
				break;
			case itm_crazytune:
				itm->name = "crazy tune";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SABER;
				break;
			case itm_gamemagazine:
				itm->name = "game magazine";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SABER;
				break;
			case itm_flowerbouquet:
				itm->name = "flower bouquet";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SABER;
				break;
			
			// Special items, Set 3 (Punch)
			case itm_rocketpunch:
				itm->name = "rocket punch";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SABER;
				break;
			case itm_drilllauncher:
				itm->name = "drill launcher";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SABER;
				break;

			// Special items, Set 4 (Other)
			case itm_eggblaster:
				itm->name = "egg blaster";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_HANDGUN;
				break;
			case itm_sambamaracas:
				itm->name = "samba maracas";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_MECHGUN;
				break;
			case itm_strikerofchao:
				itm->name = "striker of chao";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_SWORD;
				break;
			case itm_masterbeam:
				itm->name = "master beam";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_RIFLE;
				break;

			// JTE's stuff
			case itm_jteblade:
				itm->name = "jte's blades";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_DAGGER;
				break;
			case itm_jterailgun:
				itm->name = "jte's railgun";
				itm->type = SUT_SPECIAL|SUT_WEAPON|SUW_RIFLE;
				break;

			// Barriers
			case itm_barrier:
				itm->name = "barrier";
				itm->type = SUT_BARRIER;
				break;
			case itm_shield:
				itm->name = "shield";
				itm->type = SUT_BARRIER;
				break;
			case itm_coreshield:
				itm->name = "core shield";
				itm->type = SUT_BARRIER;
				break;
			case itm_gigashield:
				itm->name = "giga shield";
				itm->type = SUT_BARRIER;
				break;
			case itm_soulbarrier:
				itm->name = "soul barrier";
				itm->type = SUT_BARRIER;
				break;
			case itm_hardshield:
				itm->name = "hard shield";
				itm->type = SUT_BARRIER;
				break;
			case itm_bravebarrier:
				itm->name = "brave barrier";
				itm->type = SUT_BARRIER;
				break;
			case itm_solidshield:
				itm->name = "solid shield";
				itm->type = SUT_BARRIER;
				break;
			case itm_flamebarrier:
				itm->name = "flame barrier";
				itm->type = SUT_BARRIER;
				break;
			case itm_plasmabarrier:
				itm->name = "plasma barrier";
				itm->type = SUT_BARRIER;
				break;
			case itm_freezebarrier:
				itm->name = "freeze barrier";
				itm->type = SUT_BARRIER;
				break;
			case itm_psychicbarrier:
				itm->name = "psychic barrier";
				itm->type = SUT_BARRIER;
				break;
			case itm_generalshield:
				itm->name = "general shield";
				itm->type = SUT_BARRIER;
				break;
			case itm_protectbarrier:
				itm->name = "protect barrier";
				itm->type = SUT_BARRIER;
				break;
			case itm_gloriousshield:
				itm->name = "glorious shield";
				itm->type = SUT_BARRIER;
				break;
			case itm_imperialbarrier:
				itm->name = "imperial barrier";
				itm->type = SUT_BARRIER;
				break;
			case itm_guardiansword:
				// Yes. Guardian sword is a BARRIER.
				itm->name = "guardian sword";
				itm->type = SUT_BARRIER;
				break;
			case itm_divinitybarrier:
				itm->name = "divinity barrier";
				itm->type = SUT_BARRIER;
				break;
			case itm_ultimateshield:
				itm->name = "ultimate shield";
				itm->type = SUT_BARRIER;
				break;
			case itm_spiritualshield:
				itm->name = "spiritual shield";
				itm->type = SUT_BARRIER;
				break;
			case itm_celestialshield:
				itm->name = "celestial shield";
				itm->type = SUT_BARRIER;
				break;

			// Special barriers
			case itm_invisibleguard:
				itm->name = "invisible guard";
				itm->type = SUT_BARRIER;
				break;
			case itm_sacredguard:
				itm->name = "sacred guard";
				itm->type = SUT_BARRIER;
				break;
			case itm_spartsver116:
				itm->name = "s-parts ver1.16";
				itm->type = SUT_BARRIER;
				break;
			case itm_spartsver201:
				itm->name = "s-parts ver2.01";
				itm->type = SUT_BARRIER;
				break;
			case itm_forcewall:
				itm->name = "force wall";
				itm->type = SUT_BARRIER;
				break;
			case itm_rangerwall:
				itm->name = "ranger wall";
				itm->type = SUT_BARRIER;
				break;
			case itm_hunterwall:
				itm->name = "hunter wall";
				itm->type = SUT_BARRIER;
				break;
			case itm_attributewall:
				itm->name = "attribute wall";
				itm->type = SUT_BARRIER;
				break;
			case itm_secretgear:
				itm->name = "secret gear";
				itm->type = SUT_BARRIER;
				break;
			case itm_combatgear:
				itm->name = "combat gear";
				itm->type = SUT_BARRIER;
				break;
			case itm_protoregenegear:
				itm->name = "proto regene gear";
				itm->type = SUT_BARRIER;
				break;
			case itm_regenerategear:
				itm->name = "regenerate gear";
				itm->type = SUT_BARRIER;
				break;
			case itm_regenegearadv:
				itm->name = "regene gear adv.";
				itm->type = SUT_BARRIER;
				break;
			case itm_custombarrierver00:
				itm->name = "custom barrier ver00";
				itm->type = SUT_BARRIER;
				break;
			case itm_tripolicshield:
				itm->name = "tripolic shield";
				itm->type = SUT_BARRIER;
				break;
			case itm_standstillshield:
				itm->name = "standstill shield";
				itm->type = SUT_BARRIER;
				break;
			case itm_saftyheart:
				itm->name = "safty heart";
				itm->type = SUT_BARRIER;
				break;
			case itm_kasamibracer:
				itm->name = "kasami bracer";
				itm->type = SUT_BARRIER;
				break;

			// God's shields
			case itm_godsshieldSUZAKU:
				itm->name = "god's shield SUZAKU";
				itm->type = SUT_BARRIER;
				break;
			case itm_godsshieldGENBU:
				itm->name = "god's shield GENBU";
				itm->type = SUT_BARRIER;
				break;
			case itm_godsshieldBYAKKO:
				itm->name = "god's shield BYAKKO";
				itm->type = SUT_BARRIER;
				break;
			case itm_godsshieldSEIRYU:
				itm->name = "god's shield SEIRYU";
				itm->type = SUT_BARRIER;
				break;

			// Rings
			case itm_redring:
				itm->name = "red ring";
				itm->type = SUT_BARRIER;
				break;
			case itm_bluering:
				itm->name = "blue ring";
				itm->type = SUT_BARRIER;
				break;
			case itm_yellowring:
				itm->name = "yellow ring";
				itm->type = SUT_BARRIER;
				break;
			case itm_purplering:
				itm->name = "purple ring";
				itm->type = SUT_BARRIER;
				break;
			case itm_greenring:
				itm->name = "green ring";
				itm->type = SUT_BARRIER;
				break;
			case itm_blackring:
				itm->name = "black ring";
				itm->type = SUT_BARRIER;
				break;
			case itm_whitering:
				itm->name = "white ring";
				itm->type = SUT_BARRIER;
				break;

			// Frames
			case itm_frame:
				itm->name = "frame";
				itm->type = SUT_FRAME;
				break;
			case itm_armor:
				itm->name = "armor";
				itm->type = SUT_FRAME;
				break;
			case itm_psyarmor:
				itm->name = "psy armor";
				itm->type = SUT_FRAME;
				break;
			case itm_gigaframe:
				itm->name = "giga frame";
				itm->type = SUT_FRAME;
				break;
			case itm_soulframe:
				itm->name = "soul frame";
				itm->type = SUT_FRAME;
				break;
			case itm_crossarmor:
				itm->name = "cross armor";
				itm->type = SUT_FRAME;
				break;
			case itm_solidframe:
				itm->name = "solid frame";
				itm->type = SUT_FRAME;
				break;
			case itm_bravearmor:
				itm->name = "brave armor";
				itm->type = SUT_FRAME;
				break;
			case itm_hyperframe:
				itm->name = "hyper frame";
				itm->type = SUT_FRAME;
				break;
			case itm_grandarmor:
				itm->name = "grand armor";
				itm->type = SUT_FRAME;
				break;
			case itm_shockframe:
				itm->name = "shock frame";
				itm->type = SUT_FRAME;
				break;
			case itm_kingsframe:
				itm->name = "king's frame";
				itm->type = SUT_FRAME;
				break;
			case itm_dragonframe:
				itm->name = "dragon frame";
				itm->type = SUT_FRAME;
				break;
			case itm_absorbarmor:
				itm->name = "absorb armor";
				itm->type = SUT_FRAME;
				break;
			case itm_protectframe:
				itm->name = "protect frame";
				itm->type = SUT_FRAME;
				break;
			case itm_generalarmor:
				itm->name = "general armor";
				itm->type = SUT_FRAME;
				break;
			case itm_perfectframe:
				itm->name = "perfect frame";
				itm->type = SUT_FRAME;
				break;
			case itm_valiantframe:
				itm->name = "valiant frame";
				itm->type = SUT_FRAME;
				break;
			case itm_imperialarmor:
				itm->name = "imperial armor";
				itm->type = SUT_FRAME;
				break;
			case itm_holinessarmor:
				itm->name = "holiness armor";
				itm->type = SUT_FRAME;
				break;
			case itm_guardianarmor:
				itm->name = "guardian armor";
				itm->type = SUT_FRAME;
				break;
			case itm_divinityarmor:
				itm->name = "divinity armor";
				itm->type = SUT_FRAME;
				break;
			case itm_ultimateframe:
				itm->name = "ultimate frame";
				itm->type = SUT_FRAME;
				break;
			case itm_celestialarmor:
				itm->name = "celestial armor";
				itm->type = SUT_FRAME;
				break;

			// Fields
			case itm_hunterfield:
				itm->name = "hunter field";
				itm->type = SUT_FRAME;
				break;
			case itm_rangerfield:
				itm->name = "ranger field";
				itm->type = SUT_FRAME;
				break;
			case itm_forcefield:
				itm->name = "force field";
				itm->type = SUT_FRAME;
				break;

			// Special frames
			case itm_revivalgarment:
				itm->name = "revival garment";
				itm->type = SUT_FRAME;
				break;
			case itm_spiritgarment:
				itm->name = "spirit garment";
				itm->type = SUT_FRAME;
				break;
			case itm_stinkframe:
				itm->name = "stink frame";
				itm->type = SUT_FRAME;
				break;
			case itm_senseplate:
				itm->name = "sense plate";
				itm->type = SUT_FRAME;
				break;
			case itm_gravitonplate:
				itm->name = "graviton plate";
				itm->type = SUT_FRAME;
				break;
			case itm_attributeplate:
				itm->name = "attribute plate";
				itm->type = SUT_FRAME;
				break;
			case itm_customframev00:
				itm->name = "custom frame v00";
				itm->type = SUT_FRAME;
				break;
			case itm_guardwave:
				itm->name = "guard wave";
				itm->type = SUT_FRAME;
				break;
			case itm_luminousfield:
				itm->name = "luminousfield";
				itm->type = SUT_FRAME;
				break;
			case itm_chuchufever:
				itm->name = "chu chu fever";
				itm->type = SUT_FRAME;
				break;
			case itm_loveheart:
				itm->name = "love heart";
				itm->type = SUT_FRAME;
				break;
			case itm_flamegarment:
				itm->name = "flame garment";
				itm->type = SUT_FRAME;
				break;
			case itm_virusarmorlafuteria:
				itm->name = "virus armor lafuteria";
				itm->type = SUT_FRAME;
				break;
			case itm_brightnesscircle:
				itm->name = "brightness circle";
				itm->type = SUT_FRAME;
				break;
			case itm_aurafield:
				itm->name = "aura field";
				itm->type = SUT_FRAME;
				break;
			case itm_electroframe:
				itm->name = "electro frame";
				itm->type = SUT_FRAME;
				break;
			case itm_sacredcloth:
				itm->name = "sacred cloth";
				itm->type = SUT_FRAME;
				break;
			case itm_smokingplate:
				itm->name = "smoking plate";
				itm->type = SUT_FRAME;
				break; 	

			default:
				break;
		}
		// Slope calculation
		itm->damage = SU_CalcItemDamage(i);
		itm->rarity = SU_CalcItemRarity(i);

		// Comment this out when the slope calculation isn't buggy! X_x
		if(itm->name && itm->damage && itm->rarity)
			CONS_Printf("Item: %21s | Damage: %3d | Rarity: %3d\n",itm->name,itm->damage,itm->rarity);
	}
}

#endif
