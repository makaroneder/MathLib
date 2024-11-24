#ifdef __x86_64__
#include "../../Scheduler.hpp"
#include "Scheduler.hpp"
#include "../Arch.hpp"
#include <Logger.hpp>

MathLib::Array<Task> tasks;

Task::Task(void) {}
Task::Task(const MathLib::FunctionPointer<void, size_t>& entry) : state(State::None), entry(entry) {}
void Task::Handle(Registers* prev, size_t index) {
    if (state == State::None) {
        regs = *prev;
        regs.ip = (uintptr_t)entry.GetFunction();
        regs.di = (uintptr_t)entry.GetData();
        regs.si = index;
        state = State::Running;
    }
    MathLib::Swap<Registers>(regs, *prev);
}
void Schedule(const void*, Registers* regs) {
    if (currentTask != SIZE_MAX) {
        tasks.At(currentTask).regs = *regs;
        do {
            currentTask = (currentTask + 1) % tasks.GetSize();
        } while (tasks.At(currentTask).state == Task::State::Dead);
        tasks.At(currentTask).Handle(regs, currentTask);
    }
}
bool ArchCreateTask(const MathLib::FunctionPointer<void, size_t>& task) {
    return tasks.Add(Task(task));
}
bool ArchDestroyTask(size_t task) {
    if (task >= tasks.GetSize()) return false;
    if (task + 1 == tasks.GetSize()) tasks = MathLib::Array<Task>(tasks.GetValue(), task);
    else tasks.At(task).state = Task::State::Dead;
    return true;
}

#endif