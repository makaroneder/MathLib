#include "DummyThread.hpp"

namespace MathLib {
    void DummyThread::Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) {
        function(interval);
    }
    void DummyThread::Join(void) {}
}