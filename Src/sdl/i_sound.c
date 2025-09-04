// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id: i_sound.c,v 1.12 2004/04/18 12:53:42 hurdler Exp $
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log: i_sound.c,v $
// Revision 1.12  2004/04/18 12:53:42  hurdler
// fix Heretic issue with SDL and OS/2
//
// Revision 1.11  2003/07/13 13:16:15  hurdler
// go RC1
//
// Revision 1.10  2001/08/20 20:40:42  metzgermeister
// *** empty log message ***
//
// Revision 1.9  2001/05/16 22:33:35  bock
// Initial FreeBSD support.
//
// Revision 1.8  2001/05/14 19:02:58  metzgermeister
//   * Fixed floor not moving up with player on E3M1
//   * Fixed crash due to oversized string in screen message ... bad bug!
//   * Corrected some typos
//   * fixed sound bug in SDL
//
// Revision 1.7  2001/04/14 14:15:14  metzgermeister
// fixed bug no sound device
//
// Revision 1.6  2001/04/09 20:21:56  metzgermeister
// dummy for I_FreeSfx
//
// Revision 1.5  2001/03/25 18:11:24  metzgermeister
//   * SDL sound bug with swapped stereo channels fixed
//   * separate hw_trick.c now for HW_correctSWTrick(.)
//
// Revision 1.4  2001/03/09 21:53:56  metzgermeister
// *** empty log message ***
//
// Revision 1.3  2000/11/02 19:49:40  bpereira
// no message
//
// Revision 1.2  2000/09/10 10:56:00  metzgermeister
// clean up & made it work again
//
// Revision 1.1  2000/08/21 21:17:32  metzgermeister
// Initial import to CVS
//
//
// DESCRIPTION:
//      System interface for sound.
//
//-----------------------------------------------------------------------------

#include <math.h>
#include "../doomtype.h"

#ifdef FREEBSD
#include <SDL.h>
#ifdef HAVE_MIXER
#include <SDL_mixer.h>
#endif
#else
#include <SDL/SDL.h>
#ifdef HAVE_MIXER
#include <SDL/SDL_mixer.h>
#endif
#endif

#if defined (__WIN32__)
#include <direct.h>
#elif defined (__GNUC__)
#include <unistd.h>
#endif
#include "../z_zone.h"

#include "../m_swap.h"
#include "../i_system.h"
#include "../i_sound.h"
#include "../m_argv.h"
#include "../m_misc.h"
#include "../w_wad.h"

#include "../doomdef.h"
#include "../doomstat.h"
#include "../s_sound.h"

#include "../d_main.h"

#include "../hardware/hw3sound.h"
#ifdef HW3SOUND
#include "hwsym_sdl.h"
#endif

// The number of internal mixing channels,
//  the samples calculated for each mixing step,
//  the size of the 16bit, 2 hardware channel (stereo)
//  mixing buffer, and the samplerate of the raw data.

// Needed for calling the actual sound output.
#ifdef _WIN32_WCE
#define NUM_CHANNELS            8
#else
#define NUM_CHANNELS            32
#endif

#define INDEXOFSFX(x) ((char *)x - (char *)S_sfx) / sizeof(sfxinfo_t)

static Uint16 samplecount = 1024; //Alam: 1KB samplecount at 22050hz is 46.439909297052154195011337868481ms of buffer

static Uint32 lengths[NUMSFX];     // The actual lengths of all sound effects.

typedef struct chan_struct
{
	// The channel data pointers, start and end.
	Uint8* data; //static unsigned char *channels[NUM_CHANNELS];
	Uint8* end; //static unsigned char *channelsend[NUM_CHANNELS];

	// pitch
	int step;          //static unsigned int channelstep[NUM_CHANNELS];  // The channel step amount...
	int stepremainder; //static unsigned int channelstepremainder[NUM_CHANNELS]; // ... and a 0.16 bit remainder of last step.

	// Time/gametic that the channel started playing,
	//  used to determine oldest, which automatically
	//  has lowest priority.
	tic_t starttic; //static int channelstart[NUM_CHANNELS];

	// The sound handle, determined on registration,
	// used to unregister/stop/modify,
	int handle; //static int channelhandles[NUM_CHANNELS];

	// SFX id of the playing sound effect.
	// Used to catch duplicates (like chainsaw).
	void *id; //static int channelids[NUM_CHANNELS];
	int sfxid;
	int vol;
	int sep;

	// Hardware left and right channel volume lookup.
	Sint16* leftvol_lookup; //static int *channelleftvol_lookup[NUM_CHANNELS];
	Sint16* rightvol_lookup; //static int *channelrightvol_lookup[NUM_CHANNELS];
} chan_t;

