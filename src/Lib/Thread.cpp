#include "Thread.hpp"

namespace MathLib {
    void Thread::RunAndJoin(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) {
        Run(function, interval);
        Join();
    }
    Array<Interval<ssize_t>> SplitJob(const Interval<ssize_t>& interval, size_t threads) {
        Array<Interval<ssize_t>> ret = Array<Interval<ssize_t>>(threads);
        const num_t step = interval.GetSize() / threads;
        for (size_t i = 0; i < threads; i++) {
            const num_t sectionStart = interval.GetMin() + i * step;
            ret.At(i) = Interval<ssize_t>(sectionStart, (i == threads - 1) ? interval.GetMax() : sectionStart + step - 1);
        }
        return ret;
    }
}