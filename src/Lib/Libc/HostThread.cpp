#ifndef Freestanding
#ifndef __MINGW32__
#include "HostThread.hpp"

namespace MathLib {
    HostThread::HostThread(void) {
        EmptyBenchmark
    }
    void HostThread::Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) {
        StartBenchmark
        thread = std::thread([&function, interval](void) -> void {
            function(interval);
        });
        EndBenchmark
    }
    void HostThread::Join(void) {
        StartBenchmark
        thread.join();
        EndBenchmark
    }
}

#endif
#endif