static chan_t channels[NUM_CHANNELS];

// Pitch to stepping lookup
static int steptable[256];

// Volume lookups.
static Sint16 vol_lookup[128 * 256];

static boolean soundStarted = false;
static boolean StreamingSound = true;

//SDL's Audio
static SDL_AudioSpec audio;

boolean musicStarted = false;
#ifdef HAVE_MIXER
/* FIXME: Make this file instance-specific */
#define MIDI_TMPFILE    "srb2music"
#define MIDI_TMPFILE2    "srb2wav"
static int musicvol = 62;

static double loopstartFMOD = 0.0f;
static boolean loopingFMOD = false;
static void I_Mix_HookMusicFinished(void);

#if (MIX_MAJOR_VERSION > 1) || (MIX_MINOR_VERSION > 2) || (MIX_MINOR_VERSION > 1 && MIX_PATCHLEVEL > 3)
#define MIXER_POS //Mix_GetMusicType and Mix_FadeInMusicPos
#else
#define Mix_FadeInMusicPos(music,loops,ms,position) Mix_FadeInMusic(music,loops,ms)
#endif

const static int MIDIfade = 500;
const static int FMODfade = 1;

static Mix_Music *music[2] = { NULL, NULL };
#endif


static inline void Snd_LockAudio(void) //Alam: Lock audio data and uninstall audio callback
{
	if(nosound) return;
	if(nomusic && nofmod) SDL_LockAudio();
#ifdef HAVE_MIXER
	else if(musicStarted) Mix_SetPostMix(NULL, NULL);
#endif
	while(StreamingSound){SDL_Delay(1);} //Wait for it
}

static inline void Snd_UnlockAudio(void) //Alam: Unlock audio data and reinstall audio callback
{
	if(nosound) return;
	if(nomusic && nofmod) SDL_UnlockAudio();
#ifdef HAVE_MIXER
	else if(musicStarted) Mix_SetPostMix(audio.callback, audio.userdata);
#endif
}

//
// This function loads the sound data from the WAD lump,
//  for single sound.
//
static inline void *getsfx(int sfxlump, Uint32 *len)
{
	Uint8 *sfx, *paddedsfx = NULL;
	Uint32 i, size = *len, paddedsize = 0;

	sfx = (Uint8 *) W_CacheLumpNum(sfxlump, PU_STATIC);

	if(sfx)
	{
		SDL_AudioCVT sfxcvt;

		if(SDL_BuildAudioCVT(&sfxcvt, AUDIO_U8, 1, *((unsigned short*)sfx+1), AUDIO_U8, 1, audio.freq))
		{//Alam: Setup the AudioCVT for the SFX

			sfxcvt.len = size-8; //Alam: Chop off the header
			sfxcvt.buf = malloc(sfxcvt.len * sfxcvt.len_mult); //Alam: make room
			if (sfxcvt.buf) memcpy(sfxcvt.buf, sfx+8, sfxcvt.len); //Alam: copy the sfx sample

			if(sfxcvt.buf && SDL_ConvertAudio(&sfxcvt) == 0) //Alam: let convert it!
			{
					size = sfxcvt.len_cvt + 8;

					// Pads the sound effect out to the mixing buffer size.
					// The original realloc would interfere with zone memory.
					paddedsize = ((sfxcvt.len_cvt + (samplecount - 1)) / samplecount) * samplecount;

					// Allocate from zone memory.
					paddedsfx = (Uint8 *) Z_Malloc(paddedsize + 8, PU_STATIC, 0);
					// This should interfere with zone memory handling,
					//  which does not kick in in the soundserver.

					// Now copy and pad.
					memcpy(paddedsfx+8, sfxcvt.buf, sfxcvt.len_cvt);
					free(sfxcvt.buf);
					memcpy(paddedsfx,sfx,8);
			}
			else //Alam: the convert failed, not needed or i couldn't malloc the buf
			{
				if (sfxcvt.buf) free(sfxcvt.buf);
				// Pads the sound effect out to the mixing buffer size.
				// The original realloc would interfere with zone memory.
				paddedsize = ((size - 8 + (samplecount - 1)) / samplecount) * samplecount;

				// Allocate from zone memory.
				paddedsfx = (Uint8 *) Z_Malloc(paddedsize + 8, PU_STATIC, 0);
				// This should interfere with zone memory handling,
				//  which does not kick in in the soundserver.

				// Now copy and pad.
				memcpy(paddedsfx, sfx, size);
			}
		}
		else
		{
			// Pads the sound effect out to the mixing buffer size.
			// The original realloc would interfere with zone memory.
			paddedsize = ((size - 8 + (samplecount - 1)) / samplecount) * samplecount;

			// Allocate from zone memory.
			paddedsfx = (Uint8 *) Z_Malloc(paddedsize + 8, PU_STATIC, 0);
			// This should interfere with zone memory handling,
			//  which does not kick in in the soundserver.

			// Now copy and pad.
			memcpy(paddedsfx, sfx, size);

		}
	}
	else //Alam: Couldn't get the zone memory for it?
	{
		*len = paddedsize;
		return paddedsfx;
	}
	for (i = size; i < paddedsize + 8; i++)
		paddedsfx[i] = 128;

	// Remove the cached lump.
	Z_Free(sfx);

	// Preserve padded length.
	*len = paddedsize;

	// Return allocated padded data.
	return paddedsfx;
}

