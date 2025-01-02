#include "g_game.h"
#include "d_player.h"
#include "d_net.h"
#include "r_things.h" // SetPlayerSkinByNum
#include "p_local.h"

#include "Pathfinding.h"
#include "CS_Global.h"

#include <math.h>

#define point_direction(x1, y1, x2, y2) ((atan2((y2) - (y1), (x2) - (x1))) * (180 / 3.1415926535))
#define point_distance(x1, y1, x2, y2) (sqrt((((x2) - (x1)) * ((x2) - (x1))) + (((y2) - (y1)) * ((y2) - (y1)))))

// NOTE: The game might take a while to close as NEt_WaitAllAckReceived() delays this for five seconds.

Path global_path;
boolean following_path = 0;
int current_node_id = 0;

mobj_t* nearnodes_list[1024];
int near_nodes = 0;

void Move_Towards_Direction(player_t*, float, char);

void CPU_Setup()
{
	return;
}

#define TESTPLAYERCPU
void Add_CPU()
{
    #ifndef TESTPLAYERCPU
    int i;
    if (! server)
    {
        players[2].cpu_player = 1;
        return;
    }
	G_AddPlayer(2);
	playeringame[2] = 1;
	players[2].cpu_player = 1;
	playernode[2] = 2; // Is this needed?
	nodetoplayer[2] = 2;

	for (i = 0; i < 0; i ++)
	{
	    CONS_Printf("%i, %i, %i\n", (mynodes[i].x), (mynodes[i].y), (mysectors[mynodes[i].sector].floor_height));
        P_SpawnMobj(FLOAT_TO_FIXED(mynodes[i].x), FLOAT_TO_FIXED(mynodes[i].y), FLOAT_TO_FIXED(mysectors[mynodes[i].sector].floor_height), MT_GFZFLOWER1);
	}
	#endif

#ifdef RABBIT
    #ifdef TESTPLAYERCPU
    players[0].cpu_player = 1;
    #endif
#endif
}

