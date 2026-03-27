#pragma once
#include <cstdint>

const int CHUNK_SIZE = 1400;

#pragma pack(push, 1)
struct Packet {
    uint32_t frame_id;
    uint16_t seq_no;
    uint16_t total_chunks;
    uint64_t timestamp;
    char data[CHUNK_SIZE];
};
#pragma pack(pop)