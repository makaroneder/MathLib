#ifndef VBE_H
#define VBE_H
#include <stdint.h>

struct VBEInfo {
    static constexpr const char* expectedSignature = "VESA";
    char signature[4];
    uint16_t version;
    uint16_t oemOffset;
    uint16_t oemSegment;
    uint32_t capabilities;
    uint16_t videoModeOffset;
    uint16_t videoModeSegment;
    uint16_t memory;
    uint32_t resv[123];

    bool IsValid(void) const;
} __attribute__((packed));
struct VBEModeInfo {
    uint8_t reserved1 : 7;
    bool hasLinearFramebuffer : 1;
    uint8_t reserved2;
    uint8_t windowA;
    uint8_t windowB;
    uint16_t granularity;
    uint16_t windowSize;
    uint16_t segmentA;
    uint16_t segmentB;
    uint32_t windowFunction;
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint8_t wChr;
    uint8_t yChr;
    uint8_t planes;
    uint8_t bitsPerPixel;
    uint8_t banks;
    uint8_t memoryModel;
    uint8_t bankSize;
    uint8_t imagePages;
    uint8_t reserved3;
    uint8_t redMask;
    uint8_t redPosition;
    uint8_t greenMask;
    uint8_t greenPosition;
    uint8_t blueMask;
    uint8_t bluePosition;
    uint8_t reservedMask;
    uint8_t reservedPosition;
    uint8_t directColorAttributes;
    uint32_t framebuffer;
    uint32_t offScreenMemoryOffset;
    uint32_t offScreenMemorySize;
    uint16_t reserved4[103];
} __attribute__((packed));

#endif