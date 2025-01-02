#include "CS_Global.h"
#include "CS_Console.h"

//#include "p_local.h"
//#include "r_things.h" // skins
#include "g_game.h"
#include "p_local.h" // Thinker stuff.
//#include "r_state.h"
#include "i_video.h" // Used by RunPlayers for invincibility sparkles?
#include "dstrings.h"

UINT8 state_to_mini[] = {0, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, // S_PLAY_PLG1
                        6, 6, 6, 6, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 11, // S_PLAY_PAIN
                        12, 12, 12, 13, 13, 14, 14, 15, 16, 17, 17, 18, 18, 19, 20, // S_PLAY_SUPERHIT
                        21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 23, 24, 25};
UINT8 mini_to_state[] = {S_NULL, S_PLAY_STND, S_PLAY_TAP1, S_PLAY_RUN1, S_PLAY_ATK1, S_PLAY_PLG1,
                         S_PLAY_SPD1, S_PLAY_ABL1, S_PLAY_SPC1, S_PLAY_CLIMB1, S_PLAY_GASP, S_PLAY_PAIN,
                         S_PLAY_DIE1, S_PLAY_TEETER1, S_PLAY_FALL1, S_PLAY_CARRY, S_PLAY_SUPERSTAND, S_PLAY_SUPERWALK1,
                         S_PLAY_SUPERFLY1, S_PLAY_SUPERTEETER, S_PLAY_SUPERHIT, S_PLAY_SUPERTRANS1, S_PLAY_BOX1A,
                         S_PLAY_BOX1B, S_PLAY_BOX1C, S_PLAY_BOX1D};

extern INT32 var1, var2;

fixed_t TEST_RINGSPD[MAXPLAYERS];
boolean USE_TESTRINGSPD[MAXPLAYERS];

// Needed for compiling on some systems
void A_ChangeColorAbsolute(mobj_t *actor);

fixed_t R_PointToDist3(fixed_t x, fixed_t y, fixed_t z, fixed_t xx, fixed_t yy, fixed_t zz);

extern int jitter[MAXPLAYERS];

void UpdatePlayers(void)
{
    int i, l;

    for (i = 0; i < MAXPLAYERS; i ++)
    {
//        static tic_t last_update[MAXPLAYERS], last_updatetime[MAXPLAYERS];
        static int relative_time[MAXPLAYERS][TICRATE];
		int updateat;

        if (! virtualplayers[i].in_game || ! virtualplayers[i].mo) // It *is* possible that the player will have no mo.
            continue;                                              // This happens to clients when they've just joined.

        // VERSION 2:
        /*if (! (leveltime % TICRATE))
        {
            last_update[i] = virtualplayers[i].last_update;
            last_updatetime[i] = virtualplayers[i].update_times[0];
        }*/

        // VERSION 3:
        // The great thing about this is that it's used as an average, so I don't need to make yet *another* stacked list!
        relative_time[i][I_GetTime() % TICRATE] = (int) I_GetTime() - (int) virtualplayers[i].update_times[0];

        INT64 avg = 0;
        INT32 min = 0x7FFFFFFF;
        INT32 max = 0x80000000;

        for (l = 0; l < TICRATE; l ++)
        {
            avg += relative_time[i][l];
            if (relative_time[i][l] < min)
                min = relative_time[i][l];
            if (relative_time[i][l] > max)
                max = relative_time[i][l];
        }

        avg /= TICRATE;

        jitter[i] = max - min;

        // The original code worked great for accurately-timed messages, but in real life they can be much more irregular.
        // artificial_delay was the old name for cv_csdelay, before "Automatic" was added as a possible value.
        // VERSION 1:
        //int updateat = artificial_delay - (I_GetTime() - virtualplayers[i].last_update);
        // VERSION 2:
        //updateat = virtualplayers[i].update_times[0] - last_updatetime[i] - (leveltime % TICRATE) + artificial_delay;
        // VERSION 3:
        if (cv_csdelay.value == -1)
            updateat = virtualplayers[i].update_times[0] - ((INT32) I_GetTime() - (INT32) min) + (max - min);
        else
            updateat = virtualplayers[i].update_times[0] - ((INT32) I_GetTime() - (INT32) min) + cv_csdelay.value;

        /*if (virtualplayers[i].update_times[updateat] < virtualplayers[i].player_times[i])
            CONS_Printf("JUMPBACK LIKELY.\n");
        if (virtualplayers[i].update_times[updateat] == virtualplayers[i].player_times[i])
            CONS_Printf("STILLFLEX LIKELY.\n");*/

        if (updateat < 0 || updateat >= UPDHISTORY) updateat = 0; // It's not convenient, but we don't want a timeout either.
        if (virtualplayers[i].update_times[updateat] < virtualplayers[i].player_times[i]) // MORE HACK.
            virtualplayers[i].update_history[updateat].event_count = virtualplayers[i].event_count; // HACK: No repeat events wanted.

        if (virtualplayers[i].update_times[updateat] != 0xFFFFFFFF)
            UpdatePlayer(i, &virtualplayers[i].update_history[updateat]);

        // ANOTHER HACK: don't throw thousands of rings at once! It's bad for the environment.
        virtualplayers[i].update_history[updateat].throwing_ring = 0;

        RunPlayer(i);
    }
}