// used to (re)calculate channel params based on vol, sep, pitch
static inline void I_SetChannelParams(chan_t *c, int vol, int sep, int step)
{
	int leftvol;
	int rightvol;
	c->vol = vol;
	c->sep = sep;
	c->step = step;

	// x^2 separation, that is, orientation/stereo.
	//  range is: 0 (left) - 255 (right)

	// Volume arrives in range 0..255 and it must be in 0..cv_soundvolume...
	vol = (vol * cv_soundvolume.value) >> 7;
	// note: >> 6 would use almost the entire dynamical range, but
	// then there would be no "dynamical room" for other sounds :-/

	leftvol  = vol - ((vol*sep*sep) >> 16); ///(256*256);
	sep = 255 - sep;
	rightvol = vol - ((vol*sep*sep) >> 16);

	// Sanity check, clamp volume.
	if (rightvol < 0 || rightvol > 127)
	{
		rightvol = 63;
		//I_Error("rightvol out of bounds");
	}

	if (leftvol < 0 || leftvol > 127)
	{
		leftvol = 63;
		//I_Error("leftvol out of bounds");
	}

	// Get the proper lookup table piece
	//  for this volume level???
	c->leftvol_lookup = &vol_lookup[leftvol*256];
	c->rightvol_lookup = &vol_lookup[rightvol*256];
}

//
// This function adds a sound to the
//  list of currently active sounds,
//  which is maintained as a given number
//  (eight, usually) of internal channels.
// Returns a handle.
//
static inline int addsfx(int sfxid, int volume, int step, int seperation)
{
	static unsigned short handlenums = 0;

	int i;
	int rc = -1;

	tic_t oldest = gametic;
	int oldestnum = 0;
	int slot;

	// Play these sound effects only one at a time.
#if 0
	if (sfxid == sfx_stnmov
#if 0
	 ||sfxid == sfx_sawup || sfxid == sfx_sawidl || sfxid == sfx_sawful || sfxid == sfx_sawhit || sfxid == sfx_pistol
#endif
	 )
	{
		// Loop all channels, check.
		for (i = 0; i < NUM_CHANNELS; i++)
		{
			// Active, and using the same SFX?
			if ((channels[i].data) && (channels[i].sfxid == sfxid))
			{
				// Reset.
				channels[i].data = NULL;
				// We are sure that iff,
				//  there will only be one.
				break;
			}
		}
	}
#endif

	// Loop all channels to find oldest SFX.
	for (i = 0; (i < NUM_CHANNELS) && (channels[i].data); i++)
	{
		if (channels[i].starttic < oldest)
		{
			oldestnum = i;
			oldest = channels[i].starttic;
		}
	}

	// Tales from the cryptic.
	// If we found a channel, fine.
	// If not, we simply overwrite the first one, 0.
	// Probably only happens at startup.
	if (i == NUM_CHANNELS)
		slot = oldestnum;
	else
		slot = i;

	// Okay, in the less recent channel,
	//  we will handle the new SFX.
	// Set pointer to raw data.
	channels[slot].data = (Uint8 *) S_sfx[sfxid].data + 8; //Alam: offset of the sound header
	// Set pointer to end of raw data.
	channels[slot].end = channels[slot].data + lengths[sfxid];

	// Reset current handle number, limited to 0..100.
	if (!handlenums)
		handlenums = 100;

	// Assign current handle number.
	// Preserved so sounds could be stopped (unused).
	channels[slot].handle = rc = handlenums++;

	// ???
	channels[slot].stepremainder = 0;
	// Should be gametic, I presume.
	channels[slot].starttic = gametic;

	I_SetChannelParams(&channels[slot], volume, seperation, step);

	// Preserve sound SFX id,
	//  e.g. for avoiding duplicates of chainsaw.
	channels[slot].id = S_sfx[sfxid].data;

	channels[slot].sfxid = sfxid;

	// You tell me.
	return rc;
}

