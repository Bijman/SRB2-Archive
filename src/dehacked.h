// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
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
/// \brief Dehacked files.

void DEH_LoadDehackedLump(int lump);

extern boolean deh_loaded, modcredits;

#ifdef HOLIDAYS
#ifndef __DEHACKED_H__
#define __DEHACKED_H__
#define MAXLINELEN 1024
typedef struct
{
	char *data;
	char *curpos;
	int size;
} MYFILE;
#define myfeof(a) (a->data + a->size <= a->curpos)
char *myfgets(char *buf, int bufsize, MYFILE *f);
#endif
#endif
