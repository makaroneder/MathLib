#ifdef __x86_64__
#include "../Arch.hpp"
#include "RTC.hpp"
#include <Host.hpp>

RTC::RTC(bool nmi_, Register centuryRegister) : CMOS(false), centuryRegister(centuryRegister) {
    ArchSetInterrupts(false);
    if (!RegisterIRQDevice(IRQ::RTC, this)) MathLib::Panic("Failed to register IRQ");
    Write(Register::StatusRegisterB, Read(Register::StatusRegisterB) | (1 << 6));
    if (!SetRate(3)) MathLib::Panic("Failed to set RTC rate");
    SetNMI(nmi_);
    ArchSetInterrupts(true);
    (void)Read(Register::StatusRegisterC);
}
RTC::~RTC(void) {
    if (!RegisterIRQDevice(IRQ::RTC, nullptr)) MathLib::Panic("Failed to unregister IRQ");
}
uint8_t RTC::GetRate(void) const {
    ArchSetInterrupts(false);
    const uint8_t ret = Read(Register::StatusRegisterA) & 0b1111;
    ArchSetInterrupts(true);
    return ret;
}
bool RTC::SetRate(uint8_t rate) {
    if (rate <= 2 || rate > 15) return false;
    ArchSetInterrupts(false);
    Write(Register::StatusRegisterA, (Read(Register::StatusRegisterA) & 0xf0) | rate);
    ArchSetInterrupts(true);
    return true;
}
void RTC::OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) {
    (void)Read(Register::StatusRegisterC);
    InterruptTimer::OnInterrupt(interrupt, regs, error);
}
MathLib::num_t RTC::GetFrequency(void) const {
    return 32768 >> (GetRate() - 1);
}
bool RTC::SetFrequency(MathLib::num_t frequency) {
    // TODO:
    (void)frequency;
    return false;
}
bool RTC::IsUpdating(void) const {
    return Read(Register::StatusRegisterA) & 1 << 7;
}
Date RTC::GetDate(void) const {
    while (IsUpdating()) {}
    Date date;
    uint8_t century = 0;
    while (true) {
        uint8_t tmpCentury = 0;
        Date tmp = Date(Read(Register::RTCSeconds), Read(Register::RTCMinutes), Read(Register::RTCHours), Read(Register::RTCDay), Read(Register::RTCMonth), Read(Register::RTCYear));
        if ((uint8_t)centuryRegister) tmpCentury = Read(centuryRegister);
        if (date == tmp && century == tmpCentury) break;
        date = tmp;
        century = tmpCentury;
    }
    const uint8_t regB = Read(Register::StatusRegisterB);
    if (!(regB & 1 << 2)) {
        date = Date(date.second % 16 + date.second / 16 * 10, date.minute % 16 + date.minute / 16 * 10, date.hour % 16 + date.hour / 16 * 10, date.day % 16 + date.day / 16 * 10, date.month % 16 + date.month / 16 * 10, date.year % 16 + date.year / 16 * 10);
        century = century % 16 + century / 16 * 10;
    }
    if (!(regB & 1 << 1) && date.hour & 1 << 7) date.hour = ((date.hour & ((1 << 7) - 1)) + 12) % 24;
    if ((uint8_t)centuryRegister) date.year += century * 100;
    else {
        date.year += Date::currentYear / 100 * 100;
        if (date.year < Date::currentYear) date.year += 100;
    }
    return date;
}
void RTC::SetDate(const Date& date) {
    (void)date;
    // TODO:
}

#endif