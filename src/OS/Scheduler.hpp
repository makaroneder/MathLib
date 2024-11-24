#ifndef Scheduler_H
#define Scheduler_H
#include <FunctionPointer.hpp>

extern size_t currentTask;

bool StartScheduler(const MathLib::FunctionPointer<void, size_t>& task);

#endif