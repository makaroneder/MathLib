#include "RawThreadWrapper.hpp"

namespace MathLib {
    RawThreadWrapper::RawThreadWrapper(void) : raw(nullptr) {
        EmptyBenchmark
    }
    RawThreadWrapper::RawThreadWrapper(Thread* raw) : raw(raw) {
        EmptyBenchmark
    }
    RawThreadWrapper::~RawThreadWrapper(void) {
        StartBenchmark
        DeallocThread(raw);
        EndBenchmark
    }
    void RawThreadWrapper::Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) {
        StartBenchmark
        raw->Run(function, interval);
        EndBenchmark
    }
    void RawThreadWrapper::Join(void) {
        StartBenchmark
        raw->Join();
        EndBenchmark
    }
}