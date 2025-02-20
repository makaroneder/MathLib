#ifdef __x86_64__
#ifndef FACS_H
#define FACS_H
#include <stdint.h>

struct FACS {
    static constexpr const char* expectedSignature = "FACS";
    char signature[4];
    uint32_t length;
    uint32_t hardwareSignature;
    uint32_t firmwareWakingVector;
    uint32_t globalLock;
    bool s4BIOS : 1;
    bool wakingVector64Support : 1;
    uint32_t reserved1 : 30;
    uint64_t firmwareWakingVector2;
    uint8_t version;
    uint32_t reserved2 : 24;
    bool wakingVector64Requirement : 1;
    uint32_t reserved3 : 31;
    uint64_t reserved4[3];
} __attribute__((packed));

#endif
#endif