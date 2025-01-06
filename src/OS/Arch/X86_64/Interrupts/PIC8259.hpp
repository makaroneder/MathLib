#ifdef __x86_64__
#ifndef PIC8259_H
#define PIC8259_H
#include "PIC.hpp"

struct PIC8259 : PIC {
    enum class Ports : uint16_t {
        MasterCommand = 0x20,
        MasterData,
        SlaveCommand = 0xa0,
        SlaveData,
    };
    enum class Command {
        EndOfInterrupt = 0x20,
        ReadIRR = 0xa,
        ReadISR,
    };
    enum class InitializationControlWord1 : uint8_t {
        UseInitializationControlWord4 = 0,
        SinglePIC,
        Interval4,
        LevelTriggered,
        Initialize,
    };
    enum class InitializationControlWord4 {
        Mode8086 = 0,
        AutoEndOfInterrupt,
        MasterBuffer,
        BufferMode,
        SpeciallyFullyNestedMode,
    };

    PIC8259(void);
    [[nodiscard]] bool Init(uint8_t base);
    [[nodiscard]] virtual uint8_t GetBase(void) const override;
    [[nodiscard]] virtual bool SendEndOfInterrupt(uint8_t irq) override;

    protected:
    [[nodiscard]] virtual bool SetMask(uint16_t mask) override;

    private:
    uint8_t base;
};

#endif
#endif