// Emacs style mode select   -*- C++ -*-
// 
//-----------------------------------------------------------------------------
//
// $Id: i_system.c,v 1.12 2004/03/23 17:38:51 araftis Exp $
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
// $Log: i_system.c,v $
// Revision Mac  2004/03/23 17:38:51  araftis
// Added support for Mac OS X (version 10.3+). Also fixed a number of endian
// bugs that were introduced by the over zealous use of the SHORT macro. This
// version also removes support for Mac OS 9 and earlier.	
//
// Revision 1.12  2003/05/04 04:24:08  sburke
// Add Solaris support.
//
// Revision 1.11  2002/01/03 19:20:07  bock
// Add FreeBSD code to I_GetFreeMem.
// Modified Files:
//     makefile linux_x/i_system.c sdl/i_system.c
//
// Revision 1.10  2001/12/31 16:56:39  metzgermeister
// see Dec 31 log
// .
//
// Revision 1.9  2001/08/20 20:40:42  metzgermeister
// *** empty log message ***
//
// Revision 1.8  2001/05/16 22:33:35  bock
// Initial FreeBSD support.
//
// Revision 1.7  2001/03/12 21:03:10  metzgermeister
//   * new symbols for rendererlib added in SDL
//   * console printout fixed for Linux&SDL
//   * Crash fixed in Linux SW renderer initialization
//
// Revision 1.6  2001/02/24 13:35:23  bpereira
// no message
//
// Revision 1.5  2000/11/02 19:49:40  bpereira
// no message
//
// Revision 1.4  2000/10/16 21:20:53  hurdler
// remove unecessary code
//
// Revision 1.3  2000/09/26 17:58:06  metzgermeister
// I_Getkey implemented
//
// Revision 1.2  2000/09/10 10:56:00  metzgermeister
// clean up & made it work again
//
// Revision 1.1  2000/08/21 21:17:32  metzgermeister
// Initial import to CVS
//
//
//
// DESCRIPTION:
//
//-----------------------------------------------------------------------------

#ifndef _WIN32_WCE
#include <signal.h>
#endif

#ifdef __WIN32__
#include <windows.h>
typedef BOOL (WINAPI *MyFunc)(LPCSTR RootName, PULARGE_INTEGER pulA, PULARGE_INTEGER pulB, PULARGE_INTEGER pulFreeBytes);
#endif	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __GNUC__
#include <unistd.h>
#elif defined(_MSC)
#include <direct.h>
#endif
#ifndef SDLIO
#ifndef _WIN32_WCE
#include <fcntl.h>
#endif
#endif

#if !defined (SDLMAIN)
#include <stdio.h>
#if defined(__WIN32__)
#include <conio.h>
#endif
#endif

#ifndef FREESBD
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#if defined(LINUX)
#if !defined(FREEBSD) && !defined(__APPLE_CC__)
#include <sys/vfs.h>
#else
#include <sys/param.h>
#include <sys/mount.h>
/*For meminfo*/
#include <sys/types.h>
#include <kvm.h>
#include <nlist.h>
#include <sys/vmmeter.h>
#endif
#endif

#ifdef LMOUSE2
#include <termios.h>
#endif

#ifdef _WIN32_WCE
#include "SRB2CE\cehelp.h"
#endif

// Locations for searching the srb2.srb
#if defined(LINUX)
#define DEFAULTWADLOCATION1 "/usr/local/games/srb2rp"
#define DEFAULTWADLOCATION2 "/usr/games/srb2rp"
#define DEFAULTSEARCHPATH1 "/usr/local"
#define DEFAULTSEARCHPATH2 "/usr/games"
#elif defined(__WIN32__)
#define DEFAULTWADLOCATION1 "c:\\games\\srb2rp"
#define DEFAULTWADLOCATION2 "\\games\\srb2rp"
#define DEFAULTSEARCHPATH1 "c:\\games"
#define DEFAULTSEARCHPATH2 "\\games"
#endif
#define WADKEYWORD "Srb2RP.wad"
// holds wad path
static char returnWadPath[256];

//Alam_GBC: SDL

#include "../doomdef.h"
#include "../m_misc.h"
#include "../i_video.h"
#include "../i_sound.h"
#include "../i_system.h"

#include "../d_net.h"
#include "../g_game.h"

#ifndef SDLMAIN
#include "endtxt.h"
#endif

#include "../i_joy.h"

#include "../m_argv.h"

#ifdef MAC_ALERT
#include "macosx/mac_alert.h"
#endif

#include "../d_main.h"

static SDL_Joystick *Joydev = NULL;
static int joyaxes = 0;
static int joy_scale = 0;
static int joybuts = 0;
static int SDLoldjoy = -1;
static int joyhats = 0;
static int joystick_started = 0;
JoyType_t Joystick;

#define DEADZONE 153 //Alam_GBC: SDL

#ifdef LMOUSE2
int fdmouse2 = -1;
int mouse2_started = 0;
#endif

void I_AddExitFunc(void (*func)());

