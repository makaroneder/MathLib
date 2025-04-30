#ifndef Idle_H
#define Idle_H
#include <Job/Job.hpp>

struct Idle : MathLib::Job {
    protected:
    [[nodiscard]] virtual MathLib::JobState RunInternal(void) override;
};

#endif