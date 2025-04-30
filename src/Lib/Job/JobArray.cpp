#include "JobArray.hpp"

namespace MathLib {
    JobArray::~JobArray(void) {
        StartBenchmark
        for (Job*& job : jobs) delete job;
        EndBenchmark
    }
    bool JobArray::IsEmpty(void) const {
        StartAndReturnFromBenchmark(jobs.IsEmpty());
    }
    bool JobArray::Add(Job* job) {
        StartAndReturnFromBenchmark(job && jobs.Add(job));
    }
}