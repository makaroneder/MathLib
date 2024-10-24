#ifdef __x86_64__
#include "RTC.hpp"

RTC::RTC(bool nmi_, Register centuryRegister) : CMOS(false), centuryRegister(centuryRegister) {
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
bool RTC::IsUpdating(void) const {
    return Read(Register::StatusRegisterA) & 1 << 7;
}
Date RTC::GetDate(void) const {
    while (IsUpdating()) {}
    Date date;
    uint8_t century = 0;
    while (true) {
        Date tmp;
        uint8_t tmpCentury = 0;
        tmp.second = Read(Register::RTCSeconds);
        tmp.minute = Read(Register::RTCMinutes);
        tmp.hour = Read(Register::RTCHours);
        tmp.day = Read(Register::RTCDay);
        tmp.month = Read(Register::RTCMonth);
        tmp.year = Read(Register::RTCYear);
        if ((uint8_t)centuryRegister) tmpCentury = Read(centuryRegister);
        if (date == tmp && century == tmpCentury) break;
        date = tmp;
        century = tmpCentury;
    }
    const uint8_t regB = Read(Register::StatusRegisterB);
    if (!(regB & 1 << 2)) {
        date.second = date.second % 16 + date.second / 16 * 10;
        date.minute = date.minute % 16 + date.minute / 16 * 10;
        date.hour = date.hour % 16 + date.hour / 16 * 10;
        date.day = date.day % 16 + date.day / 16 * 10;
        date.month = date.month % 16 + date.month / 16 * 10;
        date.year = date.year % 16 + date.year / 16 * 10;
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