//
// SFX API
// Note: this was called by S_Init.
// However, whatever they did in the
// old DPMS based DOS version, this
// were simply dummies in the Linux
// version.
// See soundserver initdata().
//
// Well... To keep compatibility with legacy doom, I have to call this in
// I_InitSound since it is not called in S_Init... (emanne@absysteme.fr)

static inline void I_SetChannels()
{
	// Init internal lookups (raw data, mixing buffer, channels).
	// This function sets up internal lookups used during
	//  the mixing process.
	int i;
	int j;

	int *steptablemid = steptable + 128;

	if (nosound)
		return;

	// This table provides step widths for pitch parameters.
	for (i = -128; i < 128; i++)
		steptablemid[i] = (int) (pow(2.0, (i / 64.0)) * 65536.0);

	// Generates volume lookup tables
	//  which also turn the unsigned samples
	//  into signed samples.
	for (i = 0; i < 128; i++)
		for (j = 0; j < 256; j++)
			vol_lookup[i * 256 + j] = (i * (j - 128) * 256) / 127;
}

static inline int FindChannel(int handle)
{
	int i;

	for (i = 0; i < NUM_CHANNELS; i++)
		if (channels[i].handle == handle)
			return i;

	// not found
	return -1;
}

void I_SetSfxVolume(int volume)
{
	int i;
	//Snd_LockAudio(); //Alam: too much?
	for (i = 0; i < NUM_CHANNELS; i++)
		if(channels[i].data) I_SetChannelParams(&channels[i], channels[i].vol, channels[i].sep, channels[i].step);
	//Snd_UnlockAudio(); //Alam: too much?

}

void *I_GetSfx(sfxinfo_t* sfx)
{
	if (sfx->lumpnum < 0)
		sfx->lumpnum = S_GetSfxLumpNum(sfx);
//	else if (sfx->lumpnum != S_GetSfxLumpNum(sfx))
//		I_FreeSfx(sfx);

#ifdef HW3SOUND
	if (hws_mode != HWS_DEFAULT_MODE)
		return W_CacheLumpNum(sfx->lumpnum, PU_STATIC);
#endif

#if 0
	if(sfx->link)
	{
		sfx->link->data = I_GetSfx(sfx->link); //Alam: Look for real data
		lengths[INDEXOFSFX(sfx)] = lengths[INDEXOFSFX(sfx->link)]; //Alam: length from link
		sfx->lumpnum = sfx->link->lumpnum;
		return sfx->link->data;
	}
#endif

	if(sfx->data)
		return sfx->data; //Alam: I have it done!

	lengths[INDEXOFSFX(sfx)] = W_LumpLength (sfx->lumpnum);

	return getsfx(sfx->lumpnum, &lengths[INDEXOFSFX(sfx)]);

}

void I_FreeSfx(sfxinfo_t * sfx)
{
//	if (sfx->lumpnum<0)
//		return;

#ifdef HW3SOUND
	if (hws_mode != HWS_DEFAULT_MODE)
	{
		if (sfx->data)
			Z_Free(sfx->data);
	}
	else
#endif
	{
		int i;

		for (i = 1; i < NUMSFX; i++)
		{
			// Alias? Example is the chaingun sound linked to pistol.
			if (S_sfx[i].data == sfx->data)
			{
				if(&S_sfx[i] != sfx) S_sfx[i].data = NULL;
				S_sfx[i].lumpnum = -1;
				lengths[i] = 0;
			}
		}
		Snd_LockAudio();
		// Loop all channels, check.
		for (i = 0; i < NUM_CHANNELS; i++)
		{
			// Active, and using the same SFX?
			if (channels[i].data && channels[i].id == sfx->data)
			{
				channels[i].data = NULL; // Reset.
			}
		}
		Snd_UnlockAudio();
		if(sfx->data) Z_Free(sfx->data);
		sfx->data = NULL;
	}
}

//
// Starting a sound means adding it
//  to the current list of active sounds
//  in the internal channels.
// As the SFX info struct contains
//  e.g. a pointer to the raw data,
//  it is ignored.
// As our sound handling does not handle
//  priority, it is ignored.
// Pitching (that is, increased speed of playback)
//  is set, but currently not used by mixing.
//
int I_StartSound(int id, int vol, int sep, int pitch, int priority)
{
	if (nosound)
		return 0;

	priority = 128;
	pitch = priority;
	//if(!S_sfx[id].data) S_sfx[id].data = I_GetSfx(&S_sfx[id]); //Alam: Get new stuff?
	// Returns a handle (not used).
	Snd_LockAudio();
	id = addsfx(id, vol, steptable[pitch], sep);
	Snd_UnlockAudio();

	return id;
}