#if 0
static void signal_handler(int num)
{
	//static char msg[] = "oh no! back to reality!\r\n";
	char*       sigmsg;
	char        sigdef[64];

	switch (num)
	{
	case SIGINT:
		sigmsg = "interrupt";
		break;
	case SIGILL:
		sigmsg = "illegal instruction - invalid function image";
		break;
	case SIGFPE:
		sigmsg = "floating point exception";
		break;
	case SIGSEGV:
		sigmsg = "segment violation";
		break;
	case SIGTERM:
		sigmsg = "Software termination signal from kill";
		break;
#if  !defined(LINUX)
	case SIGBREAK:
		sigmsg = "Ctrl-Break sequence";
		break;
#endif
	case SIGABRT:
		sigmsg = "abnormal termination triggered by abort call";
		break;
	default:
		sprintf(sigdef,"signal number %d", num);
		sigmsg = sigdef;
	}
	
#ifdef LOGMESSAGES
	if (logstream != INVALID_HANDLE_VALUE)
	{
		I_OutputMsg ("signal_handler() error: %s\n", sigmsg);
	}
#endif
	signal(num, SIG_DFL);               //default signal action
	raise(num);
	I_Quit();
}
#endif


//
// StartupKeyboard
//
void I_StartupKeyboard (void)
{
#ifdef SIGILL
//	signal(SIGILL , signal_handler);
#endif
#ifdef SIGINT
	signal(SIGINT , (void (*)(int)) I_Quit);
#endif
#ifdef SIGSEGV
//	signal(SIGSEGV , signal_handler);
#endif
#ifdef SIGBREAK
	signal(SIGBREAK , (void (*)(int)) I_Quit);
#endif
#ifdef SIGABRT
//	signal(SIGABRT , signal_handler);
#endif
#ifdef SIGTERM
	signal(SIGTERM , (void (*)(int)) I_Quit);
#endif

}

//
//I_StartupTimer
//
void I_StartupTimer    (void)
{
	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_NOPARACHUTE) < 0)
		I_Error("SDL_Timer: Need by SRB2!, Error: %s",SDL_GetError()); //Alam: Doh!
}

//
//I_OutputMsg
//
void I_OutputMsg       (char *fmt, ...) 
{
#if !defined(SDLMAIN)
	va_list     argptr;
	va_start (argptr,fmt);
	vfprintf (stderr,fmt,argptr);
	va_end (argptr);
	// 2004-03-03 AJR Since not all messages end in newline, some were getting displayed late.	
	fflush(stderr);
#endif
}


int I_ConsoleKey()
{
#if defined(LINUX) && !defined (SDLMAIN)
	int key;
	fgets( (char*) &key, 1, stdin);
	return key;
#elif defined (__WIN32__) && !defined (SDLMAIN)
	int key=getch();
	if(key==0) key=getch()+256;
	return key;
#else
	return 0;
#endif
}

//
// I_GetKey
//
int I_GetKey (void)
{
	// Warning: I_GetKey emties the event queue till next keypress
	event_t*    ev;
	int rc=0;

	if(!graphics_started)
	{
		rc=I_ConsoleKey();
		if(rc)
			return rc;
		else
			return KEY_ENTER;
	}

	// return the first keypress from the event queue
	for ( ; eventtail != eventhead ; eventtail = (++eventtail)&(MAXEVENTS-1) )
	{
		ev = &events[eventtail];
		if(ev->type == ev_keydown)
		{
			rc = ev->data1;
			continue;
		}
	}

	return rc;
}

//
// I_JoyScale
//
void I_JoyScale()
{
	Joystick.bGamepadStyle = (cv_joyscale.value==0)?true:false;
	joy_scale = (cv_joyscale.value==0)?1:cv_joyscale.value;
}

void I_ShutdownJoystick()
{
	int i;
	event_t event;
	event.type=ev_keyup;
	event.data2 = 0;
	event.data3 = 0;


	// emulate the up of all joystick buttons
	for(i=0;i<JOYBUTTONS;i++)
	{
		event.data1=KEY_JOY1+i;
		D_PostEvent(&event);
	}

	// reset joystick position
	event.type = ev_joystick;
	event.data1 = 0;
	D_PostEvent(&event);

	joystick_started = joyaxes = joyhats = joybuts = 0;
	if(Joydev) SDL_JoystickClose(Joydev);
	Joydev = NULL;
	if (!joystick_started && SDL_WasInit(SDL_INIT_JOYSTICK)!=0)
	{
		SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
		if(cv_usejoystick.value==0) CONS_Printf("I_Joystick: SDL's Joystick system shutdowned\n");
	}
}

