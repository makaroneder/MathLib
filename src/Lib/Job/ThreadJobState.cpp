#include "ThreadJobState.hpp"
#include "../Typedefs.hpp"

namespace MathLib {
    ThreadJobState::ThreadJobState(void) {
        EmptyBenchmark
    }
    ThreadJobState::ThreadJobState(size_t state, void* data) : state(state), jobState(JobState::Running), data(data) {
        EmptyBenchmark
    }
}