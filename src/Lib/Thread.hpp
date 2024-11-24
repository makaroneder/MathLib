#ifndef Thread_H
#define Thread_H
#include "Interfaces/Function.hpp"
#include "Interval.hpp"

namespace MathLib {
    struct Thread : Allocatable {
        virtual void Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) = 0;
        virtual void Join(void) = 0;
        void RunAndJoin(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval);
    };
    Array<Interval<ssize_t>> SplitJob(const Interval<ssize_t>& interval, size_t threads);
}

#endif