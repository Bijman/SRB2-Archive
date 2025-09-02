// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id: dehacked.c,v 1.15 2002/01/12 12:41:05 hurdler Exp $
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
// $Log: dehacked.c,v $
// Revision 1.15  2002/01/12 12:41:05  hurdler
// very small commit
//
// Revision 1.14  2002/01/12 02:21:36  stroggonmeth
// Big commit
//
// Revision 1.13  2001/07/16 22:35:40  bpereira
// - fixed crash of e3m8 in heretic
// - fixed crosshair not drawed bug
//
// Revision 1.12  2001/06/30 15:06:01  bpereira
// fixed wronf next level name in intermission
//
// Revision 1.11  2001/04/30 17:19:24  stroggonmeth
// HW fix and misc. changes
//
// Revision 1.10  2001/02/10 12:27:13  bpereira
// no message
//
// Revision 1.9  2001/01/25 22:15:41  bpereira
// added heretic support
//
// Revision 1.8  2000/11/04 16:23:42  bpereira
// no message
//
// Revision 1.7  2000/11/03 13:15:13  hurdler
// Some debug comments, please verify this and change what is needed!
//
// Revision 1.6  2000/11/02 17:50:06  stroggonmeth
// Big 3Dfloors & FraggleScript commit!!
//
// Revision 1.5  2000/08/31 14:30:55  bpereira
// no message
//
// Revision 1.4  2000/04/16 18:38:07  bpereira
// no message
//
// Revision 1.3  2000/04/05 15:47:46  stroggonmeth
// Added hack for Dehacked lumps. Transparent sprites are now affected by colormaps.
//
// Revision 1.2  2000/02/27 00:42:10  hurdler
// fix CR+LF problem
//
// Revision 1.1.1.1  2000/02/22 20:32:32  hurdler
// Initial import into CVS (v1.29 pr3)
//
//
// DESCRIPTION:
//      Load dehacked file and change table and text from the exe
//
//-----------------------------------------------------------------------------


#include "doomdef.h"

#include "command.h"
#include "console.h"

#include "g_game.h"

#include "sounds.h"
#include "info.h"

#include "m_cheat.h"
#include "d_think.h"
#include "dstrings.h"
#include "m_argv.h"
#include "p_inter.h"

//SoM: 4/4/2000: DEHACKED LUMPS
#include "z_zone.h"
#include "w_wad.h"

#include "m_menu.h"
#include "f_finale.h"
#ifdef SPMOVES
#include "spmoves.h"
#endif

boolean deh_loaded = false;

boolean modcredits = false;

char *myfgets(char *buf, int bufsize, MYFILE *f)
{
    int i=0;
    if( myfeof(f) )
        return NULL;
    // we need on byte for null terminated string
    bufsize--;
    while(i<bufsize && !myfeof(f) )
    {
        char c = *f->curpos++;
        if( c!='\r' )
            buf[i++]=c;
        if( c=='\n' )
            break;
    }
    buf[i] = '\0';
    //CONS_Printf("fgets [0]=%d [1]=%d '%s'\n",buf[0],buf[1],buf);

    return buf;
}

char *myfgetsforscenetext(char *buf, int bufsize, MYFILE *f)
{
    int i=0;
    if( myfeof(f) )
        return NULL;
    // we need on byte for null terminated string
    bufsize--;
    while(i<bufsize && !myfeof(f) )
    {
        char c = *f->curpos++;
        if( c!='\r' )
            buf[i++]=c;
        if( c=='#' )
            break;
    }
	i++;
    buf[i] = '\0';
    //CONS_Printf("fgets [0]=%d [1]=%d '%s'\n",buf[0],buf[1],buf);

    return buf;
}

char *myfgetsforplayertext(char *buf, int bufsize, MYFILE *f)
{
    int i=0;
    if( myfeof(f) )
        return NULL;
    // we need on byte for null terminated string
    bufsize--;
    while(i<bufsize && !myfeof(f) )
    {
        char c = *f->curpos++;
        if( c!='\r' )
            buf[i++]=c;
        if( c=='#' )
            break;
    }
    buf[i] = '\0';
    //CONS_Printf("fgets [0]=%d [1]=%d '%s'\n",buf[0],buf[1],buf);

    return buf;
}

size_t  myfread( char *buf, size_t size, size_t count, MYFILE *f )
{
    size_t byteread = size-(f->curpos-f->data);
    if( size*count < byteread )
        byteread = size*count;
    if( byteread>0 )
    {
        ULONG i;
        for(i=0;i<byteread;i++)
        {
            char c=*f->curpos++;
            if( c!='\r' )
                buf[i]=c;
            else
                i--;
        }
    }
    return byteread/size;
}

static int deh_num_error=0;

static void deh_error(char *first, ...)
{
    va_list     argptr;

    if (devparm || cv_debug)
    {
       char buf[1000];

       va_start (argptr,first);
       vsprintf (buf, first,argptr);
       va_end (argptr);

       CONS_Printf("%s\n",buf);
    }

    deh_num_error++;
}

