#include "Job.hpp"
#include "../Typedefs.hpp"

namespace MathLib {
    Job::Job(void) : state(0) {
        EmptyBenchmark
    }
    size_t Job::GetState(void) const {
        StartAndReturnFromBenchmark(state);
    }
    JobState Job::Step(void) {
        StartBenchmark
        const JobState ret = RunInternal();
        if (ret == JobState::Done) state++;
        ReturnFromBenchmark(ret);
    }
    bool Job::Run(void) {
        StartBenchmark
        while (true) {
            const JobState ret = Step();
            if (ret == JobState::Done) ReturnFromBenchmark(true);
            if (ret == JobState::Failed) ReturnFromBenchmark(false);
        }
    }
}