
#ifndef __ENEMY__
#define __ENEMY__
void P_Thrust(); // Tails
void P_InstaThrust(); // Tails 08-26-2001
fixed_t P_ReturnThrustX();
fixed_t P_ReturnThrustY();
void P_ExplodeMissile(); // Tails 08-26-2001
void I_PlayCD();
void P_HomingAttack(mobj_t* source, mobj_t* enemy);
boolean PIT_RadiusAttack(mobj_t* thing);

typedef enum
{
    DI_EAST,
    DI_NORTHEAST,
    DI_NORTH,
    DI_NORTHWEST,
    DI_WEST,
    DI_SOUTHWEST,
    DI_SOUTH,
    DI_SOUTHEAST,
    DI_NODIR,
    NUMDIRS

} dirtype_t;


dirtype_t opposite[9];
dirtype_t diags[4];

void A_Explode();
void A_Pain();
void A_Fall();
void A_Look();
void A_Chase();
void A_FaceTarget();
void A_Scream();
void A_CustomPower(); // Use this for a custom power
void A_BlueShield(); // Obtained Blue Shield Tails 12-05-99
void A_YellowShield(); // Obtained Yellow Shield Tails 03-15-2000
void A_RingBox(); // Obtained Ring Box Tails
void A_Invincibility(); // Obtained Invincibility Box Tails
void A_SuperSneakers(); // Obtained Super Sneakers Box Tails
void A_BunnyHop(); // have bunny hop tails
void A_BubbleSpawn(); // Randomly spawn bubbles Tails 03-07-2000
void A_BubbleRise(); // Bubbles float to surface Tails 03-07-2000
void A_ExtraLife(); // Extra Life Tails 03-12-2000
void A_BlackShield(); // Obtained Black Shield Tails 04-08-2000
void A_GreenShield(); // Obtained Green Shield Tails 04-08-2000
void A_RedShield(); // Tails 12-18-2002
void A_ScoreRise(); // Rise the score logo Tails 04-16-2000
void A_AttractChase(); // Ring Chase Tails 02-27-2000
void A_ThrownRing(); // Sparkle trail for red ring Tails 03-13-2001
void A_SetSolidSteam(); // Tails 05-29-2001
void A_UnsetSolidSteam(); // Tails 05-29-2001
void A_JetChase(); // Tails 08-18-2001
void A_JetbThink(); // Jetty-Syn Bomber Thinker Tails 08-18-2001
void A_JetgThink(); // Jetty-Syn Gunner Thinker Tails 08-25-2001
void A_JetgShoot(); // Jetty-Syn Shoot Function Tails 08-25-2001
void A_MouseThink(); // Mouse Thinker Tails 08-26-2001
void A_DetonChase(); // Deton Chaser Tails 08-26-2001
void A_CapeChase(); // Fake little Super Sonic cape Tails 11-12-2001
void A_RotateSpikeBall(); // Spike ball rotation Tails 11-15-2001
void A_SnowBall(); // Snowball function for Snow Buster Tails 12-12-2001
void A_CrawlaCommanderThink(); // Crawla Commander Tails 12-30-2001
void A_SmokeTrailer();
void A_MixUp();
void A_BossScream();
void A_Invinciblerize();
void A_DeInvinciblerize();
void A_EggmanBox();
void A_TurretFire();
void A_TurretStop();
void A_SkimChase();
void A_SkullAttack();
void A_SparkFollow();
#endif

