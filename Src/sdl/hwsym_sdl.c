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
//
// DESCRIPTION:
//      Tool for dynamic referencing of hardware rendering functions
//
//      Declaration and definition of the HW rendering 
//      functions do have the same name. Originally, the
//      implementation was stored in a separate library.
//      For SDL, we need some function to return the addresses,
//      otherwise we have a conflict with the compiler.
//      
//-----------------------------------------------------------------------------

#include "hwsym_sdl.h"
#ifdef HWRENDER
#include "ogl_sdl.h"
#include "../hardware/r_opengl/r_opengl.h"
#endif
#ifdef HW3SOUND
#include "../hardware/hw3dsdrv.h"
EXPORT void HWRAPI (Update2DSoundParms) (int handle, int vol, int sep);
#endif

//
// Stupid function to return function addresses
//
void *hwSym(char *funcName)
{
	if(0 == strcmp("FinishUpdate", funcName))
		return NULL; //&FinishUpdate;
#ifdef HWRENDER
	else if(0 == strcmp("Init", funcName))
		return &Init;
	else if(0 == strcmp("Draw2DLine", funcName))
		return &Draw2DLine;
	else if(0 == strcmp("DrawPolygon", funcName))
		return &DrawPolygon;
	else if(0 == strcmp("SetBlend", funcName))
		return &SetBlend;
	else if(0 == strcmp("ClearBuffer", funcName))
		return &ClearBuffer;
	else if(0 == strcmp("SetTexture", funcName))
		return &SetTexture;
	else if(0 == strcmp("ReadRect", funcName))
		return &ReadRect;
	else if(0 == strcmp("GClipRect", funcName))
		return &GClipRect;
	else if(0 == strcmp("ClearMipMapCache", funcName))
		return &ClearMipMapCache;
	else if(0 == strcmp("SetSpecialState", funcName))
		return &SetSpecialState;
	else if(0 == strcmp("SetPalette", funcName))
		return &OglSdlSetPalette;
	else if(0 == strcmp("GetTextureUsed", funcName))
		return &GetTextureUsed;
	else if(0 == strcmp("DrawMD2", funcName))
		return &DrawMD2;
	else if(0 == strcmp("SetTransform", funcName))
		return &SetTransform;
	else if(0 == strcmp("GetRenderVersion", funcName))
		return &GetRenderVersion;
#endif
#ifdef HW3SOUND
	else if(0 == strcmp("Startup", funcName))
		return &Startup;
	else if(0 == strcmp("Shutdown", funcName))
		return &Shutdown;
	else if(0 == strcmp("Add3DSource", funcName))
		return &Add3DSource;
	else if (0 == strcmp("Add2DSource", funcName))
		return &Add2DSource;
	else if(0 == strcmp("StopSource", funcName))
		return &StopSource;
	else if(0 == strcmp("StartSource", funcName))
		return &StartSource;
	else if(0 == strcmp("GetHW3DSVersion", funcName))
		return &GetHW3DSVersion;
	else if(0 == strcmp("BeginFrameUpdate", funcName))
		return &BeginFrameUpdate;
	else if(0 == strcmp("EndFrameUpdate", funcName))
		return &EndFrameUpdate;
	else if(0 == strcmp("IsPlaying", funcName))
		return &IsPlaying;
	else if(0 == strcmp("UpdateListener", funcName))
		return &UpdateListener;
	else if(0 == strcmp("SetGlobalSfxVolume", funcName))
		return &SetGlobalSfxVolume;
	else if(0 == strcmp("SetCone", funcName))
		return &SetCone;
	else if(0 == strcmp("Update2DSource", funcName))
		return &Update2DSoundParms;
	else if(0 == strcmp("Update3DSource", funcName))
		return &Update3DSource;
	else if(0 == strcmp("UpdateSourceVolume", funcName))
		return &UpdateSourceVolume;
	else if(0 == strcmp("Reload3DSource", funcName))
		return &Reload3DSource;
	else if(0 == strcmp("KillSource", funcName))
		return &KillSource;
#endif
	else
		return NULL;
}
