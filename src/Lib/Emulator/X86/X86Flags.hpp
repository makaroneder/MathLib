#ifndef Emulator_X86_Flags_H
#define Emulator_X86_Flags_H
#include <stdint.h>

union X86Flags {
    uint64_t value;
    struct {
        bool carry : 1;
        bool reserved1 : 1;
        bool parity : 1;
        bool reserved2 : 1;
        bool auxiliaryCarry : 1;
        bool reserved3: 1;
        bool zero : 1;
        bool sign : 1;
        bool trap : 1;
        bool interruptEnable : 1;
        bool direction : 1;
        bool overflow : 1;
        uint8_t ioPrivilege : 2;
        bool nestedTask : 1;
        bool reserved4: 1;
        bool resume : 1;
        bool virtual8086 : 1;
        bool alignmentCheck : 1;
        bool virtualInterrupt : 1;
        bool virtualInterruptPending : 1;
        bool id : 1;
        uint64_t reserved5 : 42;
    } __attribute__((packed));
    X86Flags(uint64_t value = 0);
} __attribute__((packed));

#endif