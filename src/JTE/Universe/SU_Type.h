// SU - SRB2 Universe
// By Jason the Echidna
// MINE! NO STEALY!

#ifdef UNIVERSE
#ifndef __SU_TYPE__
#define __SU_TYPE__

// SU items! Yes, I am that insane, thank you very much!
typedef enum
{
	itm_invalid = 0,

	// Weapons: Sabers (Sword type)
	itm_saber,
	itm_brand,
	itm_buster,
	itm_pallasch,
	itm_gladius,

	// Weapons: Swords (Longsword type)
	itm_sword,
	itm_gigush,
	itm_breaker,
	itm_claymore,
	itm_calibur,

	// Weapons: Daggers (Double-hit Sword type)
	itm_dagger,
	itm_knife,
	itm_blade,
	itm_edge,
	itm_ripper,

	// Weapons: Partisans (Longsword type)
	itm_partisan,
	itm_halibert,
	itm_glaive,
	itm_berdys,
	itm_gungnir,

	// Weapons: Slicers (Shoot type)
	itm_slicer,
	itm_spinner,
	itm_cutter,
	itm_sawcer,
	itm_diska,

	// Weapons: Handguns (Shoot type)
	itm_handgun,
	itm_autogun,
	itm_lockgun,
	itm_railgun,

	// Weapons: Rifles (Shoot type)
	itm_rifle,
	itm_sniper,
	itm_blaster,
	itm_beam,
	itm_laser,

	// Weapons: Mechguns (Triple-Shot Double-Gun type)
	itm_mechgun,
	itm_assault,
	itm_repeater,
	itm_gatling,
	itm_vulcan,

	// Weapons: Shot (Triple-Spread type)
	itm_shot,
	itm_spread,
	itm_cannon,
	itm_launcher,
	itm_arms,

	// Weapons: Cane (Sword type)
	itm_cane,
	itm_stick,
	itm_mace,
	itm_club,

	// Weapons: Rod (Longsword type)
	itm_rod,
	itm_pole,
	itm_pillar,
	itm_striker,

	// Weapons: Wand (Double-hit sword type)
	itm_wand,
	itm_staff,
	itm_baton,
	itm_scepter,

	// Weapons: Claws (Sword type)
	itm_photonclaw,
	itm_silenceclaw,
	itm_neisclaw,

	// Weapons: Knuckles (Sword type)
	itm_braveknuckle,
	itm_angryfist,
	itm_godhand,
	itm_sonicknuckle,

	// Weapons: Red photon class
	itm_redsaber,
	itm_redsword,
	itm_reddagger,
	itm_redpartisan,
	itm_redslicer,
	itm_redhandgun,
	itm_redmechgun,

	// Weapons: Rare
	itm_doublesaber,
	itm_twinbrand,
	itm_spreadneedle,
	itm_holyray,
	itm_psychogun,
	itm_heavenpunisher,

	// Weapons: Silly
	itm_akikosfryingpan,
	itm_eggblaster,
	itm_ancientsaber,
	itm_battlefan,
	itm_toyhammer,
	itm_crazytune,
	itm_rocketpunch,
	itm_sambamaracas,
	itm_drilllauncher,
	itm_strikerofchao,
	itm_masterbeam,
	itm_gamemagazine,
	itm_flowerbouquet,

	// Weapons: JTE! ^_^
	itm_jteblade,
	itm_jterailgun,

	// Basic barriers, by level
	itm_barrier,
	itm_shield,
	itm_coreshield,
	itm_gigashield,
	itm_soulbarrier,
	itm_hardshield,
	itm_bravebarrier,
	itm_solidshield,
	itm_flamebarrier,
	itm_plasmabarrier,
	itm_freezebarrier,
	itm_psychicbarrier,
	itm_generalshield,
	itm_protectbarrier,
	itm_gloriousshield,
	itm_imperialbarrier,
	itm_guardiansword,
	itm_divinitybarrier,
	itm_ultimateshield,
	itm_spiritualshield,
	itm_celestialshield,

	// Special barriers
	itm_invisibleguard,
	itm_sacredguard,

	// Sparts...
	itm_spartsver116,
	itm_spartsver201,

	// Walls...
	itm_forcewall,
	itm_rangerwall,
	itm_hunterwall,
	itm_attributewall,

	// Gear barriers...
	itm_secretgear,
	itm_combatgear,
	itm_protoregenegear,
	itm_regenerategear,
	itm_regenegearadv,

	// Custom barrier?...
	itm_custombarrierver00,

	// Um...
	itm_tripolicshield,
	itm_standstillshield,
	itm_saftyheart,
	itm_kasamibracer,

	// God's shields!1
	itm_godsshieldSUZAKU,
	itm_godsshieldGENBU,
	itm_godsshieldBYAKKO,
	itm_godsshieldSEIRYU,

	// Rings.
	itm_redring,
	itm_bluering,
	itm_yellowring,
	itm_purplering,
	itm_greenring,
	itm_blackring,
	itm_whitering,

	//Basic frames, by level.
	itm_frame,
	itm_armor,
	itm_psyarmor,
	itm_gigaframe,
	itm_soulframe,
	itm_crossarmor,
	itm_solidframe,
	itm_bravearmor,
	itm_hyperframe,
	itm_grandarmor,
	itm_shockframe,
	itm_kingsframe,
	itm_dragonframe,
	itm_absorbarmor,
	itm_protectframe,
	itm_generalarmor,
	itm_perfectframe,
	itm_valiantframe,
	itm_imperialarmor,
	itm_holinessarmor,
	itm_guardianarmor,
	itm_divinityarmor,
	itm_ultimateframe,
	itm_celestialarmor,

	// Class specific protection fields
	itm_hunterfield,
	itm_rangerfield,
	itm_forcefield,

	// Rare armor
	itm_revivalgarment,
	itm_spiritgarment,
	itm_stinkframe,
	itm_senseplate,
	itm_gravitonplate,
	itm_attributeplate,
	itm_customframev00,
	itm_guardwave,

	// Special armor
	itm_luminousfield,
	itm_chuchufever,
	itm_loveheart,
	itm_flamegarment,
	itm_virusarmorlafuteria, // Say THAT ten times fast!
	itm_brightnesscircle,
	itm_aurafield,
	itm_electroframe,
	itm_sacredcloth,
	itm_smokingplate,

	NUMSUITEMS
} suitemtype_t;

