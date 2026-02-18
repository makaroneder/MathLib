#ifdef __x86_64__
#ifndef SCI_H
#define SCI_H
#include "FADT.hpp"
#include "../Interrupts/InterruptDevice.hpp"

struct SCI : InterruptDevice {
    SCI(void);
    SCI(FADT* fadt);
    ~SCI(void);
    [[nodiscard]] bool Reboot(void);
    virtual void OnInterrupt(uintptr_t, Registers*, uintptr_t) override;

    private:
    FADT* fadt;
};

#endif
#endif