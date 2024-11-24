#include "Scheduler.hpp"
#include "Arch/Arch.hpp"

size_t currentTask = SIZE_MAX;
bool StartScheduler(const MathLib::FunctionPointer<void, size_t>& task) {
    currentTask = 0;
    return ArchCreateTask(task);
}