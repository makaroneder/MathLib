#ifdef __x86_64__
#ifndef RTC_H
#define RTC_H
#include "CMOS.hpp"
#include "InterruptTimer.hpp"
#include "../../DateKeeper.hpp"

struct RTC : CMOS, DateKeeper, InterruptTimer {
    RTC(bool nmi_, Register centuryRegister = (Register)0);
    virtual ~RTC(void) override;
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) override;
    virtual MathLib::num_t GetFrequency(void) const override;
    virtual bool SetFrequency(MathLib::num_t frequency) override;
    virtual Date GetDate(void) const override;
    virtual void SetDate(const Date& date) override;

    private:
    Register centuryRegister;

    bool IsUpdating(void) const;
    uint8_t GetRate(void) const;
    bool SetRate(uint8_t rate);
};

#endif
#endif