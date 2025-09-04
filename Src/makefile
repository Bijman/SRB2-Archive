#############################################################################
#
# $Id: makefile,v 1.75 2004/04/20 00:34:26 andy $
#     GNU Make makefile for SRB2
#
# Copyright (C) 1998-2000 by DooM Legacy Team.
# Copyright (C) 2003-2004 by Sonic Team Jr.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
#     -DUSEASM -> use assembly routines instead of C ones, where possible.
#     -DPC_DOS -> use dos specific code (eg:textmode stuff)...
#     -DLINUX  -> use for the linux specific
#     -D__OS2__  -> use for the OS/2 specific
#
# Sets:
#     Compile the DGJPP/DOS version with 'make WATTCP=1'
#     Compile the DirectX/Mingw version with 'make MINGW=1'
#     Compile the SDL/Mingw version with 'make MINGW=1 SDL=1'
#     Compile the SDL/Linux version with 'make LINUX=1 SDL=1'
#     Compile the SDL/Cygwin version with 'make CYGWIN32=1 SDL=1'
#     Compile the SDL/FreeBSD version with 'gmake FREEBSD=1 FBSD_SDL=1'
#
# Addon:
#     Compile with GCC 2.97 version or lower, add 'GCC29=1'
#     Compile a debug/profile version, add 'PROFILEMODE=1'
#     Compile without NASM's tmap.nas or on a non-i386 system, add NOASM=1
#
# Addon for SDL:
#     Compile without SDL_Mixer, add NOMIXER=1
#     Compile without OpenGl, add NOHW=1
#     Compile Mingw/SDL without S_DS3S, add NODS=1
#     Comple with S_FMOD3D, add FMOD=1 (WIP)
#     Comple with S_OPENAL, add OPENAL=1 (WIP)
#     To link with the whole SDL_Image lib to load Icons, add SDL_IMAGE=1 but it isn't not realy needed
#     To link with SDLMain to hide console or make on a console-less binary, add SDLMAIN=1
#
# Old Sets:
#     Compile the Linux version with 'make LINUX=1'
#     Compile the FreeBSD version with 'gmake FREEBSD=1'
#     Compile the Solaris/SDL version with 'make SOLARIS=1'
#     Compile the OS/2 version with  'make OS2=1'

#
#############################################################################

# gcc or g++
#CC=@gcc
CC=gcc

#if use PGCC or EGCS
PGCC=1

# -W -Wno-unused -Wno-sign-compare
WFLAGS=-Wall

#uncomment if you want to use the POLL_POINTER hack
#POLL_POINTER=-DPOLL_POINTER

#WITH_DGA=1


#indicate platform and what interface use with
ifndef OS2
ifndef LINUX
ifndef CYGWIN32
ifndef FREEBSD
ifndef SOLARIS
ifndef MINGW
	DJGPPDOS=1
endif
endif
endif
endif
endif
endif

#determine the interface directory (where you put all i_*.c)
i_cdmus_o=$(O)/i_cdmus.o
i_net_o=$(O)/i_net.o
i_system_o=$(O)/i_system.o
i_sound_o=$(O)/i_sound.o
i_main_o=$(O)/i_main.o
BIN=../bin
NASM = nasm
ifdef DJGPPDOS
	INTERFACE=DJGPPDOS
	NASMFORMAT=coff
else
	ifdef LINUX
		INTERFACE=linux_x
		NASMFORMAT=elf -DLINUX
	endif
	ifdef CYGWIN32
		INTERFACE=linux_x
		NASMFORMAT=win32
		NASM=Nasmw
	endif
	ifdef MINGW
		INTERFACE=win32
		NASMFORMAT=win32
		NASM=Nasmw
	endif

	ifdef FREEBSD
		INTERFACE=linux_x
		NASMFORMAT=elf -DLINUX
	endif
	ifdef SOLARIS
		INTERFACE=sdl
		NASMFORMAT=elf -DLINUX
	endif
	ifdef OS2
		INTERFACE=os2
		NASMFORMAT=coff
	endif
	ifdef SDL
		INTERFACE=sdl
	endif
endif

ifdef DJGPPDOS
	# subdirectory for objects


	O=../objs/djgppdos
ifdef WATTCP
	O=../objs/djgppdos/wattcp
endif

	#use the x86 asm code
ifndef NOASM
	USEASM=1
endif
	# options
	OPTS=-DPC_DOS
ifdef WATTCP
	OPTS := $(OPTS) -DWATTCP
endif

	DEBUGLIBS=-lalleg
ifdef WATTCP
	DEBUGLIBS := $(DEBUGLIBS) -lwatt
else
	DEBUGLIBS := $(DEBUGLIBS) -lsocket
endif
	LIBS=-lalleg
ifdef WATTCP
	LIBS := -lwatt $(LIBS)
else
	LIBS := -lsocket $(LIBS)
endif
	SFLAGS=

ifdef PROFILEMODE
	CFLAGS=-g
endif

	OBJS=$(O)/i_video.o $(O)/vid_vesa.o

	# name of the exefile
ifdef WATTCP
	EXENAME=srb2dos
else
	EXENAME=srb2w16
endif

endif #ifdef DJGPPDOS

ifdef LINUX

	# subdirectory for objects
	O=../objs/linux
	MUSSERV=linux_x/musserv
	SNDSERV=linux_x/sndserv

	#use the x86 asm code
ifndef CYGWIN32
ifndef NOASM
	USEASM=1
endif
endif
	OPTS=-DLINUX -DOLD_SOUND_DRIVER -DMUSSERV -DSNDSERV -DDIRECTFULLSCREEN -DSTDC_HEADERS
ifndef GGI
	X=1
endif

ifdef LJOYSTICK
	OPTS := $(OPTS) -DLJOYSTICK
endif
ifdef LMOUSE2
	OPTS := $(OPTS) -DLMOUSE2
endif

	ifdef X

		OPTS := $(OPTS) -DVID_X11 -DHWRENDER $(POLL_POINTER) -I/usr/X11R6/include
		LDFLAGS=-L/usr/X11R6/lib
		LIBS=-lXext -lX11 -lm -ldl -lXxf86vm
		ifdef WITH_DGA
			OPTS += -DWITH_DGA
			LIBS += -lXxf86dga -lXext
		endif #WITH_DGA
		# name of the exefile
		EXENAME=llxsrb2
		SFLAGS=-g $(OPTS)
		OBJS=$(O)/i_video_xshm.o $(O)/dosstr.o $(O)/searchp.o $(O)/endtxt.o \
		 $(O)/hw_bsp.o $(O)/hw_draw.o $(O)/hw_light.o $(O)/hw_main.o \
		 $(O)/hw_md2.o $(O)/hw_cache.o $(O)/hw_trick.o

	else
		OPTS := $(OPTS) -DVID_GGI
		LDFLAGS=
		LIBS=-lggi -lm
		# name of the exefile
		EXENAME=llggisrb2
		SFLAGS=-g $(OPTS)
		OBJS=$(O)/i_video_ggi.o $(O)/dosstr.o $(O)/searchp.o $(O)/endtxt.o \
#		 $(O)/hw_bsp.o $(O)/hw_draw.o $(O)/hw_light.o $(O)/hw_main.o \
#		 $(O)/hw_md2.o $(O)/hw_cache.o $(O)/hw_trick.o

	endif
endif

ifdef MINGW
	O=../objs/mingw
ifndef NOASM
	USEASM=1
endif
	OPTS=-DSTDC_HEADERS -D_WINDOWS
	LIBS=-lmingw32 -mwindows -lddraw -ldinput -ldsound -ldxguid -lfmod -lwinmm -lwsock32
	LDFLAGS=-L"./win32/FMOD"
	EXENAME=srb2
	SFLAGS=-g $(OPTS)
ifndef SDL
	i_cdmus_o=$(O)/win_cd.o
	i_net_o=$(O)/win_net.o
	i_system_o=$(O)/win_sys.o $(O)/SRB2.res
	i_sound_o=$(O)/win_snd.o $(O)/mid2strm.o
	i_main_o=$(O)/win_dbg.o $(O)/win_main.o
endif
	OBJS=$(O)/dx_error.o $(O)/fabdxlib.o $(O)/win_vid.o
ifdef NOHW
	OPTS := $(OPTS) -DNOHW
else
	OBJS := $(OBJS) $(O)/win_dll.o $(O)/hw_bsp.o $(O)/hw_draw.o $(O)/hw_light.o \
		 $(O)/hw_main.o $(O)/hw_md2.o $(O)/hw_cache.o $(O)/hw_trick.o $(O)/hw3sound.o
endif
endif

ifdef FREEBSD

	# subdirectory for objects
	O=../objs/freebsd
ifndef SDL
	i_cdmus_o=$(O)/i_cdmus_fbsd.o
endif
	MUSSERV=linux_x/musserv
	SNDSERV=linux_x/sndserv
	OSVERSION:=$(shell /sbin/sysctl -n kern.osreldate)
	OSMAJOR:=$(shell echo $(OSVERSION) | sed 's/.....$$//')
	ifneq ($(OSMAJOR), 5)
		GCC29=1
	endif

	#use the x86 asm code
ifndef NOASM
	USEASM=1
endif
ifndef FBSD_SDL
	OPTS=-DLINUX -DFREEBSD -DOLD_SOUND_DRIVER -DSNDSERV -DDIRECTFULLSCREEN
ifndef FBSD_GGI
	X=1
