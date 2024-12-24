#ifndef MathLib_DummyThread_H
#define MathLib_DummyThread_H
#include "Thread.hpp"

namespace MathLib {
    struct DummyThread : Thread {
        virtual void Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) override;
        virtual void Join(void) override;
    };
}

#endif