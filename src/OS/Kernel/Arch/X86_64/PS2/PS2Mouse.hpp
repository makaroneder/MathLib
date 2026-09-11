#ifdef __x86_64__
#ifndef PS2Mouse_H
#define PS2Mouse_H
#include "PS2Device.hpp"
#include <Math/Matrix.hpp>

enum class Packet0Bits : uint8_t {
    LeftButton = 0,
    RightButton,
    MiddleButton,
    Always1,
    NegativeX,
    NegativeY,
    OverflowX,
    OverflowY,
};
struct PS2Mouse : PS2Device {
    static constexpr const uint8_t validSampleRates[] = {
        10, 20, 40, 60, 80, 100, 200,
    };
    enum class Type {
        Normal,
        ZAxis,
        MoreButtons,
    };

    PS2Mouse(bool second);
    virtual ~PS2Mouse(void) override;
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) override;
    [[nodiscard]] bool SetSampleRate(uint8_t sampleRate);

    private:
    Type type;
    MathLib::Matrix<ssize_t> position;
    uint8_t packet;
    uint8_t packets[4];
};

#endif
#endif