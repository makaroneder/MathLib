#include "Job.hpp"

namespace MathLib {
    Job::Job(void) : state(0) {}
    size_t Job::GetState(void) const {
        return state;
    }
    JobState Job::Step(void) {
        const JobState ret = RunInternal();
        if (ret == JobState::Done) state++;
        return ret;
    }
    bool Job::Run(void) {
        while (true) {
            const JobState ret = Step();
            if (ret == JobState::Done) return true;
            if (ret == JobState::Failed) return false;
        }
    }
}