void I_GetJoyEvent()
{
	static event_t event = {0,0,0,0};
	int i = 0;
	static int lastjoybuttons = 0;
	int joybuttons = 0;
	Sint16 axisx, axisy;

	if (!Joydev) I_ShutdownJoystick();

	if(!joystick_started) return;

	//faB: look for as much buttons as g_input code supports,
	//	 we don't use the others
	for (i = JOYBUTTONS-1; i >= -1; i--)
	{
		joybuttons <<= 1;
		if ( joybuts >= i && SDL_JoystickGetButton(Joydev,i) )
			joybuttons |= 1;
	}

	if(joyhats != 0)
	{
		i = SDL_JoystickGetHat(Joydev, 0);

		if  (i  &  SDL_HAT_UP) joybuttons|=1<<11;
		if  (i & SDL_HAT_DOWN) joybuttons|=1<<12;
		if  (i & SDL_HAT_LEFT) joybuttons|=1<<13;
		if (i & SDL_HAT_RIGHT) joybuttons|=1<<14;
	}

	if ( joybuttons != lastjoybuttons )
	{
		int j = 1;
		int newbuttons;

		// keep only bits that changed since last time
		newbuttons = joybuttons ^ lastjoybuttons;	
		lastjoybuttons = joybuttons;

		for( i=0; i < JOYBUTTONS+1; i++, j<<=1 )
		{
			if ( newbuttons & j )		// button changed state ?
			{
				if ( joybuttons & j )
					event.type = ev_keydown;
				else
					event.type = ev_keyup;
				event.data1 = KEY_JOY1 + i-1;
				D_PostEvent (&event);
			}
		}
	}
	// send joystick axis positions
	//
	event.type = ev_joystick;
	event.data1 = 0;
	event.data2 = 0;
	event.data3 = 0;

	axisx = SDL_JoystickGetAxis(Joydev, 0);
	axisy = SDL_JoystickGetAxis(Joydev, 1);

	axisx = joy_scale?(((axisx>>5)/joy_scale)*joy_scale):(axisx>>5);
	axisy = joy_scale?(((axisy>>5)/joy_scale)*joy_scale):(axisy>>5);

	if ( Joystick.bGamepadStyle )
	{
		// gamepad control type, on or off, live or die
		if ( axisx < -(JOYAXISRANGE/2) )
			event.data2 = -1;
		else if ( axisx > (JOYAXISRANGE/2) )
			event.data2 = 1;
		if ( axisy < -(JOYAXISRANGE/2) )
			event.data3 = -1;
		else if ( axisy > (JOYAXISRANGE/2) )
			event.data3 = 1;
	}
	else
	{

		if(-DEADZONE <= axisx && axisx <= DEADZONE) axisx=0;
		if(-DEADZONE <= axisy && axisy <= DEADZONE) axisy=0;

		// analog control style , just send the raw data
		event.data2 = axisx;	// x axis
		event.data3 = axisy;	// y axis
	}
	D_PostEvent (&event);

}

int joy_open(char *fname)
{
	int joyindex = atoi(fname);
	int num_joy = 0;
	int i;

	if(joystick_started == 0)
	{
		if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) == -1)
		{
			CONS_Printf("Couldn't initialize SDL Joystick: %s\n", SDL_GetError());
			return 0;
		}
		else
		{
			//SDL_JoystickEventState(SDL_ENABLE);
			SDL_JoystickEventState(SDL_IGNORE);
			num_joy = SDL_NumJoysticks();
		}

		if(num_joy < joyindex)
		{
			CONS_Printf("Unable to use that joystick #%d/(%s), it doesn't exist\n",joyindex,fname);
			for(i=0;i<num_joy;i++)
				CONS_Printf("#: %d, Name: %s\n", i, SDL_JoystickName(i));
			I_ShutdownJoystick();
			return 0;
		}
	}
	else
	{
		if(Joydev) SDL_JoystickClose(Joydev);
		Joydev = NULL;
		//I_ShutdownJoystick();
		//joy_open(fname);
	}
	
	num_joy = SDL_NumJoysticks();

	if(joyindex <= 0 || num_joy == 0 || SDLoldjoy == joyindex)
	{
//		CONS_Printf("Unable to use that joystick #(%s), non-number\n",fname);
		if(num_joy != 0)
		{
			CONS_Printf("Hmmm, I was able to found %d joysticks on this system\n", num_joy);
			for(i=0;i<num_joy;i++)
				CONS_Printf("#: %d, Name: %s\n", i+1, SDL_JoystickName(i));
		}
		else
			CONS_Printf("Hmm, I was unable to found any joysticks on this system\n");
		if(joyindex <= 0 || num_joy == 0) return 0;
	}

	Joydev = SDL_JoystickOpen(joyindex-1);
	CONS_Printf("Joystick: %s\n",SDL_JoystickName(joyindex-1));

	if(Joydev == NULL)
	{
		CONS_Printf("Couldn't open joystick: %s\n", SDL_GetError());
		I_ShutdownJoystick();
		return 0;
	}
	else
	{
		joyaxes = SDL_JoystickNumAxes(Joydev);

		if(joyaxes<2)
		{
			CONS_Printf("Not enought axes?\n");
			I_ShutdownJoystick();
			return 0;
		}

		joybuts = SDL_JoystickNumButtons(Joydev);
		if(joybuts > JOYBUTTONS)
			joybuts = JOYBUTTONS;

		joyhats = SDL_JoystickNumHats(Joydev);
		if(joyhats > 1) //Alam_GBC: only 1 Hat?
			joyhats = 1;

		//Joystick.bGamepadStyle = !strcmp ( SDL_JoystickName (SDL_JoystickIndex(Joydev) ),"Pad");

		return joyaxes;
	}

	return 0;
}

