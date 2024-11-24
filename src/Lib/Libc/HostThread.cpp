#ifndef Freestanding
#include "HostThread.hpp"

namespace MathLib {
    HostThread::HostThread(void) {}
    void HostThread::Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) {
        thread = std::thread([&function, interval](void) -> void {
            function(interval);
        });
    }
    void HostThread::Join(void) {
        thread.join();
    }
}

#endif