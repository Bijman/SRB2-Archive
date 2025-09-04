// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id: i_video.c,v 1.13 2004/05/16 19:11:53 hurdler Exp $
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
// $Log: i_video.c,v $
// Revision 1.13  2004/05/16 19:11:53  hurdler
// that should fix issues some people were having in 1280x1024 mode (and now support up to 1600x1200)
//
// Revision 1.12  2002/07/01 19:59:59  metzgermeister
// *** empty log message ***
//
// Revision 1.11  2001/12/31 16:56:39  metzgermeister
// see Dec 31 log
// .
//
// Revision 1.10  2001/08/20 20:40:42  metzgermeister
// *** empty log message ***
//
// Revision 1.9  2001/05/16 22:33:35  bock
// Initial FreeBSD support.
//
// Revision 1.8  2001/04/28 14:25:03  metzgermeister
// fixed mouse and menu bug
//
// Revision 1.7  2001/04/27 13:32:14  bpereira
// no message
//
// Revision 1.6  2001/03/12 21:03:10  metzgermeister
//   * new symbols for rendererlib added in SDL
//   * console printout fixed for Linux&SDL
//   * Crash fixed in Linux SW renderer initialization
//
// Revision 1.5  2001/03/09 21:53:56  metzgermeister
// *** empty log message ***
//
// Revision 1.4  2001/02/24 13:35:23  bpereira
// no message
//
// Revision 1.3  2001/01/25 22:15:45  bpereira
// added heretic support
//
// Revision 1.2  2000/11/02 19:49:40  bpereira
// no message
//
// Revision 1.1  2000/09/10 10:56:00  metzgermeister
// clean up & made it work again
//
// Revision 1.1  2000/08/21 21:17:32  metzgermeister
// Initial import to CVS
//
//
//
// DESCRIPTION:
//      DOOM graphics stuff for SDL
//
//-----------------------------------------------------------------------------

#include <stdlib.h>

#ifdef FREEBSD
#include <SDL.h>
#ifdef HAVE_IMAGE
#include <SDL_image.h>
#endif
#else
#include <SDL/SDL.h>
#ifdef HAVE_IMAGE
#include <SDL/SDL_image.h>
#endif
#endif

#include "../doomdef.h"

#ifdef __WIN32__
#include <SDL/SDL_syswm.h>
#endif

#ifndef SDL_BUTTON_WHEELUP
#define SDL_BUTTON_WHEELUP   4
#endif

#ifndef SDL_BUTTON_WHEELDOWN
#define SDL_BUTTON_WHEELDOWN 5
#endif

#include "../doomstat.h"
#include "../i_system.h"
#include "../v_video.h"
#include "../m_argv.h"
#include "../m_menu.h"
#include "../d_main.h"
#include "../s_sound.h"
#include "../i_sound.h"  // midi pause/unpause
#include "../g_input.h"
#include "../st_stuff.h"
#include "../g_game.h"
#include "../i_video.h"
#include "../console.h"
#include "../command.h"
#ifdef HWRENDER
#include "../hardware/hw_main.h"
#include "../hardware/hw_drv.h"
// For dynamic referencing of HW rendering functions
#include "hwsym_sdl.h"
#include "ogl_sdl.h"
#endif

#ifndef HAVE_IMAGE
#define LOAD_XPM //I want XPM!
#include "IMG_xpm.c" //Alam: I don't want to add SDL_Image.dll/so
#define HAVE_IMAGE //I have SDL_Image, sortof
#endif

#ifdef HAVE_IMAGE
#include "SDL_icon.xpm"
#endif

void VID_PrepareModeList(void);
extern boolean musicStarted;
void static inline SDL_SetMode(int width, int height, int bpp, Uint32 flags);
int VID_GetModeForSize(int w, int h);

// maximum number of windowed modes (see windowedModes[][])
#if defined (_WIN32_WCE) || defined(DC)
#define MAXWINMODES (7)
#else
#define MAXWINMODES (15)
#endif

//Hudler: 16/10/99: added for OpenGL gamma correction
RGBA_t  gamma_correction = {0x7F7F7F7F};

extern consvar_t cv_fullscreen; // for fullscreen support

static int numVidModes = 0;

static char vidModeName[33][32]; // allow 33 different modes

rendermode_t rendermode=render_soft;

boolean highcolor = false;

// synchronize page flipping with screen refresh
// unused and for compatibilityy reason
consvar_t cv_vidwait = {"vid_wait", "On", CV_SAVE, CV_OnOff};
consvar_t cv_stretch = {"stretch", "On", CV_SAVE|CV_NOSHOWHELP, CV_OnOff};

byte graphics_started = 0; // Is used in console.c and screen.c

// To disable fullscreen at startup; is set in VID_PrepareModeList
boolean allow_fullscreen = false;
static boolean disable_fullscreen = false;
#define USE_FULLSCREEN (disable_fullscreen||!allow_fullscreen)?0:cv_fullscreen.value
#define USE_MOUSEINPUT cv_usemouse.value && SDL_GetAppState() & SDL_APPACTIVE

// SDL vars
//[segabor] !!! I had problem compiling this source with gcc 3.3
// maybe gcc 3.2 does it better
//static       SDL_Surface *vidSurface=NULL;
             SDL_Surface *vidSurface=NULL;
static       SDL_Surface *bufSurface=NULL;
static       SDL_Surface *icoSurface=NULL;
static       SDL_Color    localPalette[256];
static       SDL_Rect   **modeList=NULL;
static       Uint8        BitsPerPixel = 8;
const static Uint32       surfaceFlagsW = SDL_HWPALETTE|SDL_RESIZABLE;
const static Uint32       surfaceFlagsF = SDL_HWPALETTE|SDL_FULLSCREEN;

