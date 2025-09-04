// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id: ogl_sdl.c,v 1.6 2001/06/25 20:08:06 bock Exp $
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
//
//
// $Log: ogl_sdl.c,v $
// Revision 1.6  2001/06/25 20:08:06  bock
// Fix bug (BSD?) with color depth > 16 bpp
//
// Revision 1.5  2001/05/16 22:33:35  bock
// Initial FreeBSD support.
//
// Revision 1.4  2001/03/09 21:53:56  metzgermeister
// *** empty log message ***
//
// Revision 1.3  2000/11/02 19:49:40  bpereira
// no message
//
// Revision 1.2  2000/09/10 10:56:01  metzgermeister
// clean up & made it work again
//
// Revision 1.1  2000/08/21 21:17:32  metzgermeister
// Initial import to CVS
//
//
// DESCRIPTION:
//      SDL specific part of the OpenGL API for Doom Legacy
//
//-----------------------------------------------------------------------------

#ifdef FREEBSD
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif


#if defined( __WIN32__ )
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "../hardware/r_opengl/r_opengl.h"

void I_OutputMsg(char* error, ...);
float byteasfloat(byte fbyte);

#if defined( SDL ) && ( !defined( LOGMESSAGES ) || defined( SDLIO ) )
#undef DEBUG_TO_FILE
#endif

#ifdef DEBUG_TO_FILE
#include <stdarg.h>
#if defined (__WIN32__) 
#include <direct.h>
#else
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#ifndef SDLIO
#include <fcntl.h>
#endif
extern int logstream;
#endif

//[segabor] static SDL_Surface *vidSurface = NULL; //use the one from i_video_sdl.c instead?
extern SDL_Surface *vidSurface; //use the one from i_video_sdl.c instead?
int     oglflags = 0;

void HWR_Startup(void);

boolean OglSdlSurface(int w, int h, boolean isFullscreen)
{
	int cbpp;
	char *renderer;

	if(vidSurface)
	{
		//Alam: SDL_Video system free vidSurface for me
#ifdef VOODOOSAFESWITCHING
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		SDL_InitSubSystem(SDL_INIT_VIDEO);
#endif
	}

	/*
	 * We want at least 1 bit R, G, and B,
	 * and at least 16 bpp. Why 1 bit? May be more?
	 */
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	cbpp = SDL_VideoModeOK(w, h, 16, SDL_OPENGL|(isFullscreen?SDL_FULLSCREEN:SDL_RESIZABLE));
	if (cbpp < 16)
		return true; //Alam: Let just say we did, ok?
	vidSurface = SDL_SetVideoMode(w, h, cbpp, SDL_OPENGL|(isFullscreen?SDL_FULLSCREEN:SDL_RESIZABLE));
	if(!vidSurface)
		return false;

	gl_extensions = glGetString(GL_EXTENSIONS);
	// Get info and extensions.
	//BP: why don't we make it earlier ?
	//Hurdler: we cannot do that before intialising gl context
	renderer = strdup(glGetString(GL_RENDERER));
	DBG_Printf("Vendor     : %s\n", glGetString(GL_VENDOR) );
	DBG_Printf("Renderer   : %s\n", renderer );
	DBG_Printf("Version    : %s\n", glGetString(GL_VERSION) );
	DBG_Printf("Extensions : %s\n", gl_extensions );
	oglflags = 0;

#ifdef __WIN32__
	// BP: disable advenced feature that don't work on somes hardware
	// Hurdler: Now works on G400 with bios 1.6 and certified drivers 6.04
	if( strstr(renderer, "810" ) )   oglflags |= GLF_NOZBUFREAD;
#elif defined(LINUX)
	// disable advanced features not working on somes hardware
	if( strstr(renderer, "G200" ) )   oglflags |= GLF_NOTEXENV;
	if( strstr(renderer, "G400" ) )   oglflags |= GLF_NOTEXENV;
#endif
	free(renderer);
	DBG_Printf("oglflags   : 0x%X\n", oglflags );

#ifdef USE_PALETTED_TEXTURE
	usePalettedTexture = isExtAvailable("GL_EXT_paletted_texture");
	if(usePalettedTexture)
	{
		glColorTableEXT=(PFNGLCOLORTABLEEXTPROC)SDL_GL_GetProcAddress("glColorTableEXT");
		if (glColorTableEXT==NULL)
			usePalettedTexture = 0;
		else
			usePalettedTexture = 1;
	}
#ifdef PARANOIA
	if(usePalettedTexture)
		I_OutputMsg("r_opengl.c: USE_PALETED_TEXTURE works\n");
#endif
#endif

	SetModelView(w, h);
	SetStates();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	HWR_Startup();
#ifdef KOS_GL_COMPATIBILITY
	textureformatGL = GL_ARGB1555;
#else
	textureformatGL = (cbpp > 16)?GL_RGBA:GL_RGB5_A1;
#endif

#ifdef DEBUG_TO_FILE
	logstream = open("ogllog.txt",O_WRONLY|O_CREAT,0666);
#endif
	return true;
}

void OglSdlFinishUpdate(boolean vidwait)
{
	SDL_GL_SwapBuffers();
}

void OglSdlShutdown(void)
{
	//Alam: SDL_Video system free vidSurface for me
#ifdef DEBUG_TO_FILE
	if(logstream!=-1) close(logstream);
#endif
}

void OglSdlSetPalette(RGBA_t *palette, RGBA_t *gamma)
{
	int i = -1;
	byte redgamma = gamma->s.red, greengamma = gamma->s.green, bulegamma = gamma->s.blue;

	//if(vidSurface && vidSurface->flags&SDL_FULLSCREEN)
		i = SDL_SetGamma(byteasfloat(redgamma), byteasfloat(greengamma), byteasfloat(bulegamma));
	if(i == 0) redgamma = greengamma = bulegamma = 0x7F; //Alam: cool
	for (i=0; i<256; i++)
	{
		myPaletteData[i].s.red   = MIN((palette[i].s.red   * redgamma)  /127, 255);
		myPaletteData[i].s.green = MIN((palette[i].s.green * greengamma)/127, 255);
		myPaletteData[i].s.blue  = MIN((palette[i].s.blue  * bulegamma) /127, 255);
		myPaletteData[i].s.alpha = palette[i].s.alpha;
	}
#ifdef USE_PALETTED_TEXTURE
	if (usePalettedTexture && glColorTableEXT)
	{
		for (i=0; i<256; i++)
		{
			palette_tex[(3*i)+0] = palette[i].s.red;
			palette_tex[(3*i)+1] = palette[i].s.green;
			palette_tex[(3*i)+2] = palette[i].s.blue;
		}
		glColorTableEXT(GL_TEXTURE_2D, GL_RGB8, 256, GL_RGB, GL_UNSIGNED_BYTE, palette_tex);
	}
#endif
	// on a changé de palette, il faut recharger toutes les textures
	// jaja, und noch viel mehr ;-)
	Flush();
}