/* ======================================================================== */
// Load a dehacked file format 6 I (BP) don't know other format
/* ======================================================================== */
/* a sample to see
                   Thing 1 (Player)       {           // MT_PLAYER
int doomednum;     ID # = 3232              -1,             // doomednum
int spawnstate;    Initial frame = 32       S_PLAY,         // spawnstate
int spawnhealth;   Hit points = 3232        100,            // spawnhealth
int seestate;      First moving frame = 32  S_PLAY_RUN1,    // seestate
int seesound;      Alert sound = 32         sfx_None,       // seesound
int reactiontime;  Reaction time = 3232     0,              // reactiontime
int attacksound;   Attack sound = 32        sfx_None,       // attacksound
int painstate;     Injury frame = 32        S_PLAY_PAIN,    // painstate
int painchance;    Pain chance = 3232       255,            // painchance
int painsound;     Pain sound = 32          sfx_plpain,     // painsound
int meleestate;    Close attack frame = 32  S_NULL,         // meleestate
int missilestate;  Far attack frame = 32    S_PLAY_ATK1,    // missilestate
int deathstate;    Death frame = 32         S_PLAY_DIE1,    // deathstate
int xdeathstate;   Exploding frame = 32     S_PLAY_XDIE1,   // xdeathstate
int deathsound;    Death sound = 32         sfx_pldeth,     // deathsound
int speed;         Speed = 3232             0,              // speed
int radius;        Width = 211812352        16*FRACUNIT,    // radius
int height;        Height = 211812352       56*FRACUNIT,    // height
int mass;          Mass = 3232              100,            // mass
int damage;        Missile damage = 3232    0,              // damage
int activesound;   Action sound = 32        sfx_None,       // activesound
int flags;         Bits = 3232              MF_SOLID|MF_SHOOTABLE|MF_DROPOFF|MF_PICKUP|MF_NOTDMATCH,
int raisestate;    Respawn frame = 32       S_NULL          // raisestate
                                         }, */

static int searchvalue(char *s)
{
  while(s[0]!='=' && s[0]!='\0') s++;
  if (s[0]=='=')
    return atoi(&s[1]);
  else
  {
    deh_error("No value found\n");
    return 0;
  }
}

static int searchvaluenoerror(char *s)
{
  while(s[0]!='=' && s[0]!='\0') s++;
  if (s[0]=='=')
    return atoi(&s[1]);
  else
  {
    return 0;
  }
}

/*
static char* GetSceneText(int num, int scenenum, char *s)
{
	char* firstpos;
  while(s[0]!='=' && s[0]!='\0') s++;
  if (s[0]=='=')
  {
	  s++; // ' '
	  s++;
	  firstpos = &s[0];
	  while(s[0]!='#' && s[0]!='\0') s++;
	  if(s[0]=='#')
	  {
		  s++;
		  s[0] = '\0';
	  }
    return firstpos;
  }
  else
  {
    deh_error("No GetSceneText string found\n");
    return 0;
  }
}*/

// Edits an animated texture slot on the array
// Tails 12-27-2003
/*static void readAnimTex(MYFILE *f,int num)
{
  char s[MAXLINELEN];
  char *word,*word2;
  int i;

  do{
    if(myfgets(s,sizeof(s),f)!=NULL)
    {
      if(s[0]=='\n') break;
      // set the value in apropriet field
      word=strupr(strtok(s," "));

	  word2=strupr(strtok(NULL," = "));

	  word2[strlen(word2)-1]='\0';

	  i = atoi(word2);

           if(!strcmp(word,"START"))
		   {
			   strncpy(harddefs[num].startname, word2, 8);
		   }
           if(!strcmp(word,"END"))
		   {
			   strncpy(harddefs[num].endname, word2, 8);
		   }
      else if(!strcmp(word,"SPEED"))	  harddefs[num].speed = i;
	  else if(!strcmp(word,"ISTEXTURE")) harddefs[num].istexture = i;

      else deh_error("readAnimTex %d : unknown word '%s'\n",num,word);
    }
  } while(s[0]!='\n' && !myfeof(f)); //finish when the line is empty
}*/

// Reads a PLAYER, not a CHARACTER
// For modifying the character select screen
static void readCharacter(MYFILE *f,int num)
{
  char s[MAXLINELEN];
  char *word,*word2;
  int i;

  do{
    if(myfgets(s,sizeof(s),f)!=NULL)
    {
      if(s[0]=='\n') break;
      // set the value in apropriet field
      word=strupr(strtok(s," "));

      if(!strcmp(word,"PLAYERTEXT"))
	  {
		  char *playertext;
		  for(i=0; i<MAXLINELEN; i++)
		  {
			  if(s[i] == '=')
			  {
				  playertext = &s[i+2];
				  break;
			  }
		  }
		  for(i=0; i<MAXLINELEN; i++)
		  {
			  if(s[i] == '\0')
			  {
				  s[i] = '\n';
				  s[i+1] = '\0';
				  break;
			  }
		  }

		  strcpy(description[num].info, playertext);
		  strcat(description[num].info, myfgetsforplayertext(playertext, sizeof(description[num].info),f));
		  continue;
	  }

	  word2=strupr(strtok(NULL," = "));

	  word2[strlen(word2)-1]='\0';

	  i = atoi(word2);

           if(!strcmp(word,"PLAYERNAME"))
		   {
			   strncpy(&description[num].text[0], word2, 64);
			   PlayerMenu[num].text = &description[num].text[0];
		   }
      else if(!strcmp(word,"MENUPOSITION"))	  PlayerMenu[num].alphaKey = i;
	  else if(!strcmp(word,"PICNAME")) strncpy(&description[num].picname[0], word2, 9);
	  else if(!strcmp(word,"STATUS")) PlayerMenu[num].status = i;

      else deh_error("readCharacter %d : unknown word '%s'\n",num,word);
    }
  } while(s[0]!='\n' && !myfeof(f)); //finish when the line is empty
}