endif
endif

	ifdef X

		OPTS := $(OPTS) -DVID_X11 -DHWRENDER $(POLL_POINTER) -I/usr/X11R6/include
		LDFLAGS=-L/usr/X11R6/lib
		LIBS=-lXext -lX11 -lm -lXxf86vm -lipx -lkvm -pthread
		# name of the exefile
		EXENAME=llxsrb2
		SFLAGS=-g $(OPTS)
		OBJS=$(O)/i_video_xshm.o $(O)/dosstr.o $(O)/searchp.o $(O)/endtxt.o \
		 $(O)/hw_bsp.o $(O)/hw_draw.o $(O)/hw_light.o $(O)/hw_main.o \
		 $(O)/hw_md2.o $(O)/hw_cache.o $(O)/hw_trick.o

	else #ifdef X
	ifdef FBSD_SDL
	INTERFACE=sdl
		OPTS := -DLINUX -DFREEBSD -DHWRENDER -DDIRECTFULLSCREEN -DHAVE_MIXER -DSDL -DVID_X11 $(POLL_POINTER) -I/usr/local/include/SDL11 -I/usr/X11R6/include
		SFLAGS=-g $(OPTS)
		BJS=$(O)/i_video.o $(O)/dosstr.o $(O)/endtxt.o \
		 $(O)/hw_bsp.o $(O)/hw_draw.o $(O)/hw_light.o $(O)/hw_main.o \
		 $(O)/hw_md2.o $(O)/hw_cache.o $(O)/hw_trick.o \
		 $(O)/r_opengl.o $(O)/ogl_sdl.o $(O)/hwsym_sdl.o

		DEBUGLIBS=$(LIBS)

		LDFLAGS=-L/usr/X11R6/lib -L/usr/local/lib
		LIBS=-lm -lSDL-1.1 -lSDL_mixer -lGL -lGLU -lipx -lkvm -pthread
		DEBUGLIBS=$(LIBS)

		# name of the exefile
		EXENAME=lsdlsrb2

	else #ifdef FBSD_SDL
		OPTS := $(OPTS) -DVID_GGI -I/usr/local/include
		LDFLAGS=
		LIBS=-lggi -lm -lipx
		# name of the exefile
		EXENAME=llggisrb2
		SFLAGS=-g $(OPTS)
		OBJS=$(O)/i_video_ggi.o $(O)/dosstr.o $(O)/searchp.o $(O)/endtxt.o \
		 $(O)/hw_bsp.o $(O)/hw_draw.o $(O)/hw_light.o $(O)/hw_main.o \
		 $(O)/hw_md2.o $(O)/hw_cache.o $(O)/hw_trick.o

	endif #ifdef FBSD_SDL
	endif #ifdef X
endif #ifdef FREEBSD

ifdef SOLARIS

	# subdirectory for objects
	O=../objs/solaris
	OPTS := -DLINUX -DHWRENDER -DDIRECTFULLSCREEN -DHAVE_MIXER -DSDL -DVID_X11
	OPTS := $(OPTS) -I/usr/local/include -I/usr/X11R6/include
	OPTS := $(OPTS) -DBSD_COMP -DSOLARIS -DNOIPX
	OPTS := $(OPTS) -DDONTDEFINEBOOL -DTRUE=1 -DFALSE=0 -DINADDR_NONE=INADDR_ANY
	OPTS := $(OPTS) -D__BIG_ENDIAN__
	SFLAGS=-g $(OPTS)

	OBJS=$(O)/i_video.o $(O)/dosstr.o $(O)/endtxt.o \
	 $(O)/hw_bsp.o $(O)/hw_draw.o $(O)/hw_light.o $(O)/hw_main.o \
	 $(O)/hw_md2.o $(O)/hw_cache.o $(O)/hw_trick.o \
	 $(O)/r_opengl.o $(O)/ogl_sdl.o $(O)/hwsym_sdl.o


	LDFLAGS = -L/usr/X/lib -L/usr/local/lib
	LIBS = -lSDL -lSDL_mixer -lGL -lGLU -lpthread -lsocket -lnsl -lm

	# name of the exefile
	EXENAME=lsdlsrb2


endif #ifdef SOLARIS

ifdef SDL
	# subdirectory for objects
ifdef MINGW
	O=../objs/Mingw/SDL
endif
	#use the x86 asm code
ifndef CYGWIN32
ifndef NOASM
	USEASM=1
endif
endif
	SFLAGS=-g $(OPTS)
	OBJS=$(O)/i_video.o $(O)/dosstr.o $(O)/endtxt.o
	DEBUGLIBS=$(LIBS)
	LDFLAGS=
	OPTS=
	LIBS=
	OPTS := $(OPTS) -DDIRECTFULLSCREEN -DSDL -DSTDC_HEADERS

ifdef NOIPX
	OPTS := $(OPTS) -DNOIPX
endif

ifdef MINGW
	OPTS := $(OPTS) -DMINGW
	OBJS := $(OBJS) $(O)/SRB2.res
else
ifdef CYGWIN32
	O=../objs/cygwin
	OBJS := $(OBJS) $(O)/SRB2.res
endif
endif
ifdef LINUX
	OPTS := $(OPTS) -DLINUX -I/usr/X11R6/include
	LDFLAGS := $(LDFLAGS) -L/usr/local/lib -L/usr/X11R6/lib
endif

ifndef NOHW
	OPTS := $(OPTS) -DHWRENDER
	#Hurdler: not really supported and not tested recently
	#OPTS := $(OPTS) -DUSE_PALETTED_TEXTURE
	OBJS := $(OBJS) $(O)/hw3sound.o $(O)/hw_bsp.o $(O)/hw_draw.o \
	 $(O)/hw_light.o $(O)/hw_main.o $(O)/hw_md2.o $(O)/hw_cache.o \
	 $(O)/hw_trick.o $(O)/r_opengl.o $(O)/ogl_sdl.o $(O)/hwsym_sdl.o
ifdef OPENAL
	OBJS := $(OBJS) $(O)/s_openal.o
	OPTS := $(OPTS) -DHW3SOUND
else
ifdef FMOD
	OBJS := $(OBJS) $(O)/s_fmod.o
	OPTS := $(OPTS) -DHW3SOUND
ifdef MINGW
	OPTS := $(OPTS) -I"./win32/FMOD"
	LDFLAGS := $(LDFLAGS) -L"./win32/FMOD"
endif
else
ifdef MINGW
ifndef NODS
	OBJS := $(OBJS) $(O)/s_ds3d.o
	OPTS := $(OPTS) -DHW3SOUND
endif
endif
endif
endif
endif

ifdef PURESDL
	OPTS := $(OPTS) -I/usr/include/ -DPURESDL
endif

	LIBS := -lSDL $(LIBS) -lm
ifndef NOMIXER
	OPTS := $(OPTS) -DHAVE_MIXER
	LIBS := $(LIBS) -lSDL_mixer
endif
ifdef SDL_IMAGE
	OPTS := $(OPTS) -DHAVE_IMAGE
	LIBS := $(LIBS) -lSDL_image
endif
ifdef MINGW
	LIBS := $(LIBS) -lwsock32
ifndef NOHW
	LIBS := $(LIBS) -lopengl32 -lglu32
endif
else
	LIBS := $(LIBS) -lpthread
ifndef NOHW
	LIBS := $(LIBS) -lGL -lGLU
endif
endif

ifdef SDLMAIN
	OPTS := $(OPTS) -DSDLMAIN
	LIBS := -lSDLmain $(LIBS)
ifdef MINGW
	LIBS := -lmingw32 $(LIBS) -mwindows
endif
endif

ifndef NOHW
ifdef OPENAL
ifdef MINGW
	LIBS := $(LIBS) -lopenal32
else
	LIBS := $(LIBS) -lopenal
endif
else
ifdef FMOD
ifdef LINUX
	LIBS := $(LIBS) -lfmod-3.73
else
	LIBS := $(LIBS) -lfmod
endif
else
ifdef MINGW
ifndef NODS
	LIBS := $(LIBS) -ldsound -luuid
endif
endif
endif
endif
endif
	DEBUGLIBS=$(LIBS)

	# name of the exefile
ifdef MINGW
	EXENAME=srb2sdl
else
ifdef CYGWIN32
	EXENAME=lsdlsrb2
else
	EXENAME=lsdlsrb2
endif
endif
endif

ifdef OS2

	# subdirectory for objects
	O=../objs/OS2

	#use the x86 asm code
ifndef NOASM
	USEASM=1
endif
	PROFILEMODE=1
	# options
	OPTS=-Zmt -D__OS2__

	DEBUGLIBS=-lmmpm2 -lsocket -g
	LIBS= $(INTERFACE)/I_pm.def -lmmpm2 -lsocket -g

	SFLAGS=-g $(OPTS)

	OBJS=$(O)/i_video.o $(O)/i_dart.o $(O)/i_dive.o $(O)/I_pm.o \
		$(O)/printf.o $(O)/I_pm.res

	# name of the exefile
	EXENAME=srb2os

endif

ifndef SOLARIS
ifndef GCC29
	M5=-march=pentium
	M4=-march=486
else
	M5=-mpentium
	M4=-m486
endif
endif

ifdef RP
	OTHERDEF = -DBUGFIXES -DRP -DDARKWORLD -DMULTISKY -DP_ENEMY_H -DNOFLY -DNOMARIO -DNOINTRO
	OBJS := $(OBJS) $(O)/srb2RP.o $(O)/DarkWorld.o
	EXENAME := $(EXENAME)RP
	# O := $(O)/RP
else
ifdef MGB2
	OTHERDEF = -DBUGFIXES -DMARIO -DP_ENEMY_H -DNOINTRO
	OBJS := $(OBJS) $(O)/mgb2.o $(O)/DarkWorld.o
	EXENAME := $(EXENAME)MGB2
	# O := $(O)/MGB2
endif
endif


ifdef PROFILEMODE

	# build with profiling information
	ifdef PGCC
		CFLAGS = -g -pg -DPARANOIA $(OTHERDEF) -O0 -fwritable-strings -fno-strict-aliasing $(OPTS) -fno-exceptions
	else
		CFLAGS = -g -pg $(OTHERDEF) $(M4) -Os -ffast-math -fwritable-strings -fno-strict-aliasing $(OPTS) -fno-exceptions
	endif
	WINDRESFLAGS = -D_DEBUG
