#ifndef MathLib_Job_FIFOQueue_H
#define MathLib_Job_FIFOQueue_H
#include "JobArray.hpp"

namespace MathLib {
    struct FIFOQueue : JobArray {
        FIFOQueue(bool fifo);
        [[nodiscard]] virtual QueueState RunQueueInternal(void) override;
        
        private:
        bool fifo;
    };
}

#endif