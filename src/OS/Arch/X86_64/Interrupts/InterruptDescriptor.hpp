#ifdef __x86_64__
#ifndef InterruptDescriptor_H
#define InterruptDescriptor_H
#include <stdint.h>

struct InterruptDescriptor {
    enum class GateType : uint8_t {
        Interrupt = 0b1110,
        Trap = 0b1111,
    };
    InterruptDescriptor(void);
    InterruptDescriptor(uintptr_t offset, uint16_t codeSegment, InterruptDescriptor::GateType gateType);

    private:
    uint16_t offset1;
    uint16_t codeSegment;
    uint8_t interruptStackTableOffset : 2;
    uint8_t reserved1 : 6;
    uint8_t type : 4;
    bool reserved3 : 1;
    uint8_t ring : 2;
    bool present : 1;
    uint64_t offset2 : 48;
    uint32_t reserved2;
} __attribute__((packed));

#endif
#endif