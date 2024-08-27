#include "RTC.hpp"

RTC::RTC(bool nmi_) : CMOS(false) {
    SetInterrupts(false);
    RegisterInterruptDevice(GetIRQBase() + 8, this);
    Write(Register::StatusRegisterB, Read(Register::StatusRegisterB) | (1 << 6));
    SetRate(3);
    SetNMI(nmi_);
    SetInterrupts(true);
    Read(Register::StatusRegisterC);
}
RTC::~RTC(void) {
    RegisterInterruptDevice(GetIRQBase() + 8, nullptr);
}
uint8_t RTC::GetRate(void) const {
    SetInterrupts(false);
    const uint8_t ret = Read(Register::StatusRegisterA) & 0b1111;
    SetInterrupts(true);
    return ret;
}
bool RTC::SetRate(uint8_t rate) {
    if (rate <= 2 || rate > 15) return false;
    SetInterrupts(false);
    Write(Register::StatusRegisterA, (Read(Register::StatusRegisterA) & 0xf0) | rate);
    SetInterrupts(true);
    return true;
}
void RTC::OnInterrupt(uintptr_t, Registers*, uintptr_t) {
    time += 1 / GetFrequency();
    Read(Register::StatusRegisterC);
}
num_t RTC::GetFrequency(void) const {
    return 32768 >> (GetRate() - 1);
}
bool RTC::SetFrequency(num_t frequency) {
    // TODO:
    (void)frequency;
    return false;
}