// first entry in the modelist which is not bigger than MAXVIDWIDTHxMAXVIDHEIGHT
static int firstEntry=0;
static boolean mousegrabok = false;
static boolean videoblitok = false;
// windowed video modes from which to choose from.

static int windowedModes[MAXWINMODES][2] =
{
#if !defined (_WIN32_WCE) && !defined(DC)
	{MAXVIDWIDTH /*1920*/, MAXVIDHEIGHT/*1200*/}, //1.60, 6.00
	{1600,1200}, // 1.33,5.00
	{1600,1000}, // 1.60,5.00
	{1536,1152}, // 1.33,4.80
	{1280, 960}, // 1.33,4.00
	{1280, 800}, // 1.60,4.00
	{1024, 768}, // 1.33,3.20
	{960 , 600}, // 1.60,3.00
#endif
	{800 , 600}, // 1.33,2.50
	{640 , 480}, // 1.33,2.00
	{640 , 400}, // 1.60,2.00
	{512 , 384}, // 1.33,1.60
	{400 , 300}, // 1.33,1.25
	{320 , 240}, // 1.33,1.00
	{320 , 200}, // 1.60,1.00
};
//
//  Translates the SDL key into Doom key
//

static int xlatekey(SDLKey sym)
{
	int rc=sym + 0x80;

	switch(sym)
	{
		case SDLK_LEFT:  rc = KEY_LEFTARROW;     break;
		case SDLK_RIGHT: rc = KEY_RIGHTARROW;    break;
		case SDLK_DOWN:  rc = KEY_DOWNARROW;     break;
		case SDLK_UP:    rc = KEY_UPARROW;       break;

		case SDLK_ESCAPE:   rc = KEY_ESCAPE;        break;
		case SDLK_SPACE:    rc = KEY_SPACE;         break;
		case SDLK_RETURN:   rc = KEY_ENTER;         break;
		case SDLK_TAB:      rc = KEY_TAB;           break;
		case SDLK_F1:       rc = KEY_F1;            break;
		case SDLK_F2:       rc = KEY_F2;            break;
		case SDLK_F3:       rc = KEY_F3;            break;
		case SDLK_F4:       rc = KEY_F4;            break;
		case SDLK_F5:       rc = KEY_F5;            break;
		case SDLK_F6:       rc = KEY_F6;            break;
		case SDLK_F7:       rc = KEY_F7;            break;
		case SDLK_F8:       rc = KEY_F8;            break;
		case SDLK_F9:       rc = KEY_F9;            break;
		case SDLK_F10:      rc = KEY_F10;           break;
		case SDLK_F11:      rc = KEY_F11;           break;
		case SDLK_F12:      rc = KEY_F12;           break;

		case SDLK_BACKSPACE: rc = KEY_BACKSPACE;    break;
		case SDLK_DELETE:    rc = KEY_DEL;          break;

		case SDLK_PAUSE:     rc = KEY_PAUSE;        break;

		case SDLK_EQUALS:
		case SDLK_PLUS:      rc = KEY_EQUALS;       break;

		case SDLK_MINUS:     rc = KEY_MINUS;        break;

		case SDLK_LSHIFT:
		case SDLK_RSHIFT:
			rc = KEY_SHIFT;
			break;

		case SDLK_CAPSLOCK:  rc = KEY_CAPSLOCK;     break;

		case SDLK_LCTRL:
		case SDLK_RCTRL:
			rc = KEY_CTRL;
			break;

		case SDLK_LALT:
		case SDLK_RALT:
			rc = KEY_ALT;
			break;

		case SDLK_NUMLOCK:   rc = KEY_NUMLOCK;    break;
		case SDLK_SCROLLOCK: rc = KEY_SCROLLLOCK; break;

		case SDLK_PAGEUP:   rc = KEY_PGUP; break;
		case SDLK_PAGEDOWN: rc = KEY_PGDN; break;
		case SDLK_END:      rc = KEY_END;  break;
		case SDLK_HOME:     rc = KEY_HOME; break;
		case SDLK_INSERT:   rc = KEY_INS;  break;

		case SDLK_KP0: rc = KEY_KEYPAD0;  break;
		case SDLK_KP1: rc = KEY_KEYPAD1;  break;
		case SDLK_KP2: rc = KEY_KEYPAD2;  break;
		case SDLK_KP3: rc = KEY_KEYPAD3;  break;
		case SDLK_KP4: rc = KEY_KEYPAD4;  break;
		case SDLK_KP5: rc = KEY_KEYPAD5;  break;
		case SDLK_KP6: rc = KEY_KEYPAD6;  break;
		case SDLK_KP7: rc = KEY_KEYPAD7;  break;
		case SDLK_KP8: rc = KEY_KEYPAD8;  break;
		case SDLK_KP9: rc = KEY_KEYPAD9;  break;

		case SDLK_KP_PERIOD:   rc = KEY_KPADDEL;   break;
		case SDLK_KP_DIVIDE:   rc = KEY_KPADSLASH; break;
		case SDLK_KP_MULTIPLY: rc = '*';           break;
		case SDLK_KP_MINUS:    rc = KEY_MINUSPAD;  break;
		case SDLK_KP_PLUS:     rc = KEY_PLUSPAD;   break;
		case SDLK_KP_ENTER:    rc = KEY_ENTER;     break;
		case SDLK_KP_EQUALS:   rc = KEY_EQUALS;    break; //Alam & Logan: WTF?

		case SDLK_LSUPER:
		case SDLK_LMETA:
			rc = KEY_LEFTWIN;  break;
		case SDLK_RSUPER:
		case SDLK_RMETA:
			rc = KEY_RIGHTWIN; break;

		case SDLK_MENU:        rc = KEY_MENU;      break;

		default:
			if (sym >= SDLK_SPACE && sym <= SDLK_DELETE)
				rc = sym - SDLK_SPACE + ' ';
			else if (sym >= 'A' && sym <= 'Z')
				rc = sym - 'A' + 'a';
			else if(sym && devparm)
				CONS_Printf("\2Unknown Keycode %i, Name: %s\n",sym, SDL_GetKeyName( sym ));
			else if(!sym) rc = 0;
			break;
	}

	return rc;
}