void UpdatePlayer(int player_id, PlayerUpdate* update)
{
    VirtualPlayer* player = &virtualplayers[player_id];
    int i;
	INT32 powers;
	Position *my_old_position, *even_older;
    tic_t tics_advanced = update->my_time - player->player_times[player_id];
    fixed_t ring_distance;

    if (! player->mo || gamestate != GS_LEVEL)
        return;

/*    if (dedicated)
        return; // players[0].mo will not exist. And hey - this is client-side!*/

    P_UnsetThingPosition(player->mo);

    player->sync_type = SYNC_PREDRELANG;

    if (tics_advanced > 0)
    {
        player->mo->momx = FixedDiv(update->x - player->realx, tics_advanced << FRACBITS);
        player->mo->momy = FixedDiv(update->y - player->realy, tics_advanced << FRACBITS);
        player->mo->momz = FixedDiv(update->z - player->realz, tics_advanced << FRACBITS);
    }

    if (! (player->sync_type & SYNC_PREDICTMOVE)) // HACK
    {
        player->mo->x = update->x;
        player->mo->y = update->y;
        player->mo->z = update->z;
    }

    player->realx = update->x;
    player->realy = update->y;
    player->realz = update->z;

    player->mo->sprite = SPR_PLAY; // RunPlayers may change the sprite later, so keep this assignment here.
    player->mo->frame = states[update->state].frame;

    player->mo->angle = update->angle;
    player->aiming    = update->aiming;

    powers = update->powers;
    player->powers[pw_invulnerability] = (powers & POWER_INVINCIBILITY) > 0;
    player->powers[pw_forceshield] = (powers & POWER_FORCESHIELDPART) > 0;
    player->powers[pw_forceshield] += ((powers & POWER_FORCESHIELDFULL) > 0) * 2;
    player->powers[pw_watershield] = (powers & POWER_FIRESHIELD) > 0;
    player->powers[pw_jumpshield] = (powers & POWER_JUMPSHIELD) > 0;
    player->powers[pw_ringshield] = (powers & POWER_RINGSHIELD) > 0;
    player->powers[pw_bombshield] = (powers & POWER_BOMBSHIELD) > 0;
    player->powers[pw_super] = (powers & POWER_SUPER) > 0;

    player->throwing_ring = update->throwing_ring;
    player->current_weapon = update->current_weapon;
    player->toss_strength = update->toss_strength;

    memcpy(player->event_list, update->event_list, sizeof (player->event_list));
    player->last_eventcount = player->event_count;
    player->event_count = update->event_count;

    for (i = 0; i < CSMAXPLAYERS; i ++)
    {
        // What are we doing here? Well, we're trying to find the closest match to our level time.
        // The smaller the difference between this player's level time and ours, the better. Our times should be synchronised.
        if (i == player_id)
            player->player_times[i] = update->my_time;
        else
            player->player_times[i] = update->my_time + update->player_times[i];
    }

    if (update->player_times[self_id] + update->my_time >= leveltime) // Overflow issue...
        player->player_times[self_id] = leveltime-1;

    // Adjust sync-types here.
    ring_distance =
    R_PointToDist3(player->realx, player->realy, player->realz, players[0].mo->x, players[0].mo->y, players[0].mo->z) -
    ((60 * (leveltime - player->player_times[self_id])) << FRACBITS);

    if (ring_distance > 500 * FRACUNIT && player->current_weapon != WEP_RAIL && player->current_weapon != WEP_GRENADE)
        player->sync_type = SYNC_RINGJUMP;

    if (player->current_weapon == WEP_RAIL)
        player->sync_type = SYNC_RELATIVEANG;

    /*if (gametype == GT_RACE && leveltime - player->player_times[self_id] < 6)
        player->sync_type = SYNC_PREDICTMOVE;*/

    if (player->sync_type & SYNC_PREDICTMOVE)
        PredictPlayerMovement(player_id);

    my_old_position = Get_HistoryItem(self_id, player->player_times[self_id]);
    even_older = Get_HistoryItem(self_id, player->player_times[self_id] - 1);

    if (my_old_position != NULL && (player->sync_type & SYNC_INVERSEPOS))
    {
        player->mo->x += players[0].mo->x - my_old_position->x;
        player->mo->y += players[0].mo->y - my_old_position->y;
        player->mo->z += players[0].mo->z - my_old_position->z;
    }

    if (my_old_position != NULL && player->current_weapon != WEP_GRENADE && (player->sync_type & SYNC_RELATIVEANG))
    {
        // Direction we're facing.
        int thisplayer_x = player->mo->x >> FRACBITS, thisplayer_y = player->mo->y >> FRACBITS;
        int my_x = players[0].mo->x >> FRACBITS, my_y = players[0].mo->y >> FRACBITS;
        int old_x = my_old_position->x >> FRACBITS, old_y = my_old_position->y >> FRACBITS;

        double relative_angle = point_direction(thisplayer_x, thisplayer_y, old_x, old_y) - point_direction(thisplayer_x, thisplayer_y, my_x, my_y);
        double use_angle = ANGTODEG(player->mo->angle) - relative_angle;

		float old_distance, new_distance, old_direction, new_direction;

        while (use_angle < 0)
            use_angle += 360;
        while (use_angle > 360)
            use_angle -= 360;

        player->mo->angle = DEGTOANG(use_angle);

        // Now, direction we're aiming.
        old_distance  = point_distance(old_x, old_y, thisplayer_x, thisplayer_y);
        new_distance  = point_distance(my_x, my_y, thisplayer_x, thisplayer_y);
        old_direction = point_direction(0, player->mo->z >> FRACBITS, old_distance, my_old_position->z >> FRACBITS);
        new_direction = point_direction(0, player->mo->z >> FRACBITS, new_distance, players[0].mo->z >> FRACBITS);

        relative_angle = new_direction - old_direction;
        use_angle = ANGTODEG(player->aiming) + relative_angle;

        while (use_angle < 0)
            use_angle += 360;
        while (use_angle > 360)
            use_angle -= 360;

        player->aiming = DEGTOANG(use_angle);
    }

    if (my_old_position != NULL && (player->sync_type & SYNC_RELATIVEPOS))
    {
        int my_x = players[0].mo->x >> FRACBITS, my_y = players[0].mo->y >> FRACBITS;
        int thisplayer_x = player->mo->x >> FRACBITS, thisplayer_y = player->mo->y >> FRACBITS;
        int old_x = my_old_position->x >> FRACBITS, old_y = my_old_position->y >> FRACBITS;

        int relative_x = thisplayer_x - old_x;
        int relative_y = thisplayer_y - old_y;

        player->fire_x = (my_x + relative_x) << FRACBITS;
        player->fire_y = (my_y + relative_y) << FRACBITS;
        player->fire_z = players[0].mo->z + (player->mo->z - my_old_position->z);
    }

    if (player->sync_type & SYNC_PREDRELANG)
    {
        if (! my_old_position || ! even_older)
            ;//CONS_Printf("Warning: Cannot fire a ring as we do not have enough history of ourselves!\n");
        else
            PredictRelativeAngle(player_id);
    }

    if (! player->shield && (player->powers[pw_forceshield] || player->powers[pw_ringshield] || player->powers[pw_bombshield] ||
        player->powers[pw_watershield] || player->powers[pw_jumpshield]))
    {
        thinker_t* th;
        mobjtype_t orbtype = MT_WHITEORB;

        for (th = thinkercap.next; th != &thinkercap; th = th->next)
        {
			mobj_t *shieldobj;

            if (th->function.acp1 != (actionf_p1)P_MobjThinker)
                continue;

            shieldobj = (mobj_t *)th;

            if (shieldobj->target == player->mo && (shieldobj->type == MT_WHITEORB || shieldobj->type == MT_BLUEORB ||
                                                    shieldobj->type == MT_YELLOWORB || shieldobj->type == MT_BLACKORB ||
                                                    shieldobj->type == MT_GREENORB))
            {
                P_SetMobjState(shieldobj, S_DISS); //kill the old one(s)
                P_RemoveMobj(shieldobj); // Needed to avoid the old "random" crashes. =/
            }
        }

		// Well, y'know, just in case...
        if (player->powers[pw_forceshield]) orbtype = MT_BLUEORB;
        if (player->powers[pw_ringshield]) orbtype = MT_YELLOWORB;
        if (player->powers[pw_bombshield]) orbtype = MT_BLACKORB;
        if (player->powers[pw_watershield]) orbtype = MT_GREENORB;
        if (player->powers[pw_jumpshield]) orbtype = MT_WHITEORB;

        player->shield = (mobj_t*) P_SpawnMobj(player->mo->x, player->mo->y, player->mo->z, orbtype);
        P_SetTarget(&player->shield->target, player->mo);

        var1 = 0;
        var2 = player->shield->info->painchance; // player->parent->brother->son->shield->spritedata->sprite->id
        A_ChangeColorAbsolute(player->shield);
    }

    P_SetThingPosition(player->mo);
    P_CheckPosition(player->mo, player->mo->x, player->mo->y);

    player->mo->floorz = tmfloorz;
    player->mo->ceilingz = tmceilingz;
}

