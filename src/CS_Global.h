#include "Networking.h"

#include "doomdef.h"
#include "p_mobj.h"
#include "p_tick.h"
#include "d_player.h" // powertype_t
#include "i_system.h" // for I_GetTime
#include "m_random.h"
#include "r_main.h"
#include "s_sound.h"
#include "hu_stuff.h"

#define CSMAXPLAYERS 12

/// ServerPacket
typedef struct
{
    UINT8 id;

    INT16 map;
    INT16 gametype;
    tic_t level_time; // Level time (approximate).

    INT8 mapswitches;

    int pointlimit;
    int timelimit;
    INT16 itemrespawntime;
    INT8 matchtype;

    // MATCH/CTF STUFF
    UINT16 red_score; // Kill me for using UINT16 instead of UINT32 - but we have to be conservative! Who wants 65,535 flags anyway?
    UINT16 blue_score;

    UINT16 flag_state;  // The state of the red and blue flag. Low bits red, high bits blue.

    INT8 emeralds[7];

    // RACE STUFF
    INT8 finished_count; // Amount of players who have finished the race.
    INT8 race_rank[CSMAXPLAYERS]; // Table of winners (race_rank[0] won).
} ServerPacket;

#pragma pack(push) // Note: This may have the side-effect of making this mod Windows-exclusive.
#pragma pack(1)

/// Player update messages. (PlayerUpdate)
typedef struct
{
    UINT8 id;

    UINT8 player_id;

    UINT8 type; // Type of player update. Can be different between gametypes, but not equal to gametype itself.

    UINT8 powers;

    INT8 player_times[CSMAXPLAYERS]; // This should be better.
    tic_t my_time; // Our time. We kind of need this to use as a basis for player_times.

    fixed_t x, y, z;

    angle_t angle;
    angle_t aiming;

    UINT8 state; // Hopefully we can fit the state into a single byte. Unless people want to start playing as objects...

    boolean throwing_ring;
    INT32 current_weapon;
    fixed_t toss_strength;

    UINT32 event_list[5];
    UINT16 event_count;
} PlayerUpdate;

/// PlayerDataUpdate
typedef struct
{
    UINT8 id;
    UINT8 player_id;

    struct sockaddr address; // For clienttoclient.
    UINT16 purportedport; // Purported port. For odd router scenarios.

    UINT8 skin;
    UINT8 colour; // I'm British. So sue me!
    UINT8 team;

    UINT32 score; // Only for the host to send.
    UINT8 laps;

    UINT16 real_time;
    boolean permadead; // Race-only for now.

    UINT32 player_links;

    char name[MAXPLAYERNAME];
} PlayerDataUpdate;

/// SayMessage
typedef struct
{
    UINT8 id;
    UINT8 type;
    UINT8 message_id; // Used to re-send messages that didn't make it.

    UINT8 player_id;

    char message[256];
} SayMessage;


/// PlayerIntro message (sent by the host)
typedef struct
{
    boolean in_game;

    struct sockaddr address; // For clienttoclient.

    UINT32 score;

    UINT16 event_count;
} PlayerIntro;

/// IntroPacket
typedef struct
{
    UINT8 id;

    UINT8 player_id; // Your brand new ID number!

    UINT8 saymessage_id; // To stop the player from receiving the last message that was sent.

    INT8 wad_count; // Amount of WADs added.
    UINT8 wad_md5[10][16]; // MD5 sum of all WADs added.
    char  wad_name[10][20];

    ServerPacket sp; // A server packet - this lets them know the current stage, etc.
    PlayerIntro pi[CSMAXPLAYERS]; // "Intro" of every player. This includes certain data essential for instant sync.
} IntroPacket;

typedef struct
{
    UINT8 id;

    char reason[128];
} RefusePacket;

#pragma pack(pop)

#define UPDHISTORY 35

/// LastPlayerStuff
typedef struct
{
    char name[MAXPLAYERNAME];

    char ctfteam;
    boolean spectator;

    UINT16 frame;

    boolean spinning;
    boolean charging;

    boolean super;
} LastPlayerStuff;

