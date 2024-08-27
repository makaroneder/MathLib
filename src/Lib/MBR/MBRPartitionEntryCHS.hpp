#ifndef MBRPartitionEntryCHS_H
#define MBRPartitionEntryCHS_H
#include <stdint.h>

struct MBRPartitionEntryCHS {
    uint8_t head;
    uint8_t sector : 6;
    uint16_t cylinder : 10;

    MBRPartitionEntryCHS(const uint32_t& lba, const uint8_t& heads = 16, const uint8_t& sectors = 63);
} __attribute__((packed));

#endif