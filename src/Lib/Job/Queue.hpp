#ifndef MathLib_Job_Queue_H
#define MathLib_Job_Queue_H
#include "QueueState.hpp"

namespace MathLib {
    struct Queue : Job {
        [[nodiscard]] virtual bool IsEmpty(void) const = 0;
        [[nodiscard]] virtual bool Add(Job* job) = 0;
        [[nodiscard]] virtual QueueState RunQueueInternal(void) = 0;
        [[nodiscard]] virtual JobState RunInternal(void) override;
    };
}

#endif