//
// I_InitJoystick
//
void I_InitJoystick (void)
{
	I_ShutdownJoystick();
	if(!strcmp(cv_usejoystick.string,"0") || M_CheckParm("-nojoy"))
		return;
	if(joy_open(cv_usejoystick.string))
		SDLoldjoy = atoi(cv_usejoystick.string);
	else
		return;
	joystick_started = 1;
	return;
}

#ifdef LMOUSE2
//
// I_GetMouse2Event
//
void I_GetMouse2Event()
{
	static unsigned char mdata[5];
	static int i = 0,om2b = 0;
	int di,j,mlp,button;
	event_t event;
	const int mswap[8] = {0,4,1,5,2,6,3,7};
	if(!mouse2_started) return;
	for(mlp=0;mlp<20;mlp++)
	{
	for(;i<5;i++)
	{
		di = read(fdmouse2,mdata+i,1);
		if(di==-1) return;
	}
	if((mdata[0]&0xf8)!=0x80)
	{
		for(j=1;j<5;j++)
			if((mdata[j]&0xf8)==0x80)
				for(i=0;i<5-j;i++) // shift
					mdata[i] = mdata[i+j];
		if(i<5) continue;
	}
	else
	{
		button = mswap[~mdata[0]&0x07];
		for(j=0;j<MOUSEBUTTONS;j++)
		{
			if(om2b&(1<<j))
			{
				if(!(button&(1<<j))) //keyup
				{
					event.type = ev_keyup;
					event.data1 = KEY_2MOUSE1+j;
					D_PostEvent(&event);
					om2b ^= 1 << j;
				}
			}
			else
			{
				if(button&(1<<j))
				{
				event.type = ev_keydown;
				event.data1 = KEY_2MOUSE1+j;
				D_PostEvent(&event);
				om2b ^= 1 << j;
				}
			}
		}
		event.data2 = ((signed char)mdata[1])+((signed char)mdata[3]);
		event.data3 = ((signed char)mdata[2])+((signed char)mdata[4]);
		if(event.data2&&event.data3)
		{
			event.type = ev_mouse2;
			event.data1 = 0;
			D_PostEvent(&event);
		}
	}
	i = 0;
  }
}

//
// I_ShutdownMouse2
//
void I_ShutdownMouse2()
{
	if(fdmouse2!=-1) close(fdmouse2);
	mouse2_started = 0;
}
#elif defined (__WIN32__)

HANDLE mouse2filehandle=0;

static void I_ShutdownMouse2 (void)
{
	event_t event;
	int i;

	if(!mouse2filehandle)
		return;

	SetCommMask( mouse2filehandle, 0 ) ;

	EscapeCommFunction( mouse2filehandle, CLRDTR ) ;
	EscapeCommFunction( mouse2filehandle, CLRRTS ) ;

	PurgeComm( mouse2filehandle, PURGE_TXABORT | PURGE_RXABORT |
								 PURGE_TXCLEAR | PURGE_RXCLEAR ) ;


	CloseHandle(mouse2filehandle);

	// emulate the up of all mouse buttons
	for(i=0;i<MOUSEBUTTONS;i++)
	{
		event.type=ev_keyup;
		event.data1=KEY_2MOUSE1+i;
		D_PostEvent(&event);
	}

	mouse2filehandle=0;
}

#define MOUSECOMBUFFERSIZE 256
int handlermouse2x,handlermouse2y,handlermouse2buttons;

static void I_PoolMouse2(void)
{
	byte buffer[MOUSECOMBUFFERSIZE];
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwLength;
	char       dx,dy;

	static int     bytenum;
	static byte    combytes[4];
	DWORD      i;

	ClearCommError( mouse2filehandle, &dwErrorFlags, &ComStat ) ;
	dwLength = min( MOUSECOMBUFFERSIZE, ComStat.cbInQue ) ;

	if (dwLength > 0)
	{
		if(!ReadFile( mouse2filehandle, buffer, dwLength, &dwLength, NULL ))
		{
			CONS_Printf("\2Read Error on secondary mouse port\n");
			return;
		}

		// parse the mouse packets
		for(i=0;i<dwLength;i++)
		{
			if((buffer[i] & 64)== 64)
				bytenum = 0;
			
			if(bytenum<4)
			   combytes[bytenum]=buffer[i];
			bytenum++;

			if(bytenum==1)
			{
				handlermouse2buttons &= ~3;
				handlermouse2buttons |= ((combytes[0] & (32+16)) >>4);
			}
			else
			if(bytenum==3)
			{
				dx = ((combytes[0] &  3) << 6) + combytes[1];
				dy = ((combytes[0] & 12) << 4) + combytes[2];
				handlermouse2x+= dx;
				handlermouse2y+= dy;
			}
			else
				if(bytenum==4) // fourth byte (logitech mouses)
				{
					if(buffer[i] & 32)
						handlermouse2buttons |= 4;
					else
						handlermouse2buttons &= ~4;
				}
		}
	}
}

