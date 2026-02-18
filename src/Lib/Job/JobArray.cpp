#include "JobArray.hpp"

namespace MathLib {
    JobArray::~JobArray(void) {
        for (Job*& job : jobs) delete job;
    }
    bool JobArray::IsEmpty(void) const {
        return jobs.IsEmpty();
    }
    bool JobArray::Add(Job* job) {
        return job && jobs.Add(job);
    }
}