extern tic_t leveltime;
void CPU_Tick(player_t* player)
{
    int i;
	angle_t mobj_angle = (players[serverplayer].cmd.angleturn << 16) / 11930464;
	static fixed_t lastx, lasty, lastz;

	if (! player->mo)
		return;
    if (! players[serverplayer].mo)
        return;

    for (i = 0; i < near_nodes; i ++)
        P_RemoveMobj(nearnodes_list[i]);
    near_nodes = 0;

    for (i = 0; i < node_count; i ++)
    {
        if (point_distance(mynodes[i].x, mynodes[i].y, players[displayplayer].mo->x >> FRACBITS, players[displayplayer].mo->y >> FRACBITS) <= 256 || mynodes[i].redlight)
        {
            if (! mynodes[i].redlight)
                continue;

            nearnodes_list[near_nodes ++] = P_SpawnMobj(mynodes[i].x << FRACBITS, mynodes[i].y << FRACBITS, mysectors[mynodes[i].sector].floor_height << FRACBITS, mynodes[i].redlight ? MT_GFZFLOWER2 : MT_GFZFLOWER1);
        }
    }

	if ((int) mobj_angle < 0)
		mobj_angle = mobj_angle + 360;

	//player->cmd.angleturn   = (32767 + (unsigned short) players[serverplayer].cmd.angleturn) | TICCMD_RECEIVED;
	player->cmd.forwardmove = 0;
	player->cmd.sidemove    = 0;
	// player->cmd.aiming - use an angle_t >> 16

    player->cmd.buttons = 0;


	if (players[serverplayer].cmd.buttons & BT_USE)
		player->cmd.buttons = players[serverplayer].cmd.buttons & ~BT_USE;

	SetPlayerSkinByNum(2, 0); // Player, Skin
	player->skincolor = players[serverplayer].skincolor;

    if (following_path)
    {
        int i;
        int my_x = player->mo->x >> FRACBITS, my_y = player->mo->y >> FRACBITS;
#if 0// CSTODO: Unused, use this?
        int future_x = (player->mo->x + FixedMul(player->mo->momx, 3 << FRACBITS)) >> FRACBITS,
			future_y = (player->mo->y + FixedMul(player->mo->momy, 3 << FRACBITS)) >> FRACBITS;
#endif

        static fixed_t predict_x;
        static fixed_t predict_y;
		static int predicted_node = 0; // CSTODO: This was supposed to be an int, right? it was just declared as static predic...

        //CONS_Printf("Predicted: %f %f\nCurrent: %f %f\n", FIXED_TO_FLOAT(predict_x), FIXED_TO_FLOAT(predict_y),
        //                                             FIXED_TO_FLOAT(players[2].mo->x), FIXED_TO_FLOAT(players[2].mo->y));

        predict_x = player->mo->x + FixedMul(player->mo->momx, 3 << FRACBITS);
        predict_y = player->mo->y + FixedMul(player->mo->momy, 3 << FRACBITS);

        if (point_distance(predict_x >> FRACBITS, predict_y >> FRACBITS,
            global_path.node_list[current_node_id]->x, global_path.node_list[current_node_id]->y) <= 60 && current_node_id !=
            predicted_node)
        {
            //CONS_Printf("Predict hit node at %i\n", leveltime + 3);
            predicted_node = current_node_id;
        }

        for (i = current_node_id; i < global_path.list_length; i ++)
        {
            int nextx = (player->mo->x + player->mo->momx) >> FRACBITS;
            int nexty = (player->mo->y + player->mo->momy) >> FRACBITS;

            float pointdist = point_distance(my_x, my_y, global_path.node_list[i]->x, global_path.node_list[i]->y);

            if (pointdist <= 30 ||
                point_distance(nextx, nexty, global_path.node_list[i]->x, global_path.node_list[i]->y) <= 30)
            {
                current_node_id = i + 1;
                CONS_Printf("Hit node %i at %i\n", i, leveltime);
                CONS_Printf("Point dist 1: %f Point dist 2: %f\n", pointdist, point_distance(nextx, nexty, global_path.node_list[i]->x, global_path.node_list[i]->y));
                if (current_node_id >= global_path.list_length)
                {
                    //following_path = 0;
                    current_node_id = 0; // Repeat the cycle!
                    //CONS_Printf("Completed path.\n");
                }
            }
        }

        // Edit: Move afterwards - or we're a tic behind!
        Move_Towards_Direction(player, point_direction(my_x, my_y,
            global_path.node_list[current_node_id]->x, global_path.node_list[current_node_id]->y), 126);

/*        if (current_node_id >= global_path.list_length - 1)
        {
            following_path = 0;
            current_node_id = 0;
            CONS_Printf("Completed path.\n");
        }*/
    }

#define ANGTODEG(x) (((double)(x)) / ((double)ANGLE_45/45))
#define DEGTOANG(x) ((unsigned int) (x * ((double) ANGLE_45/45)))

    #define CLAG 0
    // Do test firing.
    if ((leveltime % 9) == CLAG)
    {
        //player->currentweapon = WEP_EXPLODE;
        //player->powers[pw_explosionring] = 100;
        player->cmd.buttons |= BT_ATTACK;
    }
    if (! (leveltime % 9))
    {
        fixed_t playerx = players[serverplayer].mo->x;
        fixed_t playery = players[serverplayer].mo->y;
        fixed_t playerz = players[serverplayer].mo->z;
        fixed_t momx = players[serverplayer].mo->momx;
        fixed_t momy = players[serverplayer].mo->momy;
        fixed_t momz = players[serverplayer].mo->momz;
        int ringsteps = 0;//(R_PointToDist2(player->mo->x, player->mo->y, playerx, playery) >> FRACBITS) / 60 + CLAG;
		fixed_t player_relspeed;

        P_GivePlayerRings(player, 1, 1);

/*        int playerx = players[serverplayer].mo->x >> FRACBITS;
        int playery = players[serverplayer].mo->y >> FRACBITS;
        int playerz = players[serverplayer].mo->z >> FRACBITS;
        int momx = players[serverplayer].mo->momx >> FRACBITS;
        int momy = players[serverplayer].mo->momy >> FRACBITS;
        int ringsteps = (int) point_distance(player->mo->x >> FRACBITS, player->mo->y >> FRACBITS, playerx, playery) / 60 + CLAG;

        playerx += momx * ringsteps;
        playery += momy * ringsteps;
        playerz += players[serverplayer].mo->momz >> FRACBITS;

        int adjusted = 1;
        while ((int) point_distance(player->mo->x >> FRACBITS, player->mo->y >> FRACBITS, playerx, playery) / 60 + CLAG > ringsteps)
        {
            ringsteps ++;
            playerx += momx;
            playery += momy;
            playerz += players[serverplayer].mo->momz >> FRACBITS;
            players[serverplayer].mo->momz -= gravity;
            CONS_Printf("Adjusted Forward %i\n", adjusted ++);
        }

        while ((int) point_distance(player->mo->x >> FRACBITS, player->mo->y >> FRACBITS, playerx, playery) / 60 + CLAG < ringsteps)
        {
            ringsteps --;
            playerx -= momx;
            playery -= momy;
            playerz -= players[serverplayer].mo->momz >> FRACBITS;
            players[serverplayer].mo->momz += gravity;
            CONS_Printf("Adjusted Back %i\n", adjusted ++);
        }

        player->cmd.angleturn = DEGTOANG(point_direction(player->mo->x >> FRACBITS, player->mo->y >> FRACBITS, playerx, playery)) >> 16;

        player->cmd.aiming = R_PointToAngle2(0, player->mo->z + (player->mo->height / 3), R_PointToDist2(playerx << FRACBITS, playery << FRACBITS, player->mo->x, player->mo->y), playerz << FRACBITS) >> 16;*/

        playerx = virtualplayers[0].mo->x;
        playery = virtualplayers[0].mo->y;
        playerz = virtualplayers[0].mo->z;
        momx = virtualplayers[0].mo->momx;
        momy = virtualplayers[0].mo->momy;
        momz = virtualplayers[0].mo->momz;
        momx = virtualplayers[0].mo->x - lastx;
        momy = virtualplayers[0].mo->y - lasty;
        momz = virtualplayers[0].mo->z - lastz;

        player_relspeed =
            R_PointToDist2(player->mo->x, player->mo->y, playerx + momx, playery + momy) -
            R_PointToDist2(player->mo->x, player->mo->y, playerx, playery);

        if (player_relspeed < 60 << FRACBITS) // Don't bother otherwise - they're going faster than the rings.
        {
            fixed_t ring_distance = 0;
			fixed_t lastdist;

            playerx += FixedMul(momx, CLAG << FRACBITS);
            playery += FixedMul(momy, CLAG << FRACBITS);
            for (i = 0; i < CLAG; i ++)
            {
                momz -= gravity;
                playerz += momz;
            }

            lastdist = R_PointToDist2(player->mo->x, player->mo->y, playerx, playery);

            for (ringsteps = 0; ; ringsteps ++)
            {
                float dist = sqrt(FIXED_TO_FLOAT(playerx - player->mo->x) * FIXED_TO_FLOAT(playerx - player->mo->x) +
                             FIXED_TO_FLOAT(playery - player->mo->y) * FIXED_TO_FLOAT(playery - player->mo->y) +
                            FIXED_TO_FLOAT(playerz - player->mo->z) * FIXED_TO_FLOAT(playerz - player->mo->z));

                ring_distance = FixedMul(60 << FRACBITS, ringsteps << FRACBITS);

                if (ring_distance > FLOAT_TO_FIXED(dist))
                    break;

                playerx += momx;
                playery += momy;
                momz -= gravity;
                playerz += momz;

                P_CheckPosition(player->mo, playerx, playery);
                if (playerz < tmfloorz)
                {
                    playerz = tmfloorz;
                    momz = 0;
                }
                tmthing = NULL;

                if (R_PointToDist2(player->mo->x, player->mo->y, playerx, playery) - lastdist > 60 << FRACBITS) // Not good.
                {
                    CONS_Printf("Cannot target (Relspeed %i)\n", (R_PointToDist2(player->mo->x, player->mo->y, playerx, playery) - lastdist) >> FRACBITS);
                    break;
                }

                lastdist = R_PointToDist2(player->mo->x, player->mo->y, playerx, playery);
            }
        }

        // Ringsteps is +1'd because the player will tick as we fire it.

//        playerx += FixedMul(momx, ringsteps << FRACBITS);
//        playery += FixedMul(momy, ringsteps << FRACBITS);
//        for (i = 0; i < ringsteps; i ++)
//        {
//            momz -= gravity;
//            playerz += momz;
//        }

//        if (playerz < players[serverplayer].mo->floorz) // Note: Should be future floorz.
//            playerz = players[serverplayer].mo->floorz;

        player->cmd.angleturn = R_PointToAngle2(player->mo->x + player->mo->momx, player->mo->y + player->mo->momy, playerx, playery) >> 16;
        player->cmd.aiming = R_PointToAngle2(0, player->mo->z + (player->mo->height / 3), R_PointToDist2(playerx, playery, player->mo->x + player->mo->momx, player->mo->y + player->mo->momy), playerz + (player->mo->height / 3)) >> 16;
    }

    lastx = virtualplayers[0].mo->x;
    lasty = virtualplayers[0].mo->y;
    lastz = virtualplayers[0].mo->z;

    player->cmd.angleturn |= TICCMD_RECEIVED;

    if (players[serverplayer].cmd.buttons & BT_TOSSFLAG)
        displayplayer = 2;
}

