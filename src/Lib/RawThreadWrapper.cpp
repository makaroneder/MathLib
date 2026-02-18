#include "RawThreadWrapper.hpp"

namespace MathLib {
    RawThreadWrapper::RawThreadWrapper(void) : raw(nullptr) {}
    RawThreadWrapper::RawThreadWrapper(Thread* raw) : raw(raw) {}
    RawThreadWrapper::~RawThreadWrapper(void) {
        DeallocThread(raw);
    }
    void RawThreadWrapper::Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) {
        raw->Run(function, interval);
    }
    void RawThreadWrapper::Join(void) {
        raw->Join();
    }
}