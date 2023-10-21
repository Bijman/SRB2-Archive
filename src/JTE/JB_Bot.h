// JB - JTE Bots
// By Jason the Echidna
// MINE! NO STEALY!

#ifdef JTEBOTS
#ifndef __JB_BOT__
#define __JB_BOT__
#include "../r_defs.h"

typedef struct botdontlook_s
{
	mobj_t *data;
	unsigned short timer;
	struct botdontlook_s *next;
	struct botdontlook_s *last;
} botdontlook_t;
#ifdef SUCKYBOTWAYPOINTS
typedef struct botwaypoint_s
{
	fixed_t x,y,z;
	sector_t *sec;
	boolean springpoint;
	struct botwaypoint_s *next;
} botwaypoint_t;
#endif
typedef struct bot_s
{
	player_t* player; // Your player struct
	byte ownernum; // Your owner's number
	boolean springmove; // If you hit a diagonal spring or not
	mobj_t* target; // The mobj you're following
#ifdef SUCKYBOTWAYPOINTS
	botwaypoint_t *waypoint; // Your waypoint in race
	fixed_t waydist; // Distance from the last waypoint to the next one
#endif
	unsigned short targettimer; // How long you've been trying to get to the same mobj
	botdontlook_t *targetlist; // Don't look at these mobjs again until timer runs out.
} bot_t;

extern bot_t bots[MAXPLAYERS];
extern char charselbots[15][16];

void JB_BotWaitAdd(int skin);
void JB_AddWaitingBots(int playernum);
boolean JB_BotAdd(byte skin, int playernum, byte color, char *name);
void JB_Got_BotAdd(char** p, int playernum);
void JB_SpawnBot(int botnum);

#endif
#endif