void I_StopSound(int handle)
{
	// You need the handle returned by StartSound.
	// Would be looping all channels,
	//  tracking down the handle,
	//  an setting the channel to zero.
	int i;

	Snd_LockAudio();

	i = FindChannel(handle);

	if (i != -1)
		channels[i].data = NULL;

	Snd_UnlockAudio();
	return;
}

int I_SoundIsPlaying(int handle)
{
	int isplaying = false, chan;
	//Snd_LockAudio(); //Too Much?
	chan = FindChannel(handle);
	if(chan > -1)
		isplaying = (channels[chan].data != NULL);
	//Snd_UnlockAudio(); //Too Much?
	return isplaying;
}

//
// Not used by SDL version
//
void I_SubmitSound(void){}

//
// This function loops all active (internal) sound
//  channels, retrieves a given number of samples
//  from the raw sound data, modifies it according
//  to the current (internal) channel parameters,
//  mixes the per channel samples into the given
//  mixing buffer, and clamping it to the allowed
//  range.
//
// This function currently supports only 16bit.
//
void I_UpdateSound()
{
    /*
       Pour une raison que j'ignore, la version SDL n'appelle jamais
       ce truc directement. Fonction vide pour garder une compatibilité
       avec le point de vue de legacy...
     */

    // Himmel, Arsch und Zwirn
}

static void I_UpdateSound_sdl(void *userdata, Uint8 *stream, int len)
{
	// Mix current sound data.
	// Data, from raw sound
	register Sint32 dr; // Right 16bit stream
	register Uint8 sample; // Center 8bit sfx
	register Sint32 dl; // Left 16bit stream

	// Pointers in audio stream
	Sint16 *rightout = (Sint16 *) stream; // currect right
	Sint16 *leftout = rightout + 1;// currect left
	const Uint8 step = 2; // Step in stream, left and right, thus two.

	int chan; // Mixing channel index.

	if (nosound || !userdata)
		return;

	// Determine end of the stream
	len /= 4; // not 8bit mono samples, 16bit stereo ones

	StreamingSound = true;

	// Mix sounds into the mixing buffer.
	// Loop over len
	while (len--)
	{
		// Reset left/right value.
		dl = *leftout;
		dr = *rightout;

		// Love thy L2 chache - made this a loop.
		// Now more channels could be set at compile time
		//  as well. Thus loop those channels.
		for (chan = 0; chan < NUM_CHANNELS; chan++)
		{
			// Check channel, if active.
			if (channels[chan].data)
			{
				// Get the raw data from the channel.
				sample = *channels[chan].data;
				// Add left and right part
				//  for this channel (sound)
				//  to the current data.
				// Adjust volume accordingly.
				dl += channels[chan].leftvol_lookup[sample];
				dr += channels[chan].rightvol_lookup[sample];
				// Increment index ???
				channels[chan].stepremainder += channels[chan].step;
				// MSB is next sample???
				channels[chan].data += channels[chan].stepremainder >> 16;
				// Limit to LSB???
				channels[chan].stepremainder &= 65536 - 1;

				// Check whether we are done.
				if (channels[chan].data >= channels[chan].end)
					channels[chan].data = NULL;
			}
		}

		// Clamp to range. Left hardware channel.
		// Has been char instead of short.

		if (dl > 0x7fff)
			*leftout = 0x7fff;
		else if (dl < -0x8000)
			*leftout = -0x8000;
		else
			*leftout = dl;

		// Same for right hardware channel.
		if (dr > 0x7fff)
			*rightout = 0x7fff;
		else if (dr < -0x8000)
			*rightout = -0x8000;
		else
			*rightout = dr;

		// Increment current pointers in stream
		leftout += step;
		rightout += step;

	}
	StreamingSound = false;
}

void I_UpdateSoundParams(int handle, int vol, int sep, int pitch)
{
	// Would be using the handle to identify
	//  on which channel the sound might be active,
	//  and resetting the channel parameters.

	int i;
	pitch = 128;

	//Snd_LockAudio(); //Alam: too much?

	if (FindChannel(handle) < 0)
		return;
	else i = FindChannel(handle);

	if (channels[i].data)
		I_SetChannelParams(&channels[i], vol, sep, steptable[pitch]);
	//Snd_UnlockAudio(); //Alam: too much?
	return;
}


