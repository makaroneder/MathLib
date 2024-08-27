#ifndef PathTable_H
#define PathTable_H
#include <stdint.h>

struct PathTable {
    uint8_t identifierLength;
    uint8_t extendedAttributeRecordLength;
    uint32_t extent;
    uint16_t parentDirectory;
    char identifier[];
} __attribute__((packed));

#endif