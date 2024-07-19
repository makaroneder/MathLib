#ifndef InterruptDescriptor_H
#define InterruptDescriptor_H
#include <stdint.h>

struct InterruptDescriptor {
    enum class GateType : uint8_t {
        Interrupt = 0b1110,
        Trap = 0b1110,
    };
    uint16_t offset1;
    uint16_t codeSegment;
    uint8_t interruptStackTableOffset;
    uint8_t attributes;
    uint16_t offset2;
    uint32_t offset3;
    uint32_t reserved;

    InterruptDescriptor(uintptr_t offset = 0, uint16_t codeSegment = 0, uint8_t attributes = 0);
} __attribute__((packed));

#endif