boolean MakePlayerUpdate(PlayerUpdate* update)
{
    int i;
	INT32 *powers;

    if (gamestate != GS_LEVEL || ! players[0].mo)
        return 0;

    update->id = 0x02;

    update->player_id = self_id;

    update->x = players[0].mo->x;
    update->y = players[0].mo->y;
    update->z = players[0].mo->z;

    //update->frame  = players[0].mo->frame;
    /*for (i = 0; i < NUMSTATES; i ++)
    {
        if (players[0].mo->state == &states[i])
            update->state = i;
    }*/
    update->state = players[0].mo->state - states;

    update->angle  = players[0].mo->angle;
    update->aiming = players[0].aiming;

    powers = players[0].powers;
    update->powers = 0;
    if (powers[pw_invulnerability] > 1) update->powers |= POWER_INVINCIBILITY;
    if (powers[pw_forceshield] == 1) update->powers |= POWER_FORCESHIELDPART;
    if (powers[pw_forceshield] > 1) update->powers |= POWER_FORCESHIELDFULL;
    if (powers[pw_watershield]) update->powers |= POWER_FIRESHIELD;
    if (powers[pw_jumpshield]) update->powers |= POWER_JUMPSHIELD;
    if (powers[pw_ringshield]) update->powers |= POWER_RINGSHIELD;
    if (powers[pw_bombshield]) update->powers |= POWER_BOMBSHIELD;
    if (powers[pw_super]) update->powers |= POWER_SUPER;

    update->throwing_ring = self.throwing_ring;
    update->toss_strength = self.last_tossstrength;

    if (self.throwing_ring) // Extra behaviour added so that spectators can see this player choose their murder tool.
        update->current_weapon = self.last_weapon;
    else
        update->current_weapon = players[0].currentweapon;

    #if MAXMOVE != 60*FRACUNIT
    Oh noes! Make sure there is room in weaponstate for self.last_tossstrength.
    #endif
    #if NUM_WEAPONS != 7
    This may need updating!
    #endif

    self.throwing_ring = 0;

    // Send the event list.
    memcpy(update->event_list, self.event_list, sizeof (self.event_list));
    update->event_count = self.event_count;

    for (i = 0; i < CSMAXPLAYERS; i ++)
    {
        update->player_times[i] = virtualplayers[i].player_times[i] - leveltime;

        if (i == self_id)
        {
            update->my_time = leveltime;
            update->player_times[i] = 0;
        }
    }

    return 1;
}