static void readthing(MYFILE *f,int num, int ver)
{
  char s[MAXLINELEN];
  char *word;
  int value;

  do{
    if(myfgets(s,sizeof(s),f)!=NULL)
    {
      if(s[0]=='\n') break;
      value=searchvalue(s);
      // set the value in apropriet field
      word=strupr(strtok(s," "));
           if(!strcmp(word,"MAPTHINGNUM"))           mobjinfo[num].doomednum   =value;
      else if(!strcmp(word,"SPAWNSTATE"))      mobjinfo[num].spawnstate  =value;
      else if(!strcmp(word,"SPAWNHEALTH"))          mobjinfo[num].spawnhealth =value;
      else if(!strcmp(word,"SEESTATE"))        mobjinfo[num].seestate    =value;
      else if(!strcmp(word,"SEESOUND"))        mobjinfo[num].seesound    =value;
      else if(!strcmp(word,"REACTIONTIME"))     mobjinfo[num].reactiontime=value;
      else if(!strcmp(word,"ATTACKSOUND"))       mobjinfo[num].attacksound =value;
      else if(!strcmp(word,"PAINSTATE"))       mobjinfo[num].painstate   =value;
      else if(!strcmp(word,"PAINCHANCE"))      mobjinfo[num].painchance  =value;
      else if(!strcmp(word,"PAINSOUND"))      mobjinfo[num].painsound  =value;
      else if(!strcmp(word,"MELEESTATE"))        mobjinfo[num].meleestate  =value;
      else if(!strcmp(word,"MISSILESTATE"))          mobjinfo[num].missilestate=value;
      else if(!strcmp(word,"DEATHSTATE"))        mobjinfo[num].deathstate  =value;
	  else if(!strcmp(word,"DEATHSOUND"))        mobjinfo[num].deathsound  =value;
      else if(!strcmp(word,"XDEATHSTATE"))    mobjinfo[num].xdeathstate =value;
      else if(!strcmp(word,"SPEED"))        mobjinfo[num].speed       =value;
      else if(!strcmp(word,"FRACSPEED")
	&& ver == 108)        mobjinfo[num].speed       =value*FRACUNIT;
      else if(!strcmp(word,"RADIUS")
	&& ver != 108)        mobjinfo[num].radius      =value;
      else if(!strcmp(word,"RADIUS")
	&& ver == 108)        mobjinfo[num].radius      =value*FRACUNIT;
      else if(!strcmp(word,"HEIGHT")
	&& ver != 108)       mobjinfo[num].height      =value;
      else if(!strcmp(word,"HEIGHT")
	&& ver == 108)       mobjinfo[num].height      =value*FRACUNIT;
      else if(!strcmp(word,"MASS"))         mobjinfo[num].mass        =value;
      else if(!strcmp(word,"DAMAGE"))      mobjinfo[num].damage      =value;
      else if(!strcmp(word,"ACTIVESOUND"))       mobjinfo[num].activesound =value;
      else if(!strcmp(word,"FLAGS"))         mobjinfo[num].flags       =value;
      else if(!strcmp(word,"RAISESTATE"))      mobjinfo[num].raisestate  =value;
      /*else if(!strcmp(word,"COLOR")
	&& ver == 108)       mobjinfo[num].seesound   =value;*/
      else if(!strcmp(word,"FLEESTATE")
	&& ver == 108)    mobjinfo[num].xdeathstate =value;
      /*else if(!strcmp(word,"JUMPSOUND")
	&& ver == 108)       mobjinfo[num].activesound =value;
      else if(!strcmp(word,"MISSILE")
	&& ver == 108)         mobjinfo[num].mass        =value;
      else if(!strcmp(word,"THOK")
	&& ver == 108)      mobjinfo[num].painchance  =value;*/
      else deh_error("Thing %d : unknown word '%s'\n",num,word);
    }
  } while(s[0]!='\n' && !myfeof(f)); //finish when the line is empty
}

#include "hardware/hw_light.h"
static void readlight(MYFILE *f,int num)
{
  char s[MAXLINELEN];
  char *word;
  int value;

  do{
    if(myfgets(s,sizeof(s),f)!=NULL)
    {
      if(s[0]=='\n') break;
      value=searchvalue(s);
      // set the value in apropriet field
      word=strupr(strtok(s," "));
           if(!strcmp(word,"TYPE"))         lspr[num].type =value;
      else if(!strcmp(word,"OFFSETX"))      lspr[num].light_xoffset =value;
      else if(!strcmp(word,"OFFSETY"))      lspr[num].light_yoffset =value;
	  else if(!strcmp(word,"CORONACOLOR"))      lspr[num].corona_color =value;
	  else if(!strcmp(word,"CORONARADIUS"))      lspr[num].corona_radius =value;
	  else if(!strcmp(word,"DYNAMICCOLOR"))      lspr[num].dynamic_color =value;
	  else if(!strcmp(word,"DYNAMICRADIUS"))
	  {
		  lspr[num].dynamic_radius =value;

		  // Update the sqrradius! Is this really needed? *shrug*
		  lspr[num].dynamic_sqrradius = lspr[num].dynamic_radius*lspr[num].dynamic_radius;
	  }
      else deh_error("Light %d : unknown word '%s'\n",num,word);
    }
  } while(s[0]!='\n' && !myfeof(f)); //finish when the line is empty
}

static void readspritelight(MYFILE *f,int num)
{
  char s[MAXLINELEN];
  char *word;
  int value;

  do{
    if(myfgets(s,sizeof(s),f)!=NULL)
    {
      if(s[0]=='\n') break;
      value=searchvalue(s);
      // set the value in apropriet field
      word=strupr(strtok(s," "));
           if(!strcmp(word,"LIGHTTYPE")) t_lspr[num] = &lspr[value];
      else deh_error("Sprite %d : unknown word '%s'\n",num,word);
    }
  } while(s[0]!='\n' && !myfeof(f)); //finish when the line is empty
}

