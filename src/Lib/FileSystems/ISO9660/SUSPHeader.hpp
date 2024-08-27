#ifndef SUSPHeader_H
#define SUSPHeader_H
#include <stdint.h>

struct SUSPHeader {
    char signature[2];
    uint8_t length;
    uint8_t version;
} __attribute__((packed));

#endif