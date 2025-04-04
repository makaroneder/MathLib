#ifdef __x86_64__
#ifndef PS2Device_H
#define PS2Device_H
#include "../Interrupts/InterruptDevice.hpp"
#include <Expected.hpp>

struct PS2Device : InterruptDevice {
    enum class Response : uint8_t {
        Acknowledge = 0xfa,
        Resend = 0xfe,
    };
    PS2Device(bool second);
    [[nodiscard]] MathLib::Expected<uint8_t> Read(void) const;
    [[nodiscard]] bool Write(uint8_t value);
    [[nodiscard]] MathLib::Expected<uint8_t> SendCommand(uint8_t command);
    [[nodiscard]] MathLib::Expected<uint16_t> GetID(void);
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) override;

    protected:
    [[nodiscard]] bool RegisterDevice(bool init);

    private:
    bool second;
};

#endif
#endif