float Angle_to_X(float angle)
{
    angle += 90;
	while (angle > 360) angle -= 360;
	while (angle < 0)   angle += 360;

    return sin(angle * 3.141592f / 180);

	if (angle > 180)
		angle = 360 - angle;

	return 1 - ((angle / 180) * 2);
}

float Angle_to_Y(float angle)
{
	return Angle_to_X(angle - 90); // Lawl, cheap re-use.
}

void Move_Towards_Direction(player_t* player, float angle, char speed)
{
	float player_angle = (float) (player->mo->angle) / 11930464;

	if (player_angle < 0)
		player_angle = player_angle + 360;

	player->cmd.sidemove    = Angle_to_X(angle - player_angle + 90) * speed;
	player->cmd.forwardmove = Angle_to_X(angle - player_angle)      * speed; // Interesting note: Angle_Y with the same arguments as
																				   // the sidemove line would have the same effect!
//	CONS_Printf("player_angle is %i\nSine is %f\nCosine is %f\n", player_angle, (float) Angle_to_X(player_angle), Angle_to_Y(player_angle));
}

Node* Nearest_Node(int x, int y)
{
    int i;
    float last_distance = point_distance(mynodes[0].x, mynodes[0].y, x, y);
    int nearest_node = 0;

    if (! node_count)
        return NULL;

    for (i = 0; i < node_count; i ++)
    {
        if (point_distance(mynodes[i].x, mynodes[i].y, x, y) < last_distance)
        {
            nearest_node = i;
            last_distance = point_distance(mynodes[i].x, mynodes[i].y, x, y);
        }
    }

    return &mynodes[nearest_node];
}

