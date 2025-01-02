#include "CS_Global.h"
#include "CS_Compression.h"

#include "doomstat.h" // NUMGAMETYPES

/*
Bit maximum values:

1: 1
2: 3
3: 7
4: 15
5: 31
6: 63
7: 127
8: 255
*/

int eventbits[MAXEVENT + 1] = {0, 2, 0, 0, 4, 25, 12, 0, 1, 3, 1, 3}; // Roughly how many bits are used for each event. (minus evID)

#define write(data, bits) StreamWrite(str, data, bits)
#define read(bits) StreamRead(str, bits)

void CompressPlayerUpdate(PlayerUpdate* pu, BitStream* str, int sending_to)
{
    UINT8 time_small = (MaxBits(pu->my_time) - 1) / 8;
    int i;
    INT8 tossstr;

    if (! MaxBits(pu->my_time)) // If you were to subtract 1 from this (shown above), time_small would become 0xFF. NOT WANTED.
        time_small = 0;

    write(pu->id, 3);
    write(pu->player_id, 5);
    write(pu->type, 1);
    write(pu->powers, 8);

    write(time_small, 2);

    if (sending_to <= 0) // Not sure why it would be smaller than 0...
    {
        for (i = 0; i < CSMAXPLAYERS; i ++)
            write(pu->player_times[i], 8);

        write(pu->my_time, (time_small + 1) * 8);
    }
    else
    {
        write(pu->my_time, (time_small + 1) * 8);
        write(pu->player_times[sending_to], 8);
    }

    write(pu->x >> FRACBITS, 16);
    write(pu->y >> FRACBITS, 16);
    write(pu->z >> FRACBITS, 16);

    write(pu->angle >> 16, 16);
    write(pu->aiming >> 16, 16);

    write(pu->state, 6); // I counted 62 states.

    tossstr = (FixedMul(pu->toss_strength, 2 << FRACBITS) >> FRACBITS) - 16;
    if (tossstr < 0)
        tossstr = 0;

    write(pu->throwing_ring, 1);
    write(pu->current_weapon, 3);
    write(tossstr, 4);

    for (i = 0; i < 5; i ++)
    {
        int ev_id = pu->event_list[i] & 0x1F;

        write(ev_id, 4);
        write(pu->event_list[i] >> 5, eventbits[ev_id]);
    }

    //write(pu->event_count, 16);
    write(pu->event_count, 4); // BETA 4.

    StreamNextByte(str); // Let's try not to forget this. It's pretty important (we send messages in byte counts, not bit counts).
}

void DecompressPlayerUpdate(void* input, int input_length, PlayerUpdate* pu, int received_from)
{
    UINT8 time_small;
    int i;
    BitStream real;
    BitStream* str = &real;

    // StreamStart is not called for this - we'll deal with this ourselves for now.
    str->position = 0;

    str->data = input;
    str->data_allocated = input_length;
    str->data_length = input_length;

    pu->id = read(3);
    pu->player_id = read(5);
    pu->type = read(1);
    pu->powers = read(8);

    time_small = read(2);

    if (received_from > 0)
    {
        for (i = 0; i < CSMAXPLAYERS; i ++)
            pu->player_times[i] = read(8);

        pu->my_time = read((time_small + 1) * 8);
    }
    else
    {
        for (i = 0; i < CSMAXPLAYERS; i ++)
            pu->player_times[i] = 0;

        pu->my_time = read((time_small + 1) * 8);
        pu->player_times[self_id] = read(8);
    }

    pu->x = read(16) << FRACBITS;
    pu->y = read(16) << FRACBITS;
    pu->z = read(16) << FRACBITS;

    pu->angle = read(16) << 16;
    pu->aiming = read(16) << 16;

    pu->state = read(6);

    pu->throwing_ring = read(1);
    pu->current_weapon = read(3);
    pu->toss_strength = FixedDiv((read(4) + 16) << FRACBITS, 2 << FRACBITS);

    for (i = 0; i < 5; i ++)
    {
        int ev_id = read(4);

        pu->event_list[i] = (read(eventbits[ev_id]) << 5) | ev_id;
    }

    //pu->event_count = read(16);
    pu->event_count = read(4); // BETA 4.

    return; // StreamEnd isn't needed either! =P
}