static void readlevelheader(MYFILE *f,int num)
{
  char s[MAXLINELEN];
  char *word,*word2,*tmp;
  int i;

  do{
    if(myfgets(s,sizeof(s),f)!=NULL)
    {
      if(s[0]=='\n') break;

	  // set the value in appropriate field (...whatever THAT means! flippant remark added by Graue 11-16-2003)

	  // First remove trailing newline, if there is one Graue 11-16-2003
	  if((tmp = strchr(s, '\n')) != NULL)
		  *tmp = '\0';

	  // Get the part before the " = "
	  tmp = strchr(s, '=');
	  *(tmp-1) = '\0';
	  word=strupr(s);

	  // Now get the part after
	  tmp+=2;
	  word2=strupr(tmp);

	  i = atoi(word2); // used for numerical settings

           if(!strcmp(word,"LEVELNAME"))           strncpy(mapheaderinfo[num-1].lvlttl, word2, 33);
      else if(!strcmp(word,"INTERSCREEN"))      strncpy(mapheaderinfo[num-1].interscreen, word2, 9);
      else if(!strcmp(word,"ACT"))          mapheaderinfo[num-1].actnum = i;
	  else if(!strcmp(word,"NOZONE"))   mapheaderinfo[num-1].nozone = i; // Tails 12-21-2003
      else if(!strcmp(word,"TYPEOFLEVEL"))        mapheaderinfo[num-1].typeoflevel    =i;
      else if(!strcmp(word,"NEXTLEVEL"))        mapheaderinfo[num-1].nextlevel    =i;
      else if(!strcmp(word,"MUSICSLOT"))     mapheaderinfo[num-1].musicslot=i;
      else if(!strcmp(word,"FORCECHARACTER"))       mapheaderinfo[num-1].forcecharacter =i;
      else if(!strcmp(word,"WEATHER"))       mapheaderinfo[num-1].weather =i;
      else if(!strcmp(word,"SKYNUM"))      mapheaderinfo[num-1].skynum=i;
      #ifdef SPMOVES
      else if(!strcmp(word,"SKY2NUM"))      mapheaderinfo[num-1].sky2num=i;
      else if(!strcmp(word,"SKY3NUM"))      mapheaderinfo[num-1].sky3num=i;
      else if(!strcmp(word,"SKY4NUM"))      mapheaderinfo[num-1].sky4num=i;
      else if(!strcmp(word,"SKY5NUM"))      mapheaderinfo[num-1].sky5num=i;
      else if(!strcmp(word,"SKY6NUM"))      mapheaderinfo[num-1].sky6num=i;
      else if(!strcmp(word,"SKY7NUM"))      mapheaderinfo[num-1].sky7num=i;
      else if(!strcmp(word,"SKY8NUM"))      mapheaderinfo[num-1].sky8num=i;
      else if(!strcmp(word,"SKY9NUM"))      mapheaderinfo[num-1].sky9num=i;
      else if(!strcmp(word,"QUESTTYPE"))    mapheaderinfo[num-1].questtype=i;
      #endif
      else if(!strcmp(word,"SCRIPTNAME"))      strncpy(mapheaderinfo[num-1].scriptname, word2, 255);
	  else if(!strcmp(word,"SCRIPTISLUMP"))  mapheaderinfo[num-1].scriptislump = i;
      else if(!strcmp(word,"CUTSCENENUM"))        mapheaderinfo[num-1].cutscenenum  =i;
      else if(!strcmp(word,"COUNTDOWN"))          mapheaderinfo[num-1].countdown=i;
      else deh_error("Level Header %d : unknown word '%s'\n",num,word);
    }
  } while(s[0]!='\n' && !myfeof(f)); //finish when the line is empty
}

