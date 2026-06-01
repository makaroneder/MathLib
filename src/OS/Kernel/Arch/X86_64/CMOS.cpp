#ifdef __x86_64__
#include "../Arch.hpp"
#include "CMOS.hpp"
#include "IO.hpp"

CMOS* cmos = nullptr;
CMOSFloppyData::CMOSFloppyData(uint8_t data) : data(data) {}
uint8_t CMOSFloppyData::GetMaster(void) const {
    return (data >> 4) & 0b1111;
}
uint8_t CMOSFloppyData::GetSlave(void) const {
    return data & 0b1111;
}
MathLib::String CMOSFloppyData::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    return "{\n"_M +
        padd + "\tMaster: 0x" + MathLib::ToString(GetMaster(), 16, 1) + '\n' +
        padd + "\tSlave: 0x" + MathLib::ToString(GetSlave(), 16, 1) + '\n' +
    padd + "}";
}
CMOS::CMOS(bool nmi_) : nmi(false) {
    ArchSetInterrupts(false);
    SetNMI(nmi_);
    ArchSetInterrupts(true);
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
    (void)Read((Register)ReadPort<uint8_t>(0x70));
}

#endif