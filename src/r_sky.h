// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id: r_sky.h,v 1.4 2001/03/21 18:24:39 stroggonmeth Exp $
//
// Copyright (C) 1993-1996 by id Software, Inc.
// Portions Copyright (C) 1998-2000 by DooM Legacy Team.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//
// $Log: r_sky.h,v $
// Revision 1.4  2001/03/21 18:24:39  stroggonmeth
// Misc changes and fixes. Code cleanup
//
// Revision 1.3  2001/03/13 22:14:20  stroggonmeth
// Long time no commit. 3D floors, FraggleScript, portals, ect.
//
// Revision 1.2  2000/02/27 00:42:11  hurdler
// fix CR+LF problem
//
// Revision 1.1.1.1  2000/02/22 20:32:32  hurdler
// Initial import into CVS (v1.29 pr3)
//
//
// DESCRIPTION:
//      Sky rendering.
//
//-----------------------------------------------------------------------------


#ifndef __R_SKY__
#define __R_SKY__

#include "m_fixed.h"

#ifdef __GNUG__
#pragma interface
#endif

// SKY, store the number for name.
#define                 SKYFLATNAME  "F_SKY1"
#define                 SKY2FLATNAME  "F_SKY2"
#define                 SKY3FLATNAME  "F_SKY3"
#define                 SKY4FLATNAME  "F_SKY4"
#define                 SKY5FLATNAME  "F_SKY5"
#define                 SKY6FLATNAME  "F_SKY6"
#define                 SKY7FLATNAME  "F_SKY7"
#define                 SKY8FLATNAME  "F_SKY8"
#define                 SKY9FLATNAME  "F_SKY9"

// The sky map is 256*128*4 maps.
#define ANGLETOSKYSHIFT         22

extern  int             skytexture;
extern  int             sky2texture;
extern  int             sky3texture;
extern  int             sky4texture;
extern  int             sky5texture;
extern  int             sky6texture;
extern  int             sky7texture;
extern  int             sky8texture;
extern  int             sky9texture;

extern int              skytexturemid;
extern int              sky2texturemid;
extern int              sky3texturemid;
extern int              sky4texturemid;
extern int              sky5texturemid;
extern int              sky6texturemid;
extern int              sky7texturemid;
extern int              sky8texturemid;
extern int              sky9texturemid;

extern fixed_t          skyscale;
extern int              skymode;  //current sky old (0) or new(1),
                                  // see SCR_SetMode

// Needed to store the number of the dummy sky flat.
// Used for rendering, as well as tracking projectiles etc.
extern int              skyflatnum;
extern int              sky2flatnum;
extern int              sky3flatnum;
extern int              sky4flatnum;
extern int              sky5flatnum;
extern int              sky6flatnum;
extern int              sky7flatnum;
extern int              sky8flatnum;
extern int              sky9flatnum;

//added:12-02-98: declare the asm routine which draws the sky columns
void R_DrawSkyColumn (void);

// Called once at startup.
void R_InitSkyMap (void);

// call after skytexture is set to adapt for old/new skies
void R_SetupSkyDraw (void);

void        R_StorePortalRange(void);
void        R_InitPortals();
void        R_ClearPortals();
void        R_DrawPortals();

void R_SetSkyScale (void);

#endif
