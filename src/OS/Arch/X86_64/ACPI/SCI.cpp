#ifdef __x86_64__
#include "SCI.hpp"
#include "../IO.hpp"
#include <Logger.hpp>

SCI::SCI(void) : fadt(nullptr) {}
SCI::SCI(FADT* fadt) : fadt(fadt) {
    if (fadt->smiCommandPort && fadt->acpiEnable && fadt->acpiDisable) {
        WritePort<uint8_t>(fadt->smiCommandPort, fadt->acpiEnable);
        if (fadt->revision >= 0x02) {
            const FADT2* fadt2 = (const FADT2*)fadt;
            while (!(fadt2->pm1aControlBlock2.Read(GenericAddressStructure::AccessSize::U16Access).Get("Failed to read PM1A control block") & 1)) {}
        }
        else while (!(ReadPort<uint16_t>(fadt->pm1aControlBlock) & 1)) {}
    }
    if (!RegisterIRQDevice((IRQ)fadt->sciInterrupt, this)) MathLib::Panic("Failed to register IRQ");
}
SCI::~SCI(void) {
    if (fadt && !RegisterIRQDevice((IRQ)fadt->sciInterrupt, nullptr)) MathLib::Panic("Failed to unregister IRQ");
}
bool SCI::Reboot(void) {
    if (fadt->revision >= 0x02) {
        FADT2* fadt2 = (FADT2*)fadt;
        return fadt2->resetRegister.Write(fadt2->resetValue);
    }
    else return false;
}
void SCI::OnInterrupt(uintptr_t, Registers*, uintptr_t) {
    LogString("SCI\n");
    // TODO:
}

#endif