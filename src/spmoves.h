#ifdef SPMOVES

#if !defined(SPMOVES_HEAD)
#define SPMOVES_HEAD

#define NUMCUSTACTIONS 10
#define MAXCUSTACTIONSIZE 10

#if (NUMCUSTACTIONS != 10)
#error Remember to update the actions lists in P_Enemy.c, Dehacked.c, and info.c!
#endif

boolean showmultiegghud;

// PSO items! Yes, I am that insane, thank you very much!
typedef enum
{
    itm_emerald,

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

    itm_invisibleguard,
    itm_sacredguard,

    itm_spartsver116,
    itm_spartsver201,

    itm_forcewall,
    itm_rangerwall,
    itm_hunterwall,
    itm_attributewall,

    itm_secretgear,
    itm_combatgear,
    itm_protoregenegear,
    itm_regenerategear,
    itm_regenegearadv,

    itm_custombarrierver00,

    itm_tripolicshield,
    itm_standstillshield,
    itm_saftyheart,
    itm_kasamibracer,

    itm_godsshieldSUZAKU,
    itm_godsshieldGENBU,
    itm_godsshieldBYAKKO,
    itm_godsshieldSEIRYU,

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

	// Quest items
    itm_omegasatkdagger,

    NUMPSOITEMS

} psoitemtype_t;

typedef enum
{
    PSOI_INVALID,

    PSOI_SABER,
    PSOI_SWORD,
    PSOI_DAGGER,
    PSOI_PARTISAN,
    PSOI_SLICER,
    PSOI_HANDGUN,
    PSOI_RIFLE,
    PSOI_MECHGUN,
    PSOI_SHOT,
    PSOI_CANE,
    PSOI_ROD,
    PSOI_WAND,
    PSOI_CLAW,
    PSOI_KNUCKLE,
    PSOI_DOUBLESABER,
    PSOI_SPREAD,
    PSOI_PUNCH,

    PSOI_BARRIER,
    PSOI_WALL,
    PSOI_SPBARRIER,

    PSOI_FRAME,
    PSOI_FIELD,
    PSOI_SPFRAME,

    PSOI_SLOT,
    PSOI_SPECIAL,

    NUMPSOITEMTYPES
} psotype_t;

typedef enum
{
    PSOF_QUESTITEM = 0x00000001,
    PSOF_RAILSHOTS = 0x00000002
} psoflags_t;

typedef struct psoitem_s
{
    char name[128];

    psotype_t type;

    mobjtype_t firetype;
    mobjtype_t playertracer;

    int damage;

    long buyprice;
    long sellprice;

    int rarity;

    int usesound;

    int flags;

} psoitem_t;

#ifdef SPMBETA
actionf_p1 actions[NUMCUSTACTIONS][MAXCUSTACTIONSIZE];
#endif

extern psoitem_t psoitems[NUMPSOITEMS];
#ifdef SPMBETA
extern void JTE_ReadAction(MYFILE *f, int num);
#endif
extern void JTE_EmblemSpawn();
extern void JTE_SPMovesFiles();
extern void JTE_InitCharData();
extern void JTE_InitSPMoves();

extern void A_CheckFlee();
extern void A_BirdFlap();
extern void A_BirdFall();
extern void A_SPMovesChao();

extern void M_PSOQuestMenu();
extern void M_PSOBankMenu();
extern void M_PSOShopMenu();

extern void JTE_PSOQuest1();
extern void JTE_PSOQuest2();
extern void JTE_PSOQuest3();
extern void JTE_PSOQuest4();
extern void JTE_PSOQuest5();
extern void JTE_PSOQuest6();
extern void JTE_PSOQuest7();
extern void JTE_PSOQuest8();
extern void JTE_PSOQuest9();
extern void JTE_PSOQuest10();
extern void JTE_PSOQuest11();
extern void JTE_PSOQuest12();
extern void JTE_PSOQuest13();
extern void JTE_PSOQuest14();
extern void JTE_PSOQuest15();
extern void JTE_PSOQuest16();
extern void JTE_PSOQuest17();
extern void JTE_PSOQuest18();
extern void JTE_PSOQuest19();
extern void JTE_PSOQuest20();

int multieggs, jtetokens;
int spm_shadow, spm_mechaknux, spm_eggman, spm_jte, spm_jte2, spm_ss005, spm_cyan, spm_fox, spm_hikaru, spm_chompy, spm_zim;
#endif

#if defined(__R_DEFS__) && !defined(SPMOVES_H2)
#define SPMOVES_H2
extern boolean JTE_ChaoAttack(mobj_t *mobj);
extern void JTE_CreateItem (sector_t* sec, sector_t* destsec);
extern boolean JTE_EnemyDropStuff(mobj_t* target);
extern boolean JTE_SPMovesFuse(mobj_t *mobj);
extern boolean JTE_SPMovesMobjProg(mobj_t *mobj);
extern boolean JTE_PSODamage (mobj_t*   target,
                       mobj_t*   inflictor,
                       mobj_t*   source,
                       int       damage);
#endif

#if !defined(NUMMULTIEGGS)
#define NUMMULTIEGGS 15
#define NUMJTETOKENS 9
typedef struct
{
  signed short x;
  signed short y;
  signed short z;

  byte    player;

  int flagnum;
  byte level;
} emblem_t;

emblem_t multiegglocations[NUMMULTIEGGS]; // Multiplayer Easter Eggs.
emblem_t jtetokenlocations[NUMJTETOKENS]; // JTE Coins.
#endif

#if defined(player_t) && !defined(SPMOVES_H3)
#define SPMOVES_H3
extern void JTE_PlayerSPMove(player_t* player);
extern void JTE_PlayerLevelUp(player_t* player, int amount);
extern void JTE_PlayerGameplayStuff(player_t* player);
extern boolean JTE_PlayerFire(player_t *player);
#endif

#endif

