#include "DummyThread.hpp"
#include "Threads.hpp"

namespace MathLib {
    void Thread::RunAndJoin(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) {
        StartBenchmark
        Run(function, interval);
        Join();
        EndBenchmark
    }
    void RunParallel(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) {
        const size_t threads = GetThreadCount();
        Thread* thread = threads ? (Thread*)new Threads(threads) : (Thread*)new DummyThread();
        thread->RunAndJoin(function, interval);
        delete thread;
    }
    Array<Interval<ssize_t>> SplitJob(const Interval<ssize_t>& interval, size_t threads) {
        StartBenchmark
        Array<Interval<ssize_t>> ret = Array<Interval<ssize_t>>(threads);
        const num_t step = interval.GetSize() / threads;
        for (size_t i = 0; i < threads; i++) {
            const num_t sectionStart = interval.GetMin() + i * step;
            ret.At(i) = Interval<ssize_t>(sectionStart, (i == threads - 1) ? interval.GetMax() : sectionStart + step - 1);
        }
        ReturnFromBenchmark(ret);
    }
}