void RunPlayer(int player_id)
{
    int i;
    VirtualPlayer* player = &virtualplayers[player_id];
    UINT16 frame = 0;
    boolean spawnmo = false;

    if (! player->mo)
        return;

    RunPlayerMessages(player_id);

    if (player->spectator)
    {
        player->mo->sprite = SPR_DISS;
        player->mo->frame = 0;
        player->mo->flags &= ~MF_SHOOTABLE; // No autoaiming or ring dissipating is wanted.
        player->powers[pw_emeralds] = 0; // Just in case they weren't dropped.
        player->flashing = 0;
    }
    else
        player->mo->flags |= MF_SHOOTABLE;

    for (i = 0; i < NUMSTATES; i ++)
    {
        if ((player->mo->frame & FF_FRAMEMASK) == (states[i].frame & FF_FRAMEMASK))
        {
            frame = i;
            break;
        }
    }

    // Rolling.
    if (frame >= S_PLAY_ATK1 && frame <= S_PLAY_ATK4 && R_PointToDist2(0, 0, player->mo->momx, player->mo->momy) >
        15 << FRACBITS && (player->mo->z == player->mo->floorz || (player->last.spinning && player->mo->momz <= 0)))
    {
        spawnmo = 1;

        if (player->last.charging)
            S_StartSound(player->mo, sfx_zoom); // Whooosh!
    }

    // Charging a spindash.
    if (frame >= S_PLAY_ATK1 && frame <= S_PLAY_ATK4 && ! player->mo->momx && ! player->mo->momy  && ! player->mo->momz &&
        player->mo->z == player->mo->floorz)
    {
        player->last.charging = 1;

        if ((leveltime % (TICRATE/10)) == 0)
        {
            spawnmo = 1;
            S_StartSound(player->mo, sfx_spndsh);
        }
    }
    else
		player->last.charging = 0;

    // Charging a spindash or rolling.
    if (spawnmo)
    {
        mobj_t* mo = P_SpawnMobj(
            player->mo->x,
            player->mo->y,
            player->mo->z - FixedDiv(FIXEDSCALE(player->mo->info->height, player->mo->scale) - player->mo->height, 3 * FRACUNIT),
            MT_THOK);

        mo->flags |= MF_TRANSLATION;
        mo->color = player->mo->color;
        P_SetTarget(&mo->target, player->mo);
        mo->floorz = mo->z;
        mo->ceilingz = mo->z + mo->height;
        P_SetScale(mo, player->mo->scale);
        mo->destscale = player->mo->scale;
    }

    // Fire shield spawns.
    if (frame >= S_PLAY_ATK1 && frame <= S_PLAY_ATK4 && player->powers[pw_watershield] && player->mo->z == player->mo->floorz &&
        (player->mo->momx || player->mo->momy) && (leveltime & 1))
    {
        fixed_t newx;
        fixed_t newy;
        fixed_t ground;
        mobj_t *flame;
        angle_t travelangle;
        int fuseskip = leveltime - player->player_times[self_id];
        if (fuseskip >= TICRATE*6 || fuseskip <= -TICRATE*6)
            fuseskip = TICRATE*6-1;

        if (player->powers[pw_gravityboots])
            ground = player->mo->ceilingz - mobjinfo[MT_SPINFIRE].height - 1;
        else
            ground = player->mo->floorz + 1;

        travelangle = R_PointToAngle2(0, 0, player->mo->momx, player->mo->momy);

        newx = player->mo->x + P_ReturnThrustX(player->mo, travelangle + ANGLE_135, 24*FRACUNIT);
        newy = player->mo->y + P_ReturnThrustY(player->mo, travelangle + ANGLE_135, 24*FRACUNIT);
        flame = P_SpawnMobj(newx, newy, ground, MT_SPINFIRE);
        P_SetTarget(&flame->target, player->mo);
        flame->angle = travelangle;
        flame->fuse = TICRATE*6-fuseskip;
        if (player->powers[pw_gravityboots]) flame->eflags |= MFE_VERTICALFLIP;

        flame->momx = 8;
        P_XYMovement(flame);

        newx = player->mo->x + P_ReturnThrustX(player->mo, travelangle - ANGLE_135, 24*FRACUNIT);
        newy = player->mo->y + P_ReturnThrustY(player->mo, travelangle - ANGLE_135, 24*FRACUNIT);
        flame = P_SpawnMobj(newx, newy, ground, MT_SPINFIRE);
        P_SetTarget(&flame->target, player->mo);
        flame->angle = travelangle;
        flame->fuse = TICRATE*6-fuseskip;
        if (player->powers[pw_gravityboots]) flame->eflags |= MFE_VERTICALFLIP;

        flame->momx = 8;
        P_XYMovement(flame);
    }

    // Invincibility sparkles.
    if (mariomode && player->powers[pw_invulnerability] && !player->powers[pw_super])
	{
		player->mo->flags |= MF_TRANSLATION;
		player->mo->color = (leveltime % MAXSKINCOLORS);
	}
    else if ((player->powers[pw_invulnerability] || (player->powers[pw_super] && !(player->skin == 0))) && leveltime % (TICRATE/7) == 0
        && (!player->powers[pw_super] || (player->powers[pw_super] && !(player->skin == 0))))
    {
        fixed_t destx, desty;

        if (!splitscreen && rendermode != render_soft)
        {
            angle_t viewingangle;
            mobj_t* displayplayermo = players[0].mo;

            if (csdisplayplayer != self_id)
                displayplayermo = virtualplayers[csdisplayplayer].mo;

            if (!cv_chasecam.value && displayplayermo)
                viewingangle = R_PointToAngle2(player->mo->x, player->mo->y, displayplayermo->x, displayplayermo->y);
            else
                viewingangle = R_PointToAngle2(player->mo->x, player->mo->y, camera.x, camera.y);

            destx = player->mo->x + P_ReturnThrustX(player->mo, viewingangle, FRACUNIT);
            desty = player->mo->y + P_ReturnThrustY(player->mo, viewingangle, FRACUNIT);
        }
        else
        {
            destx = player->mo->x;
            desty = player->mo->y;
        }

        P_SpawnMobj(destx, desty, player->mo->z, MT_IVSP);
    }

    // Super colour.
    if (player->powers[pw_super] && player->mo)
        player->mo->color = 15;
    else
        player->mo->color = player->colour; // Return to original colour instantly.

    // CTF: Got flag?
    if (csdisplayplayer != player_id)
    {
        fixed_t spawn_z = player->mo->z + FIXEDSCALE(player->mo->info->height, player->mo->scale)+16*FRACUNIT;

        if (player->powers[pw_gravityboots])
            spawn_z = player->mo->z - FIXEDSCALE(player->mo->info->height, player->mo->scale)+16*FRACUNIT;

        if (player_id == flagholder[1])
            P_SpawnMobj(player->mo->x, player->mo->y, spawn_z, MT_GOTFLAG);
        if (player_id == flagholder[2])
            P_SpawnMobj(player->mo->x, player->mo->y, spawn_z, MT_GOTFLAG2);
    }

    // Gravity flip.
    if (player->powers[pw_gravityboots] && ! (player->mo->eflags & MFE_VERTICALFLIP))
        player->mo->eflags |= MFE_VERTICALFLIP;
    else if (! player->powers[pw_gravityboots])
        player->mo->eflags &= ~MFE_VERTICALFLIP;

    // Tossdelay counter.
    if (player->toss_delay)
        player->toss_delay --;

    // Flashing counter.
    if (player->flashing > 0)
        player->flashing --;
    if (player->flashing < 0)
        player->flashing = 0;

    // Player flashing after hit.
    if (player->last.frame == S_PLAY_PAIN && frame != S_PLAY_PAIN)
    {
        int ping = (INT32) leveltime - (INT32) player->player_times[self_id];
        player->flashing = flashingtics - ping;
    }

    // HACK: if you're dead, don't pick anything up. Flashing are good.
    // HACK: also, makes the player flash when they respawn.
    if (frame >= S_PLAY_DIE1 && frame <= S_PLAY_DIE3)
        player->flashing = (flashingtics - ((INT32) leveltime - (INT32) player->player_times[self_id])) & ~1; // & ~1 for player visibility.

    // Visible flashing.
    if (player->flashing & 1)
    {
        player->mo->sprite = SPR_DISS;
        player->mo->frame = 0;
    }

    // Safe flashing.
    if (player->flashing < 0 || player->flashing > flashingtics) // Don't take any risks of infinite-ish flashing.
        player->flashing = 0;

    // State and health correction, for future autoaim compatibility.
    if (player->mo->state != &states[player->mo->info->spawnstate])
        player->mo->state = &states[player->mo->info->spawnstate];
    if (player->mo->health != player->mo->info->spawnhealth)
        player->mo->health = player->mo->info->spawnhealth;

    // Super dissipate; lose the emeralds.
    if (! player->powers[pw_super] && player->last.super)
        player->powers[pw_emeralds] = 0; // Only the host needs to do this, but the clients might as well, too.

    player->last.frame = frame;
    player->last.spinning = spawnmo;
    player->last.spectator = player->spectator;
    player->last.ctfteam = player->ctfteam;
    player->last.super = player->powers[pw_super];
    memcpy(player->last.name, player->name, MAXPLAYERNAME);

/*    player->mo->tics --;

    if (player->mo->tics <= 0)
    {
        UINT8 next_state = S_NULL;

		player->mo->frame = states[states[frame].nextstate].frame;
		player->mo->tics = states[states[frame].nextstate].tics;
    }*/
}

