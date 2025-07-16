#include "QueueState.hpp"
#include "../Benchmark.hpp"

namespace MathLib {
    QueueState::QueueState(Job* job, JobState state) : job(job), state(state) {
        EmptyBenchmark
    }
}