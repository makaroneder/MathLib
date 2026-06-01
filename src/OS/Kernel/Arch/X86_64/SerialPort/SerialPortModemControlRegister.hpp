#ifdef __x86_64__
#ifndef SerialPortModemControlRegister_H
#define SerialPortModemControlRegister_H
#include <stdint.h>

struct SerialPortModemControlRegister {
    SerialPortModemControlRegister(void);

    bool dataTerminalReady : 1;
    bool requestToSend : 1;
    bool out1 : 1;
    bool out2 : 1;
    bool loop : 1;
    uint8_t reserved : 3;
} __attribute__((packed));

#endif
#endif