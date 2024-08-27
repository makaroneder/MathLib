#ifndef Time_H
#define Time_H
#include <Typedefs.hpp>

struct Timer : Allocatable {
    Timer(void);
    num_t GetTime(void) const;
    void Sleep(num_t duration);
    virtual num_t GetFrequency(void) const = 0;
    virtual bool SetFrequency(num_t frequency) = 0;

    protected:
    num_t time;
};
extern Timer* mainTimer;

#endif