void RunPlayerMessages(int player_id)
{
    VirtualPlayer* player = &virtualplayers[player_id];
    LastPlayerStuff* last = &player->last;

    // Changing their name.
    if (memcmp(last->name, player->name, MAXPLAYERNAME))
        CONS_Printf(text[RENAMED_TO], last->name, player->name);

    // Changing their team.
    if (gametype == GT_CTF || (gametype == GT_MATCH && cv_matchtype.value))
    {
        if (player->ctfteam == 1 && last->ctfteam != 1)
            CONS_Printf(text[REDTEAM_SWITCH], player->name);
        if (player->ctfteam == 2 && last->ctfteam != 2)
            CONS_Printf(text[BLUETEAM_SWITCH], player->name);
    }
    // Joining the game (non-team modes).
    else if (! player->spectator && last->spectator)
        CONS_Printf(text[INGAME_SWITCH], player->name);

    // Becoming a spectator.
    if (player->spectator && ! last->spectator)
        CONS_Printf(text[SPECTATOR_SWITCH], player->name);

    // Going super.
    if (player->powers[pw_super] && ! last->super && gametype != GT_COOP)
    {
        S_StartSound(NULL, sfx_supert);
		HU_SetCEchoFlags(0);
		HU_SetCEchoDuration(5);
		HU_DoCEcho(va("%s\\is now super.\\\\\\\\", player->name));
		I_OutputMsg("%s is now super.\n", player->name);
    }

    // Going unsuper.
    if (! player->powers[pw_super] && last->super && gametype != GT_COOP)
    {
        S_StartSound(NULL, sfx_s3k_52);
        HU_SetCEchoFlags(0);
        HU_SetCEchoDuration(5);
        HU_DoCEcho(va("%s\\is no longer super.\\\\\\\\", player->name));
        I_OutputMsg("%s is no longer super.\n", player->name);
    }
}

