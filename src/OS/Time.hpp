#ifndef Time_H
#define Time_H
#include <Typedefs.hpp>

struct Timer : MathLib::Allocatable {
    Timer(void);
    MathLib::num_t GetTime(void) const;
    void Sleep(MathLib::num_t duration);
    virtual MathLib::num_t GetFrequency(void) const = 0;
    virtual bool SetFrequency(MathLib::num_t frequency) = 0;

    protected:
    MathLib::num_t time;
};
extern Timer* mainTimer;

#endif