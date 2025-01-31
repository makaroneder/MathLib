#ifdef __x86_64__
#ifndef Exception_H
#define Exception_H
#include "../InterruptDevice.hpp"

struct Exception : InterruptDevice {
    enum class Type {
        DivisionError = 0,
        DebugException,
        NonMaskableInterrupt,
        Breakpoint,
        Overflow,
        BoundRageExceeded,
        InvalidOpcode,
        DeviceNotAvailable,
        DoubleFault,
        LegacyCoprocessorSegmentOverrun,
        InvalidTSS,
        SegmentNotPresent,
        StackSegmentFault,
        GeneralProtectionFault,
        PageFault,
        X87FloatException = 16,
        AlignmentCheck,
        MachineCheck,
        SIMDFloatException,
        VirtualizationException,
        ControlProtectionException,
        HypervisorInjectionException = 28,
        VMMCommunicationException,
        SecurityException,
    };
    Exception(Type type);
    virtual ~Exception(void) override;
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) override;
    [[nodiscard]] virtual MathLib::String GetPanicMessage(uintptr_t error) const = 0;

    private:
    Type type;
};

#endif
#endif