#ifndef JobManager_H
#define JobManager_H
#include <Job/Job.hpp>

[[nodiscard]] bool InitJobManger(void);
[[nodiscard]] bool RunJobs(void);
[[nodiscard]] bool AddJob(MathLib::Job* job);

#endif