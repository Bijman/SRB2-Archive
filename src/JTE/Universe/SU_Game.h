// SU - SRB2 Universe
// By Jason the Echidna
// MINE! NO STEALY!

#ifdef UNIVERSE
#ifndef __SU_GAME__
#define __SU_GAME__

void SU_Init(void);
void SU_LevelUp(player_t *player, int amount);
boolean SU_EnemyDropStuff(mobj_t* target);
void SU_GameplayStuff(player_t *player);
boolean SU_Fuse(mobj_t* mobj);
boolean SU_MobjProg(mobj_t* mobj);
boolean SU_SpecialItems(mobj_t* toucher, mobj_t* special);
void SU_UseSpecialWeapon(player_t* player, suitemtype_t item);
void SU_UseSpecialShield(player_t* player, suitemtype_t item);
void SU_UseSpecialItem(player_t* player, suitemtype_t item);
boolean SU_DamageMobj(mobj_t* target, mobj_t* inflictor, mobj_t* source, int damage);
void SU_FiringCode(player_t* player);
void SU_BarrelPop(mobj_t* mobj);

#endif
#endif
