#ifndef Freestanding
#ifndef __MINGW32__
#ifndef MathLib_Libc_HostThread_H
#define MathLib_Libc_HostThread_H
#include "../Thread.hpp"
#include <thread>

namespace MathLib {
    struct HostThread : Thread {
        HostThread(void);
        virtual void Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) override;
        virtual void Join(void) override;

        private:
        std::thread thread;
    };
}

#endif
#endif
#endif