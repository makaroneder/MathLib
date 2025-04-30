#ifndef MathLib_Job_ShiftingQueue_H
#define MathLib_Job_ShiftingQueue_H
#include "JobArray.hpp"

namespace MathLib {
    struct ShiftingQueue : Queue {
        ShiftingQueue(JobArray* base, ssize_t shift);
        virtual ~ShiftingQueue(void) override;
        [[nodiscard]] virtual bool IsEmpty(void) const override;
        [[nodiscard]] virtual bool Add(Job* job) override;
        [[nodiscard]] virtual QueueState RunQueueInternal(void) override;

        private:
        JobArray* base;
        ssize_t shift;
    };
}

#endif