void ResetPlayer(int player_id)
{
    int i;

    struct sockaddr address;
    boolean in_game;
    tic_t last_update;
    char name[MAXPLAYERNAME];
    UINT16 event_count;
    UINT16 last_eventcount;
    char ctfteam;
    boolean spectator;

    // Save
    memcpy(&address, &virtualplayers[player_id].address, sizeof (struct sockaddr));
    memcpy(name, virtualplayers[player_id].name, MAXPLAYERNAME);
    in_game = virtualplayers[player_id].in_game;
    last_update = virtualplayers[player_id].last_update;
    event_count = virtualplayers[player_id].event_count;
    last_eventcount = virtualplayers[player_id].last_eventcount;
    ctfteam = virtualplayers[player_id].ctfteam;
    spectator = virtualplayers[player_id].spectator;

    if (in_game)
        virtualplayers[player_id].event_count = virtualplayers[player_id].update_history[0].event_count; // MORE DUMB HACK FOR EVENT DUMB REPEAT GLITCH.

    memset(&virtualplayers[player_id], 0, sizeof (VirtualPlayer));

    // Restore
    if (in_game) // Better not restore if the player wasn't there anyway...
    {
        virtualplayers[player_id].spectator = spectator;
        virtualplayers[player_id].ctfteam = ctfteam;
        virtualplayers[player_id].last_eventcount = last_eventcount;
        virtualplayers[player_id].event_count = event_count;
        virtualplayers[player_id].last_update = last_update;
        virtualplayers[player_id].in_game = in_game;
        memcpy(virtualplayers[player_id].name, name, MAXPLAYERNAME);
        memcpy(virtualplayers[player_id].last.name, name, MAXPLAYERNAME);
        memcpy(&virtualplayers[player_id].address, &address, sizeof (struct sockaddr));

        // So this shouldn't *really* be done, but it isn't really a problem either. It fixes some message issues.
        virtualplayers[player_id].last.spectator = spectator;
        virtualplayers[player_id].last.ctfteam = ctfteam;
    }
    // If we're not restoring...
    else
    {
        if (gametype == GT_MATCH || gametype == GT_CTF || gametype == GT_TAG)
        {
            virtualplayers[player_id].spectator = 1;
            virtualplayers[player_id].last.spectator = 1;
        }

        // Reset name.
        sprintf(virtualplayers[player_id].name, "Player %i", player_id);
        sprintf(virtualplayers[player_id].last.name, "Player %i", player_id);
    }

    // Reset update history.
    // LXShadow: I know I wrote this but... um... is this needed? Better safe than sorry, I suppose.
    for (i = 0; i < UPDHISTORY; i ++)
        virtualplayers[player_id].update_times[i] = 0xFFFFFFFF;
}

