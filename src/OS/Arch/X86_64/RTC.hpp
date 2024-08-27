#ifndef RTC_H
#define RTC_H
#include "CMOS.hpp"
#include "../../Time.hpp"
#include "Interrupts/Interrupts.hpp"

// TODO: Add functions to get and set date
struct RTC : CMOS, Timer, InterruptDevice {
    RTC(bool nmi_);
    virtual ~RTC(void) override;
    virtual void OnInterrupt(uintptr_t interrupt, Registers* regs, uintptr_t error) override;
    virtual num_t GetFrequency(void) const override;
    virtual bool SetFrequency(num_t frequency) override;

    private:
    uint8_t GetRate(void) const;
    bool SetRate(uint8_t rate);
};

#endif