#ifdef HW3SOUND
static int Init3DSDriver(void)
{
	//return false;
	HW3DS.pfnStartup            = hwSym("Startup");
	HW3DS.pfnShutdown           = hwSym("Shutdown");
	HW3DS.pfnAdd3DSource        = hwSym("Add3DSource");
	HW3DS.pfnAdd2DSource        = hwSym("Add2DSource");
	HW3DS.pfnStopSource         = hwSym("StopSource");
	HW3DS.pfnStartSource        = hwSym("StartSource");
	HW3DS.pfnGetHW3DSVersion    = hwSym("GetHW3DSVersion");
	HW3DS.pfnBeginFrameUpdate   = hwSym("BeginFrameUpdate");
	HW3DS.pfnEndFrameUpdate     = hwSym("EndFrameUpdate");
	HW3DS.pfnIsPlaying          = hwSym("IsPlaying");
	HW3DS.pfnUpdateListener     = hwSym("UpdateListener");
	HW3DS.pfnSetGlobalSfxVolume = hwSym("SetGlobalSfxVolume");
	HW3DS.pfnSetCone            = hwSym("SetCone");
	HW3DS.pfnUpdate2DSoundParms = hwSym("Update2DSound");
	HW3DS.pfnUpdate3DSource     = hwSym("Update3DSource");
	HW3DS.pfnUpdateSourceVolume = hwSym("UpdateSourceVolume");
	HW3DS.pfnReload3DSource     = hwSym("Reload3DSource");
	HW3DS.pfnKillSource         = hwSym("KillSource");
	return true;
}
#endif

