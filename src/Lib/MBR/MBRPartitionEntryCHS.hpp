#ifndef MBR_PartitionEntryCHS_H
#define MBR_PartitionEntryCHS_H
#include <stdint.h>

struct MBRPartitionEntryCHS {
    uint8_t head;
    uint8_t sector : 6;
    uint16_t cylinder : 10;

    MBRPartitionEntryCHS(uint32_t lba, uint8_t heads = 16, uint8_t sectors = 63);
} __attribute__((packed));

#endif