else


	# build a normal optimised version
	ifdef PGCC
		CFLAGS = $(OPTS) $(M5) $(OTHERDEF) -Os -fomit-frame-pointer -fwritable-strings -fno-strict-aliasing
		# -fgcse -fcse-follow-jumps -fcse-skip-blocks -frerun-cse-after-loop -ffunction-cse \
		# -fexpensive-optimizations -fforce-mem -fstrength-reduce -fthread-jumps  \
		# -fregmove -fschedule-insns -fschedule-insns2 -fmove-all-movables \
		# -fcaller-saves -finline-functions -mieee-fp -mfp-ret-in-387 -m80387 -mhard-float -finline
	else
		CFLAGS = $(M4) $(OTHERDEF) -Os -ffast-math -fomit-frame-pointer -fwritable-strings -fno-strict-aliasing $(OPTS)
	endif
	LIBS := -s $(LIBS)
	WINDRESFLAGS = -DNDEBUG
endif

SNDSERV_EXTRAFLAGS:=
ifdef HAVE_ESD
SNDSERV_EXTRAFLAGS:= HAVE_ESD=1
endif

# not too sophisticated dependency
OBJS := $(i_main_o)    \
		$(O)/dstrings.o  \
		$(O)/i_tcp.o     \
		$(O)/tables.o    \
		$(O)/f_finale.o  \
		$(O)/f_wipe.o    \
		$(O)/d_main.o    \
		$(O)/d_net.o     \
		$(O)/d_items.o   \
		$(O)/g_game.o    \
		$(O)/m_menu.o    \
		$(O)/m_misc.o    \
		$(O)/m_argv.o    \
		$(O)/m_bbox.o    \
		$(O)/m_fixed.o   \
		$(O)/m_swap.o    \
		$(O)/m_cheat.o   \
		$(O)/m_random.o  \
		$(O)/am_map.o    \
		$(O)/p_ceilng.o  \
		$(O)/p_doors.o   \
		$(O)/p_enemy.o   \
		$(O)/p_floor.o   \
		$(O)/p_genlin.o  \
		$(O)/p_info.o    \
		$(O)/p_inter.o   \
		$(O)/p_lights.o  \
		$(O)/p_map.o     \
		$(O)/p_maputl.o  \
		$(O)/p_plats.o   \
		$(O)/p_pspr.o    \
		$(O)/p_setup.o   \
		$(O)/p_sight.o   \
		$(O)/p_spec.o    \
		$(O)/p_switch.o  \
		$(O)/p_mobj.o    \
		$(O)/p_telept.o  \
		$(O)/p_tick.o    \
		$(O)/p_saveg.o   \
		$(O)/p_user.o    \
		$(O)/r_bsp.o     \
		$(O)/r_data.o    \
		$(O)/r_draw.o    \
		$(O)/r_main.o    \
		$(O)/r_plane.o   \
		$(O)/r_segs.o    \
		$(O)/r_sky.o     \
		$(O)/r_things.o  \
		$(O)/w_wad.o     \
		$(O)/wi_stuff.o  \
		$(O)/v_video.o   \
		$(O)/st_lib.o    \
		$(O)/st_stuff.o  \
		$(O)/hu_stuff.o  \
		$(O)/s_sound.o   \
		$(O)/z_zone.o    \
		$(O)/info.o      \
		$(O)/sounds.o    \
		$(O)/p_fab.o     \
		$(O)/dehacked.o  \
		$(O)/qmus2mid.o  \
		$(O)/g_input.o   \
		$(O)/screen.o    \
		$(O)/console.o   \
		$(O)/command.o   \
		$(O)/d_clisrv.o  \
		$(O)/r_splats.o  \
		$(O)/g_state.o   \
		$(O)/t_func.o    \
		$(O)/t_oper.o    \
		$(O)/t_parse.o   \
		$(O)/t_prepro.o  \
		$(O)/t_script.o  \
		$(O)/t_spec.o    \
		$(O)/t_vari.o    \
		$(O)/d_netfil.o  \
		$(O)/mserv.o     \
		$(O)/p_hsight.o  \
		$(O)/s_amb.o     \
		$(O)/sb_bar.o    \
		$(O)/p_heretic.o \
		$(O)/md5.o       \
		$(O)/filesrch.o  \
		$(i_cdmus_o)     \
		$(i_net_o)       \
		$(i_system_o)    \
		$(i_sound_o)     \
		$(OBJS)



ifdef USEASM
	OBJS:= $(O)/tmap.o $(O)/vid_copy.o $(OBJS)
	OPTS:= $(OPTS) -DUSEASM
endif



ifdef MINGW
ifndef SDL
all:	 pre-build $(BIN)/$(EXENAME) dll
endif
endif

ifdef LINUX
ifdef SDL
all:	 pre-build $(BIN)/$(EXENAME)
else
all:	 pre-build $(BIN)/$(EXENAME) dll musserv sndserv
endif
else
ifdef FREEBSD
all:	 pre-build $(BIN)/$(EXENAME) dll sndserv
else
all:	 pre-build $(BIN)/$(EXENAME)
endif
endif

pre-build:
ifndef DJGPPDOS
	rm -f $(O)/d_main.o
	rm -f $(O)/d_netcmd.o
	rm -f $(O)/srb2RP.o
endif