void I_GetMouse2Event()
{
	static byte lastbuttons2=0; //mouse movement
	event_t         event;

	if(!mouse2filehandle)
		return;

	I_PoolMouse2();
	// post key event for buttons
	if (handlermouse2buttons!=lastbuttons2)
	{
		int i,j=1,k;
		k=(handlermouse2buttons ^ lastbuttons2); // only changed bit to 1
		lastbuttons2=handlermouse2buttons;

		for(i=0;i<MOUSEBUTTONS;i++,j<<=1)
			if(k & j)
			{
				if(handlermouse2buttons & j)
					event.type=ev_keydown;
				else
					event.type=ev_keyup;
				event.data1=KEY_2MOUSE1+i;
				D_PostEvent(&event);
			}
	}

	if ((handlermouse2x!=0)||(handlermouse2y!=0))
	{
		event.type=ev_mouse2;
		event.data1=0;
//		event.data1=buttons;    // not needed
		event.data2=handlermouse2x<<1;
		event.data3=-handlermouse2y<<1;
		handlermouse2x=0;
		handlermouse2y=0;

		D_PostEvent(&event);
	}
}
#endif

//
// I_StartupMouse2
// 
void I_StartupMouse2 (void)
{
#ifdef LMOUSE2
	struct termios m2tio;
	int i,dtr,rts;
	I_ShutdownMouse2();
	if(cv_usemouse2.value == 0) return;
	if((fdmouse2 = open(cv_mouse2port.string,O_RDONLY|O_NONBLOCK|O_NOCTTY))==-1)
	{
		CONS_Printf("Error opening %s!\n",cv_mouse2port.string);
		return;
	}
	tcflush(fdmouse2, TCIOFLUSH);
	m2tio.c_iflag = IGNBRK;
	m2tio.c_oflag = 0;
	m2tio.c_cflag = CREAD|CLOCAL|HUPCL|CS8|CSTOPB|B1200;
	m2tio.c_lflag = 0;
	m2tio.c_cc[VTIME] = 0;
	m2tio.c_cc[VMIN] = 1;
	tcsetattr(fdmouse2, TCSANOW, &m2tio);
	strupr(cv_mouse2opt.string);
	for(i=0,rts = dtr = -1;i<strlen(cv_mouse2opt.string);i++)
	{
		if(cv_mouse2opt.string[i]=='D')
		{
			if(cv_mouse2opt.string[i+1]=='-')
				dtr = 0;
			else
				dtr = 1;
		}
		if(cv_mouse2opt.string[i]=='R')
		{
			if(cv_mouse2opt.string[i+1]=='-')
				rts = 0;
			else
				rts = 1;
		}
		if((dtr!=-1)||(rts!=-1))
		{
			if(!ioctl(fdmouse2, TIOCMGET, &i))
			{
				if(!dtr)
					i &= ~TIOCM_DTR;
				else if(dtr>0)
					i |= TIOCM_DTR;
			}
			if(!rts)
				i &= ~TIOCM_RTS;
			else if(rts>0)
				i |= TIOCM_RTS;
			ioctl(fdmouse2, TIOCMSET, &i);
		}
	}
	mouse2_started = 1;
	I_AddExitFunc (I_ShutdownMouse2);
#elif defined (__WIN32__)
	DCB        dcb ;

	if(mouse2filehandle)
		I_ShutdownMouse2();

	if(cv_usemouse2.value==0)
		return;

	if(!mouse2filehandle)
	{
		// COM file handle
		mouse2filehandle = CreateFile( cv_mouse2port.string, GENERIC_READ | GENERIC_WRITE,
									   0,                     // exclusive access
									   NULL,                  // no security attrs
									   OPEN_EXISTING,
									   FILE_ATTRIBUTE_NORMAL, 
									   NULL );
		if( mouse2filehandle == (HANDLE)(-1) )
		{
			int e=GetLastError();
			if( e==5 )
				CONS_Printf("\2Can't open %s : Access denied\n"
							"The port is probably already used by one other device (mouse, modem,...)\n",cv_mouse2port.string);
			else
				CONS_Printf("\2Can't open %s : error %d\n",cv_mouse2port.string,e);
			mouse2filehandle=0;
			return;
		}
	}

	// getevent when somthing happens
	//SetCommMask( mouse2filehandle, EV_RXCHAR ) ;
	
	// buffers
	SetupComm( mouse2filehandle, MOUSECOMBUFFERSIZE, MOUSECOMBUFFERSIZE ) ;
	
	// purge buffers
	PurgeComm( mouse2filehandle, PURGE_TXABORT | PURGE_RXABORT |
								 PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

	// setup port to 1200 7N1
	dcb.DCBlength = sizeof( DCB ) ;

	GetCommState( mouse2filehandle, &dcb ) ;

	dcb.BaudRate = CBR_1200;
	dcb.ByteSize = 7;
	dcb.Parity = NOPARITY ;
	dcb.StopBits = ONESTOPBIT ;

	dcb.fDtrControl = DTR_CONTROL_ENABLE ;
	dcb.fRtsControl = RTS_CONTROL_ENABLE ;

	dcb.fBinary = TRUE ;
	dcb.fParity = TRUE ;

	SetCommState( mouse2filehandle, &dcb ) ;
	I_AddExitFunc (I_ShutdownMouse2);
#endif
}

//
// I_Tactile
//
void I_Tactile(int on,int off,int total )
{
	// UNUSED.
	on = off = total = 0;
}

ticcmd_t emptycmd;

ticcmd_t* I_BaseTiccmd(void)
{
	return &emptycmd;
}

//
// I_GetTime
// returns time in 1/TICRATE second tics
//
ULONG  I_GetTime (void)
{
	Uint32        ticks;
	static Uint32 basetime=0;

	// milliseconds since SDL initialization
	ticks = SDL_GetTicks();

	if (!basetime)
		basetime = ticks;

	return (ticks - basetime)*TICRATE/1000;
}

//
// I_Init
//
void I_Init (void)
{
	char title[30];

	I_StartupSound();
	I_InitMusic();

	sprintf(title, "SRB2 %s", VERSIONSTRING);
	 // Window title
	 SDL_WM_SetCaption(title, "SRB2");
}

//
// I_Quit
//
void I_Quit (void)
{
	static int quiting=0;
	/* prevent recursive I_Quit() */
	if(quiting) return;
	quiting = 1;
	M_SaveConfig (NULL);   //save game config, cvars..
	G_SaveGameData(); // Tails 12-08-2002
	//added:16-02-98: when recording a demo, should exit using 'q' key,
	//        but sometimes we forget and use 'F10'.. so save here too.
	if (demorecording)
		G_CheckDemoStatus();
	D_QuitNetGame ();
	I_ShutdownMusic();
	I_ShutdownSound();
	I_ShutdownCD();
	// use this for 1.28 19990220 by Kin
	I_ShutdownGraphics();
	SDL_Quit();
	I_ShutdownSystem();
	/* if option -noendtxt is set, don't print the text */
#ifndef SDLMAIN
	if (!M_CheckParm("-noendtxt") && W_CheckNumForName("ENDOOM")!=-1)
	{
		printf("\r");
		ShowEndTxt();
	}
#endif
	exit(0);
}

void I_WaitVBL(int count)
{
	SDL_Delay(1);
}

void I_BeginRead(void)
{
}

void I_EndRead(void)
{
}

byte* I_AllocLow(int length)
{
	byte* mem;

	mem = (byte *)malloc (length);
	memset (mem,0,length);
	return mem;
}

//
// I_Error
//
extern boolean demorecording;

void I_Error (char *error, ...)
{
	va_list     argptr;
	static int errorcount = 0; // phuck recursive errors
	static boolean shutdowning = false;
#ifdef MAC_ALERT
	char    buffer[8192];
#endif

	// added 11-2-98 recursive error detecting
	if(shutdowning)
	{
		errorcount++;
		// try to shutdown each subsystem separately
		if(errorcount == 2)
			I_ShutdownMusic();
		if(errorcount == 3)
			I_ShutdownSound();
		if(errorcount == 4)
			I_ShutdownCD();
		if(errorcount == 5)
			I_ShutdownGraphics();
		if(errorcount == 6)
			SDL_Quit();
		if(errorcount == 7)
			I_ShutdownSystem();
		if(errorcount == 8)
		{
			M_SaveConfig(NULL);
			G_SaveGameData();
		}
		if(errorcount > 20)
		{
#ifdef MAC_ALERT
			va_start(argptr, error);
			vsprintf(buffer, error, argptr);
			va_end(argptr);
			// 2004-03-03 AJR Since the Mac user is most likely double clicking to run the game, give them a panel.
			MacShowAlert("Recursive Error", buffer, "Quit", NULL, NULL);
#else
			// Don't print garbage
			va_start(argptr,error);
			vfprintf (stderr,error,argptr);
			va_end(argptr);
#endif
			exit(-1); // recursive errors detected
		}
	}
	shutdowning = true;


#ifndef MAC_ALERT
	// Message first.
	va_start (argptr,error);
	fprintf (stderr, "Error: ");
	vfprintf (stderr,error,argptr);
	fprintf (stderr, "\n");
	va_end (argptr);

	fflush( stderr );
#endif
	M_SaveConfig (NULL);   //save game config, cvars..
	G_SaveGameData(); // Tails 12-08-2002

	// Shutdown. Here might be other errors.
	if (demorecording)
		G_CheckDemoStatus();

	D_QuitNetGame ();
	I_ShutdownMusic();
	I_ShutdownSound();
	I_ShutdownCD();
	// use this for 1.28 19990220 by Kin
	I_ShutdownGraphics();
	SDL_Quit();
	I_ShutdownSystem();
#ifdef MAC_ALERT
	va_start(argptr, error);
	vsprintf(buffer, error, argptr);
	va_end(argptr);
	// 2004-03-03 AJR Since the Mac user is most likely double clicking to run the game, give them a panel.
	MacShowAlert("Critical Error", buffer, "Quit", NULL, NULL);
#endif
#if defined (PARANOIA) && defined(__CYGWIN__)
		*(int *)2 = 4; //Alam: Debug!
#endif
	exit(-1);
}
#define MAX_QUIT_FUNCS     16
typedef void (*quitfuncptr)();
static quitfuncptr quit_funcs[MAX_QUIT_FUNCS] =
				{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
					NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
				};
//
//  Adds a function to the list that need to be called by I_SystemShutdown().
//
void I_AddExitFunc(void (*func)())
{
	int c;

	for (c=0; c<MAX_QUIT_FUNCS; c++)
	{
		if (!quit_funcs[c])
		{
			quit_funcs[c] = func;
			break;
		}
	}
}


//
//  Removes a function from the list that need to be called by
//   I_SystemShutdown().
//
void I_RemoveExitFunc(void (*func)())
{
	int c;

	for (c=0; c<MAX_QUIT_FUNCS; c++)
	{
		if (quit_funcs[c] == func)
		{
			while (c<MAX_QUIT_FUNCS-1)
			{
				quit_funcs[c] = quit_funcs[c+1];
				c++;
			}
			quit_funcs[MAX_QUIT_FUNCS-1] = NULL;
			break;
		}
	}
}

//
//  Closes down everything. This includes restoring the initial
//  pallete and video mode, and removing whatever mouse, keyboard, and
//  timer routines have been installed.
//
//  NOTE : Shutdown user funcs. are effectively called in reverse order.
//
void I_ShutdownSystem()
{
	int c;

#ifdef SDLIO
if(logstream) SDL_RWclose(logstream);
#endif
	for (c=MAX_QUIT_FUNCS-1; c>=0; c--)
		if (quit_funcs[c])
			(*quit_funcs[c])();

}

void I_GetDiskFreeSpace(INT64 *freespace)
{
#if defined (LINUX)
#ifdef SOLARIS
	*freespace = MAXINT;
	return;
#else
	struct statfs stfs;
	if(statfs(".",&stfs)==-1)
	{
		*freespace = MAXINT;
		return;
	}
	*freespace = stfs.f_bavail*stfs.f_bsize;
#endif
#elif defined (__WIN32__)

	static MyFunc pfnGetDiskFreeSpaceEx=NULL;
	static boolean testwin95 = false;

	INT64 usedbytes;

	if(!testwin95)
	{
		HINSTANCE h = LoadLibraryA("kernel32.dll");

		if (h)
		{
			pfnGetDiskFreeSpaceEx = (MyFunc)GetProcAddress(h,"GetDiskFreeSpaceExA");
			FreeLibrary(h);
		}
		testwin95 = true;
	}
	if (pfnGetDiskFreeSpaceEx)
	{
		if (!pfnGetDiskFreeSpaceEx(NULL,(PULARGE_INTEGER)freespace,(PULARGE_INTEGER)&usedbytes,NULL))
			*freespace = MAXINT;
	}
	else
	{
		DWORD SectorsPerCluster, BytesPerSector, NumberOfFreeClusters, TotalNumberOfClusters;
		GetDiskFreeSpace(NULL, &SectorsPerCluster, &BytesPerSector,
							&NumberOfFreeClusters, &TotalNumberOfClusters);
		*freespace = BytesPerSector*SectorsPerCluster*NumberOfFreeClusters;
	}
#elif defined(__DC__)
	*freespace = 16*1024*1024;
#else // Dummy for platform independent; 1GB should be enough
	*freespace = 1024*1024*1024;
#endif
}

char *I_GetUserName(void)
{
#ifndef _WIN32_WCE
	static char username[MAXPLAYERNAME];
	char  *p;

#ifdef __WIN32__
	DWORD i = MAXPLAYERNAME;

	if (!GetUserName(username, &i))
#endif
	{
		if (!(p = getenv("USER")))
			if (!(p = getenv("user")))
				if (!(p = getenv("USERNAME")))
					if (!(p = getenv("username")))
						return NULL;
		strncpy(username, p, MAXPLAYERNAME);
	}


	if (strcmp(username, "") != 0)
	 return username;
#endif
	return NULL; // dummy for platform independent version
}

int  I_mkdir(const char *dirname, int unixright)
{
//[segabor]
#if defined(LINUX) || defined(__MACH__) || defined (__CYGWIN__) || defined(__OS2__)
	return mkdir(dirname, unixright);
#elif defined(__WIN32__) || defined(_WIN32_WCE)
	SECURITY_ATTRIBUTES ntrights= { 0, NULL, TRUE }; //TODO: should implement ntright under nt...
#ifdef _WIN32_WCE
	return CreateDirectory((const wchar_t *)dirname,&ntrights); //TODO: wide version
#else
	return CreateDirectoryEx(".",dirname,&ntrights);
#endif
#endif
	return false;
}

static boolean isWadPathOk(char *path)
{
	char wad3path[256];

	sprintf(wad3path, "%s/%s", path, WADKEYWORD);

	if(access(wad3path, R_OK))
		return false; // no access
	else return true;
}

// search for srb2.srb in the given path
static char *searchWad(char *searchDir) 
{
	return NULL; //Alam TODO: use filesearch?
}

// go through all possible paths and look for srb2.srb
static char *locateWad(void)
{
	char *WadPath;
	char *userhome;
	
	// does SRB2WADDIR exist?
	WadPath = getenv("SRB2WADDIR");
	if(WadPath && isWadPathOk(WadPath))
		return WadPath;
	
	// examine current dir
	strcpy(returnWadPath, ".");
	if(isWadPathOk(returnWadPath))
		return returnWadPath;
	
#ifdef DEFAULTWADLOCATION1
	// examine default dirs
	strcpy(returnWadPath, DEFAULTWADLOCATION1);
	if(isWadPathOk(returnWadPath))
		return returnWadPath;
#endif
#ifdef DEFAULTWADLOCATION2
	strcpy(returnWadPath, DEFAULTWADLOCATION2);
	if(isWadPathOk(returnWadPath))
		return returnWadPath;
#endif
	
	// find in $HOME
	userhome = getenv("HOME");
	if(userhome)
	{
		WadPath = searchWad(userhome);
		if(WadPath)
			return WadPath;
	}
#ifdef DEFAULTSEARCHPATH1
	// find in /usr/local
	WadPath = searchWad(DEFAULTSEARCHPATH1);
	if(WadPath)
		return WadPath;
#endif
#ifdef DEFAULTSEARCHPATH2
	// find in /usr/games
	WadPath = searchWad(DEFAULTSEARCHPATH2);
	if(WadPath)
		return WadPath;
#endif
	// if nothing was found
	return NULL;
}

void I_LocateWad(void)
{
	char *waddir;

	waddir = locateWad();

	if(waddir) // change to the directory where we found srb2.srb
#ifdef __WIN32__
		SetCurrentDirectory(waddir);
#elif !defined(_WIN32_WCE)
		chdir(waddir);
#else
		return;
#endif
}

#ifdef LINUX
#define MEMINFO_FILE "/proc/meminfo"
#define MEMTOTAL "MemTotal:"
#define MEMFREE "MemFree:"
#endif

// quick fix for compil
ULONG I_GetFreeMem(ULONG *total)
{
#if defined(LINUX)
	/* LINUX covers all the unix OS's.*/
#ifdef FREEBSD
	struct  vmmeter sum;
	kvm_t *kd;
	struct nlist namelist[]=
	{
#define X_SUM   0
		{"_cnt"},
		{ NULL }
	};
	if ((kd = kvm_open(NULL, NULL, NULL, O_RDONLY, "kvm_open")) == NULL)
	{
		*total = 0L;
		return 0;
	}
	if (kvm_nlist(kd, namelist) != 0)
	{
		kvm_close (kd);
		*total = 0L;
		return 0;
	}
	if (kvm_read(kd,namelist[X_SUM].n_value ,&sum, sizeof(sum)) != sizeof(sum))
	{
		kvm_close (kd);
		*total = 0L;
		return 0;
	}
	kvm_close (kd);

	*total = sum.v_page_count * sum.v_page_size;
	return sum.v_free_count * sum.v_page_size;
#else
#ifdef SOLARIS
	/* Just guess */
	*total = 32 << 20;
	return   32 << 20;
#else
	/* Linux */
	char buf[1024];    
	char *memTag;
	ULONG freeKBytes;
	ULONG totalKBytes;
	int n;
	int meminfo_fd = -1;

	meminfo_fd = open(MEMINFO_FILE, O_RDONLY);
	n = read(meminfo_fd, buf, 1023);
	close(meminfo_fd);

	if(n<0)
	{
		// Error
		*total = 0L;
		return 0;
	}

	buf[n] = '\0';
	if(NULL == (memTag = strstr(buf, MEMTOTAL)))
	{
		// Error
		*total = 0L;
		return 0;
	}

	memTag += sizeof(MEMTOTAL);
	totalKBytes = atoi(memTag);

	if(NULL == (memTag = strstr(buf, MEMFREE)))
	{
		// Error
		*total = 0L;
		return 0;
	}

	memTag += sizeof(MEMFREE);
	freeKBytes = atoi(memTag);

	*total = totalKBytes << 10;
	return freeKBytes << 10;
#endif /* SOLARIS */
#endif /* FREEBSD */
#elif defined(__WIN32__) || defined(_WIN32_WCE)
	MEMORYSTATUS info;

	info.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus( &info );
	if( total )
		*total = info.dwTotalPhys;
	return info.dwAvailPhys;
#elif defined(__OS2__)
	ULONG pr_arena;

	DosQuerySysInfo( QSV_TOTPHYSMEM, QSV_TOTPHYSMEM,
							(PVOID) total, sizeof(ULONG));
	DosQuerySysInfo( QSV_MAXPRMEM, QSV_MAXPRMEM,
				(PVOID) &pr_arena, sizeof(ULONG));

	return pr_arena;
#else
	/*  Not Linux.*/
	*total = 16<<20;
	return 16<<20;
#endif /* LINUX */
}
