#ifdef __x86_64__
#ifndef SerialPortInterruptIdentificationRegister_H
#define SerialPortInterruptIdentificationRegister_H
#include <stdint.h>

struct SerialPortInterruptIdentificationRegister {
    enum class InterruptState : uint8_t {
        ModemStatus = 0b00,
        TransmitterEmpty = 0b01,
        ReceiverDataAvailable = 0b10,
        ReceiverLineStatus = 0b11,
    };
    enum class FirstInFirstOutBufferState : uint8_t {
        None = 0b00,
        Unusable = 0b01,
        Enabled = 0b10,
    };

    bool interruptNotPending : 1;
    uint8_t interruptState : 2;
    bool timeoutInterruptPending : 1;
    uint8_t reserved : 2;
    uint8_t firstInFirstOutBufferState : 2;
} __attribute__((packed));

#endif
#endif