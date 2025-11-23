#include "JobManager.hpp"
#include <Job/ShiftingQueue.hpp>
#include <Job/FIFOQueue.hpp>

MathLib::Queue* jobs = nullptr;
bool InitJobManger(void) {
    if (jobs) return false;
    jobs = new MathLib::ShiftingQueue(new MathLib::FIFOQueue(true), -1);
    return jobs;
}
bool RunJobs(void) {
    return jobs && jobs->Step() != MathLib::JobState::Failed;
}
bool AddJob(MathLib::Job* job) {
    return jobs && job && jobs->Add(job);
}