typedef enum
{
	info_saber,
	info_sword,
	info_dagger,
	info_partisan,
	info_slicer,
	info_handgun,
	info_rifle,
	info_mechgun,
	info_shot,
	info_cane,
	info_rod,
	info_wand,
	info_photonclaw,
	info_braveknuckle,
	info_barrier,
	info_frame,
	info_rare,
	info_quest,

	NUMINFOTYPES
} suinfotype_t;
typedef struct
{
	unsigned int base;
	unsigned int max;
} suslope_t;
typedef struct
{
	suslope_t damage;
	suslope_t rarity;
	suitemtype_t first;
	suitemtype_t last;
} suitminfo_t;

typedef enum
{
	SUT_WEAPON  = 0x01,
	SUT_BARRIER = 0x02,
	SUT_FRAME   = 0x04,
	SUT_SLOT    = 0x08,
	SUT_MAG     = 0x10,
	SUT_SPECIAL = 0x20,

	SUW_SABER       = 0x000000040,
	SUW_SWORD       = 0x000000080,
	SUW_DAGGER      = 0x000000100,
	SUW_PARTISAN    = 0x000000200,
	SUW_SLICER      = 0x000000400,
	SUW_HANDGUN     = 0x000000800,
	SUW_RIFLE       = 0x000001000,
	SUW_MECHGUN     = 0x000002000,
	SUW_SHOT        = 0x000004000,
	SUW_CANE        = 0x000008000,
	SUW_ROD         = 0x000010000,
	SUW_WAND        = 0x000020000,
	SUW_CLAW        = 0x000040000,
	SUW_KNUCKLE     = 0x000080000,
	SUW_DOUBLESABER = 0x000100000,
	SUW_SPREAD      = 0x000200000,
	SUW_PUNCH       = 0x000400000
} sutype_t;

typedef struct suitem_s
{
	const char *name;

	sutype_t type;

	mobjtype_t firetype;
	mobjtype_t playertracer;

	unsigned short damage;

	unsigned short buyprice;
	unsigned short sellprice;

	byte rarity;
	unsigned short usesound;
} suitem_t;

extern suitem_t suitems[NUMSUITEMS];
void SU_InitItems(void);

#endif
#endif
