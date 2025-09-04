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
//
// DESCRIPTION:
//      Main program, simply calls D_SRB2Main high level loop.
//
//-----------------------------------------------------------------------------

#include "../doomdef.h"

#include "../m_argv.h"
#include "../d_main.h"

#ifdef __GNUC__
#include <unistd.h>
#elif defined(_MSC_VER)
#include <windows.h>
#endif

#ifdef SDLMAIN
#include <SDL/SDL_main.h>
#endif

#if defined (LOGMESSAGES)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#undef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE -1
int logstream = INVALID_HANDLE_VALUE;
#endif

#ifndef O_TEXT
#define O_TEXT 0
#endif

#ifndef O_SEQUENTIAL
#define O_SEQUENTIAL 0
#endif

int main(int argc, char **argv)
{
	myargc = argc;
	myargv = argv;

#ifdef LOGMESSAGES
	//unlink("./srb2log.txt"); //Alam: poo! why can't open TRUNC the log file?
	//Hurdler: only write log if we have the permission in the current directory
	logstream = open("./srb2log.txt", O_WRONLY|(0?O_APPEND:O_TRUNC)|O_CREAT|O_SEQUENTIAL|O_TEXT,0666);
	if (logstream < 0)
	{
		logstream = INVALID_HANDLE_VALUE; // so we haven't to change the current source code
	}
#endif
#ifdef __WIN32__
#ifdef __MINGW32__
	LoadLibrary("exchndl.dll");
#endif
#ifdef USEASM
	{
		// Disable write-protection of code segment
		DWORD OldRights;
		BYTE *pBaseOfImage = (BYTE *)GetModuleHandle(0);
		IMAGE_OPTIONAL_HEADER *pHeader = (IMAGE_OPTIONAL_HEADER *)
			(pBaseOfImage + ((IMAGE_DOS_HEADER*)pBaseOfImage)->e_lfanew +
			sizeof(IMAGE_NT_SIGNATURE) + sizeof(IMAGE_FILE_HEADER));
		if (!VirtualProtect(pBaseOfImage+pHeader->BaseOfCode,pHeader->SizeOfCode,PAGE_EXECUTE_READWRITE,&OldRights))
			I_Error ("Could not make code writable\n");
	}
#endif
#endif

	D_DoomMain ();
	D_DoomLoop ();
	return 0;
}