void I_ShutdownSound(void)
{
	if (nosound || !soundStarted)
		return;

	CONS_Printf("I_ShutdownSound: ");

#ifdef HW3SOUND
	if (hws_mode != HWS_DEFAULT_MODE)
	{
		HW3S_Shutdown();
		return;
	}
#endif

	if (!soundStarted)
		return;

	if(nomusic && nofmod)
		SDL_CloseAudio();
	CONS_Printf("shut down\n");
	soundStarted = false;
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void I_StartupSound()
{
#ifndef HAVE_MIXER
	nomusic = nofmod = true;
#endif

	memset(channels, 0, sizeof(channels)); //Alam: Clean it

	audio.freq = 22050;
	audio.format = AUDIO_S16SYS;
	audio.channels = 2;
	audio.samples = samplecount;
	audio.callback = I_UpdateSound_sdl;
	audio.userdata = channels;

	if(dedicated)
	{
		nomusic = nofmod = true;
		return;
	}

	// Configure sound device
	CONS_Printf("I_StartupSound:\n");

	// Open the audio device
	if (M_CheckParm ("-freq") && M_IsNextParm())
	{
		audio.freq = atoi(M_GetNextParm());
		if(!audio.freq) audio.freq = 22050;
		audio.samples = samplecount*(int)(audio.freq/22050); //Alam: to keep it around the same XX ms
		CONS_Printf (" requested frequency of %d hz\n", audio.freq);
	}

	if (nosound)
		return;

#ifdef HW3SOUND
	if (M_CheckParm("-3dsound") || M_CheckParm("-ds3d"))
	{
		hws_mode = 1;
	}
	if (hws_mode != HWS_DEFAULT_MODE)
	{
		if (Init3DSDriver())
		{
			snddev_t            snddev;

			//nosound = true;
			//I_AddExitFunc(I_ShutdownSound);
			snddev.bps = 16;
			snddev.sample_rate = audio.freq;
#ifdef __WIN32__
			snddev.cooplevel = 0x00000002;
			snddev.hWnd = vid.WndParent;
#endif
			if (HW3S_Init(I_Error, &snddev))
			{
				CONS_Printf(" Using 3D sound driver\n");
				return;
			}
			// Falls back to default sound system
			HW3S_Shutdown();
		}
		hws_mode = HWS_DEFAULT_MODE;
	}
#endif
	if (SDL_OpenAudio(&audio, NULL) < 0)
	{
		CONS_Printf(" couldn't open audio with desired format\n");
		SDL_CloseAudio();
		nosound = true;
		return;
	}
	samplecount = audio.samples;
	if(!nomusic || !nofmod) SDL_CloseAudio();
	CONS_Printf(" configured audio device with %d samples/slice at %ikhz(%dms buffer)\n", samplecount, audio.freq/1000, (int) (((float)audio.samples * 1000.0) / audio.freq));
	// Finished initialization.
	CONS_Printf("I_InitSound: sound module ready\n");
	//[segabor]
	SDL_PauseAudio(0);
	//Mix_Pause(0);
	I_SetChannels();
	soundStarted = true;
}

//
// MUSIC API.
//

void I_ShutdownMusic(void)
{
#ifdef HAVE_MIXER
	if (nomusic && nofmod)
		return;

	if (!musicStarted)
		return;

	CONS_Printf("I_ShutdownMusic: ");

	I_StopFMODSong();
	I_UnRegisterSong(0);
	Mix_CloseAudio();

	CONS_Printf("shut down\n");
	musicStarted = false;
#endif
}

void I_InitMusic(void)
{
	if (nomusic && nofmod)
		return;

	if(dedicated)
		return;

#ifdef HAVE_MIXER
	if(audio.freq < 44100 && !M_CheckParm ("-freq")) //I want atleast 44Khz
	{
		audio.samples *= (int)(44100/audio.freq); //Alam: to keep it around the same XX ms
		audio.freq = 44100;
	}

	if (Mix_OpenAudio(audio.freq, audio.format, audio.channels, audio.samples) < 0) //open_music(&audio)
	{
		CONS_Printf("Unable to open music: %s\n", Mix_GetError());
		nomusic = nofmod = true;
		return;
	}

	samplecount = audio.samples;
	CONS_Printf(" Reconfigured audio device with %d samples/slice at %ikhz(%dms buffer)\n", samplecount, audio.freq/1000, (int) (((float)audio.samples * 1000.0) / audio.freq));
	if (!nosound) Mix_SetPostMix(audio.callback, audio.userdata);  // after mixing music, add sound effects
	Mix_HookMusicFinished(I_Mix_HookMusicFinished); //Alam: Looping!

	Mix_Resume(-1);
	if (!nosound) soundStarted = true;

	CONS_Printf("I_InitMusic: music initialized\n");
	musicStarted = true;
#endif
}

void I_PlaySong(int handle, int looping)
{
#ifdef HAVE_MIXER
	if (nomusic)
		return;

	if (music[handle])
	{
		int musicst;
		Mix_HookMusicFinished(NULL);
		musicst = Mix_FadeInMusic(music[handle], looping ? -1 : 0, MIDIfade);
		Mix_VolumeMusic(musicvol);
		if(musicst == -1)
			CONS_Printf("I_PlaySong: Couldn't play song because %s\n",Mix_GetError());
		else return;
	}
#endif
	return;
}

void I_PauseSong(int handle)
{
#ifdef HAVE_MIXER
	if (nomusic && nofmod)
		return;

	Mix_PauseMusic();
	//I_StopSong(handle);
#endif
}

void I_ResumeSong(int handle)
{
#ifdef HAVE_MIXER
	if (nomusic && nofmod)
		return;

	Mix_VolumeMusic(musicvol);
	Mix_ResumeMusic();
	//I_PlaySong(handle, true);
#endif
}

void I_StopSong(int handle)
{
#ifdef HAVE_MIXER
	if (nomusic)
		return;
	Mix_FadeOutMusic(MIDIfade);
#endif
}

void I_UnRegisterSong(int handle)
{
#ifdef HAVE_MIXER
	if (nomusic)
		return;

	Mix_HaltMusic();

	if (music[handle])
	{
		Mix_FreeMusic(music[handle]);
		music[handle] = NULL;
	}
	unlink(MIDI_TMPFILE);
#endif
}

int I_RegisterSong(void *data, int len)
{
#ifdef HAVE_MIXER
	FILE *midfile;

	if (nomusic)
		return 0;

	midfile = fopen(MIDI_TMPFILE, "wb");
	if (!midfile)
	{
		CONS_Printf("Couldn't write MIDI to %s\n", MIDI_TMPFILE);
		return 0;
	}

	if (memcmp(data, "MThd", 4) == 0) // support mid file in WAD !!!
	{
		fwrite(data, 1, len, midfile);
	}
	else
	{
		CONS_Printf("Music Lump is not a MIDI lump\n");
		return 0;
	}

	fclose(midfile);

	music[0] = Mix_LoadMUS(MIDI_TMPFILE);

	if (music[0] == NULL)
	{
		CONS_Printf("Couldn't load MIDI from %s: %s\n", MIDI_TMPFILE, Mix_GetError());
	}
	//else Mix_VolumeMusic(musicvol);
#endif
	return (0);
}

void I_SetMusicVolume(int volume)
{
#ifdef HAVE_MIXER
	if (nomusic && nofmod)
		return;

	musicvol = volume * 2;
	Mix_VolumeMusic(musicvol);
#endif
}

boolean I_StartFMODSong (char* musicname, int looping)
{
#ifdef HAVE_MIXER
	char filename[9];
	void *data, *musicdata;
	int lumpnum;
	FILE *midfile;

	if (nofmod)
		return 0;

	sprintf(filename, "o_%s", musicname);

	lumpnum = W_CheckNumForName(filename);

	I_StopFMODSong();

	//unlink(MIDI_TMPFILE2);

	if(lumpnum == -1)
	{
	// Alam_GBC: like in win32/win_snd.c: Graue 02-29-2004: don't worry about missing music, there might still be a MIDI
		//CONS_Printf("Music lump %s not found!\n", filename);
		return 0; // No music found. Oh well!
	}

	midfile = fopen(MIDI_TMPFILE2, "wb");
	if (!midfile)
	{
		CONS_Printf("Couldn't write WAV to %s\n", MIDI_TMPFILE2);
		return 0;
	}

	data = W_CacheLumpName ( filename, PU_CACHE );

	musicdata = data;

	fwrite(musicdata, 1, W_LumpLength(lumpnum), midfile);
	fclose(midfile);

	Z_Free(data);

	music[1] = Mix_LoadMUS(MIDI_TMPFILE2);

	if(!music[1])
	{
		//CONS_Printf("Couldn't load music lump %s from the file %s\n",filename,MIDI_TMPFILE2);
	}
	else
	{
		loopingFMOD = looping;
#ifdef MIXER_POS
		if(Mix_GetMusicType(music[1]) == MUS_OGG) //Only Ogg files
			if(loopingFMOD) looping = false; //Alam: SDL_mixer will play while i look for the loop point
#endif
		Mix_HookMusicFinished(NULL);
		if(Mix_FadeInMusic(music[1], looping ? -1 : 0,FMODfade) == -1)
		{
			if(devparm)
				CONS_Printf("I_StartFMODSong: Couldn't play song %s because %s\n",musicname, Mix_GetError());
			return false;
		}
		Mix_VolumeMusic(musicvol);
		// Scan the Ogg Vorbis file for the COMMENT= field for a custom loop point
		if(!looping && loopingFMOD)
		{
			int scan;
			byte* dataum;
			char looplength[64];
			unsigned int loopstart = 0;
			int newcount = 0;
			int lumplength = W_LumpLength(lumpnum);

			Mix_HookMusicFinished(I_Mix_HookMusicFinished);

			data = W_CacheLumpName ( filename, PU_CACHE );

			dataum = (byte *)data;

			for(scan = 0;scan < lumplength; scan++)
			{
				if(*dataum++ == 'C'){
				if(*dataum++ == 'O'){
				if(*dataum++ == 'M'){
				if(*dataum++ == 'M'){
				if(*dataum++ == 'E'){
				if(*dataum++ == 'N'){
				if(*dataum++ == 'T'){
				if(*dataum++ == '='){
				if(*dataum++ == 'L'){
				if(*dataum++ == 'O'){
				if(*dataum++ == 'O'){
				if(*dataum++ == 'P'){
				if(*dataum++ == 'P'){
				if(*dataum++ == 'O'){
				if(*dataum++ == 'I'){
				if(*dataum++ == 'N'){
				if(*dataum++ == 'T'){
				if(*dataum++ == '=')
				{

					while(*dataum != 1 && newcount != 63)
					{
						looplength[newcount++] = *dataum++;
					}

					looplength[newcount] = '\n';

					loopstart = atoi(looplength);

				}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
				else
					dataum--;}
			}

			Z_Free(data);

			if(loopstart > 0)
			{
				loopstartFMOD = 8.0+loopstart; //8 PCM chucks off
				loopstartFMOD /= 44100.0f; //PCM to secs
#ifdef PARANOIA
				//CONS_Printf("looping start at %i\n",(int)loopstartFMOD);
#endif
			}
			else
			{
				loopstartFMOD = 0.0f; // loopingFMOD true, but couldn't find start loop point
			}
		}
		else
			loopstartFMOD = 0.0f;
		return 1;
	}
#endif
	return 0;
}

void I_StopFMODSong()
{
#ifdef HAVE_MIXER
	if (nofmod)
		return;

	Mix_HaltMusic();

	if(music[1])
	{
		Mix_FreeMusic(music[1]);
		music[1] = NULL;
	}
#endif
}

void I_SetFMODVolume(int volume)
{
#ifdef HAVE_MIXER
	//if(music[1] == NULL) return;

	I_SetMusicVolume(volume);
#endif
}

#ifdef HAVE_MIXER
static void I_Mix_HookMusicFinished(void)
{
	int musicst = 0;
	Mix_HookMusicFinished(NULL);
	if(!music[1])
		return;
	musicst = Mix_FadeInMusicPos(music[1],loopstartFMOD?0:-1,FMODfade,loopstartFMOD);
	if(loopstartFMOD)
		Mix_HookMusicFinished(I_Mix_HookMusicFinished);
	if(musicst == 0)
		Mix_VolumeMusic(musicvol);
	else if (devparm)
		CONS_Printf("I_Mix_HookMusicFinished: Couldn't loop song because %s\n",Mix_GetError());
}
#endif
