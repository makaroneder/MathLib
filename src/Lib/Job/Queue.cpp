#include "Queue.hpp"
#include "../Benchmark.hpp"

namespace MathLib {
    JobState Queue::RunInternal(void) {
        StartBenchmark
        if (IsEmpty()) ReturnFromBenchmark(JobState::Done);
        const QueueState queueState = RunQueueInternal();
        switch (queueState.state) {
            case JobState::Failed: {
                delete queueState.job;
                ReturnFromBenchmark(JobState::Failed);
            }
            case JobState::Running: ReturnFromBenchmark(JobState::Running);
            case JobState::Done: {
                delete queueState.job;
                ReturnFromBenchmark(IsEmpty() ? JobState::Done : JobState::Running);
            }
            default: ReturnFromBenchmark(JobState::Failed);
        }
    }
}