/// Virtual players.
typedef struct
{
    struct sockaddr address;
    UINT16 purportedport;

    char name[MAXPLAYERNAME];

    tic_t player_times[MAXPLAYERS]; // leveltimes of each player as perceived by this client. (Can be used to calculate ping.)

    mobj_t* mo;
    boolean in_game;

    angle_t aiming;

    boolean throwing_ring;
    INT32 current_weapon;
    fixed_t toss_strength;

    INT16 flashing; // Used by the server to determine whether or not the flags can be touched.
    mobj_t* shield;
    INT32 powers[NUMPOWERS];

    UINT32 event_list[5];
    UINT16 event_count;
    UINT16 last_eventcount;

    fixed_t fire_x;
    fixed_t fire_y;
    fixed_t fire_z;

    fixed_t realx, realy, realz;

    UINT32 score;
    INT32 skin; // Backup skin for intermission screen; is ID instead of pointer.
    INT32 colour;
    char ctfteam;
    boolean spectator;

    // Race things.
    UINT8 laps;
    boolean permadead;
    UINT32 real_time;

    tic_t toss_delay;

    tic_t last_update_playertime; // The time of this player during the last update that was RECEIVED - not executed.
    tic_t last_update; // The time that the last update was RECEIVED - not executed.
    tic_t last_message; // The last time any message at all was received. Considered separate from last_update due to csdelay.
    UINT8 sync_type;

    UINT8 saymessage_id; // ID of the last SayMessage received. Used by the server for re-sending.

    UINT32 player_links; // For clienttoclient.

    PlayerUpdate update_history [UPDHISTORY];
    tic_t        update_times   [UPDHISTORY];

    tic_t   lasthittime; // The last leveltime we think we hit this player.
    mobj_t* lasthitring; // USE THIS VARIABLE ONLY FOR POINTER COMPARISON.

    LastPlayerStuff last; // Stuff like "last name", "last boolean spectator", etc. For now, please don't touch the other "last" variables in the VirtualPlayer struct.
} VirtualPlayer;

// Self player variables.
typedef struct
{
    boolean throwing_ring;
    INT32 last_weapon;
    fixed_t last_tossstrength;

    boolean had_gravityboots;

    UINT32 event_list[5];
    UINT16 event_count;

    UINT32 player_links;
} SelfPlayer;

typedef struct
{
    fixed_t x, y, z;
} Position;

#define MAXHISTORY 80 // Supports about 2 seconds of ping, woohoo!... this is gonna be a terrible netgame.

#define ANGTODEG(x) (((double)(x)) / ((double)ANGLE_45/45))
#define DEGTOANG(x) ((unsigned int) (x * ((double) ANGLE_45/45)))

#define point_direction(x1, y1, x2, y2) ((atan2((y2) - (y1), (x2) - (x1))) * (180 / 3.1415926535))
#define point_distance(x1, y1, x2, y2) (sqrt((((x2) - (x1)) * ((x2) - (x1))) + (((y2) - (y1)) * ((y2) - (y1)))))

#define SYNC_RINGJUMP     1
#define SYNC_RELATIVEANG  2
#define SYNC_RELATIVEPOS  4
#define SYNC_PREDICTMOVE  8
#define SYNC_INVERSEPOS  16
#define SYNC_INVERSEJUMP 32
#define SYNC_PREDRELANG  64

#define MAXCSEVENTS 5

#define EVENTIDSHIFT 5

#define EVENT_SOUNDPLAY  1 // Warning: EVENT_SOUNDPLAY is used in files that do not include CS_Global.h (map.c, P_DoSpring)
#define EVENT_POPMONITOR 2 // Make sure you update them if you change this event!
#define EVENT_THOK       3
#define EVENT_GOTSAYMSG  4
#define EVENT_PLAYERHIT  5
#define EVENT_MONITORPOP 6
#define EVENT_NUKE       7
#define EVENT_DROPMYFLAG 8
#define EVENT_GETEMMY    9
#define EVENT_GRAVITY   10
#define EVENT_NEWLAP    11 // This is also used in files that don't include CS_Global.h. (p_spec.c)
#define MAXEVENT 11 // Highest event ID.

#define FLAGSTATE_BASE     0
#define FLAGSTATE_DROPPED  1
#define FLAGSTATE_CAPTURED 2
#define FLAGSTATE_HELD     3
#define FLAGSTATE_DROPNOW  4 // Special case for SERVERS ONLY. Happens when a player is to drop the flag.

#define POWER_INVINCIBILITY     1
#define POWER_RINGSHIELD        2
#define POWER_JUMPSHIELD        4
#define POWER_BOMBSHIELD        8
#define POWER_FIRESHIELD       16
#define POWER_FORCESHIELDFULL  32
#define POWER_FORCESHIELDPART  64
#define POWER_SUPER           128

