#ifdef __x86_64__
#ifndef PIT_H
#define PIT_H
#include "InterruptTimer.hpp"

struct PIT : InterruptTimer {
    enum class Ports : uint16_t {
        Channel0 = 0x40,
        Channel1,
        Channel2,
        CommandRegister,
    };
    enum class Channel : uint8_t {
        Channel0 = 0,
        Channel1,
        Channel2,
        ReadBackCommand,
    };
    enum class AccessMode : uint8_t {
        LatchCountValueCommand = 0,
        LowByteOnly,
        HighByteOnly,
        LowAndHighByte,
    };
    enum class OperatingMode : uint8_t {
        InterruptOnTerminalCount = 0,
        ReTriggerableOneShot,
        RateGenerator,
        SquareWaveGenerator,
        SoftwareTriggeredStrobe,
        HardwareTriggeredStrobe,
    };
    static constexpr size_t baseFrequency = 1193182;

    PIT(void);
    virtual ~PIT(void) override;
    virtual MathLib::num_t GetFrequency(void) const override;
    virtual bool SetFrequency(MathLib::num_t frequency) override;

    private:
    size_t GetReloadValue(void) const;
    bool SetReloadValue(size_t value);
};

#endif
#endif