#ifndef MathLib_Job_Thread_H
#define MathLib_Job_Thread_H
#include "ThreadJobState.hpp"
#include "../Thread.hpp"
#include "Job.hpp"

namespace MathLib {
    struct ThreadJob : Job {
        ThreadJob(Thread* thread, Function<void, Interval<ssize_t>>* func, const Interval<ssize_t>& interval);
        virtual ~ThreadJob(void) override;
        [[nodiscard]] virtual JobState RunInternal(void) override;

        private:
        Interval<ssize_t> interval;
        Function<void, Interval<ssize_t>>* func;
        Thread* thread;
        ThreadJobState threadState;
        bool start;
    };
}

#endif