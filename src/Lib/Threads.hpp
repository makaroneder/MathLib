#ifndef Threads_H
#define Threads_H
#include "Thread.hpp"
#include "Host.hpp"

namespace MathLib {
    struct Threads : Thread {
        Threads(size_t size = GetThreadCount());
        virtual ~Threads(void) override;
        virtual void Run(const Function<void, Interval<ssize_t>>& function, const Interval<ssize_t>& interval) override;
        virtual void Join(void) override;

        private:
        Array<Thread*> threads;
    };
}

#endif