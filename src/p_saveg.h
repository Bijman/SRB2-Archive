// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// Copyright (C) 1993-1996 by id Software, Inc.
// Copyright (C) 1998-2000 by DooM Legacy Team.
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
//-----------------------------------------------------------------------------
/// \file
/// \brief Savegame I/O, archiving, persistence

#ifndef __P_SAVEG__
#define __P_SAVEG__

#ifdef __GNUG__
#pragma interface
#endif

// Persistent storage/archiving.
// These are the load / save game routines.

void P_SaveGame(void);
void P_SaveNetGame(void);
boolean P_LoadGame(INT16 mapoverride);
boolean P_LoadNetGame(void);

typedef struct
{
	UINT8 skincolor;
	UINT8 skin;
	INT32 score;
	INT32 lives;
	INT32 continues;
	UINT16 emeralds;
#ifdef RIDERS
	INT32 cheese;
	INT32 chip;
	INT32 birdie;
	INT32 chargei;

	INT32 charexgear; // Ex Gear definition
					  //
					  // 0 = Railings
					  // 1 = Air Rings
					  // 2 = Bustable Blocks
					  //
	INT32 thokcolor;
	INT32 prefthokcolor;

	INT32 runframe;

	INT32 starpostwp;
	INT32 position;
	INT32 cutawayz;
	INT32 cutawaymobjz;
	INT32 heldDir; // Held dir of controls; 1 = forward, 0 = none, -1 = backward
	INT32 turning;
	INT32 newfloorz;
	INT32 lakitu;
	INT32 airtime;
#endif
} savedata_t;

extern savedata_t savedata;
extern UINT8 *save_p;

#endif
