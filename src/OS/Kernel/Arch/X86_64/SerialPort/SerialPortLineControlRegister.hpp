#ifdef __x86_64__
#ifndef SerialPortLineControlRegister_H
#define SerialPortLineControlRegister_H
#include <stdint.h>

struct SerialPortLineControlRegister {
    enum class StopBits : uint8_t {
        One,
        OneAndHalf,
        Two,
    };
    enum class Parity : uint8_t {
        None = 0b000,
        Odd = 0b001,
        Even = 0b011,
        Mark = 0b101,
        Space = 0b111,
    };
    SerialPortLineControlRegister(void);
    [[nodiscard]] uint8_t GetDataBits(void) const;
    [[nodiscard]] StopBits GetStopBits(void) const;

    uint8_t dataBits : 2;
    bool stopBits : 1;
    uint8_t parity : 3;
    bool breakEnable : 1;
    bool divisorLatchAccess : 1;
} __attribute__((packed));

#endif
#endif