static void readcutscenescene(MYFILE *f,int num, int scenenum)
{
  char s[MAXLINELEN];
  char *word,*word2;
  int i;

  do{
    if(myfgets(s,sizeof(s),f)!=NULL)
    {
      if(s[0]=='\n') break;
      // set the value in apropriet field
      word=strupr(strtok(s," "));

      if(!strcmp(word,"SCENETEXT"))
	  {
		  char *scenetext;
		  for(i=0; i<MAXLINELEN; i++)
		  {
			  if(s[i] == '=')
			  {
				  scenetext = &s[i+2];
				  break;
			  }
		  }
		  for(i=0; i<MAXLINELEN; i++)
		  {
			  if(s[i] == '\0')
			  {
				  s[i] = '\n';
				  s[i+1] = '\0';
				  break;
			  }
		  }

		  strcpy(cutscenes[num].scene[scenenum].text, scenetext);
		  strcat(cutscenes[num].scene[scenenum].text, myfgetsforscenetext(scenetext, sizeof(cutscenes[num].scene[scenenum].text),f));
		  continue;
	  }

	  word2=strupr(strtok(NULL," = "));

	  word2[strlen(word2)-1]='\0';

	  i = atoi(word2);

           if(!strcmp(word,"NUMBEROFPICS"))     cutscenes[num].scene[scenenum].numpics   =i;
      else if(!strcmp(word,"PIC1NAME"))			strncpy(cutscenes[num].scene[scenenum].picname[0], word2, 9);
      else if(!strcmp(word,"PIC2NAME"))			strncpy(cutscenes[num].scene[scenenum].picname[1], word2, 9);
      else if(!strcmp(word,"PIC3NAME"))			strncpy(cutscenes[num].scene[scenenum].picname[2], word2, 9);
      else if(!strcmp(word,"PIC4NAME"))			strncpy(cutscenes[num].scene[scenenum].picname[3], word2, 9);
      else if(!strcmp(word,"PIC5NAME"))			strncpy(cutscenes[num].scene[scenenum].picname[4], word2, 9);
      else if(!strcmp(word,"PIC6NAME"))			strncpy(cutscenes[num].scene[scenenum].picname[5], word2, 9);
      else if(!strcmp(word,"PIC7NAME"))			strncpy(cutscenes[num].scene[scenenum].picname[6], word2, 9);
      else if(!strcmp(word,"PIC8NAME"))			strncpy(cutscenes[num].scene[scenenum].picname[7], word2, 9);
      else if(!strcmp(word,"PIC1HIRES"))		cutscenes[num].scene[scenenum].pichires[0]  =i;
      else if(!strcmp(word,"PIC2HIRES"))		cutscenes[num].scene[scenenum].pichires[1]  =i;
      else if(!strcmp(word,"PIC3HIRES"))		cutscenes[num].scene[scenenum].pichires[2]  =i;
      else if(!strcmp(word,"PIC4HIRES"))		cutscenes[num].scene[scenenum].pichires[3]  =i;
      else if(!strcmp(word,"PIC5HIRES"))		cutscenes[num].scene[scenenum].pichires[4]  =i;
      else if(!strcmp(word,"PIC6HIRES"))		cutscenes[num].scene[scenenum].pichires[5]  =i;
      else if(!strcmp(word,"PIC7HIRES"))		cutscenes[num].scene[scenenum].pichires[6]  =i;
      else if(!strcmp(word,"PIC8HIRES"))		cutscenes[num].scene[scenenum].pichires[7]  =i;
      else if(!strcmp(word,"PIC1DURATION"))		cutscenes[num].scene[scenenum].picduration[0]  =i;
      else if(!strcmp(word,"PIC2DURATION"))     cutscenes[num].scene[scenenum].picduration[1]  =i;
	  else if(!strcmp(word,"PIC3DURATION"))     cutscenes[num].scene[scenenum].picduration[2]  =i;
	  else if(!strcmp(word,"PIC4DURATION"))     cutscenes[num].scene[scenenum].picduration[3]  =i;
	  else if(!strcmp(word,"PIC5DURATION"))     cutscenes[num].scene[scenenum].picduration[4]  =i;
	  else if(!strcmp(word,"PIC6DURATION"))     cutscenes[num].scene[scenenum].picduration[5]  =i;
	  else if(!strcmp(word,"PIC7DURATION"))     cutscenes[num].scene[scenenum].picduration[6]  =i;
	  else if(!strcmp(word,"PIC8DURATION"))     cutscenes[num].scene[scenenum].picduration[7]  =i;
      else if(!strcmp(word,"PIC1XCOORD"))		cutscenes[num].scene[scenenum].xcoord[0]  =i;
      else if(!strcmp(word,"PIC2XCOORD"))       cutscenes[num].scene[scenenum].xcoord[1]  =i;
	  else if(!strcmp(word,"PIC3XCOORD"))       cutscenes[num].scene[scenenum].xcoord[2]  =i;
	  else if(!strcmp(word,"PIC4XCOORD"))       cutscenes[num].scene[scenenum].xcoord[3]  =i;
	  else if(!strcmp(word,"PIC5XCOORD"))       cutscenes[num].scene[scenenum].xcoord[4]  =i;
	  else if(!strcmp(word,"PIC6XCOORD"))       cutscenes[num].scene[scenenum].xcoord[5]  =i;
	  else if(!strcmp(word,"PIC7XCOORD"))       cutscenes[num].scene[scenenum].xcoord[6]  =i;
	  else if(!strcmp(word,"PIC8XCOORD"))       cutscenes[num].scene[scenenum].xcoord[7]  =i;
      else if(!strcmp(word,"PIC1YCOORD"))		cutscenes[num].scene[scenenum].ycoord[0]  =i;
      else if(!strcmp(word,"PIC2YCOORD"))       cutscenes[num].scene[scenenum].ycoord[1]  =i;
	  else if(!strcmp(word,"PIC3YCOORD"))       cutscenes[num].scene[scenenum].ycoord[2]  =i;
	  else if(!strcmp(word,"PIC4YCOORD"))       cutscenes[num].scene[scenenum].ycoord[3]  =i;
	  else if(!strcmp(word,"PIC5YCOORD"))       cutscenes[num].scene[scenenum].ycoord[4]  =i;
	  else if(!strcmp(word,"PIC6YCOORD"))       cutscenes[num].scene[scenenum].ycoord[5]  =i;
	  else if(!strcmp(word,"PIC7YCOORD"))       cutscenes[num].scene[scenenum].ycoord[6]  =i;
	  else if(!strcmp(word,"PIC8YCOORD"))       cutscenes[num].scene[scenenum].ycoord[7]  =i;
      else if(!strcmp(word,"MUSICSLOT"))		cutscenes[num].scene[scenenum].musicslot = i;
      else if(!strcmp(word,"TEXTXPOS"))			cutscenes[num].scene[scenenum].textxpos = i;
      else if(!strcmp(word,"TEXTYPOS"))			cutscenes[num].scene[scenenum].textypos = i;

      else deh_error("CutSceneScene %d : unknown word '%s'\n",num,word);
    }
  } while(s[0]!='\n' && !myfeof(f)); //finish when the line is empty
}

static void readcutscene(MYFILE *f,int num)
{
  char s[MAXLINELEN];
  char *word,*word2;
  int i;

  do{
    if(myfgets(s,sizeof(s),f)!=NULL)
    {
      if(s[0]=='\n') break;
      // set the value in apropriet field
      word=strupr(strtok(s," "));
	  word2=strupr(strtok(NULL," "));

	  i = atoi(word2);

      if(!strcmp(word,"NUMSCENES"))			cutscenes[num].numscenes = i;
	  else if(!strcmp(word, "SCENE"))
	  {
		if(i > 0 && i < 129)
		  readcutscenescene(f, num, i-1);
		else
		  deh_error("Scene number %d out of range\n", i);
		}
      else deh_error("Cutscene %d : unknown word '%s', Scene <num> expected.\n",num,word);
    }
  } while(s[0]!='\n' && !myfeof(f)); //finish when the line is empty
}

/*
Sprite number = 10
Sprite subnumber = 32968
Duration = 200
Next frame = 200
*/

typedef struct
{
	actionf_t action;
	char *name;
} actionpointer_t;

