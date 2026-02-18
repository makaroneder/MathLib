#include "Queue.hpp"

namespace MathLib {
    JobState Queue::RunInternal(void) {
        if (IsEmpty()) return JobState::Done;
        const QueueState queueState = RunQueueInternal();
        switch (queueState.state) {
            case JobState::Failed: {
                delete queueState.job;
                return JobState::Failed;
            }
            case JobState::Running: return JobState::Running;
            case JobState::Done: {
                delete queueState.job;
                return IsEmpty() ? JobState::Done : JobState::Running;
            }
            default: return JobState::Failed;
        }
    }
}