void RemovePlayer(int player_id)
{
    VirtualPlayer* player = &virtualplayers[player_id];

    // Remove shields associated with this player.
    thinker_t* th;

    for (th = thinkercap.next; th != &thinkercap; th = th->next)
    {
		mobj_t *shieldobj;

        if (th->function.acp1 != (actionf_p1)P_MobjThinker)
            continue;

        shieldobj = (mobj_t *)th;

        if (shieldobj->target == player->mo && (shieldobj->type == MT_WHITEORB || shieldobj->type == MT_BLUEORB ||
                                                shieldobj->type == MT_YELLOWORB || shieldobj->type == MT_BLACKORB ||
                                                shieldobj->type == MT_GREENORB))
        P_RemoveMobj(shieldobj); //kill the old one(s)
    }

    if (player->mo && gamestate == GS_LEVEL) // player->mo should be null when gamestate != GS_LEVEL anyway, but just in case...
        P_RemoveMobj(player->mo);

/*    if (CS_host)
    {
        if (redholder == player_id)  redstate = FLAGSTATE_BASE;
        if (blueholder == player_id) bluestate = FLAGSTATE_BASE;
    }*/

    player->in_game = 0;
    player->mo = NULL;

    if (CS_client)
        self.player_links &= ~(1 << player_id); // Reset player link.

    if (CS_host)
        finished_flags &= ~(1 << player_id); // Remove from these race rankings.
}

void PredictPlayerMovement(int player_id)
{
    mobj_t* mo = virtualplayers[player_id].mo;

    int ping = (leveltime - virtualplayers[player_id].player_times[self_id]) / 2;
    int i;

    #define AVGWIDTH 2

    int avg_count = 0;
    fixed_t avg_momx[AVGWIDTH], avg_momy[AVGWIDTH], avg_momz[AVGWIDTH];

    for (i = 0; i < MAXHISTORY && avg_count < AVGWIDTH; i ++)
    {
        Position* uno = Get_HistoryItem(player_id, virtualplayers[player_id].player_times[player_id] - i + 1);
        Position* dos = Get_HistoryItem(player_id, virtualplayers[player_id].player_times[player_id] - i);
        Position temp = {mo->x, mo->y, mo->z};

        if (i == 0)
            uno = &temp;
        if (! uno || ! dos)
            continue;

        avg_momx[avg_count] = uno->x - dos->x;
        avg_momy[avg_count] = uno->y - dos->y;
        avg_momz[avg_count] = uno->z - dos->z;
        avg_count ++;
    }

    if (avg_count < AVGWIDTH)
        return;

    mo->momx = 0; mo->momy = 0; mo->momz = 0;
    for (i = 0; i < AVGWIDTH; i ++)
    {
        mo->momx += FixedDiv(avg_momx[i], AVGWIDTH << FRACBITS);
        mo->momy += FixedDiv(avg_momy[i], AVGWIDTH << FRACBITS);
        mo->momz += FixedDiv(avg_momz[i], AVGWIDTH << FRACBITS);
    }

    mo->x = virtualplayers[player_id].realx;
    mo->y = virtualplayers[player_id].realy;
    mo->z = virtualplayers[player_id].realz;
    P_SetThingPosition(mo);

    for (i = 0; i < ping; i ++)
    {
        P_UnsetThingPosition(mo);
        /*mo->x += mo->momx;
        mo->y += mo->momy;*/
        mo->z += mo->momz;
        P_SetThingPosition(mo);

        P_TryMove(mo, mo->x + mo->momx, mo->y + mo->momy, 0);

        P_CheckPosition(mo, mo->x, mo->y);

        if (mo->z < tmfloorz)
        {
            P_UnsetThingPosition(mo);
            mo->z = tmfloorz;
            P_SetThingPosition(mo);
            mo->momz = 0;
        }

        mo->momz -= gravity;

//        P_SceneryXYFriction(mo, mo->x, mo->y);
    }

    P_UnsetThingPosition(mo);
}

fixed_t R_PointToDist3(fixed_t x, fixed_t y, fixed_t z, fixed_t xx, fixed_t yy, fixed_t zz)
{
    return R_PointToDist2(0, z, R_PointToDist2(x, y, xx, yy), zz);
}

