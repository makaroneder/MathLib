#include "Threads.hpp"

namespace MathLib {
    Threads::Threads(size_t size) {
        StartBenchmark
        if (size > GetThreadCount()) Panic("Too many threads requested");
        for (size_t i = 0; i < size; i++) {
            Thread* thread = AllocThread();
            if (!thread || !threads.Add(thread)) Panic("Failed to allocate memory for thread");
        }
        EndBenchmark
    }
    Threads::~Threads(void) {
        StartBenchmark
        for (Thread*& thread : threads) DeallocThread(thread);
        EndBenchmark
    }
    void Threads::Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) {
        StartBenchmark
        const Array<Interval<ssize_t>> intervals = SplitJob(interval, threads.GetSize());
        for (size_t i = 0; i < threads.GetSize(); i++) threads.At(i)->Run(function, intervals.At(i));
        EndBenchmark
    }
    void Threads::Join(void) {
        StartBenchmark
        for (Thread*& thread : threads) thread->Join();
        EndBenchmark
    }
}