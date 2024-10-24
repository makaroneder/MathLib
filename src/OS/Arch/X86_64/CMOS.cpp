#ifdef __x86_64__
#include "Interrupts/Interrupts.hpp"
#include "CMOS.hpp"
#include "IO.hpp"

CMOS* cmos = nullptr;
CMOSFloppyData::CMOSFloppyData(uint8_t data) : data(data) {}
CMOS::CMOS(bool nmi_) : nmi(false) {
    SetInterrupts(false);
    SetNMI(nmi_);
    SetInterrupts(true);
}
void CMOS::AccessRegister(Register reg) const {
    WritePort<uint8_t>(0x70, (uint8_t)reg | (!nmi << 7));
}
uint8_t CMOS::Read(Register reg) const {
    AccessRegister(reg);
    return ReadPort<uint8_t>(0x71);
}
void CMOS::Write(Register reg, uint8_t value) {
    AccessRegister(reg);
    WritePort<uint8_t>(0x71, value);
}
CMOSFloppyData CMOS::GetFloppyData(void) const {
    return CMOSFloppyData(Read(Register::FloppyData));
}
void CMOS::SetNMI(bool nmi_) {
    nmi = nmi_;
    Read((Register)ReadPort<uint8_t>(0x70));
}

#endif