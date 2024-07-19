#ifndef PIC_H
#define PIC_H
#include <Allocatable.hpp>
#include <stdint.h>

struct PIC : Allocatable {
    virtual bool SendEndOfInterrupt(uint8_t irq) = 0;
    virtual bool SetMask(uint8_t irq, bool value) = 0;
    virtual uint8_t GetBase(void) const = 0;
};

#endif