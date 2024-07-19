#ifndef SCI_H
#define SCI_H
#include "FADT.hpp"
#include "../Interrupts/Interrupts.hpp"

struct SCI : InterruptDevice {
    SCI(void);
    SCI(const FADT* fadt);
    ~SCI(void);
    bool Reboot(void);
    virtual void OnInterrupt(uintptr_t, Registers*, uintptr_t) override;

    private:
    const FADT* fadt;
};

#endif