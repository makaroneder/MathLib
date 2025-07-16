#include "DummyThread.hpp"
#include "Benchmark.hpp"

namespace MathLib {
    void DummyThread::Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) {
        EmptyBenchmark
        function(interval);
        EndBenchmark
    }
    void DummyThread::Join(void) {
        EmptyBenchmark
    }
}