#ifndef RTC_H
#define RTC_H
#include "CMOS.hpp"
#include "../../Time.hpp"
#include "../../DateKeeper.hpp"
#include "Interrupts/Interrupts.hpp"

struct RTC : CMOS, Timer, DateKeeper, InterruptDevice {
    RTC(bool nmi_, Register centuryRegister = (Register)0);
    virtual ~RTC(void) override;
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) override;
    virtual num_t GetFrequency(void) const override;
    virtual bool SetFrequency(num_t frequency) override;
    virtual Date GetDate(void) const override;
    virtual void SetDate(const Date& date) override;

    private:
    Register centuryRegister;

    bool IsUpdating(void) const;
    uint8_t GetRate(void) const;
    bool SetRate(uint8_t rate);
};

#endif