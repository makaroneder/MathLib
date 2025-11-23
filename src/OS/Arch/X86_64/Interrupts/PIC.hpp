#ifdef __x86_64__
#ifndef PIC_H
#define PIC_H
#include <Allocator/Allocatable.hpp>
#include <stdint.h>

struct PIC : MathLib::Allocatable {
    PIC(void);
    [[nodiscard]] virtual uint8_t GetBase(void) const = 0;
    [[nodiscard]] virtual bool SendEndOfInterrupt(uint8_t irq) = 0;
    void SetIRQMask(uint8_t irq, bool value);
    [[nodiscard]] bool UpdateMask(void);

    protected:
    [[nodiscard]] virtual bool SetMask(uint16_t mask) = 0;

    private:
    uint16_t mask;
};

#endif
#endif