#include "Threads.hpp"

Threads::Threads(size_t size) {
    for (size_t i = 0; i < size && GetThreadCount(); i++) threads.Add(AllocThread());
}
Threads::~Threads(void) {
    for (size_t i = 0; i < threads.GetSize(); i++) DeallocThread(threads.At(i));
}
void Threads::Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) {
    const Array<Interval<ssize_t>> intervals = SplitJob(interval, threads.GetSize());
    for (size_t i = 0; i < threads.GetSize(); i++) threads.At(i)->Run(function, intervals.At(i));
}
void Threads::Join(void) {
    for (Thread*& thread : threads) thread->Join();
}