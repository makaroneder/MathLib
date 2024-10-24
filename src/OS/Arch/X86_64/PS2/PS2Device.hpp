#ifdef __x86_64__
#ifndef PS2Device_H
#define PS2Device_H
#include <Expected.hpp>

struct PS2Device {
    enum class Response : uint8_t {
        Acknowledge = 0xfa,
        Resend = 0xfe,
    };
    PS2Device(bool second);
    Expected<uint8_t> Read(void) const;
    bool Write(uint8_t value);
    Expected<uint8_t> SendCommand(uint8_t command);
    Expected<uint16_t> GetID(void);

    private:
    bool second;
};

#endif
#endif