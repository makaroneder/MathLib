#ifdef __x86_64__
#ifndef SerialPortFirstInFirstOutControlRegister_H
#define SerialPortFirstInFirstOutControlRegister_H
#include <stdint.h>

struct SerialPortFirstInFirstOutControlRegister {
    enum class InterruptTriggerLevel : uint8_t {
        One = 0b00,
        Four = 0b01,
        Eight = 0b10,
        Fourteen = 0b11,
    };
    SerialPortFirstInFirstOutControlRegister(void);

    bool enable : 1;
    bool clearReceive : 1;
    bool clearTransmit : 1;
    bool dmaModeSelect : 1;
    uint8_t reserved : 2;
    uint8_t interruptTriggerLevel : 2;
} __attribute__((packed));

#endif
#endif