actionpointer_t actionpointers[] =
{
  {{A_Explode},              "A_EXPLODE"},
  {{A_Pain},                 "A_PAIN"},
  {{A_Fall},                 "A_FALL"},
  {{A_MonitorPop},           "A_MONITORPOP"},
  {{A_Look},                 "A_LOOK"},
  {{A_Chase},                "A_CHASE"},
  {{A_FaceTarget},           "A_FACETARGET"},
  {{A_Scream},               "A_SCREAM"},
  {{A_BossDeath},            "A_BOSSDEATH"},
  {{A_CustomPower},          "A_CUSTOMPOWER"},
  {{A_BlueShield},           "A_BLUESHIELD"},
  {{A_YellowShield},         "A_YELLOWSHIELD"},
  {{A_RingBox},              "A_RINGBOX"},
  {{A_Invincibility},        "A_INVINCIBILITY"},
  {{A_SuperSneakers},        "A_SUPERSNEAKERS"},
  {{A_BunnyHop},             "A_BUNNYHOP"},
  {{A_BubbleSpawn},          "A_BUBBLESPAWN"},
  {{A_BubbleRise},           "A_BUBBLERISE"},
  {{A_ExtraLife},            "A_EXTRALIFE"},
  {{A_BlackShield},          "A_BLACKSHIELD"},
  {{A_GreenShield},          "A_GREENSHIELD"},
  {{A_RedShield},            "A_REDSHIELD"},
  {{A_ScoreRise},            "A_SCORERISE"},
  {{A_AttractChase},         "A_ATTRACTCHASE"},
  {{A_DropMine},             "A_DROPMINE"},
  {{A_FishJump},             "A_FISHJUMP"},
  {{A_SignPlayer},           "A_SIGNPLAYER"},
  {{A_ThrownRing},           "A_THROWNRING"},
  {{A_SetSolidSteam},        "A_SETSOLIDSTEAM"},
  {{A_UnsetSolidSteam},      "A_UNSETSOLIDSTEAM"},
  {{A_JetChase},             "A_JETCHASE"},
  {{A_JetbThink},            "A_JETBTHINK"},
  {{A_JetgThink},            "A_JETGTHINK"},
  {{A_JetgShoot},            "A_JETGSHOOT"},
  {{A_MouseThink},           "A_MOUSETHINK"},
  {{A_DetonChase},           "A_DETONCHASE"},
  {{A_CapeChase},            "A_CAPECHASE"},
  {{A_RotateSpikeBall},      "A_ROTATESPIKEBALL"},
  {{A_SnowBall},             "A_SNOWBALL"},
  {{A_CrawlaCommanderThink}, "A_CRAWLACOMMANDERTHINK"},
  {{A_SmokeTrailer},         "A_SMOKETRAILER"},
  {{A_RingExplode},          "A_RINGEXPLODE"},
  {{A_MixUp},                "A_MIXUP"},
  {{A_BossScream},           "A_BOSSSCREAM"},
  {{A_Invinciblerize},       "A_INVINCIBLERIZE"},
  {{A_DeInvinciblerize},     "A_DEINVINCIBLERIZE"},
  {{A_Boss2PogoSFX},         "A_BOSS2POGOSFX"},
  {{A_EggmanBox},            "A_EGGMANBOX"},
  {{A_TurretFire},           "A_TURRETFIRE"},
  {{A_TurretStop},           "A_TURRETSTOP"},
  {{A_SkimChase},            "A_SKIMCHASE"},
  {{A_PumaJump},             "A_PUMAJUMP"},
  {{A_SkullAttack},          "A_SKULLATTACK"},
  {{A_CyberAttack},          "A_CYBERATTACK"},
  {{A_SparkFollow},          "A_SPARKFOLLOW"},
#ifdef SPMOVES
  {{A_CheckFlee},            "A_CHECKFLEE"},
  {{A_BirdFlap},             "A_BIRDFLAP"},
  {{A_BirdFall},             "A_BIRDFALL"},
  {{A_SPMovesChao},          "A_SPMOVESCHAO"},
#endif

  // This NULL entry must be the last in the list
  {{NULL},             NULL},  // Ty 05/16/98
};

static void readframe(MYFILE* f,int num)
{
  char s[MAXLINELEN];
  char *word1,*word2;
  int i;
  int j;
  do{
    if(myfgets(s,sizeof(s),f)!=NULL)
    {
      if(s[0]=='\n') break;

	  for(j=0; s[j] != '\n'; j++)
	  {
		  if(s[j] == '=')
		  {
			  j++;
			  j++;
			  j = atoi(&s[j]);
			  break;
		  }
	  }
      // set the value in apropriet field
      word1=strupr(strtok(s," "));

	  word2=strupr(strtok(NULL," = "));

	  word2[strlen(word2)-1]='\0';

	  i = atoi(word2);

	  if(!strcmp(word1,"SPRITENUMBER")) states[num].sprite = i;
	  else if(!strcmp(word1,"SPRITESUBNUMBER")) states[num].frame = i;
      else if(!strcmp(word1,"DURATION"))     states[num].tics     =i;
      else if(!strcmp(word1,"NEXT"))         states[num].nextstate=i;
	  else if(!strcmp(word1,"ACTION"))
	  {
		  unsigned int z;
		  boolean found = false;
		  char actiontocompare[32];

		  strncpy(actiontocompare, word2, 31);

		  for(z=0; z<32; z++)
		  {
			  if(actiontocompare[z] == '\n' || actiontocompare[z] == '\r')
			  {
				actiontocompare[z] = '\0';
				break;
			  }
		  }

		  z = 0;

		  while (actionpointers[z].name != NULL)
		  {
				if(!strcmp(strupr(actiontocompare), actionpointers[z].name))
				{
					 states[num].action = actionpointers[z].action;
					 states[num].action.acv = actionpointers[z].action.acv; // assign
					 states[num].action.acp1 = actionpointers[z].action.acp1;
					 found = true;
					 break;
				}
				z++;
		  }

		  if(!found)
			  deh_error("Action %s, unknown action\n", actiontocompare);
	  }
      else deh_error("Frame %d : unknown word '%s'\n",num,word1);
    }
  } while(s[0]!='\n' && !myfeof(f));
}