void AI_Call()
{
    int i;
	int x1, y1, x2, y2;

    if (following_path)
    {
        free(global_path.node_list);
        global_path.node_list = NULL;
        current_node_id = 0;
    }
    following_path = 0;

	x1 = (int) FIXED_TO_FLOAT(players[2].mo->x);
	y1 = (int) FIXED_TO_FLOAT(players[2].mo->y);
	x2 = (int) FIXED_TO_FLOAT(players[serverplayer].mo->x);
	y2 = (int) FIXED_TO_FLOAT(players[serverplayer].mo->y);

	CONS_Printf("Creating path from (%i,%i) to (%i,%i)...\n",
		x1, y1, x2, y2);

	Create_Node_Path((x1 / 64) * 64, (y1 / 64) * 64, (x2 / 64) * 64, (y2 / 64) * 64,
		&global_path);

    //players[2].mo->x = 832 << FRACBITS;
    //players[2].mo->y = -1632 << FRACBITS;

/*    global_path.node_list = realloc(global_path.node_list, 8 * sizeof (Node*));

    global_path.node_list[0] = Nearest_Node(832, -1632);
    global_path.node_list[1] = Nearest_Node(256, -416);
    global_path.node_list[2] = Nearest_Node(-384, 544);
    global_path.node_list[3] = Nearest_Node(224, 1376);
    global_path.node_list[4] = Nearest_Node(1984, 1760);
    global_path.node_list[5] = Nearest_Node(3648, 1120);
    global_path.node_list[6] = Nearest_Node(3680, -1344);
    global_path.node_list[7] = Nearest_Node(1760, -1952);

    global_path.list_length = 8;*/

    for (i = 0; i < node_count; i ++)
        mynodes[i].redlight = 0;

    for (i = 0; i < global_path.list_length; i ++)
    {
        global_path.node_list[i]->redlight = 1;
        CONS_Printf("%i/%i: X: %i\n, Y: %i\n", i, global_path.list_length - 1, global_path.node_list[i]->x, global_path.node_list[i]->y);
    }

    if (global_path.list_length <= 0)
        return;

    current_node_id = 1;
    following_path = 1;
}

/*
	int packetsize = 0;

	netbuffer->packettype = PT_CLIENTCMD;

	if (cl_packetmissed)
		netbuffer->packettype++;
	netbuffer->u.clientpak.resendfrom = (byte)neededtic;
	netbuffer->u.clientpak.client_tic = (byte)gametic;

	if (gamestate == GS_WAITINGPLAYERS)
	{
		// send NODEKEEPALIVE packet
		netbuffer->packettype += 4;
		packetsize = sizeof (clientcmd_pak) - sizeof (ticcmd_t) - sizeof (short);
		HSendPacket(servernode, false, 0, packetsize);
	}
	else if (gamestate != GS_NULL)
	{
		memcpy(&netbuffer->u.clientpak.cmd, &localcmds, sizeof (ticcmd_t));
		netbuffer->u.clientpak.consistancy = consistancy[gametic%BACKUPTICS];

		// send a special packet with 2 cmd for splitscreen
		if (splitscreen)
		{
			netbuffer->packettype += 2;
			memcpy(&netbuffer->u.client2pak.cmd2, &localcmds2, sizeof (ticcmd_t));
			packetsize = sizeof (client2cmd_pak);
		}
		else
			packetsize = sizeof (clientcmd_pak);

		HSendPacket(servernode, false, 0, packetsize);
	}
*/
