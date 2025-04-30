#ifndef MathLib_RawThreadWrapper_H
#define MathLib_RawThreadWrapper_H
#include "Thread.hpp"
#include "Host.hpp"

namespace MathLib {
    struct RawThreadWrapper : Thread {
        RawThreadWrapper(void);
        RawThreadWrapper(Thread* raw);
        virtual ~RawThreadWrapper(void) override;
        virtual void Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) override;
        virtual void Join(void) override;

        private:
        Thread* raw;
    };
}

#endif