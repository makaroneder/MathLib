#ifndef MathLib_Job_ThreadState_H
#define MathLib_Job_ThreadState_H
#include "JobState.hpp"
#include <stddef.h>

namespace MathLib {
    struct ThreadJobState {
        size_t state;
        JobState jobState;
        void* data;

        ThreadJobState(void);
        ThreadJobState(size_t state, void* data);
    };
}

#endif