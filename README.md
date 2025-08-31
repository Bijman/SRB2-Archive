---
generator: Msftedit 5.41.15.1515;
---

Here it is! SRB2CB 1.1 source code!

/// Info /////////////

SRB2\'s Game Engine is based on a highly modified version of the
\"Doom\" game engine, which is a old engine that was developed in the
90\'s, but popular for modification due to it\'s ease of creating levels
and content and it\'s somewhat elegant and easy to understand source
code.

A lot has changed since then with SRB2\'s implementation of this engine,
but one of the things it still has in common with its original engine is
the source code\'s naming convention.

Nearly all of the source code can be divided up by subject into
alphabetically prefixed sections. Most files and functions within the
source code have one or more letter prefixes to denote their subsystem.

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\--

NOTE: A large amount of more specific documentation can be found in the
source code itself, and also in doc/SRB2Src

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\--

AM\_\*

Automap code, auto map is the system used to draw an overhead view of a
level in the game, this is only used in a few files.

D\_\*

The main program, this code executes all the core stuff needed for the
game to run. It sets up things like the time, the game\'s file
directory, etc.

F\_\*

Titlescreen, cutscene, screen transitions and related stuff.

G\_\*

The main game, this has all the core game related stuff

HU\_\*

Heads-up display, AKA, the HUD. The HUD is basically any text or images
seen on screen that aren\'t actually part of the game, such as the
console or internet messaging.

HW\_

OpenGL rendering specific code.

I\_\*

System-specific code

M\_\*

Miscellaneous things, such as the menu, PNG screenshot capture, etc.

P\_\*

Game logic, and general physics. The base of how the game plays.

R\_\*

Software mode rendering and other in-game rendering, shared between both
software and OpenGL renderers.

S\_\*

Sound system

ST\_\*

Source code for the \"status bar\", which relates to things like the
life and ring meter of the game. This also relates to palette stuff.

V\_\*

General graphic rendering and drawing to the screen, shared between both
software and OpenGL renderers.

WI\_\*

End-of level intermission screen

W\_\*

WAD file system, the file type used for game data.

Z\_\*

Zone memory allocation system, this is the system the game uses to
manage it\'s memory resources, this is only used to ease the freeing of
memory at any place in the code.

These are prefixes that are only used in functions, not file names:

A\_\*

Action functions invoked in sprite movement frames, general soc stuff.

PIT\_\*

Callback functions passed to P_BlockThingsIterator (see p_maputl.c)

These functions help with collision.

T\_\*

\"Thinker\" functions set to be called each frame for some purpose, like
moving platforms or flickering lights.

/// Compiling /////////////

Windows:

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\--

Win32 with Visual C (6SP6+Processor Pack OR 7)

\~\~\~

2 VC++ 6.0 project files are included:

Win32/DirectX/FMOD

src\\win32\\wLegacy.dsw

You\'ll need FMOD to compile this version (www.fmod.org)

or

Win32/SDL/SDL_mixer

src\\sdl\\Win32SDL.dsp

You\'ll need SDL and SDL_mixer for this version (www.libsdl.org)

Both needs NASM (http://sourceforge.net/projects/nasm)

For PNG screenshot, libPNG, and Zlib (from
http://gnuwin32.sourceforge.net/)

No warranty, support, etc. of any kind is offered,

just plain old as is.

Some bits of code are still really scary.

Go nuts!

Win32 with Dev-C++ (http://bloodshed.net/ free!)

\~\~\~

2 Dev-C++ project files are included:

Win32/DirectX/FMOD

src\\win32\\SRB2.dev

or

Win32/SDL/SDL_mixer

src\\sdl\\Win32SDL.dev

You\'ll need SDL and SDL_mixer for this version (www.libsdl.org)

libPNG and Zlib (from http://gnuwin32.sourceforge.net/)

Note there are precompiled libpng.a and libz.a for Mingw

you will need NASM for both SDL/SDL_mixer and DirectX/FMOD

and you need DirectX 6 (or up) Dev-Paks to compile DirectX version

GNU/Linux

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\--

Dependencies:

SDL 1.2.7 or better (from libsdl.org)

SDL_Mixer 1.2.2(.7 for file-less music playback) (from libsdl.org)

Nasm (use NOASM=1 if you don\'t have it or have an non-i386 system, I
think)

libPNG 1.2.7

Zlib 1.2.3

The Xiph.org libogg and libvorbis libraries

The OpenGL headers (from Mesa, usually shipped with your X.org or XFree

installation, so you needn\'t worry, most likely)

GCC 3.x toolchain and binutils

GNU Make

Build instructions:

cd src

make LINUX=1 \# you may optionally add DEBUGMODE=1 to build it

\# with debugging symbols

Solaris

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\--

Dependencies:

SDL 1.2.5 or better (from libsdl.org)

SDL_Mixer 1.2.2(.7 for file-less music playback) (from libsdl.org)

libPNG 1.2.7

Zlib 1.2.3

The Xiph.org libogg and libvorbis libraries

The OpenGL headers (from Mesa, usually shipped with your X.org or XFree

installation, so you needn\'t worry, most likely)

GCC 3.x toolchain and binutils

GNU Make

You can get all these programs/libraries from the Companion CD (except
SDL_mixer and OpenGL)

Build instructions:

cd src

gmake SOLARIS=1 \# you may optionally add DEBUGMODE=1 to build it

\# with debugging symbols

FreeBSD

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\--

Dependencies:

SDL 1.2.7 or better (from libsdl.org)

SDL_Mixer 1.2.2(.7 for file-less music playback) (from libsdl.org)

Nasm (use NOASM=1 if you don\'t have it or have an non-i386 system, I
think)

libPNG 1.2.7

Zlib 1.2.3

The Xiph.org libogg and libvorbis libraries

The OpenGL headers (from Mesa, usually shipped with your X.org or XFree

installation, so you needn\'t worry, most likely)

GCC 3.x toolchain and binutils

GNU Make

Build instructions:

cd src

gmake FREEBSD=1 \# you may optionally add DEBUGMODE=1 to build it

\# with debugging symbols

DJGPP/DOS

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\--

Dependencies:

Allegro 3.12 game programming library, (from

http://alleg.sourceforge.net/index.html)

Nasm (use NOASM=1 if you don\'t have it)

libsocket (from http://homepages.nildram.co.uk/\~phekda/richdawe/lsck/)
or

Watt-32 (from http://www.bgnett.no/\~giva/)

GCC 3.x toolchain and binutils

GNU Make

Build instructions:

cd src

make \# to link with Watt-32, add WATTCP=1

\# you may optionally add DEBUGMODE=1 to build it with debugging symbols

\# for remote debugging over the COM port, add RDB=1

Notes:

use tools\\djgpp\\all313.diff to update Allegro to a \"more usable\"
version ;)

Example: E:\\djgpp\\allegro\>patch -p# \<
D:\\SRB2Code\\1.1\\srb2\\tools\\djgpp\\all313.diff

Windows CE

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\--

Dependencies:

SDL 1.27

Build instructions:

use src\\SDL\\WinCE\\SRB2CE.vcw

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\--

Binaries will turn in up in bin/

NOTE: Read the src/makefile for more options.

\- Sonic Team Junior

http://www.srb2.org