extern sock* game_socket;

extern boolean CS_client;
extern boolean CS_host;
extern boolean CS_mode;

extern int self_id; // Player ID of self.
extern SelfPlayer self; // Some additional data of our own to send off to other players - event list, etc.

extern int csdisplayplayer;

extern VirtualPlayer virtualplayers[MAXPLAYERS];

// Player movement history. (For each player.)
// Please excuse the naming conventions....
extern Position player_histories[MAXPLAYERS][MAXHISTORY];
extern tic_t    history_times[MAXPLAYERS];

extern boolean         client_connecting;
extern struct sockaddr temp_serveraddress;
extern ServerPacket    temp_serverpacket;

extern UINT32 ban_list[100];
extern int ban_count;

extern char flagstate [3]; // flagstate[1] = flagstate of red flag, flagstate[2] = flagstate of blue flag.
extern int flagholder [3]; // -1 if nobody's holding it, -2 if it was recently dropped. (-2 case subject to change)

extern UINT32 finished_flags;
extern tic_t  finished_times[CSMAXPLAYERS];

extern INT8 race_rank[CSMAXPLAYERS];
extern INT8 finished_count;

extern boolean HACK_YOUAREBEINGNUKED;

extern UINT8 saymessage_id; // A count of say messages received (wraps after 15). The host re-sends messages until all clients are updated.
extern UINT8 mapswitches; // A count of the map sitches that the host has done. Used only to tell a client when to switch.

extern consvar_t cv_clienttoclient; // Don't want to include CS_Console or we'll need to recompile everything.
                                    // This is still an important variable though.
void CS_Init(void);
void CS_Shutdown(void);

void CS_Tick(void);

void CS_Host(const char* bindport);
void CS_Connect(const char* ip_address, const char* bindport, const char* connectport);

int CS_PlayerCount();

void SendMessages();
void ReceiveMessages();

void SendIntroPacket(int player_id);
void SendServerPacket(int player_id);
void SendRefusePacket(struct sockaddr* to, const char* reason);

void ReceiveServerPacket(ServerPacket* sp);
void ReceiveRefusePacket(RefusePacket* rp);

void BroadcastSayMessage(SayMessage* msg, int player_id);

void CheckPlayerUpdate(int player_id, PlayerUpdate* pu);
void CheckPlayerDataUpdate(PlayerDataUpdate* du);

Position* Get_HistoryItem(int player_id, tic_t time);
void UpdateHistories();

boolean MakePlayerUpdate(PlayerUpdate* update);
void MakeServerPacket(ServerPacket* sp);

void AddPlayerUpdate(int id, PlayerUpdate* update);
void UpdatePlayers(void);
void UpdatePlayer(int player_id, PlayerUpdate* update);
void RunPlayer(int player_id);
void RunPlayerMessages(int player_id);
void ResetPlayer(int player_id);
void RemovePlayer(int player_id);
void AcceptPlayer(struct sockaddr* from);

void PredictPlayerMovement (int player_id);
void PredictRelativeAngle  (int player_id);

void ExecutePlayerEvents(void);
void ExecuteHitEvent(int player_id, UINT32 event);
void ExecuteLapEvent(int player_id, UINT32 event);
void CS_AddEvent(UINT32 event);

void CS_DoHitEvent(mobj_t* inflictor, INT8 hit_type, boolean killed);
void CS_DoMonitorPopEvent(mobj_t* monitor);
void CS_DoNukeEvent(void);
void CS_DoGetEmmyEvent(mobj_t* emerald);

void CS_UpdateFlags(UINT16 flag_state);
void CS_ServerUpdateFlags();
void CS_ServerUpdateRace();

void PlayerDropRings(int player_id, int num_rings);
void PlayerDropFlag(int player_id);
void PlayerDropEmeralds(int player_id);

void CS_LevelInit(void);

void CS_MobjStuff();
void CS_RingCheckPlayers();
void CS_FireWeapons(void);

void CollideMobjWithPlayer(mobj_t* thing, VirtualPlayer* player);

void CPU_Tick(player_t* player); // CSTODO: CSBots someday? (low priority) CSTODO: Move from CS header to still-not-designed AI headers.

boolean HasShield(INT32* powers);

boolean CollisionCheck(mobj_t* good, mobj_t* evil);
mobj_t* FindFlag(INT8 flag_colour);

void CS_Say(int player_id, SayMessage* msg);
