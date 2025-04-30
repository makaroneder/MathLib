#include "QueueState.hpp"
#include "../Typedefs.hpp"

namespace MathLib {
    QueueState::QueueState(Job* job, JobState state) : job(job), state(state) {
        EmptyBenchmark
    }
}