void CompressPlayerDataUpdate(PlayerDataUpdate* du, BitStream* str)
{
    int i;

    write(du->id, 3);
    write(du->player_id, 5);

    write(IGetSockaddrIP(&du->address), 32);
    write(IGetSockaddrPort(&du->address), 16);
    write(du->purportedport, 16);

    write(du->skin, MaxBits(MAXSKINS - 1)); // - 1 is used for obvious reasons... if you think about it. (Hint: using skins[MAXSKINS] may cause crash)
    write(du->colour, MaxBits(MAXSKINCOLORS - 1));
    write(du->team, 2); // Would be great if we could just use one bit, but we might want to distinguish between spectator and player in the future.

    write(MaxBits(du->score) <= 16 ? 0 : 1, 1); // So 0 means use SHORT, 1 means use INT.
    write(du->score, MaxBits(du->score) <= 16 ? 16 : 32);
    write(du->laps, 3);

    write(du->real_time, 16);
    write(du->permadead, 1);

    write(du->player_links, CSMAXPLAYERS);

    for (i = 0; i < MAXPLAYERNAME; i ++)
    {
        write(du->name[i], 8);

        if (! du->name[i])
            break;
    }

    StreamNextByte(str); // Let's try not to forget this. It's pretty important (we send messages in byte counts, not bit counts).
}

void DecompressPlayerDataUpdate(void* input, int input_length, PlayerDataUpdate* du)
{
    int i;

    BitStream real;
    BitStream* str = &real;

    str->position = 0;

    str->data = input;
    str->data_allocated = input_length;
    str->data_length = input_length;

    du->id = read(3);
    du->player_id = read(5);

    ISetSockaddrIP(&du->address, read(32));
    ISetSockaddrPort(&du->address, read(16));
    du->address.sa_family = AF_INET;
    du->purportedport = read(16);

    du->skin = read(MaxBits(MAXSKINS - 1));
    du->colour = read(MaxBits(MAXSKINCOLORS - 1));
    du->team = read(2);

    du->score = read((read(1) + 1) * 16);
    du->laps = read(3);

    du->real_time = read(16);
    du->permadead = read(1);

    du->player_links = read(CSMAXPLAYERS);

    memset(du->name, 0, MAXPLAYERNAME); // du->name may have some random data that will not be corrected in the next loop.
                                        // It's not that bad, but we may otherwise get some rename spam that we don't want.
    for (i = 0; i < MAXPLAYERNAME; i ++)
    {
        du->name[i] = read(8);

        if (! du->name[i])
            break;
    }

    du->name[MAXPLAYERNAME - 1] = 0; // We don't want any hacking.
}

void CompressServerPacket(ServerPacket* sp, BitStream* str)
{
    int i;

    write(sp->id, 3);

    write(sp->map, MaxBits(NUMMAPS));
    write(sp->gametype, MaxBits(NUMGAMETYPES));
    write(sp->level_time, 32);

    write(sp->mapswitches, 4); // 16 switches without one update received? Highly unlikely. Should be fine.

    write(sp->pointlimit, 32);
    write(sp->timelimit, 8); // Temporarily (perhaps) 8 bits. Who would want a time limit greater than 255 minutes?
    write(sp->itemrespawntime, 9);
    write(sp->matchtype, 1);

    if (sp->gametype == GT_MATCH || sp->gametype == GT_CTF)
    {
        write(sp->red_score, 16);
        write(sp->blue_score, 16);

        write(sp->flag_state, 16);

        for (i = 0; i < 7; i ++)
            write((sp->emeralds[i] != -1), 1);

        for (i = 0; i < 7; i ++)
        {
            if (sp->emeralds[i] == -1) // Is not being held.
                continue;

            write(sp->emeralds[i], 5);
        }
    }

    if (sp->gametype == GT_RACE)
    {
        write(sp->finished_count, MaxBits(CSMAXPLAYERS));

        for (i = 0; i < sp->finished_count; i ++)
            write(sp->race_rank[i], MaxBits(CSMAXPLAYERS));
    }

    StreamNextByte(str);
}