static void readsound(MYFILE* f,int num,char *savesfxnames[])
{
  char s[MAXLINELEN];
  char *word;
  int value;

  do{
    if(myfgets(s,sizeof(s),f)!=NULL)
    {
      if(s[0]=='\n') break;
      value=searchvalue(s);
      word=strupr(strtok(s," "));
           if(!strcmp(word,"OFFSET"))   {
                                          value-=150360;
                                          if(value<=64) value/=8;
                                          else if(value<=260) value=(value+4)/8;
                                          else value=(value+8)/8;
                                          if(value>=-1 && value<sfx_freeslot0-1)
                                              S_sfx[num].name=savesfxnames[value+1];
                                          else
                                              deh_error("Sound %d : offset out of bound\n",num);
                                        }
      else if(!strcmp(word,"ZERO/ONE")) S_sfx[num].singularity=value;
      else if(!strcmp(word,"VALUE"))    S_sfx[num].priority   =value;
	  else if(!strcmp(word,"PITCH"))    S_sfx[num].pitch = value;
      else deh_error("Sound %d : unknown word '%s'\n",num,word);
    }
  } while(s[0]!='\n' && !myfeof(f));
}

static void readmaincfg(MYFILE *f)
{
  char s[MAXLINELEN];
  char *word,*word2;
  int value;
  do{
    if(myfgets(s,sizeof(s),f)!=NULL)
    {
      if(s[0]=='\n') break;
      value=searchvalue(s);
      word=strupr(strtok(s," "));
      word2=strupr(strtok(NULL," "));

      if(!strcmp(word,"SPSTAGE_START")) spstage_start = value;
      else if(!strcmp(word,"SSTAGE_START"))
	  {
		  sstage_start = value;
		  sstage_end = sstage_start+7;
	  }
      else if(!strcmp(word,"EXECCFG"))        COM_BufAddText (va("exec %s\n",word2));
      else if(!strcmp(word,"INVULNTICS"))          invulntics=value;
      else if(!strcmp(word,"SNEAKERTICS"))    sneakertics=value;
      else if(!strcmp(word,"FLASHINGTICS"))    flashingtics=value;
      else if(!strcmp(word,"TAILSFLYTICS"))      tailsflytics=value;
      else if(!strcmp(word,"UNDERWATERTICS"))    underwatertics=value;
      else if(!strcmp(word,"SPACETIMETICS"))    spacetimetics=value;
      else if(!strcmp(word,"EXTRALIFETICS"))      extralifetics=value;
      else if(!strcmp(word,"PARALOOPTICS"))      paralooptics=value; // Tails 12-15-2003
      else if(!strcmp(word,"HELPERTICS"))      helpertics=value; // Tails 12-15-2003
      else if(!strcmp(word,"INTROTOPLAY"))      introtoplay=value;

      else deh_error("Maincfg : unknown word '%s'\n",word);
    }
  } while(s[0]!='\n' && !myfeof(f));
}

#ifdef SPMOVES
static void readpsoitem(MYFILE* f,int num)
{
  char s[MAXLINELEN];
  char *word1,*word2;
  int i;
  int j;

  do{
    if(myfgets(s,sizeof(s),f)!=NULL)
    {
      if(s[0]=='\n') break;

	  for(j=0; s[j] != '\n'; j++)
	  {
		  if(s[j] == '=')
		  {
			  j++;
			  j++;
			  j = atoi(&s[j]);
			  break;
		  }
	  }
      // set the value in apropriet field
      word1=strupr(strtok(s," "));

	  word2=strupr(strtok(NULL," = "));

	  word2[strlen(word2)-1]='\0';

	  i = atoi(word2);

	  if(!strcmp(word1,"NAME")) strncpy(&psoitems[num].name[0], word2, 128);
	  else if(!strcmp(word1,"TYPE")) psoitems[num].type = i;
	  else if(!strcmp(word1,"BULLET")) psoitems[num].firetype = i;
	  else if(!strcmp(word1,"PLAYERTRACER")) psoitems[num].playertracer = i;
	  else if(!strcmp(word1,"DAMAGE")) psoitems[num].damage = i;
	  else if(!strcmp(word1,"BUYPRICE")) psoitems[num].buyprice = i;
	  else if(!strcmp(word1,"SELLPRICE")) psoitems[num].sellprice = i;
	  else if(!strcmp(word1,"RARITY")) psoitems[num].rarity = i;
	  else if(!strcmp(word1,"SOUND")) psoitems[num].usesound = i;
	  else if(!strcmp(word1,"FLAGS")) psoitems[num].flags = i;
      else deh_error("PSO item %d : unknown word '%s'\n",num,word1);
    }
  } while(s[0]!='\n' && !myfeof(f));
}
#endif