void PredictRelativeAngle(int player_id)
{
    VirtualPlayer* player = &virtualplayers[player_id];

    Position* my_old_position = Get_HistoryItem(self_id, player->player_times[self_id]);
    Position* even_older      = Get_HistoryItem(self_id, player->player_times[self_id] - 1);

    fixed_t speed = 60 << FRACBITS;
    fixed_t playerx = my_old_position->x;
    fixed_t playery = my_old_position->y;
    fixed_t playerz = my_old_position->z;
    fixed_t momx = my_old_position->x - even_older->x;
    fixed_t momy = my_old_position->y - even_older->y;
    fixed_t momz = my_old_position->z - even_older->z;
    fixed_t x_offset = 0, y_offset = 0, z_offset = 0;
    int ringsteps;

    int first_ringsteps;
    fixed_t remember_x = 0, remember_y = 0, remember_z = 0;

    fixed_t ring_distance = 0;
    fixed_t lastdist;

    subsector_t* temp;

    lastdist = R_PointToDist3(playerx, playery, playerz, player->mo->x, player->mo->y, player->mo->z);

    if (! player->mo || ! my_old_position || ! even_older)
        return;

    for (ringsteps = 1; ; ringsteps ++)
    {
        Position *test_pos;
        fixed_t dist = R_PointToDist3(playerx, playery, playerz, player->mo->x, player->mo->y, player->mo->z);

        ring_distance = FixedMul(speed, ringsteps << FRACBITS);

        if (ring_distance > dist)
        {
            x_offset = FixedMul(ring_distance, FINECOSINE(player->mo->angle>>ANGLETOFINESHIFT));
            y_offset = FixedMul(ring_distance, FINESINE(player->mo->angle>>ANGLETOFINESHIFT));
            x_offset = player->mo->x + FixedMul(x_offset, FINECOSINE(player->aiming>>ANGLETOFINESHIFT)) - playerx;
            y_offset = player->mo->y + FixedMul(y_offset, FINECOSINE(player->aiming>>ANGLETOFINESHIFT)) - playery;
            z_offset = player->mo->z + FixedMul(ring_distance, AIMINGTOSLOPE(player->aiming)) - playerz;
            break;
        }

        test_pos = Get_HistoryItem(self_id, player->player_times[self_id] + ringsteps);
        if (test_pos)
        {
            momx = test_pos->x - playerx; momy = test_pos->y - playery; momz = test_pos->z - playerz;
            playerx = test_pos->x; playery = test_pos->y; playerz = test_pos->z;
            lastdist = R_PointToDist3(playerx, playery, playerz, player->mo->x, player->mo->y, player->mo->z);
            continue;
        }

        playerx += momx;
        playery += momy;
        momz -= gravity;
        playerz += momz;

        temp = (subsector_t*) R_IsPointInSubsector(playerx, playery);
        if (temp)
        {
            if (playerz < temp->sector->floorheight)
            {
                playerz = temp->sector->floorheight;
                momz = 0;
            }
        }

        if (R_PointToDist3(playerx, playery, playerz, player->mo->x, player->mo->y, player->mo->z) - lastdist > speed)
            return; // If we simply break, then it'll practically auto-aim the ring for this player.
                    // ...not...that...any hackers might be looking at this comment.
        lastdist = R_PointToDist3(playerx, playery, playerz, player->mo->x, player->mo->y, player->mo->z);
    }

    first_ringsteps = ringsteps;
    momx = players[0].mo->momx; momy = players[0].mo->momy; momz = players[0].mo->momz;
    playerx = players[0].mo->x; playery = players[0].mo->y; playerz = players[0].mo->z;
    lastdist = R_PointToDist3(playerx, playery, playerz, player->mo->x, player->mo->y, player->mo->z);

    //x_offset = 0; y_offset = 0; z_offset = 0;
    //CONS_Printf("DEBUG AUTOAIM enabled\n");
    //if (player->throwing_ring) CONS_Printf("Ang: %f\nAim: %f\n %i", ANGTODEG(ang), ANGTODEG(aim), ang>>FRACBITS);

    for (ringsteps = 1; ; ringsteps ++)
    {
        fixed_t dist = R_PointToDist3(playerx, playery, playerz, player->mo->x, player->mo->y, player->mo->z);

        ring_distance = FixedMul(speed, ringsteps << FRACBITS);

        if (ring_distance > dist) break;

        if (ringsteps > first_ringsteps) // We may be able to speed this ring up later on.
        {
            remember_x = playerx; remember_y = playery; remember_z = playerz;
        }

        playerx += momx;
        playery += momy;
        momz -= gravity;
        playerz += momz;

        temp = (subsector_t*) R_IsPointInSubsector(playerx, playery);
        if (temp)
        {
            if (playerz < temp->sector->floorheight)
            {
                playerz = temp->sector->floorheight;
                momz = 0;
            }
        }

        if (R_PointToDist3(playerx, playery, playerz, player->mo->x, player->mo->y, player->mo->z) - lastdist > speed)
            break; // Break here, because we stand a chance.

        lastdist = R_PointToDist3(playerx, playery, playerz, player->mo->x, player->mo->y ,player->mo->z);
    }

    USE_TESTRINGSPD[player_id] = 0;

    if (ringsteps > first_ringsteps) // If possible and/or necessary, speed up the ring now.
    {
        fixed_t new_ringspeed = FixedMul(60 << FRACBITS, FixedDiv(ringsteps << FRACBITS, first_ringsteps << FRACBITS));

        if (new_ringspeed <= 80 << FRACBITS)
        {
            TEST_RINGSPD[player_id] = new_ringspeed;
            USE_TESTRINGSPD[player_id] = 1;
            playerx = remember_x; playery = remember_y; playerz = remember_z;
        }
    }

    player->mo->angle = R_PointToAngle2(player->mo->x, player->mo->y, playerx + x_offset, playery + y_offset);
    player->aiming = R_PointToAngle2(
                            0,
                            player->mo->z + (player->mo->height / 3),
                            R_PointToDist2(player->mo->x, player->mo->y, playerx + x_offset, playery + y_offset),
                            playerz + (player->mo->height / 3) + z_offset
                            );
}
