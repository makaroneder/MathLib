#ifdef __x86_64__
#ifndef SerialPortInterruptEnableRegister_H
#define SerialPortInterruptEnableRegister_H
#include <stdint.h>

struct SerialPortInterruptEnableRegister {
    bool dataAvailable : 1;
    bool transmitterEmpty : 1;
    bool receiverLineStatus : 1;
    bool modemStatus : 1;
    uint8_t reserved : 4;
} __attribute__((packed));

#endif
#endif