static void doGrabMouse(void)
{
	if(SDL_GRAB_OFF == SDL_WM_GrabInput(SDL_GRAB_QUERY))
	{
		if(mousegrabok) SDL_WM_GrabInput(SDL_GRAB_ON);
	}
}

static void doUngrabMouse(void)
{
	if(SDL_GRAB_ON == SDL_WM_GrabInput(SDL_GRAB_QUERY))
	{
		SDL_WM_GrabInput(SDL_GRAB_OFF);
	}
}

void VID_Command_NumModes_f (void)
{
	CONS_Printf ("%d video mode(s) available(s)\n", VID_NumModes());
}

void VID_Command_Info_f (void)
{
	const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo(); //Alam: Double-Check
	if(videoInfo)
	{
		CONS_Printf("Video Interface Capabilities:\n");
		if(videoInfo->hw_available)
			CONS_Printf(" Hardware surfaces\n");
		if(videoInfo->wm_available)
		{
			char videodriver[4] = {'S','D','L',0};
			CONS_Printf(" Window manager\n");
			if(!M_CheckParm("-nomousegrab"))
				if(SDL_VideoDriverName(videodriver,4))
					if(M_CheckParm("-mousegrab") ||
					strcasecmp("X11",videodriver)) //X11's XGrabPointer not good
						mousegrabok = true;
		}
		//UnusedBits1  :6
		//UnusedBits2  :1
		if(videoInfo->blit_hw)
			CONS_Printf(" Accelerated blits HW-2-HW\n");
		if(videoInfo->blit_hw_CC)
			CONS_Printf(" Accelerated blits HW-2-HW with Colorkey\n");
		if(videoInfo->wm_available)
			CONS_Printf(" Accelerated blits HW-2-HW with Alpha\n");
		if(videoInfo->blit_sw)
		{
			CONS_Printf(" Accelerated blits SW-2-HW\n");
			if(!M_CheckParm("-noblit")) videoblitok = true;
		}
		if(videoInfo->blit_sw_CC)
			CONS_Printf(" Accelerated blits SW-2-HW with Colorkey\n");
		if(videoInfo->blit_sw_A)
			CONS_Printf(" Accelerated blits SW-2-HW with Alpha\n");
		if(videoInfo->blit_fill)
			CONS_Printf(" Accelerated Color filling, Hmmm\n");
		//UnusedBits3  :16
		if(videoInfo->video_mem)
			CONS_Printf(" There %i MB of video memory\n",videoInfo->video_mem/1024);
		else
			CONS_Printf(" There no video memory for SDL\n");
		//*vfmt
	}
	else mousegrabok = true; //Alam: ok....
	if(vidSurface)
	{
		int vfBPP= vidSurface->format?vidSurface->format->BitsPerPixel:0;
		CONS_Printf("Current Video Mode:\n");
		{ //w,h,bpp,ref
			CONS_Printf(" %ix%i at %i bit color\n",vidSurface->w,vidSurface->h,vfBPP);
		}
		{ //flags
			if(vidSurface->flags&SDL_PREALLOC)
				CONS_Printf(" Uses preallocated memory, o_O\n");
			else if(vidSurface->flags&SDL_HWSURFACE)
				CONS_Printf(" Stored in video memory\n");
			else if(vidSurface->flags&SDL_SWSURFACE)
				CONS_Printf(" Stored in system memory\n");
			if(vidSurface->flags&SDL_OPENGL)
				CONS_Printf(" Stored in an OpenGL context\n");
			if(vidSurface->flags&SDL_ASYNCBLIT)
				CONS_Printf(" Uses asynchronous blits if possible\n");
			else
				CONS_Printf(" Uses synchronous blits if possible\n");
			if(vidSurface->flags&SDL_ANYFORMAT)
				CONS_Printf(" Allows any pixel-format\n");
			if(vidSurface->flags&SDL_HWPALETTE)
				CONS_Printf(" Has exclusive palette access\n");
			else
				CONS_Printf(" Has nonexclusive palette access\n");
			if(vidSurface->flags&SDL_DOUBLEBUF)
				CONS_Printf(" Double buffered\n");
			else
				CONS_Printf(" No hardware flipping\n");
			if(vidSurface->flags&SDL_FULLSCREEN)
				CONS_Printf(" Full screen\n");
			else if(vidSurface->flags&SDL_RESIZABLE)
				CONS_Printf(" Resizable window\n");
			else
				CONS_Printf(" Nonresizable window\n");
			if(vidSurface->flags&SDL_OPENGLBLIT)
				CONS_Printf(" Supports OpenGL blitting\n");
			if(vidSurface->flags&SDL_HWACCEL)
				CONS_Printf(" Uses hardware acceleration blit\n");
			if(vidSurface->flags&SDL_SRCCOLORKEY)
				CONS_Printf(" Use colorkey blitting\n");
			if(vidSurface->flags&SDL_RLEACCEL)
				CONS_Printf(" Colorkey RLE acceleration blit\n");
			if(vidSurface->flags&SDL_SRCALPHA)
				CONS_Printf(" Use alpha blending acceleration blit\n");
		}
		
	}
}

