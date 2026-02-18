#include "Threads.hpp"

namespace MathLib {
    Threads::Threads(size_t size) {
        if (size > GetThreadCount()) Panic("Too many threads requested");
        for (size_t i = 0; i < size; i++) {
            Thread* thread = AllocThread();
            if (!thread || !threads.Add(thread)) Panic("Failed to allocate memory for thread");
        }
    }
    Threads::~Threads(void) {
        for (Thread*& thread : threads) DeallocThread(thread);
    }
    void Threads::Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) {
        const Array<Interval<ssize_t>> intervals = SplitJob(interval, threads.GetSize());
        for (size_t i = 0; i < threads.GetSize(); i++) threads.AtUnsafe(i)->Run(function, intervals.AtUnsafe(i));
    }
    void Threads::Join(void) {
        for (Thread*& thread : threads) thread->Join();
    }
}