void DecompressServerPacket(void* input, int input_length, ServerPacket* sp)
{
    int i;
    boolean held[7];

    BitStream real;
    BitStream* str = &real;

    str->position = 0;

    str->data = input;
    str->data_allocated = input_length;
    str->data_length = input_length;

    sp->id = read(3);

    sp->map = read(MaxBits(NUMMAPS));
    sp->gametype = read(MaxBits(NUMGAMETYPES));
    sp->level_time = read(32);

    sp->mapswitches = read(4);

    sp->pointlimit = read(32);
    sp->timelimit = read(8);
    sp->itemrespawntime = read(9);
    sp->matchtype = read(1);

    if (sp->gametype == GT_MATCH || sp->gametype == GT_CTF)
    {
        sp->red_score = read(16);
        sp->blue_score = read(16);

        sp->flag_state = read(16);

        for (i = 0; i < 7; i ++)
            held[i] = read(1);

        for (i = 0; i < 7; i ++)
        {
            if (held[i])
                sp->emeralds[i] = read(5);
            else
                sp->emeralds[i] = -1;
        }
    }

    if (sp->gametype == GT_RACE)
    {
        sp->finished_count = read(MaxBits(CSMAXPLAYERS));

        for (i = 0; i < sp->finished_count; i ++)
            sp->race_rank[i] = read(MaxBits(CSMAXPLAYERS));
    }

    if (sp->itemrespawntime == 511) // 511 is 111111111 in binary, or '-1'.
        sp->itemrespawntime = -1;
}

void StreamStart(BitStream* stream)
{
    stream->position = 0;

    stream->data = malloc(512);
    stream->data_allocated = 512 * 8; // 512 bytes to start with.
    stream->data_length = 0;
}

void StreamEnd(BitStream* stream)
{
    free(stream->data);

    stream->position = 0;

    stream->data = NULL;
    stream->data_allocated = 0;
    stream->data_length = 0;
}

void StreamWrite(BitStream* stream, INT32 value, int bits)
{
	UINT64 *ptr;
	UINT32 var;

    // Re-allocate if necessary.
    if (stream->position + 64 >= stream->data_allocated)
    {
        stream->data_allocated += 512 * 8;

        stream->data = realloc(stream->data, stream->data_allocated / 8);
    }

    ptr = (UINT64*) ((UINT32) stream->data + (stream->position >> 3));

    var = ~(0xFFFFFFFF << (stream->position & 0x07)); // The below is hopefully fixed; if not, then an invalid BitSteam is probably being passed.
    *ptr &= var | ~(0xFFFFFFFFFFFFFFFFll >> (64 - (bits + (stream->position & 0x07))));
	*ptr |= (value & (0xFFFFFFFF >> (32 - bits))) << (stream->position & 0x07);

	stream->data_length += bits;
	stream->position += bits;
}

INT32 StreamRead(BitStream* stream, int bits)
{
	INT32 ret;

    if (stream->position + bits > stream->data_length) // Note: Pretty convinced > should be used, not >=.
        return -1; // CSTODO: If you're the host, kick the guy sending the invalid messages?
                    // Or if you're a client and a hacker is doing some sneaky clienttoclient work, kick them out of your playerlinks?
    ret = (INT32) ((*(UINT64*) ((UINT32) stream->data + (stream->position >> 3)) >> (stream->position & 0x07)) & (~(0xFFFFFFFF << bits)));

    stream->position += bits;

    return ret;
}

INT32 StreamReadSigned(BitStream* stream, int bits)
{
    INT32 value = StreamRead(stream, bits);

    if ((value >> (bits - 1)) & 1)
        value = -value;

    return value;
}

void StreamNextByte(BitStream* stream)
{
    boolean writing = (stream->data_length == stream->position);

	if (stream->position & 0x07) // If it's not something like this, then we're already on the next byte!
		stream->position += 8 - (stream->position & 0x07);

    if (writing)
        stream->data_length = stream->position;
}

int MaxBits(INT32 integer)
{
    int i;
    int bits = 0;

    for (i = 0; i < 32; i ++)
    {
        if (integer & (1 << i))
            bits = i + 1;
    }

    return bits;
}