static void VID_Command_ModeList_f(void)
{
	int i;
#ifdef HWRENDER
	if(M_CheckParm("-opengl") || rendermode == render_opengl)
		modeList = SDL_ListModes(NULL, SDL_OPENGL|SDL_FULLSCREEN);
	else
#endif
	modeList = SDL_ListModes(NULL, surfaceFlagsF|SDL_HWSURFACE); //Alam: At least hardware surface
	disable_fullscreen = M_CheckParm("-win");

	if(modeList == (SDL_Rect **)0 && cv_fullscreen.value)
	{
		CONS_Printf("No video modes present\n");
		cv_fullscreen.value=0;
	}
	else if(modeList != (SDL_Rect **)0)
	{
		if(modeList == (SDL_Rect **)-1)
			numVidModes = -1; // should not happen with fullscreen modes
		else while(modeList[numVidModes])
			numVidModes++;
	}
	CONS_Printf("Found %d FullScreen Video Modes:\n", numVidModes);
	for (i=0 ; i<numVidModes; i++)
	{ // fullscreen modes
		int modeNum = firstEntry + i;
		if(modeNum >= numVidModes)
			break;
		
		CONS_Printf("%dx%d and ",
				modeList[modeNum]->w,
				modeList[modeNum]->h);
	}
	CONS_Printf("None\n");
}

void VID_Command_Mode_f (void)
{
	int         modenum;

	if (COM_Argc()!=2)
	{
		CONS_Printf ("vid_mode <modenum> : set video mode\n");
		return;
	}

	modenum = atoi(COM_Argv(1));

	if (modenum >= VID_NumModes())
		CONS_Printf ("No such video mode\n");
	else
		setmodeneeded = modenum+1; // request vid mode change
}

extern void I_GetJoyEvent();
extern void I_GetJoyEvent2();
#if defined (LMOUSE2) || defined(__WIN32__)
extern void I_GetMouse2Event();
#endif

int I_ConsoleKey();