void DEH_LoadDehackedFile(MYFILE* f)
{
  
  char       s[1000];
  char       *word,*word2;
  int        i;
  int        maincfgver;
  // do a copy of this for cross references probleme
  actionf_t  saveactions[NUMSTATES];
  char       *savesprnames[NUMSPRITES];
  char       *savesfxnames[NUMSFX];

  deh_num_error=0;
  // save value for cross reference
  for(i=0;i<NUMSTATES;i++)
      saveactions[i]=states[i].action;
  for(i=0;i<NUMSPRITES;i++)
      savesprnames[i]=sprnames[i];
  for(i=0;i<NUMSFX;i++)
      savesfxnames[i]=S_sfx[i].name;

  // it don't test the version of doom
  // and version of dehacked file
  while(!myfeof(f))
  {
	  char origpos[32];
	  int size = 0;
	  char* traverse;

    myfgets(s,sizeof(s),f);
    if(s[0]=='\n' || s[0]=='#')
      continue;

	traverse = s;

	while(traverse[0] != '\n')
	{
		traverse++;
		size++;
	}

	strncpy(origpos, s, size);
	origpos[size] = '\0';

    word=strupr(strtok(s," "));
    if(word!=NULL)
    {
      if((word2=strupr(strtok(NULL," ")))!=NULL)
      {
        i=atoi(word2);

        if(!strcmp(word,"THING"))
        {
//          i--; // begin at 0 not 1; // Yeah, right! Just confuse the dookie out of people!

          if(i<NUMMOBJTYPES && i>=0)
            readthing(f,i,maincfgver);
          else
            deh_error("Thing %d doesn't exist\n",i);
        }
		else if(!strcmp(word, "BY"))
		{
		    #ifdef SPMOVES
		    i = 19;
		    #else
		    i = 18;
		    #endif
		    char *name; // Only 30 letters fit...
		    name = strupr(strtok(s," "));
			strcpy(credits[i].fakenames[0], name);
			strcpy(credits[i].realnames[0], name);
			credits[i].numnames = 1;
			strcpy(&credits[i].header[0],"Modification by:\n");
			modcredits = true;
		}
		else if(!strcmp(word, "CHARACTER"))
		{
			if(i<16)
				readCharacter(f, i);
			else
				deh_error("Character %d out of range\n", i);
		}
		else if(!strcmp(word, "LIGHT"))
		{
			if(i > 0 && i < NUMLIGHTS)
				readlight(f, i);
			else
				deh_error("Light number %d out of range\n", i);
		}
		else if(!strcmp(word, "SPRITE"))
		{
			if(i < NUMSPRITES)
				readspritelight(f, i);
			else
				deh_error("Sprite number %d out of range\n", i);
		}
		else if(!strcmp(word, "LEVEL"))
		{
			if(i > 0 && i <= NUMMAPS) // NO there are more than 99 maps now Graue 12-04-2003
				readlevelheader(f, i);
			else
				deh_error("Level number %d out of range\n", i);
		}
		else if(!strcmp(word, "CUTSCENE"))
		{
			if(i > 0 && i < 129)
				readcutscene(f, i-1);
			else
				deh_error("Cutscene number %d out of range\n", i);
		}
        else if(!strcmp(word,"FRAME"))
             {
               if(i<NUMSTATES && i>=0)
                  readframe(f,i);
               else
                  deh_error("Frame %d does't exist\n",i);
             }
        else if(!strcmp(word,"POINTER"))
             {
               word=strtok(NULL," "); // get frame
               if((word=strtok(NULL,")"))!=NULL)
               {
                 i=atoi(word);
                 if(i<NUMSTATES && i>=0)
                 {
                   if(myfgets(s,sizeof(s),f)!=NULL)
                     states[i].action=saveactions[searchvalue(s)];
                 }
                 else
                    deh_error("Pointer : Frame %d doesn't exist\n",i);
               }
               else
                   deh_error("pointer (Frame %d) : missing ')'\n",i);
             }
        else if(!strcmp(word,"SOUND"))
             {
               if(i<NUMSFX && i>=0)
                   readsound(f,i,savesfxnames);
               else
                   deh_error("Sound %d doesn't exist\n");
             }
        else if(!strcmp(word,"SPRITE"))
             {
               if(i<NUMSPRITES && i>=0)
               {
                 if(myfgets(s,sizeof(s),f)!=NULL)
                 {
                   int k;
                   k=(searchvalue(s)-151328)/8;
                   if(k>=0 && k<NUMSPRITES)
                       sprnames[i]=savesprnames[k];
                   else
                       deh_error("Sprite %i : offset out of bound\n",i);
                 }
               }
               else
                  deh_error("Sprite %d doesn't exist\n",i);
             }
             #ifdef SPMOVES
        else if(!strcmp(word,"PSOITEM"))
             {
               if(i<NUMPSOITEMS && i>=0)
                  readpsoitem(f,i);
               else
                  deh_error("PSO item %d doesn't exist\n",i);
             }
        #ifdef SPMBETA
        else if(!strcmp(word,"CUSTACTION"))
             {
               if(i <= NUMCUSTACTIONS && i >= 0)
                  JTE_ReadAction(f, i);
               else
                  deh_error("CUSTACTION %d is out of range.\n",i);
             }
             #endif
             #endif
	else if(!strcmp(word,"MAINCFG"))
		readmaincfg(f);
        else if(!strcmp(word,"SRB2"))
             {
               maincfgver = searchvalue(strtok(NULL,"\n"));
               if(maincfgver != 11
               #ifdef SPMOVES
               && maincfgver != 108
               #endif
               )
                  #ifdef SPMOVES
                  deh_error("Warning : patch from a different SRB2 version (%d), only verions 1.1 and 1.08 (SPMoves) are supported\n",maincfgver);
                  #else
                  deh_error("Warning : patch from a different SRB2 version (%d), only verion 1.1 is supported\n",maincfgver);
                  #endif
             }
        else deh_error("Unknown word : %s\n",word);
      }
      else
          deh_error("missing argument for '%s'\n",word);
    }
    else
        deh_error("No word in this line:\n%s\n",s);

  } // end while
  if (deh_num_error>0)
  {
      CONS_Printf("%d warning(s) in the dehacked file\n",deh_num_error);
      if (devparm)
          getchar();
  }

  deh_loaded = true;
}

// read dehacked lump in a wad (there is special trick for for deh 
// file that are converted to wad in w_wad.c)
void DEH_LoadDehackedLump(int lump)
{
    MYFILE f;

    f.size = W_LumpLength(lump);
    f.data = Z_Malloc(f.size + 1, PU_STATIC, 0);
    W_ReadLump(lump, f.data);
    f.curpos = f.data;
    f.data[f.size] = 0;

    DEH_LoadDehackedFile(&f);
    Z_Free(f.data);
}