clean:
#	rm -f $(O)/*
ifdef MINGW
	rm -f *.o *~ *.flc
	rm -f $(O)/*.o
	rm -f $(O)/*.res
else
ifdef CYGWIN32
	rm -f *.o *~ *.flc
	rm -f $(O)/*.o
	rm -f $(O)/*.res
else
ifdef SDL
	rm -f *.o *~ *.flc
	rm -f $(O)/*.o
else
ifdef LINUX
	rm -f *.o *~ *.flc
	rm -f $(O)/*.o
	rm -f $(SNDSERV)/linux/*
	rm -f $(MUSSERV)/linux/*
else
ifdef SOLARIS
	rm -f *.o *~ *.flc
	rm -f $(O)/*.o
	rm -f $(O)/sndserv/*
	rm -f $(O)/musserv/*
else
ifdef FREEBSD
	rm -f *.o *~ *.flc
	rm -f $(O)/*.o
	rm -f $(O)/sndserv/*
	rm -f $(O)/musserv/*
else
ifdef DJGPPDOS
	del $(O)/*
else
	rm -f $(O)/*
endif
endif
endif
endif
endif
endif
endif

#make a big srb2.s that is the disasm of the exe (dos only ?)
asm:
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -g $(O)/i_main.o \
	-o $(O)/tmp.exe $(DEBUGLIBS)
	objdump  -d $(O)/tmp.exe --no-show-raw-insn > srb2.s
	del $(O)\tmp.exe

# executable

$(BIN)/$(EXENAME):	$(O) $(OBJS) $(O)/d_netcmd.o
#	@mkdir $(BIN)
	@echo Linking $(EXENAME)...
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(O)/d_netcmd.o \
	-o $(BIN)/$(EXENAME) $(LIBS)

$(O):
	@mkdir $(O)
	@mkdir $(O)/sndserv
	@mkdir $(O)/musserv

ifndef SDL
#dll
ifndef MINGW
dll : $(O)/r_opengl.o $(O)/ogl_x11.o
	@echo Linking R_OpenGL.so...
	$(CC) $(CFLAGS) -Os -o $(BIN)/r_opengl.so -shared -nostartfiles $(O)/r_opengl.o $(O)/ogl_x11.o -L/usr/X11R6/lib -L/usr/local/lib -lX11 -lXext -lGL -lGLU -lm
else
dll : opengl_dll
all_dll: opengl_dll minigl_dll ds3d_dll fmod_dll
opengl_dll : $(O)/r_opengl.o $(O)/ogl_win.o hardware/r_mingw.def
	@echo Linking R_OpenGL.dll...
	dllwrap --def hardware/r_mingw.def $(O)/r_opengl.o $(O)/ogl_win.o -lopengl32 -lglu32 -lgdi32 -mwindows $(M5) $(CFLAGS) -o $(BIN)/r_opengl.dll

minigl_dll: $(O)/r_minigl.o hardware/r_mingw.def
	@echo Linking R_MiniGL.dll...
	dllwrap --def hardware/r_mingw.def $(O)/r_minigl.o -lopengl32 -lglu32 -lgdi32 -mwindows $(M5) $(CFLAGS) -o $(BIN)/r_minigl.dll

ds3d_dll: $(O)/s_ds3d.o hardware/s_mingw.def
	@echo Linking S_DS3d.dll...
	dllwrap --def hardware/s_mingw.def $(O)/s_ds3d.o -ldsound -luuid -mwindows $(M5) $(CFLAGS) -o $(BIN)/s_ds3d.dll

fmod_dll: $(O)/s_fmod.o hardware/s_mingw.def
	@echo Linking S_FMOD.dll...
	dllwrap --def hardware/s_mingw.def $(O)/s_fmod.o -L"./win32/FMOD" -I"./win32/FMOD" -lfmod -mwindows $(M5) $(CFLAGS) -o $(BIN)/s_fmod.dll

openal_dll: $(O)/s_openal.o hardware/s_mingw.def
	@echo Linking S_OpenAL.dll...
	dllwrap --def hardware/s_mingw.def $(O)/s_openal.o -lopenal32 -mwindows $(M5) $(CFLAGS) -o $(BIN)/s_openal.dll

endif

ifndef MINGW
musserv:
	@mkdir -p $(O)/linux
	cd $(MUSSERV) \
	&& make -f Makefile.linux

sndserv:
	@mkdir -p $(O)/linux
	cd $(SNDSERV) \
	&& $(MAKE) $(SNDSERV_EXTRAFLAGS)


$(O)/r_opengl.o: hardware/r_opengl/r_opengl.c hardware/r_opengl/r_opengl.h \
 doomdef.h doomtype.h g_state.h m_swap.h hardware/hw_drv.h screen.h \
 command.h hardware/hw_data.h hardware/hw_glide.h hardware/hw_defs.h \
 hardware/hw_md2.h hardware/hw_glob.h hardware/hw_main.h am_map.h \
 d_event.h d_player.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(M5) -Os -o $(O)/r_opengl.o -DHWRENDER -DVID_X11 -DLINUX $(POLL_POINTER) -shared -nostartfiles -I/usr/X11R6/include -c hardware/r_opengl/r_opengl.c
endif
else
ifdef SDL
ifdef MINGW
$(O)/r_opengl.o: hardware/r_opengl/r_opengl.c hardware/r_opengl/r_opengl.h \
 doomdef.h doomtype.h g_state.h m_swap.h hardware/hw_drv.h screen.h \
 command.h hardware/hw_data.h hardware/hw_glide.h hardware/hw_defs.h \
 hardware/hw_md2.h hardware/hw_glob.h hardware/hw_main.h am_map.h \
 d_event.h d_player.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@
else
$(O)/r_opengl.o: hardware/r_opengl/r_opengl.c hardware/r_opengl/r_opengl.h \
 doomdef.h doomtype.h g_state.h m_swap.h hardware/hw_drv.h screen.h \
 command.h hardware/hw_data.h hardware/hw_glide.h hardware/hw_defs.h \
 hardware/hw_md2.h hardware/hw_glob.h hardware/hw_main.h am_map.h \
 d_event.h d_player.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(CFLAGS) $(WFLAGS) -I/usr/X11R6/include -c $< -o $@
endif
endif

ifdef FREEBSD
ifdef FBSD_SDL
$(O)/r_opengl.o: hardware/r_opengl/r_opengl.c hardware/r_opengl/r_opengl.h \
 doomdef.h doomtype.h g_state.h m_swap.h hardware/hw_drv.h screen.h \
 command.h hardware/hw_data.h hardware/hw_glide.h hardware/hw_defs.h \
 hardware/hw_md2.h hardware/hw_glob.h hardware/hw_main.h am_map.h \
 d_event.h d_player.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(CFLAGS) $(WFLAGS) -I/usr/X11R6/include -c $< -o $@
else  #FBSD_SDL
$(O)/r_opengl.o: hardware/r_opengl/r_opengl.c hardware/r_opengl/r_opengl.h \
 doomdef.h doomtype.h g_state.h m_swap.h hardware/hw_drv.h screen.h \
 command.h hardware/hw_data.h hardware/hw_glide.h hardware/hw_defs.h \
 hardware/hw_md2.h hardware/hw_glob.h hardware/hw_main.h am_map.h \
 d_event.h d_player.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(M5) -Os -o $(O)/r_opengl.o -DHWRENDER -DVID_X11 -DLINUX -DFREEBSD $(POLL_POINTER) -shared -nostartfiles -I/usr/X11R6/include -c hardware/r_opengl/r_opengl.c
endif #FBSD_SDL
endif #FREEBSD

ifndef MINGW
ifdef SOLARIS
$(O)/r_opengl.o: hardware/r_opengl/r_opengl.c hardware/r_opengl/r_opengl.h \
 doomdef.h doomtype.h g_state.h m_swap.h hardware/hw_drv.h screen.h \
 command.h hardware/hw_data.h hardware/hw_glide.h hardware/hw_defs.h \
 hardware/hw_md2.h hardware/hw_glob.h hardware/hw_main.h am_map.h \
 d_event.h d_player.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(CFLAGS) $(WFLAGS) -I/usr/X/include -c $< -o $@
endif


ifndef FREEBSD
$(O)/ogl_x11.o: hardware/r_opengl/ogl_x11.c hardware/r_opengl/r_opengl.h \
 doomdef.h doomtype.h g_state.h m_swap.h hardware/hw_drv.h screen.h \
 command.h hardware/hw_data.h hardware/hw_glide.h hardware/hw_defs.h \
 hardware/hw_md2.h hardware/hw_glob.h hardware/hw_main.h am_map.h \
 d_event.h d_player.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(M5) -Os -o $(O)/ogl_x11.o -DHWRENDER -DVID_X11 -DLINUX $(POLL_POINTER) -shared -nostartfiles -I/usr/X11R6/include -c hardware/r_opengl/ogl_x11.c
else
$(O)/ogl_x11.o: hardware/r_opengl/ogl_x11.c hardware/r_opengl/r_opengl.h \
 doomdef.h doomtype.h g_state.h m_swap.h hardware/hw_drv.h screen.h \
 command.h hardware/hw_data.h hardware/hw_glide.h hardware/hw_defs.h \
 hardware/hw_md2.h hardware/hw_glob.h hardware/hw_main.h am_map.h \
 d_event.h d_player.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h r_defs.h
	 $(CC) $(M5) -Os -o $(O)/ogl_x11.o -DHWRENDER -DVID_X11 -DLINUX -DFREEBSD $(POLL_POINTER) -shared -nostartfiles -I/usr/X11R6/include -c hardware/r_opengl/ogl_x11.c
endif
endif
endif

ifdef MINGW
ifndef SDL
$(O)/r_opengl.o: hardware/r_opengl/r_opengl.c hardware/r_opengl/r_opengl.h \
 doomdef.h doomtype.h g_state.h m_swap.h hardware/hw_drv.h screen.h \
 command.h hardware/hw_data.h hardware/hw_glide.h hardware/hw_defs.h \
 hardware/hw_md2.h hardware/hw_glob.h hardware/hw_main.h am_map.h \
 d_event.h d_player.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(CFLAGS) $(WFLAGS) -D_WINDOWS -DHWRENDER -DMINGW -mwindows -c $< -o $@

$(O)/ogl_win.o: hardware/r_opengl/ogl_win.c hardware/r_opengl/r_opengl.h \
 doomdef.h doomtype.h g_state.h m_swap.h hardware/hw_drv.h screen.h \
 command.h hardware/hw_data.h hardware/hw_glide.h hardware/hw_defs.h \
 hardware/hw_md2.h hardware/hw_glob.h hardware/hw_main.h am_map.h \
 d_event.h d_player.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(M5) -Os -o $(O)/ogl_win.o $(WFLAGS) -D_WINDOWS -DMINGW -mwindows -c hardware/r_opengl/ogl_win.c

$(O)/r_minigl.o: hardware/r_minigl/r_minigl.c hardware/r_opengl/r_opengl.h \
 doomdef.h doomtype.h g_state.h m_swap.h hardware/hw_drv.h screen.h \
 command.h hardware/hw_data.h hardware/hw_glide.h hardware/hw_defs.h \
 hardware/hw_md2.h hardware/hw_glob.h hardware/hw_main.h am_map.h \
 d_event.h d_player.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(M5) -Os -o $(O)/r_minigl.o $(WFLAGS) -D_WINDOWS -DMINGW -mwindows -c hardware/r_minigl/r_minigl.c

$(O)/s_ds3d.o: hardware/s_ds3d/s_ds3d.c hardware/hw3dsdrv.h hardware/hw_drv.h \
 screen.h command.h doomtype.h hardware/hw_data.h doomdef.h g_state.h \
 m_swap.h hardware/hw_glide.h hardware/hw_defs.h hardware/hw_md2.h \
 hardware/hw_glob.h hardware/hw_main.h am_map.h d_event.h d_player.h \
 p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h \
 doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(M5) -Os -fno-strict-aliasing -o $(O)/s_ds3d.o $(WFLAGS) -D_WINDOWS -DMINGW -mwindows -c hardware/s_ds3d/s_ds3d.c

$(O)/s_fmod.o: hardware/s_fmod/s_fmod.c hardware/hw3dsdrv.h hardware/hw_drv.h \
 screen.h command.h doomtype.h hardware/hw_data.h doomdef.h g_state.h \
 m_swap.h hardware/hw_glide.h hardware/hw_defs.h hardware/hw_md2.h \
 hardware/hw_glob.h hardware/hw_main.h am_map.h d_event.h d_player.h \
 p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h \
 doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(M5) -Os -o $(O)/s_fmod.o $(WFLAGS) -D_WINDOWS -DMINGW -mwindows -I"./win32/FMOD" -c hardware/s_fmod/s_fmod.c

$(O)/s_openal.o: hardware/s_openal/s_openal.c hardware/hw3dsdrv.h hardware/hw_drv.h \
 screen.h command.h doomtype.h hardware/hw_data.h doomdef.h g_state.h \
 m_swap.h hardware/hw_glide.h hardware/hw_defs.h hardware/hw_md2.h \
 hardware/hw_glob.h hardware/hw_main.h am_map.h d_event.h d_player.h \
 p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h \
 doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(M5) -Os -o $(O)/s_openal.o $(WFLAGS) -D_WINDOWS -DMINGW -mwindows -c hardware/s_openal/s_openal.c

endif
endif


#dependecy made by gcc itself ! (see makedep.mak for more)
$(O)/dstrings.o: dstrings.c dstrings.h d_englsh.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/i_sound.o: $(INTERFACE)/i_sound.c doomdef.h \
 doomtype.h g_state.h m_swap.h doomstat.h doomdata.h d_player.h \
 d_items.h p_pspr.h m_fixed.h tables.h info.h d_think.h \
 p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h command.h i_system.h \
 d_event.h i_sound.h sounds.h z_zone.h m_argv.h m_misc.h \
 w_wad.h s_sound.h qmus2mid.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/i_cdmus.o: $(INTERFACE)/i_cdmus.c doomdef.h \
 doomtype.h g_state.h m_swap.h	i_sound.h sounds.h command.h \
 i_system.h d_ticcmd.h d_event.h  s_sound.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/i_video.o: $(INTERFACE)/i_video.c doomdef.h \
 doomtype.h g_state.h m_swap.h i_system.h d_ticcmd.h d_event.h \
 v_video.h r_defs.h m_fixed.h d_think.h p_mobj.h tables.h \
 doomdata.h info.h screen.h command.h m_argv.h \
 i_video.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/i_net.o: $(INTERFACE)/i_net.c doomdef.h doomtype.h \
 g_state.h m_swap.h i_system.h d_ticcmd.h d_event.h d_net.h \
 m_argv.h doomstat.h doomdata.h d_player.h d_items.h p_pspr.h \
 m_fixed.h tables.h info.h d_think.h p_mobj.h d_clisrv.h \
 d_netcmd.h command.h z_zone.h i_net.h i_tcp.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/i_tcp.o: i_tcp.c doomdef.h doomtype.h g_state.h m_swap.h i_system.h \
 d_ticcmd.h d_event.h i_net.h d_net.h m_argv.h doomstat.h doomdata.h \
 d_player.h d_items.h p_pspr.h m_fixed.h tables.h info.h d_think.h \
 p_mobj.h d_clisrv.h d_netcmd.h command.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/i_system.o: $(INTERFACE)/i_system.c doomdef.h \
 doomtype.h g_state.h m_swap.h \
 m_misc.h w_wad.h i_video.h \
 i_sound.h sounds.h d_net.h \
 g_game.h doomstat.h doomdata.h \
 d_player.h d_items.h p_pspr.h \
 m_fixed.h tables.h info.h \
 d_think.h p_mobj.h d_ticcmd.h \
 d_clisrv.h d_netcmd.h command.h \
 d_event.h d_main.h m_argv.h \
 z_zone.h g_input.h \
 console.h i_system.h \
 i_joy.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/tables.o: tables.c tables.h m_fixed.h doomtype.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/f_finale.o: f_finale.c doomdef.h doomtype.h g_state.h m_swap.h \
 am_map.h d_event.h dstrings.h d_englsh.h d_main.h w_wad.h f_finale.h \
 g_game.h doomstat.h doomdata.h d_player.h d_items.h p_pspr.h \
 m_fixed.h tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h \
 d_netcmd.h command.h hu_stuff.h wi_stuff.h r_defs.h screen.h \
 r_local.h m_bbox.h r_main.h r_data.h r_state.h r_bsp.h r_segs.h \
 r_plane.h r_sky.h r_things.h sounds.h r_draw.h s_sound.h i_video.h \
 v_video.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/f_wipe.o: f_wipe.c doomdef.h doomtype.h g_state.h m_swap.h m_random.h \
 f_wipe.h i_system.h d_ticcmd.h d_event.h i_video.h v_video.h r_defs.h \
 m_fixed.h d_think.h p_mobj.h tables.h doomdata.h info.h screen.h \
 command.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/d_main.o: d_main.c doomdef.h doomtype.h g_state.h m_swap.h command.h \
 console.h d_event.h doomstat.h doomdata.h d_player.h d_items.h \
 p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h d_ticcmd.h \
 d_clisrv.h d_netcmd.h am_map.h d_net.h dehacked.h dstrings.h \
 d_englsh.h f_wipe.h f_finale.h g_game.h g_input.h keys.h hu_stuff.h \
 w_wad.h wi_stuff.h r_defs.h screen.h i_sound.h sounds.h i_system.h \
 i_video.h m_argv.h m_menu.h m_misc.h p_setup.h p_fab.h r_main.h \
 r_data.h r_state.h r_local.h m_bbox.h r_bsp.h r_segs.h r_plane.h \
 r_sky.h r_things.h r_draw.h s_sound.h st_stuff.h v_video.h z_zone.h \
 d_main.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/d_net.o: d_net.c doomdef.h doomtype.h g_state.h m_swap.h d_clisrv.h \
 d_ticcmd.h d_netcmd.h command.h g_game.h doomstat.h doomdata.h \
 d_player.h d_items.h p_pspr.h m_fixed.h tables.h info.h d_think.h \
 p_mobj.h d_event.h i_net.h i_system.h m_argv.h d_net.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/d_items.o: d_items.c info.h d_think.h d_items.h doomdef.h doomtype.h \
 g_state.h m_swap.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/g_game.o: g_game.c doomdef.h doomtype.h g_state.h m_swap.h command.h \
 console.h d_event.h dstrings.h d_englsh.h d_main.h w_wad.h d_net.h \
 d_netcmd.h f_finale.h p_setup.h doomdata.h p_saveg.h i_system.h \
 d_ticcmd.h wi_stuff.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h am_map.h m_random.h p_local.h \
 m_bbox.h p_tick.h r_defs.h screen.h p_maputl.h p_spec.h r_data.h \
 r_state.h r_draw.h r_main.h r_sky.h s_sound.h sounds.h g_game.h \
 doomstat.h d_clisrv.h g_input.h keys.h p_fab.h m_cheat.h m_misc.h \
 m_menu.h m_argv.h hu_stuff.h st_stuff.h z_zone.h i_video.h p_inter.h \
 byteptr.h i_joy.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/m_menu.o: m_menu.c am_map.h d_event.h doomtype.h g_state.h doomdef.h \
 m_swap.h dstrings.h d_englsh.h d_main.h w_wad.h console.h r_local.h \
 tables.h m_fixed.h screen.h command.h m_bbox.h r_main.h d_player.h \
 d_items.h p_pspr.h info.h d_think.h p_mobj.h doomdata.h d_ticcmd.h \
 r_data.h r_defs.h r_state.h r_bsp.h r_segs.h r_plane.h r_sky.h \
 r_things.h sounds.h r_draw.h hu_stuff.h wi_stuff.h g_game.h \
 doomstat.h d_clisrv.h d_netcmd.h g_input.h keys.h m_argv.h s_sound.h \
 i_system.h m_menu.h v_video.h i_video.h z_zone.h p_local.h p_tick.h \
 p_maputl.h p_spec.h p_fab.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/m_misc.o: m_misc.c doomdef.h doomtype.h g_state.h m_swap.h g_game.h \
 doomstat.h doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h \
 command.h d_event.h m_misc.h w_wad.h hu_stuff.h wi_stuff.h r_defs.h \
 screen.h v_video.h z_zone.h g_input.h keys.h i_video.h d_main.h \
 m_argv.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/m_argv.o: m_argv.c doomdef.h doomtype.h g_state.h m_swap.h command.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/m_bbox.o: m_bbox.c doomtype.h m_bbox.h m_fixed.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/m_fixed.o: m_fixed.c i_system.h d_ticcmd.h doomtype.h d_event.h \
 g_state.h m_fixed.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/m_swap.o: m_swap.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/m_cheat.o: m_cheat.c doomdef.h doomtype.h g_state.h m_swap.h \
 dstrings.h d_englsh.h am_map.h d_event.h m_cheat.h g_game.h \
 doomstat.h doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h \
 command.h r_local.h screen.h m_bbox.h r_main.h r_data.h r_defs.h \
 r_state.h r_bsp.h r_segs.h r_plane.h r_sky.h r_things.h sounds.h \
 r_draw.h p_local.h p_tick.h p_maputl.h p_spec.h p_inter.h i_sound.h \
 s_sound.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/m_random.o: m_random.c doomdef.h doomtype.h g_state.h m_swap.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/am_map.o: am_map.c doomdef.h doomtype.h g_state.h m_swap.h doomstat.h \
 doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h command.h \
 g_game.h d_event.h am_map.h g_input.h keys.h m_cheat.h p_local.h \
 m_bbox.h p_tick.h r_defs.h screen.h p_maputl.h p_spec.h v_video.h \
 st_stuff.h i_system.h i_video.h r_state.h r_data.h dstrings.h \
 d_englsh.h w_wad.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_ceilng.o: p_ceilng.c doomdef.h doomtype.h g_state.h m_swap.h \
 p_local.h command.h d_player.h d_items.h p_pspr.h m_fixed.h tables.h \
 info.h d_think.h p_mobj.h doomdata.h d_ticcmd.h m_bbox.h p_tick.h \
 r_defs.h screen.h p_maputl.h p_spec.h r_state.h r_data.h s_sound.h \
 sounds.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_doors.o: p_doors.c doomdef.h doomtype.h g_state.h m_swap.h \
 dstrings.h d_englsh.h p_local.h command.h d_player.h d_items.h \
 p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h doomdata.h \
 d_ticcmd.h m_bbox.h p_tick.h r_defs.h screen.h p_maputl.h p_spec.h \
 r_state.h r_data.h s_sound.h sounds.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_enemy.o: p_enemy.c doomdef.h doomtype.h g_state.h m_swap.h g_game.h \
 doomstat.h doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h \
 command.h d_event.h p_local.h m_bbox.h p_tick.h r_defs.h screen.h \
 p_maputl.h p_spec.h r_main.h r_data.h r_state.h s_sound.h sounds.h \
 m_random.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_floor.o: p_floor.c doomdef.h doomtype.h g_state.h m_swap.h p_local.h \
 command.h d_player.h d_items.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h m_bbox.h p_tick.h r_defs.h \
 screen.h p_maputl.h p_spec.h r_state.h r_data.h s_sound.h sounds.h \
 z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_genlin.o: p_genlin.c doomdef.h g_game.h p_local.h r_data.h m_random.h \
 m_random.h s_sound.h z_zone.h command.h d_player.h d_think.h m_fixed.h \
 m_bbox.h p_tick.h r_defs.h p_maputl.h p_mobj.h screen.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_info.o: p_info.c doomdef.h dehacked.h dstrings.h p_setup.h p_info.h \
 p_mobj.h t_script.h w_wad.h z_zone.h command.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_inter.o: p_inter.c doomdef.h doomtype.h g_state.h m_swap.h \
 i_system.h d_ticcmd.h d_event.h am_map.h dstrings.h d_englsh.h \
 g_game.h doomstat.h doomdata.h d_player.h d_items.h p_pspr.h \
 m_fixed.h tables.h info.h d_think.h p_mobj.h d_clisrv.h d_netcmd.h \
 command.h m_random.h p_local.h m_bbox.h p_tick.h r_defs.h screen.h \
 p_maputl.h p_spec.h p_inter.h s_sound.h sounds.h r_main.h r_data.h \
 r_state.h st_stuff.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_lights.o: p_lights.c doomdef.h doomtype.h g_state.h m_swap.h \
 p_local.h command.h d_player.h d_items.h p_pspr.h m_fixed.h tables.h \
 info.h d_think.h p_mobj.h doomdata.h d_ticcmd.h m_bbox.h p_tick.h \
 r_defs.h screen.h p_maputl.h p_spec.h r_state.h r_data.h z_zone.h \
 m_random.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_map.o: p_map.c doomdef.h doomtype.h g_state.h m_swap.h g_game.h \
 doomstat.h doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h \
 command.h d_event.h m_bbox.h m_random.h p_local.h p_tick.h r_defs.h \
 screen.h p_maputl.h p_spec.h p_inter.h r_state.h r_data.h r_main.h \
 r_sky.h s_sound.h sounds.h r_splats.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_maputl.o: p_maputl.c p_local.h command.h doomtype.h d_player.h \
 d_items.h doomdef.h g_state.h m_swap.h p_pspr.h m_fixed.h tables.h \
 info.h d_think.h p_mobj.h doomdata.h d_ticcmd.h m_bbox.h p_tick.h \
 r_defs.h screen.h p_maputl.h p_spec.h r_main.h r_data.h r_state.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_plats.o: p_plats.c doomdef.h doomtype.h g_state.h m_swap.h p_local.h \
 command.h d_player.h d_items.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h m_bbox.h p_tick.h r_defs.h \
 screen.h p_maputl.h p_spec.h s_sound.h sounds.h r_state.h r_data.h \
 z_zone.h m_random.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_pspr.o: p_pspr.c doomdef.h doomtype.h g_state.h m_swap.h d_event.h \
 p_local.h command.h d_player.h d_items.h p_pspr.h m_fixed.h tables.h \
 info.h d_think.h p_mobj.h doomdata.h d_ticcmd.h m_bbox.h p_tick.h \
 r_defs.h screen.h p_maputl.h p_spec.h p_inter.h s_sound.h sounds.h \
 g_game.h doomstat.h d_clisrv.h d_netcmd.h g_input.h keys.h r_main.h \
 r_data.h r_state.h m_random.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_setup.o: p_setup.c doomdef.h doomtype.h g_state.h m_swap.h d_main.h \
 d_event.h w_wad.h g_game.h doomstat.h doomdata.h d_player.h d_items.h \
 p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h d_ticcmd.h \
 d_clisrv.h d_netcmd.h command.h p_local.h m_bbox.h p_tick.h r_defs.h \
 screen.h p_maputl.h p_spec.h p_setup.h i_sound.h sounds.h r_sky.h \
 r_data.h r_state.h r_things.h s_sound.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_sight.o: p_sight.c doomdef.h doomtype.h g_state.h m_swap.h p_local.h \
 command.h d_player.h d_items.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h m_bbox.h p_tick.h r_defs.h \
 screen.h p_maputl.h p_spec.h r_main.h r_data.h r_state.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_spec.o: p_spec.c doomdef.h doomtype.h g_state.h m_swap.h g_game.h \
 doomstat.h doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h \
 command.h d_event.h p_local.h m_bbox.h p_tick.h r_defs.h screen.h \
 p_maputl.h p_spec.h p_setup.h r_data.h r_state.h m_random.h s_sound.h \
 sounds.h w_wad.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_switch.o: p_switch.c doomdef.h doomtype.h g_state.h m_swap.h \
 g_game.h doomstat.h doomdata.h d_player.h d_items.h p_pspr.h \
 m_fixed.h tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h \
 d_netcmd.h command.h d_event.h p_local.h m_bbox.h p_tick.h r_defs.h \
 screen.h p_maputl.h p_spec.h s_sound.h sounds.h r_main.h r_data.h \
 r_state.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_mobj.o: p_mobj.c doomdef.h doomtype.h g_state.h m_swap.h g_game.h \
 doomstat.h doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h \
 command.h d_event.h g_input.h keys.h st_stuff.h hu_stuff.h w_wad.h \
 wi_stuff.h r_defs.h screen.h p_local.h m_bbox.h p_tick.h p_maputl.h \
 p_spec.h p_inter.h p_setup.h r_main.h r_data.h r_state.h r_things.h \
 sounds.h r_sky.h s_sound.h z_zone.h m_random.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_telept.o: p_telept.c doomdef.h doomtype.h g_state.h m_swap.h \
 g_game.h doomstat.h doomdata.h d_player.h d_items.h p_pspr.h \
 m_fixed.h tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h \
 d_netcmd.h command.h d_event.h p_local.h m_bbox.h p_tick.h r_defs.h \
 screen.h p_maputl.h p_spec.h r_state.h r_data.h s_sound.h sounds.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_tick.o: p_tick.c doomstat.h doomdata.h doomtype.h doomdef.h \
 g_state.h m_swap.h d_player.h d_items.h p_pspr.h m_fixed.h tables.h \
 info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h command.h \
 g_game.h d_event.h p_local.h m_bbox.h p_tick.h r_defs.h screen.h \
 p_maputl.h p_spec.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_saveg.o: p_saveg.c doomdef.h doomtype.h g_state.h m_swap.h g_game.h \
 doomstat.h doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h \
 command.h d_event.h p_local.h m_bbox.h p_tick.h r_defs.h screen.h \
 p_maputl.h p_spec.h r_state.h r_data.h z_zone.h w_wad.h p_setup.h \
 byteptr.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/p_user.o: p_user.c doomdef.h doomtype.h g_state.h m_swap.h d_event.h \
 g_game.h doomstat.h doomdata.h d_player.h d_items.h p_pspr.h \
 m_fixed.h tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h \
 d_netcmd.h command.h p_local.h m_bbox.h p_tick.h r_defs.h screen.h \
 p_maputl.h p_spec.h r_main.h r_data.h r_state.h s_sound.h sounds.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/r_bsp.o: r_bsp.c doomdef.h doomtype.h g_state.h m_swap.h g_game.h \
 doomstat.h doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h \
 command.h d_event.h r_local.h screen.h m_bbox.h r_main.h r_data.h \
 r_defs.h r_state.h r_bsp.h r_segs.h r_plane.h r_sky.h r_things.h \
 sounds.h r_draw.h r_splats.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/r_data.o: r_data.c doomdef.h doomtype.h g_state.h m_swap.h g_game.h \
 doomstat.h doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h \
 command.h d_event.h i_video.h r_local.h screen.h m_bbox.h r_main.h \
 r_data.h r_defs.h r_state.h r_bsp.h r_segs.h r_plane.h r_sky.h \
 r_things.h sounds.h r_draw.h p_local.h p_tick.h p_maputl.h p_spec.h \
 w_wad.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/r_draw.o: r_draw.c doomdef.h doomtype.h g_state.h m_swap.h r_local.h \
 tables.h m_fixed.h screen.h command.h m_bbox.h r_main.h d_player.h \
 d_items.h p_pspr.h info.h d_think.h p_mobj.h doomdata.h d_ticcmd.h \
 r_data.h r_defs.h r_state.h r_bsp.h r_segs.h r_plane.h r_sky.h \
 r_things.h sounds.h r_draw.h st_stuff.h d_event.h i_video.h \
 v_video.h w_wad.h z_zone.h r_draw8.c r_draw16.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/r_main.o: r_main.c doomdef.h doomtype.h g_state.h m_swap.h g_game.h \
 doomstat.h doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h \
 command.h d_event.h g_input.h keys.h r_local.h screen.h m_bbox.h \
 r_main.h r_data.h r_defs.h r_state.h r_bsp.h r_segs.h r_plane.h \
 r_sky.h r_things.h sounds.h r_draw.h r_splats.h st_stuff.h p_local.h \
 p_tick.h p_maputl.h p_spec.h i_video.h m_menu.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/r_plane.o: r_plane.c doomdef.h doomtype.h g_state.h m_swap.h console.h \
 d_event.h g_game.h doomstat.h doomdata.h d_player.h d_items.h \
 p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h d_ticcmd.h \
 d_clisrv.h d_netcmd.h command.h r_data.h r_defs.h screen.h r_state.h \
 r_local.h m_bbox.h r_main.h r_bsp.h r_segs.h r_plane.h r_sky.h \
 r_things.h sounds.h r_draw.h r_splats.h v_video.h w_wad.h \
 z_zone.h p_setup.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/r_segs.o: r_segs.c doomdef.h doomtype.h g_state.h m_swap.h r_local.h \
 tables.h m_fixed.h screen.h command.h m_bbox.h r_main.h d_player.h \
 d_items.h p_pspr.h info.h d_think.h p_mobj.h doomdata.h d_ticcmd.h \
 r_data.h r_defs.h r_state.h r_bsp.h r_segs.h r_plane.h r_sky.h \
 r_things.h sounds.h r_draw.h r_splats.h w_wad.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/r_sky.o: r_sky.c doomdef.h doomtype.h g_state.h m_swap.h r_sky.h \
 m_fixed.h r_local.h tables.h screen.h command.h m_bbox.h r_main.h \
 d_player.h d_items.h p_pspr.h info.h d_think.h p_mobj.h doomdata.h \
 d_ticcmd.h r_data.h r_defs.h r_state.h r_bsp.h r_segs.h r_plane.h \
 r_things.h sounds.h r_draw.h w_wad.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/r_things.o: r_things.c doomdef.h doomtype.h g_state.h m_swap.h \
 console.h d_event.h g_game.h doomstat.h doomdata.h d_player.h \
 d_items.h p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h \
 d_ticcmd.h d_clisrv.h d_netcmd.h command.h r_local.h screen.h \
 m_bbox.h r_main.h r_data.h r_defs.h r_state.h r_bsp.h r_segs.h \
 r_plane.h r_sky.h r_things.h sounds.h r_draw.h st_stuff.h w_wad.h \
 z_zone.h i_video.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/w_wad.o: w_wad.c doomdef.h doomtype.h g_state.h m_swap.h w_wad.h \
 z_zone.h i_video.h d_netfil.h md5.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/wi_stuff.o: wi_stuff.c doomdef.h doomtype.h g_state.h m_swap.h \
 wi_stuff.h d_player.h d_items.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h doomdata.h d_ticcmd.h g_game.h doomstat.h \
 d_clisrv.h d_netcmd.h command.h d_event.h hu_stuff.h w_wad.h r_defs.h \
 screen.h m_random.h r_local.h m_bbox.h r_main.h r_data.h r_state.h \
 r_bsp.h r_segs.h r_plane.h r_sky.h r_things.h sounds.h r_draw.h \
 s_sound.h st_stuff.h i_video.h v_video.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/v_video.o: v_video.c doomdef.h doomtype.h g_state.h m_swap.h r_local.h \
 tables.h m_fixed.h screen.h command.h m_bbox.h r_main.h d_player.h \
 d_items.h p_pspr.h info.h d_think.h p_mobj.h doomdata.h d_ticcmd.h \
 r_data.h r_defs.h r_state.h r_bsp.h r_segs.h r_plane.h r_sky.h \
 r_things.h sounds.h r_draw.h v_video.h hu_stuff.h d_event.h w_wad.h \
 wi_stuff.h i_video.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/st_lib.o: st_lib.c doomdef.h doomtype.h g_state.h m_swap.h st_lib.h \
 r_defs.h m_fixed.h d_think.h p_mobj.h tables.h doomdata.h info.h \
 screen.h command.h w_wad.h st_stuff.h d_event.h d_player.h d_items.h \
 p_pspr.h d_ticcmd.h v_video.h z_zone.h i_video.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/st_stuff.o: st_stuff.c doomdef.h doomtype.h g_state.h m_swap.h \
 am_map.h d_event.h g_game.h doomstat.h doomdata.h d_player.h \
 d_items.h p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h \
 d_ticcmd.h d_clisrv.h d_netcmd.h command.h m_cheat.h r_local.h \
 screen.h m_bbox.h r_main.h r_data.h r_defs.h r_state.h r_bsp.h \
 r_segs.h r_plane.h r_sky.h r_things.h sounds.h r_draw.h p_local.h \
 p_tick.h p_maputl.h p_spec.h p_inter.h m_random.h st_stuff.h st_lib.h \
 w_wad.h i_video.h v_video.h keys.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/hu_stuff.o: hu_stuff.c doomdef.h doomtype.h g_state.h m_swap.h \
 hu_stuff.h d_event.h w_wad.h wi_stuff.h d_player.h d_items.h p_pspr.h \
 m_fixed.h tables.h info.h d_think.h p_mobj.h doomdata.h d_ticcmd.h \
 r_defs.h screen.h command.h d_netcmd.h d_clisrv.h g_game.h doomstat.h \
 g_input.h keys.h i_video.h dstrings.h d_englsh.h st_stuff.h r_local.h \
 m_bbox.h r_main.h r_data.h r_state.h r_bsp.h r_segs.h r_plane.h \
 r_sky.h r_things.h sounds.h r_draw.h v_video.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/s_sound.o: s_sound.c doomdef.h doomtype.h g_state.h m_swap.h command.h \
 g_game.h doomstat.h doomdata.h d_player.h d_items.h p_pspr.h \
 m_fixed.h tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h \
 d_netcmd.h d_event.h m_argv.h r_main.h r_data.h r_defs.h screen.h \
 r_state.h r_things.h sounds.h i_sound.h s_sound.h w_wad.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/z_zone.o: z_zone.c doomdef.h doomtype.h g_state.h m_swap.h z_zone.h \
 i_system.h d_ticcmd.h d_event.h command.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/info.o: info.c doomdef.h doomtype.h g_state.h m_swap.h sounds.h \
 m_fixed.h p_mobj.h tables.h d_think.h doomdata.h info.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/sounds.o: sounds.c doomtype.h sounds.h r_defs.h m_fixed.h d_think.h \
 p_mobj.h tables.h doomdata.h doomdef.h g_state.h m_swap.h info.h \
 screen.h command.h r_things.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/tmap.o: tmap.nas
	nasm -o $@ -f $(NASMFORMAT) $<

$(O)/p_fab.o: p_fab.c doomdef.h doomtype.h g_state.h m_swap.h g_game.h \
 doomstat.h doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h \
 command.h d_event.h p_local.h m_bbox.h p_tick.h r_defs.h screen.h \
 p_maputl.h p_spec.h r_state.h r_data.h p_fab.h m_random.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/dehacked.o: dehacked.c doomdef.h doomtype.h g_state.h m_swap.h \
 command.h console.h d_event.h g_game.h doomstat.h doomdata.h \
 d_player.h d_items.h p_pspr.h m_fixed.h tables.h info.h d_think.h \
 p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h sounds.h m_cheat.h \
 dstrings.h d_englsh.h m_argv.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/qmus2mid.o: qmus2mid.c doomdef.h doomtype.h g_state.h m_swap.h \
 i_system.h d_ticcmd.h d_event.h byteptr.h m_swap.h $(qmus2mid_h)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/qmus2mid_sdl.o: qmus2mid_sdl.c doomdef.h doomtype.h g_state.h m_swap.h \
 i_system.h d_ticcmd.h d_event.h byteptr.h m_swap.h $(qmus2mid_h)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/vid_copy.o: vid_copy.s
	$(CC) $(OPTS) $(SFLAGS) -x assembler-with-cpp -c $< -o $@

$(O)/vid_vesa.o: $(INTERFACE)/vid_vesa.c i_system.h \
 d_ticcmd.h doomtype.h d_event.h \
 g_state.h $(INTERFACE)/vid_vesa.h doomdef.h \
 m_swap.h screen.h command.h \
 st_stuff.h d_player.h d_items.h \
 p_pspr.h m_fixed.h tables.h \
 info.h d_think.h p_mobj.h \
 doomdata.h console.h i_video.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/g_input.o: g_input.c doomdef.h doomtype.h g_state.h m_swap.h g_input.h \
 d_event.h keys.h command.h hu_stuff.h w_wad.h wi_stuff.h d_player.h \
 d_items.h p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h \
 doomdata.h d_ticcmd.h r_defs.h screen.h d_net.h console.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/screen.o: screen.c doomdef.h doomtype.h g_state.h m_swap.h screen.h \
 command.h console.h d_event.h am_map.h i_system.h d_ticcmd.h \
 i_video.h r_local.h tables.h m_fixed.h m_bbox.h r_main.h d_player.h \
 d_items.h p_pspr.h info.h d_think.h p_mobj.h doomdata.h r_data.h \
 r_defs.h r_state.h r_bsp.h r_segs.h r_plane.h r_sky.h r_things.h \
 sounds.h r_draw.h m_argv.h v_video.h st_stuff.h hu_stuff.h w_wad.h \
 wi_stuff.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/console.o: console.c doomdef.h doomtype.h g_state.h m_swap.h console.h \
 d_event.h g_game.h doomstat.h doomdata.h d_player.h d_items.h \
 p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h d_ticcmd.h \
 d_clisrv.h d_netcmd.h command.h g_input.h keys.h hu_stuff.h w_wad.h \
 wi_stuff.h r_defs.h screen.h sounds.h st_stuff.h s_sound.h v_video.h \
 i_video.h z_zone.h i_system.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/command.o: command.c doomdef.h doomtype.h g_state.h m_swap.h \
 doomstat.h doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h \
 command.h console.h d_event.h z_zone.h m_misc.h w_wad.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/d_netcmd.o: d_netcmd.c doomdef.h doomtype.h g_state.h m_swap.h \
 console.h d_event.h command.h d_netcmd.h d_clisrv.h d_ticcmd.h \
 i_system.h dstrings.h d_englsh.h g_game.h doomstat.h doomdata.h \
 d_player.h d_items.h p_pspr.h m_fixed.h tables.h info.h d_think.h \
 p_mobj.h hu_stuff.h w_wad.h wi_stuff.h r_defs.h screen.h g_input.h \
 keys.h m_menu.h r_local.h m_bbox.h r_main.h r_data.h r_state.h \
 r_bsp.h r_segs.h r_plane.h r_sky.h r_things.h sounds.h r_draw.h \
 p_inter.h p_setup.h s_sound.h m_misc.h am_map.h byteptr.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/d_clisrv.o: d_clisrv.c doomdef.h doomtype.h g_state.h m_swap.h \
 command.h i_net.h i_system.h d_ticcmd.h d_event.h d_net.h d_netcmd.h \
 d_clisrv.h d_main.h w_wad.h g_game.h doomstat.h doomdata.h d_player.h \
 d_items.h p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h \
 hu_stuff.h wi_stuff.h r_defs.h screen.h keys.h m_argv.h m_menu.h \
 console.h d_netfil.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/r_splats.o: r_splats.c r_draw.h r_defs.h m_fixed.h doomtype.h \
 d_think.h p_mobj.h tables.h doomdata.h doomdef.h g_state.h m_swap.h \
 info.h screen.h command.h r_main.h d_player.h d_items.h p_pspr.h \
 d_ticcmd.h r_data.h r_state.h r_plane.h r_splats.h w_wad.h z_zone.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/g_state.o: g_state.c g_state.h doomtype.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/d_netfil.o: d_netfil.c doomdef.h doomtype.h g_state.h m_swap.h \
 doomstat.h doomdata.h d_player.h d_items.h p_pspr.h m_fixed.h \
 tables.h info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h \
 command.h g_game.h d_event.h i_net.h i_system.h m_argv.h d_net.h \
 d_netfil.h z_zone.h md5.h filesrch.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/t_func.o: t_func.c command.h doomstat.h doomtype.h d_main.h \
 g_game.h hu_stuff.h info.h m_random.h p_mobj.h p_tick.h \
 p_spec.h p_inter.h r_data.h r_main.h r_segs.h s_sound.h w_wad.h \
 z_zone.h t_parse.h t_spec.h t_script.h t_oper.h t_vari.h t_func.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/t_oper.o: t_oper.c command.h doomstat.h doomtype.h z_zone.h \
 t_parse.h t_vari.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/t_parse.o: t_parse.c command.h doomtype.h doomdef.h s_sound.h \
 w_wad.h z_zone.h t_parse.h t_spec.h t_script.h t_oper.h t_vari.h t_func.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/t_prepro.o: t_prepro.c command.h doomtype.h w_wad.h z_zone.h \
 t_parse.h t_spec.h t_script.h t_oper.h t_vari.h t_func.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/t_script.o: t_script.c command.h g_game.h r_state.h p_info.h \
 p_mobj.h p_spec.h p_setup.h w_wad.h z_zone.h t_parse.h t_script.h \
 t_vari.h t_func.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/t_spec.o: t_spec.c command.h doomtype.h z_zone.h \
 t_parse.h t_spec.h t_vari.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/i_main.o: $(INTERFACE)/i_main.c doomdef.h \
 doomtype.h g_state.h m_swap.h \
 m_argv.h d_main.h d_event.h \
 w_wad.h i_system.h d_ticcmd.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/md5.o: md5.c md5.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(O)/filesrch.o: $(INTERFACE)/filesrch.c filesrch.h d_netfil.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

ifdef SDL
$(O)/s_openal.o: hardware/s_openal/s_openal.c hardware/hw3dsdrv.h hardware/hw_drv.h \
 screen.h command.h doomtype.h hardware/hw_data.h doomdef.h g_state.h \
 m_swap.h hardware/hw_glide.h hardware/hw_defs.h hardware/hw_md2.h \
 hardware/hw_glob.h hardware/hw_main.h am_map.h d_event.h d_player.h \
 p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h \
 doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

$(O)/s_fmod.o: hardware/s_fmod/s_fmod.c hardware/hw3dsdrv.h hardware/hw_drv.h \
 screen.h command.h doomtype.h hardware/hw_data.h doomdef.h g_state.h \
 m_swap.h hardware/hw_glide.h hardware/hw_defs.h hardware/hw_md2.h \
 hardware/hw_glob.h hardware/hw_main.h am_map.h d_event.h d_player.h \
 p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h \
 doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

$(O)/s_ds3d.o: hardware/s_ds3d/s_ds3d.c hardware/hw3dsdrv.h hardware/hw_drv.h \
 screen.h command.h doomtype.h hardware/hw_data.h doomdef.h g_state.h \
 m_swap.h hardware/hw_glide.h hardware/hw_defs.h hardware/hw_md2.h \
 hardware/hw_glob.h hardware/hw_main.h am_map.h d_event.h d_player.h \
 p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h \
 doomdata.h d_ticcmd.h r_defs.h
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@
endif

$(O)/%.o: %.c
	@echo $< needs deps
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

$(O)/%.o: $(INTERFACE)/%.c
	@echo $< needs deps
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

$(O)/%.o: hardware/%.c
	@echo $< needs deps
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

$(O)/SRB2.res: win32/srb2rp.rc win32/afxres.h win32/resource.h
	windres.exe -i win32/Srb2rp.rc -O rc $(WINDRESFLAGS) --include-dir=win32 -o $(O)/SRB2.res -O coff

$(O)/win_cd.o: $(INTERFACE)/win_cd.c doomdef.h doomtype.h g_state.h m_swap.h \
 $(INTERFACE)/win_main.h command.h i_sound.h sounds.h i_system.h d_ticcmd.h \
 m_fixed.h d_event.h s_sound.h
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

$(O)/win_dbg.o: $(INTERFACE)/win_dbg.c doomdef.h doomtype.h g_state.h m_swap.h \
 $(INTERFACE)/win_dbg.h $(INTERFACE)/win_main.h m_argv.h
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

$(O)/win_dll.o: $(INTERFACE)/win_dll.c doomdef.h doomtype.h g_state.h m_swap.h \
 hardware/hw_drv.h screen.h command.h hardware/hw_data.h \
 hardware/hw_glide.h hardware/hw_defs.h hardware/hw_md2.h \
 hardware/hw_glob.h hardware/hw_main.h am_map.h d_event.h d_player.h \
 p_pspr.h m_fixed.h tables.h info.h d_think.h p_mobj.h \
 doomdata.h d_ticcmd.h r_defs.h hardware/hw3dsdrv.h $(INTERFACE)/win_dll.h \
 $(INTERFACE)/win_main.h
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

$(O)/win_main.o: $(INTERFACE)/win_main.c doomdef.h doomtype.h g_state.h m_swap.h \
 doomstat.h doomdata.h d_player.h p_pspr.h m_fixed.h tables.h \
 info.h d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h command.h \
 $(INTERFACE)/resource.h m_argv.h d_main.h d_event.h w_wad.h hardware/hw_data.h \
 hardware/hw_glide.h screen.h i_system.h keys.h console.h \
 $(INTERFACE)/fabdxlib.h $(INTERFACE)/win_main.h $(INTERFACE)/win_dbg.h I_sound.h sounds.h \
 g_input.h
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

$(O)/win_net.o: $(INTERFACE)/win_net.c doomdef.h doomtype.h g_state.h m_swap.h \
 m_argv.h
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

$(O)/win_snd.o: $(INTERFACE)/win_snd.c doomdef.h doomtype.h g_state.h m_swap.h \
 $(INTERFACE)/win_main.h command.h i_sound.h sounds.h s_sound.h i_system.h \
 d_ticcmd.h m_fixed.h d_event.h m_argv.h w_wad.h hardware/hw_data.h \
 hardware/hw_glide.h screen.h z_zone.h doomstat.h doomdata.h d_player.h \
 p_pspr.h tables.h info.h d_think.h p_mobj.h d_clisrv.h \
 d_netcmd.h $(INTERFACE)/dx_error.h $(INTERFACE)/mid2strm.h \
 hardware/hw3sound.h hardware/hw3dsdrv.h hardware/hw_drv.h \
 hardware/hw_defs.h hardware/hw_md2.h hardware/hw_glob.h \
 hardware/hw_main.h am_map.h r_defs.h $(INTERFACE)/win_dll.h
	$(CC) $(CFLAGS) $(WFLAGS) -I"./win32/FMOD" -c $< -o $@

$(O)/win_sys.o: $(INTERFACE)/win_sys.c doomdef.h doomtype.h g_state.h m_swap.h \
 m_misc.h w_wad.h hardware/hw_data.h hardware/hw_glide.h screen.h \
 command.h i_video.h i_sound.h sounds.h d_net.h g_game.h doomstat.h \
 doomdata.h d_player.h p_pspr.h m_fixed.h tables.h info.h \
 d_think.h p_mobj.h d_ticcmd.h d_clisrv.h d_netcmd.h d_event.h d_main.h \
 m_argv.h z_zone.h g_input.h keys.h $(INTERFACE)/win_main.h i_joy.h \
 $(INTERFACE)/fabdxlib.h
	$(CC) $(CFLAGS) $(WINDRESFLAGS) $(WFLAGS) -c $< -o $@

$(O)/win_vid.o: $(INTERFACE)/win_vid.c doomdef.h doomtype.h g_state.h m_swap.h \
 i_system.h d_ticcmd.h m_fixed.h d_event.h m_argv.h v_video.h r_defs.h \
 d_think.h p_mobj.h tables.h doomdata.h info.h screen.h command.h \
 st_stuff.h d_player.h p_pspr.h i_video.h z_zone.h \
 $(INTERFACE)/fabdxlib.h $(INTERFACE)/win_main.h $(INTERFACE)/win_vid.h $(INTERFACE)/win_dll.h \
 hardware/hw_drv.h hardware/hw_data.h hardware/hw_glide.h \
 hardware/hw_defs.h hardware/hw_md2.h hardware/hw_glob.h \
 hardware/hw_main.h am_map.h
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

$(O)/dx_error.o: $(INTERFACE)/dx_error.c $(INTERFACE)/dx_error.h
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

$(O)/mid2strm.o: $(INTERFACE)/mid2strm.c doomdef.h doomtype.h g_state.h m_swap.h \
 $(INTERFACE)/mid2strm.h
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

$(O)/fabdxlib.o: $(INTERFACE)/fabdxlib.c doomdef.h doomtype.h g_state.h m_swap.h \
 i_system.h d_ticcmd.h m_fixed.h d_event.h $(INTERFACE)/dx_error.h \
 $(INTERFACE)/fabdxlib.h
	$(CC) $(CFLAGS) $(WFLAGS) -c $< -o $@

ifdef OS2
$(O)/%.res: os2/%.rc
	rc.exe -i .\os2 -r $< $@
endif

#############################################################
#
#############################################################