void I_GetEvent(void)
{
	SDL_Event inputEvent;
	static boolean sdlquit = false; //Alam: once, just once
	event_t event = {0,0,0,0};

	if(!graphics_started)
	{
		event.type = ev_keydown;
		event.data1 = I_ConsoleKey();
		I_OutputMsg("%i",event.data1);
		if(event.data1) D_PostEvent(&event);
		event.type = ev_keyup;
		if(event.data1) D_PostEvent(&event);
		return;
	}

	if(SDL_WasInit(SDL_INIT_JOYSTICK)!=0) SDL_JoystickUpdate();
	I_GetJoyEvent();
#if defined (LMOUSE2) || defined(__WIN32__)
	I_GetMouse2Event();
#endif

	while(SDL_PollEvent(&inputEvent))
	{
		memset(&event,0,sizeof(event_t));
		switch(inputEvent.type)
		{
		case SDL_ACTIVEEVENT:
			if(SDL_APPACTIVE & inputEvent.active.state || SDL_APPINPUTFOCUS & inputEvent.active.state)
			{
				// pause music when alt-tab
				if( inputEvent.active.gain /*&& !paused */)
				{
					if(cv_usemouse.value) I_StartupMouse();
					//if(!netgame && !con_destlines) paused = false;
					if(gamestate == GS_LEVEL || gamestate == GS_DEMOSCREEN)
						if(musicStarted && !paused) I_ResumeSong(0); //resume it
				}
				else /*if (!paused)*/
				{
					static boolean windownnow = false;
					doUngrabMouse();
					if(!netgame && gamestate == GS_LEVEL) paused = true;
					memset(gamekeydown, 0, NUMKEYS);
					//S_PauseSound();
					if(gamestate == GS_LEVEL || gamestate == GS_DEMOSCREEN)
						if(musicStarted) I_PauseSong(0); //pause it
					if(cv_fullscreen.value && windownnow)
						CV_SetValue(&cv_fullscreen,0);
					else if(cv_fullscreen.value && !windownnow)
						windownnow = true;
					else windownnow = false;
				}
			}
			if(SDL_APPMOUSEFOCUS&inputEvent.active.state && USE_MOUSEINPUT && !inputEvent.active.gain)
				SDL_WarpMouse((Uint16)(vid.width/2), (Uint16)(vid.height/2));
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			if(inputEvent.type == SDL_KEYUP)
				event.type = ev_keyup;
			else 
				event.type = ev_keydown;
			event.data1 = xlatekey(inputEvent.key.keysym.sym);
			if(event.data1) D_PostEvent(&event);
			break;
		case SDL_MOUSEMOTION:
			if(USE_MOUSEINPUT)
			{
				// If the event is from warping the pointer back to middle
				// of the screen then ignore it.
				if ((inputEvent.motion.x == vid.width/2) &&
					(inputEvent.motion.y == vid.height/2))
				{
					break;
				}
				else
				{
					event.data2 = +inputEvent.motion.xrel;
					event.data3 = -inputEvent.motion.yrel;
				}
				event.type = ev_mouse;
				D_PostEvent(&event);
				// Warp the pointer back to the middle of the window
				//  or we cannot move any further if it's at a border.
				if ((inputEvent.motion.x < (vid.width/2 )-(vid.width/4 )) ||
					 (inputEvent.motion.y < (vid.height/2)-(vid.height/4)) ||
					 (inputEvent.motion.x > (vid.width/2 )+(vid.width/4 )) ||
					 (inputEvent.motion.y > (vid.height/2)+(vid.height/4) ) )
				{
					//if(SDL_GRAB_ON == SDL_WM_GrabInput(SDL_GRAB_QUERY) || !mousegrabok)
						SDL_WarpMouse((Uint16)(vid.width/2), (Uint16)(vid.height/2));
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			if(USE_MOUSEINPUT)
			{
				if(inputEvent.type == SDL_MOUSEBUTTONUP)
					event.type = ev_keyup;
				else
					event.type = ev_keydown;
				if(inputEvent.button.button <= MOUSEBUTTONS)
				{
					event.data1 = KEY_MOUSE1 + inputEvent.button.button - SDL_BUTTON_LEFT;
				}
				if(inputEvent.button.button==SDL_BUTTON_WHEELUP || inputEvent.button.button==SDL_BUTTON_WHEELDOWN)
				{
					if(inputEvent.type == SDL_MOUSEBUTTONUP)
						event.data1 = 0; //Alam: dumb! this could be a real button with some mouses
					else
						event.data1 = KEY_MOUSEWHEELUP + inputEvent.button.button - SDL_BUTTON_WHEELUP;
				}
				if(event.data1) D_PostEvent(&event);
			}
			break;
		case SDL_QUIT:
			if(!sdlquit)
			{
				sdlquit = true;
				M_QuitResponse('y');
			}
			break;
		case SDL_VIDEORESIZE:
			if(gamestate == GS_LEVEL || gamestate == GS_DEMOSCREEN || gamestate == GS_TITLESCREEN || gamestate == GS_EVALUATION)
				setmodeneeded = VID_GetModeForSize(inputEvent.resize.w,inputEvent.resize.h)+1;
			SDL_SetMode(vidSurface->w, vidSurface->h, BitsPerPixel, surfaceFlagsW);
			if(!vidSurface)
				I_Error("Could not set vidmode: %s\n",SDL_GetError());
			break;
		default:
			break;
		}
	}
	gamekeydown[KEY_MOUSEWHEELUP] = 0; //reset wheel like in win32, I don't understand it but works
	gamekeydown[KEY_MOUSEWHEELDOWN] = 0;
}

void I_StartupMouse(void)
{
	static boolean firsttimeonmouse = true;
	if(!firsttimeonmouse)
		SDL_WarpMouse(vid.width/2, vid.height/2); // warp to center
	else 
		firsttimeonmouse = false;
	if(cv_usemouse.value)
	{
		doGrabMouse();
	}
	else
	{
		doUngrabMouse();
	}
	return;
}

//
// I_OsPolling
//
void I_OsPolling(void)
{
	I_GetEvent();
}

//
// I_UpdateNoBlit
//
void I_UpdateNoBlit(void)
{
	//if(!vidSurface)
		return;
	if(vidSurface->flags&SDL_DOUBLEBUF)
		SDL_Flip(vidSurface); //Alam: just flip?
	else
		SDL_UpdateRect(vidSurface, 0, 0, 0, 0); //Alam: just update?
}

#define FPSPOINTS  35
#define SCALE      4
#define PUTDOT(xx,yy,cc) screens[0][((yy)*vid.width+(xx))*vid.bpp]=(cc)

static int fpsgraph[FPSPOINTS];

void static inline displayticrate()
{
	int j,l,i;
	static tic_t lasttic;
	tic_t        tics,t;

	t = I_GetTime();
	tics = t - lasttic;
	lasttic = t;
	if (tics > 20) tics = 20;

	for (i=0;i<FPSPOINTS-1;i++)
		fpsgraph[i]=fpsgraph[i+1];
	fpsgraph[FPSPOINTS-1]=20-tics;

	if( rendermode == render_soft )
	{
		int k;
		// draw dots
		for(j=0;j<=20*SCALE*vid.dupy;j+=2*SCALE*vid.dupy)
		{
			l=(vid.height-1-j)*vid.width*vid.bpp;
			for (i=0;i<FPSPOINTS*SCALE*vid.dupx;i+=2*SCALE*vid.dupx)
				screens[0][l+i]=0xff;
		}

		// draw the graph
		for (i=0;i<FPSPOINTS;i++)
			for(k=0;k<SCALE*vid.dupx;k++)
				PUTDOT(i*SCALE*vid.dupx+k, vid.height-1-(fpsgraph[i]*SCALE*vid.dupy),0xff);
	}
#ifdef HWRENDER
	else
	{
		fline_t p;
		for(j=0;j<=20*SCALE*vid.dupy;j+=2*SCALE*vid.dupy)
		{
			l=(vid.height-1-j);
			for (i=0;i<FPSPOINTS*SCALE*vid.dupx;i+=2*SCALE*vid.dupx)
			{
				p.a.x = i;
				p.a.y = l;
				p.b.x = i+1;
				p.b.y = l;
				HWR_drawAMline(&p, 0xff);
			}
		}

		for (i=1;i<FPSPOINTS;i++)
		{
			p.a.x = SCALE * (i-1);
			p.a.y = vid.height-1-fpsgraph[i-1]*SCALE*vid.dupy;
			p.b.x = SCALE * i;
			p.b.y = vid.height-1-fpsgraph[i]*SCALE*vid.dupy;
			HWR_drawAMline(&p, 0xff);
		}
	}
#endif
}

void I_StartFrame(void){}

//
// I_FinishUpdate
//
void I_FinishUpdate(void)
{
	if(!vidSurface)
		return; //Alam: No software or OpenGl surface
	if (cv_ticrate.value)
		displayticrate();
	if(render_soft == rendermode)
	{
		int lockedsf = 0, blited = 0, vfBPP = vidSurface->format?vidSurface->format->BitsPerPixel:0;
		if(bufSurface && (videoblitok || vfBPP != vid.bpp*8) ) //Alam: New Way to send video data
		{
			if(SDL_MUSTLOCK(bufSurface)) SDL_UnlockSurface(bufSurface);
			blited = SDL_BlitSurface(bufSurface,NULL,vidSurface,NULL);
			if(SDL_MUSTLOCK(bufSurface)) lockedsf = SDL_LockSurface(bufSurface);
		}
		else //Alam: DOS Way, for backup
		{
			if(SDL_MUSTLOCK(vidSurface)) lockedsf = SDL_LockSurface(vidSurface);
			if(lockedsf == 0 && vidSurface->pixels)
			{
				VID_BlitLinearScreen (screens[0], vidSurface->pixels, vid.width*vid.bpp,
									vid.height, vid.width*vid.bpp, vidSurface->pitch );
				if(SDL_MUSTLOCK(vidSurface)) SDL_UnlockSurface(vidSurface);
			}
		}
		if(lockedsf == 0 && blited == 0 && vidSurface->flags&SDL_DOUBLEBUF)
			SDL_Flip(vidSurface);
		else if(blited != 2 && lockedsf == 0) //Alam: -2 for Win32 Direct, yea, i know
			SDL_UpdateRect(vidSurface, 0, 0, 0, 0); //Alam: almost always
		else if(devparm)
			I_OutputMsg("%s\n",SDL_GetError());
	}
#ifdef HWRENDER
	else
	{
		OglSdlFinishUpdate(cv_vidwait.value);
	}
#endif
	return;
}


//
// I_ReadScreen
//
void I_ReadScreen(byte* scr)
{
	if (rendermode != render_soft)
		I_Error ("I_ReadScreen: called while in non-software mode");
	else
		VID_BlitLinearScreen ( vid.buffer, scr,
				vid.width*vid.bpp, vid.height,
				vid.width*vid.bpp, vid.rowbytes );
}

//
// I_SetPalette
//
void I_SetPalette(RGBA_t* palette)
{
	int i;
	for(i=0; i<256; i++)
	{
		localPalette[i].r = palette[i].s.red;
		localPalette[i].g = palette[i].s.green;
		localPalette[i].b = palette[i].s.blue;
	}
	SDL_SetColors(vidSurface, localPalette, 0, 256);
	if(bufSurface) SDL_SetColors(bufSurface, localPalette, 0, 256);
	return;
}

// return number of fullscreen + X11 modes
int VID_NumModes(void)
{
	if(USE_FULLSCREEN && numVidModes != -1)
		return numVidModes - firstEntry;
	else
		return MAXWINMODES;
}

char *VID_GetModeName(int modeNum)
{
	if(USE_FULLSCREEN && numVidModes != -1) // fullscreen modes
	{
		modeNum += firstEntry;
		if(modeNum >= numVidModes)
			return NULL;

		sprintf(&vidModeName[modeNum][0], "%dx%d",
				modeList[modeNum]->w,
				modeList[modeNum]->h);
	}
	else // windowed modes
	{
		if(modeNum > MAXWINMODES)
			return NULL;

		sprintf(&vidModeName[modeNum][0], "%dx%d",
				windowedModes[modeNum][0],
				windowedModes[modeNum][1]);
	}
	return &vidModeName[modeNum][0];
}

int VID_GetModeForSize(int w, int h)
{
	int matchMode = -1, i;
	if(USE_FULLSCREEN && numVidModes != -1)
	{
		for(i=firstEntry; i<numVidModes; i++)
		{
			if(modeList[i]->w == w &&
				modeList[i]->h == h)
			{
				matchMode = i;
				break;
			}
		}
		if(-1 == matchMode) // use smaller mode
		{
			w -= w%BASEVIDWIDTH;
			h -= h%BASEVIDHEIGHT;
			for(i=firstEntry; i<numVidModes; i++)
			{
				if(modeList[i]->w == w &&
					modeList[i]->h == h)
				{
					matchMode = i;
					break;
				}
			}
			if(-1 == matchMode) // use smallest mode
				matchMode = numVidModes-1;
		}
		matchMode -= firstEntry;
	}
	else
	{
		for(i=0; i<MAXWINMODES; i++)
		{
			if(windowedModes[i][0] == w &&
				windowedModes[i][1] == h)
			{
				matchMode = i;
				break;
			}
		}
		if(-1 == matchMode) // use smaller mode
		{
			w -= w%BASEVIDWIDTH;
			h -= h%BASEVIDHEIGHT;
			for(i=0; i<MAXWINMODES; i++)
			{
				if(windowedModes[i][0] == w &&
					windowedModes[i][1] == h)
				{
					matchMode = i;
					break;
				}
			}
			if(-1 == matchMode) // use smallest mode
				matchMode = MAXWINMODES-1;
		}
	}
	return matchMode;
}

void VID_PrepareModeList(void)
{
	int i;

	if(disable_fullscreen?0:cv_fullscreen.value) // only fullscreen needs preparation
	{
		if(-1 != numVidModes)
		{
			for(i=0; i<numVidModes; i++)
			{
				if(modeList[i]->w <= MAXVIDWIDTH &&
					modeList[i]->h <= MAXVIDHEIGHT)
				{
					firstEntry = i;
					break;
				}
			}
		}
	}
	allow_fullscreen = true;
	return;
}

void static inline SDL_SetMode(int width, int height, int bpp, Uint32 flags)
{
	if(cv_vidwait.value && videoblitok && SDL_VideoModeOK(vid.width, vid.height, bpp, flags|SDL_HWSURFACE|SDL_DOUBLEBUF) >= bpp)
		vidSurface = SDL_SetVideoMode(vid.width, vid.height, bpp, flags|SDL_HWSURFACE|SDL_DOUBLEBUF);
	else if(videoblitok && SDL_VideoModeOK(vid.width, vid.height, bpp, flags|SDL_HWSURFACE) >= bpp)
		vidSurface = SDL_SetVideoMode(vid.width, vid.height, bpp, flags|SDL_HWSURFACE);
	else if(SDL_VideoModeOK(vid.width, vid.height, bpp, flags|SDL_SWSURFACE) >= bpp)
		vidSurface = SDL_SetVideoMode(vid.width, vid.height, bpp, flags|SDL_SWSURFACE);
	else return;
	
}

int VID_SetMode(int modeNum)
{
	doUngrabMouse();
	vid.recalc = true;
	// Window title
	SDL_WM_SetCaption("SRB2RP"VERSIONSTRING, "SRB2RP");

	if(render_soft == rendermode)
	{
		//Alam: SDL_Video system free vidSurface for me
		if(vid.buffer) free(vid.buffer);
		if(bufSurface) SDL_FreeSurface(bufSurface);
	}

	if(USE_FULLSCREEN)
	{
		if(numVidModes != -1)
		{
			modeNum += firstEntry;
			vid.width = modeList[modeNum]->w;
			vid.height = modeList[modeNum]->h;
		}
		else
		{
			vid.width = windowedModes[modeNum][0];
			vid.height = windowedModes[modeNum][1];
		}
		if(render_soft == rendermode)
		{
			SDL_SetMode(vid.width, vid.height, BitsPerPixel, surfaceFlagsF);
			if(!vidSurface)
			{
				cv_fullscreen.value=0;
				modeNum = VID_GetModeForSize(vid.width,vid.height);
				vid.width = windowedModes[modeNum][0];
				vid.height = windowedModes[modeNum][1];
				SDL_SetMode(vid.width, vid.height, BitsPerPixel, surfaceFlagsW);
				if(!vidSurface)
					I_Error("Could not set vidmode: %s\n",SDL_GetError());
			}
		}
#ifdef HWRENDER
		else // (render_soft != rendermode)
		{
			if(!OglSdlSurface(vid.width, vid.height, true))
			{
				cv_fullscreen.value=0;
				modeNum = VID_GetModeForSize(vid.width,vid.height);
				vid.width = windowedModes[modeNum][0];
				vid.height = windowedModes[modeNum][1];
				if(!OglSdlSurface(vid.width, vid.height,false))
					I_Error("Could not set vidmode: %s\n",SDL_GetError());
			}
		}
#endif
	}
	else //(cv_fullscreen.value)
	{
		vid.width = windowedModes[modeNum][0];
		vid.height = windowedModes[modeNum][1];
		if(render_soft == rendermode)
		{
			SDL_SetMode(vid.width, vid.height, BitsPerPixel, surfaceFlagsW);
			if(!vidSurface)
				I_Error("Could not set vidmode: %s\n",SDL_GetError());
		}
#ifdef HWRENDER
		else //(render_soft != rendermode)
		{
			if(!OglSdlSurface(vid.width, vid.height, false))
				I_Error("Could not set vidmode: %s\n",SDL_GetError());
		}
#endif
	}
	modeNum = vid.modenum = VID_GetModeForSize(vidSurface->w,vidSurface->h);
	if(USE_FULLSCREEN && numVidModes != -1)
	{
		modeNum += firstEntry;
		vid.width = modeList[modeNum]->w;
		vid.height = modeList[modeNum]->h;
	}
	else
	{
		vid.width = windowedModes[modeNum][0];
		vid.height = windowedModes[modeNum][1];
	}
	if(render_soft == rendermode)
	{
		vid.rowbytes = vid.width*vid.bpp;
		vid.buffer = malloc(vid.rowbytes*vid.height*NUMSCREENS);
		if(!vid.buffer) I_Error ("Not enough memory for video buffer\n");
		else bufSurface = SDL_CreateRGBSurfaceFrom(vid.buffer,vid.width,vid.height,vid.bpp*8,vid.rowbytes,
vidSurface->format->Rmask, vidSurface->format->Gmask, vidSurface->format->Bmask, vidSurface->format->Amask);
		if(!bufSurface) CONS_Printf ("\2Not enough memory for back-buffer surface: %s\n",SDL_GetError());
	}
	if(!cv_stretch.value)
	{
		// Adjust the height to match
		if((float)vid.width/vid.height != ((float)BASEVIDWIDTH/BASEVIDHEIGHT))
		{
			vid.height = vid.width * ((float)BASEVIDHEIGHT/BASEVIDWIDTH);
			vid.fdupx = (float)vid.width/BASEVIDWIDTH;
			vid.fdupy = (float)vid.height/BASEVIDHEIGHT;
		}
	}

	I_StartupMouse();

	return true;
}

void I_StartupGraphics(void)
{
	const SDL_VideoInfo *videoInfo = NULL;

	if(graphics_started || dedicated)
		return;

	COM_AddCommand ("vid_nummodes", VID_Command_NumModes_f);
	COM_AddCommand ("vid_info", VID_Command_Info_f);
	COM_AddCommand ("vid_modelist", VID_Command_ModeList_f);
	COM_AddCommand ("vid_mode", VID_Command_Mode_f);
	CV_RegisterVar (&cv_vidwait);
	CV_RegisterVar (&cv_stretch);

	// Initialize Audio as well, otherwise Win32's DirectX can not use audio
	if(SDL_InitSubSystem(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0)
	{
		CONS_Printf("Couldn't initialize SDL's Audio/Video: %s\n", SDL_GetError());
		if(SDL_WasInit(SDL_INIT_VIDEO)==0) return;
	}
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY>>1,SDL_DEFAULT_REPEAT_INTERVAL<<2);
	//SDL_EnableKeyRepeat(0,0);
	videoInfo = SDL_GetVideoInfo();
//[segabor]: it's ok on Mac OS X with SDL
#ifdef __MACH__
	if(videoInfo && videoInfo->vfmt) //Alam: Watch it?
	{
		BitsPerPixel = videoInfo->vfmt->BitsPerPixel;
		vid.bpp     = videoInfo->vfmt->BytesPerPixel;
	}
	highcolor = (vid.bpp == 2) ? true:false;
#else
	// Get video info for screen resolutions
	//videoInfo = SDL_GetVideoInfo();
	// even if I set vid.bpp and highscreen properly it does seem to
	// support only 8 bit  ...  strange
	// so lets force 8 bit
#ifdef DC
	BitsPerPixel = 16;
#else
	BitsPerPixel = 8;
#endif
	// Set color depth; either 1=256pseudocolor or 2=hicolor
	vid.bpp = 1 /*videoInfo->vfmt->BytesPerPixel*/;
	highcolor = (vid.bpp == 2) ? true:false;
#endif
	VID_Command_ModeList_f();
	// default size for startup
	vid.buffer = NULL;
	vid.direct = NULL;
	vid.WndParent = NULL;
	vid.width = BASEVIDWIDTH;
#ifdef DC
	vid.height = 240;
#else
	vid.height = BASEVIDHEIGHT;
#endif
	vid.recalc = true;

	// Window title
	SDL_WM_SetCaption("SRB2RP: Starting up", "SRB2RP");

	// Window icon
#ifdef HAVE_IMAGE
	icoSurface = IMG_ReadXPMFromArray(SDL_icon_xpm);
#endif
	SDL_WM_SetIcon(icoSurface, NULL);

//[segabor]: Mac hack
//    if(M_CheckParm("-opengl"))
#ifdef HWRENDER
	if(M_CheckParm("-opengl") || rendermode == render_opengl)
	{
		rendermode = render_opengl;
		HWD.pfnInit             = hwSym("Init");
		HWD.pfnFinishUpdate     = hwSym("FinishUpdate");
		HWD.pfnDraw2DLine       = hwSym("Draw2DLine");
		HWD.pfnDrawPolygon      = hwSym("DrawPolygon");
		HWD.pfnSetBlend         = hwSym("SetBlend");
		HWD.pfnClearBuffer      = hwSym("ClearBuffer");
		HWD.pfnSetTexture       = hwSym("SetTexture");
		HWD.pfnReadRect         = hwSym("ReadRect");
		HWD.pfnGClipRect        = hwSym("GClipRect");
		HWD.pfnClearMipMapCache = hwSym("ClearMipMapCache");
		HWD.pfnSetSpecialState  = hwSym("SetSpecialState");
		HWD.pfnSetPalette       = hwSym("SetPalette");
		HWD.pfnGetTextureUsed   = hwSym("GetTextureUsed");
		HWD.pfnDrawMD2          = hwSym("DrawMD2");
		HWD.pfnSetTransform     = hwSym("SetTransform");
		HWD.pfnGetRenderVersion = hwSym("GetRenderVersion");
		// check gl renderer lib
		if (HWD.pfnGetRenderVersion() != VERSION)
			I_Error ("The version of the renderer doesn't match the version of the executable\nBe sure you have installed SRB2 properly.\n");
		vid.width = 640; // hack to make voodoo cards work in 640x480
		vid.height = 480;
		if(!OglSdlSurface(vid.width, vid.height, (USE_FULLSCREEN)))
			if(!OglSdlSurface(vid.width, vid.height, !(USE_FULLSCREEN)))
				rendermode = render_soft;
	}
#endif
	if(render_soft == rendermode)
	{
		SDL_SetMode(vid.width, vid.height, BitsPerPixel, surfaceFlagsW);
		if(!vidSurface)
		{
			CONS_Printf("Could not set vidmode: %s\n",SDL_GetError());
			mousegrabok = true; //Alam: ahhh.
			return;
		}
		vid.rowbytes = vid.width * vid.bpp;
		vid.buffer = malloc(vid.width * vid.height * vid.bpp * NUMSCREENS);
		if(!vid.buffer) CONS_Printf ("Not enough memory for video buffer\n");
		else bufSurface = SDL_CreateRGBSurfaceFrom(vid.buffer,vid.width,vid.height,vid.bpp*8,vid.rowbytes,
vidSurface->format->Rmask, vidSurface->format->Gmask, vidSurface->format->Bmask, vidSurface->format->Amask);
		if(!bufSurface) CONS_Printf ("\2Not enough memory for back-buffer surface: %s\n",SDL_GetError());
	}
	VID_Command_Info_f();
	SDL_ShowCursor(SDL_DISABLE);
	doUngrabMouse();
#ifdef __WIN32__
	{
		SDL_SysWMinfo Win32WM;
		memset(&Win32WM,0,sizeof(SDL_SysWMinfo));
		SDL_VERSION(&Win32WM.version)
		SDL_GetWMInfo(&Win32WM);
		vid.WndParent = Win32WM.window;
	}
#endif
	graphics_started = true;
	return;
}

void I_ShutdownGraphics(void)
{
	// was graphics initialized anyway?
	if (!graphics_started)
		return;
	CONS_Printf("I_ShutdownGraphics: ");
	doUngrabMouse();
	if(icoSurface) SDL_FreeSurface(icoSurface);
	icoSurface = NULL;
	if(render_soft == rendermode)
	{
		//Alam: SDL_Video system free vidSurface for me
		if(vid.buffer) free(vid.buffer);
		vid.buffer = NULL;
		if(bufSurface) SDL_FreeSurface(bufSurface);
		bufSurface = NULL;
	}
#ifdef HWRENDER
	else
	{
		OglSdlShutdown();
	}
#endif
	CONS_Printf("shut down\n");
	graphics_started = false;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
