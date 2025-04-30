#include "ThreadJob.hpp"

namespace MathLib {
    ThreadJob::ThreadJob(MathLib::Thread* thread, MathLib::Function<void, MathLib::Interval<ssize_t>>* func, const MathLib::Interval<ssize_t>& interval) : interval(interval), func(func), thread(thread), threadState(), start(true) {
        EmptyBenchmark
    }
    ThreadJob::~ThreadJob(void) {
        StartBenchmark
        delete thread;
        EndBenchmark
    }
    JobState ThreadJob::RunInternal(void) {
        StartBenchmark
        if (start) {
            start = false;
            threadState = ThreadJobState(GetState(), func->data);
            func->data = &threadState;
            thread->Run(*func, interval);
        }
        const JobState ret = threadState.jobState;
        if (ret != JobState::Running) start = true;
        ReturnFromBenchmark(ret);
    }
}