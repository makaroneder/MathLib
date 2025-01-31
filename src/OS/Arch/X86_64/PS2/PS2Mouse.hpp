#ifdef __x86_64__
#ifndef PS2Mouse_H
#define PS2Mouse_H
#include "PS2Device.hpp"
#include <Math/Matrix.hpp>

struct PS2MousePacket1 {
    bool leftButton : 1;
    bool rightButton : 1;
    bool middleButton : 1;
    uint8_t alwaysOne : 1;
    uint8_t xSign : 1;
    uint8_t ySign : 1;
    uint8_t xOverflow : 1;
    uint8_t yOverflow : 1;
} __attribute__((packed));
struct PS2Mouse5ButtonsPacket {
    uint8_t z : 4;
    bool button4 : 1;
    bool button5 : 1;
    uint8_t alwaysZero : 2;
} __attribute__((packed));
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
    MathLib::Matrix<size_t> position;
    uint8_t packet;
    uint8_t packets[4];
};

#endif
#endif