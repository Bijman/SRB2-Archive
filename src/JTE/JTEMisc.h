// JTE Misc
// By Jason the Echidna
// MINE! NO STEALY!

#ifndef JTEMISC_H
#define JTEMISC_H

#if defined(CCPAL) || defined(HOLIDAYS)
void JTE_ReinitLightTables(void);
#endif

#ifdef SPRITESHADOWS
#include "../m_fixed.h"
fixed_t JTE_OpaqueFloorAtPos(fixed_t x, fixed_t y, fixed_t z, fixed_t height);
#endif

#ifdef A_TORNADOPARTICLE
#include "../p_mobj.h"
void A_TornadoParticle(mobj_t* mobj);
#endif

#ifdef JTEMISC
#include "../d_player.h"
void JTE_DeSuper(player_t* player);
void JTE_HwLights(void);
#endif

#ifdef MOBJSCALE
#include "../p_mobj.h"
void JTE_SetScale(mobj_t *mobj, unsigned short newscale);
#ifdef SCALEVAR
#include "../command.h"
extern consvar_t cv_myscale;
void JTE_InitScale(void);
void JTE_SendMyScale(void);
#endif
#endif

#ifdef SPRITEOFFS
typedef struct {
	float x[4],y[4],z[4];
} spriteoffs_t;
extern spriteoffs_t *spriteoffs[NUMSPRITES];
void JTE_InitSpriteOffs(void);
#endif

#ifdef HOLIDAYS
enum
{
	H_NONE = 0, // Nothing special. :(

	H_VALN, // Valentine's day
	H_PTRK, // Saint Patrik's day
	H_FOOL, // April Fool's day
	H_ESTR, // Easter
	H_WEEN, // Halloween
	H_THXG, // Thanksgiving
	H_XMAS, // Christmas
	H_NEWY, // New Year

	NUMHOLIDAYS
};
extern const char *holidaynicks[NUMHOLIDAYS];
typedef struct holidayreplace_s
{
	char original[9];
	char replace[9];
	byte type;
	struct holidayreplace_s *next;
} holidayreplace_t;
extern holidayreplace_t *holidayreplace;
void JTE_InitHolidaySprites(void);
void JTE_LoadHolidayCfg(void);
#ifdef __DEHACKED_H__
void JTE_ReadReplace(MYFILE *f, byte type);
#endif
void JTE_TitlePrecip(void);
#endif

#endif
