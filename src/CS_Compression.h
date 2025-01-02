typedef struct
{
    void* data;

    int data_length; // Data lengths are measured in bits.
    int data_allocated;

    int position; // Bit position in the stream. Usually used only for reading.
} BitStream;

void CompressPlayerUpdate(PlayerUpdate* pu, BitStream* str, int sending_to);
void DecompressPlayerUpdate(void* input, int input_length, PlayerUpdate* pu, boolean from_server);

void CompressPlayerDataUpdate(PlayerDataUpdate* du, BitStream* str);
void DecompressPlayerDataUpdate(void* input, int input_length, PlayerDataUpdate* du);

void CompressServerPacket(ServerPacket* sp, BitStream* str);
void DecompressServerPacket(void* input, int input_length, ServerPacket* sp);

void StreamStart (BitStream* stream);
void StreamEnd   (BitStream* stream);

void StreamWrite(BitStream* stream, INT32 value, int bits);
INT32 StreamRead(BitStream* stream, int bits);
INT32 StreamReadSigned(BitStream* stream, int bits);

void StreamNextByte(BitStream* stream);

int MaxBits(INT32 integer);
