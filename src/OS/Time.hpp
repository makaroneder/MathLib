#ifndef Time_H
#define Time_H
#include <Typedefs.hpp>

struct Timer : MathLib::Allocatable {
    Timer(void);
    [[nodiscard]] MathLib::num_t GetTime(void) const;
    void Sleep(MathLib::num_t duration);
    [[nodiscard]] virtual MathLib::num_t GetFrequency(void) const = 0;
    [[nodiscard]] virtual bool SetFrequency(MathLib::num_t frequency) = 0;

    protected:
    MathLib::num_t time;
};
extern Timer* mainTimer;

#endif