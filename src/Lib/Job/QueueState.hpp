#ifndef MathLib_Job_QueueState_H
#define MathLib_Job_QueueState_H
#include "Job.hpp"

namespace MathLib {
    struct QueueState {
        Job* job;
        JobState state;

        QueueState(Job* job, JobState state);
    };
}

#endif