#include "Task.hpp"
#include "Arch/Arch.hpp"

MathLib::Array<Task> tasks = MathLib::MakeArrayFromSingle<Task>(Task(MathLib::FunctionPointer<void>(nullptr, nullptr), nullptr, 0));
size_t currentTask = 0;

Task::Task(void) : state(nullptr) {}
Task::Task(const MathLib::FunctionPointer<void>& function, void* stack, size_t stackSize) : state(ArchCreateTask(function, stack, stackSize)) {}
void Task::Switch(Task& next) {
    ArchSwitchTasks(state, next.state);
}
bool AddTask(const Task& task) {
    return tasks.Add(task);
}
void Schedule(void) {
    const size_t tmp = currentTask;
    currentTask = (currentTask + 1) % tasks.GetSize();
    tasks.At(tmp).Switch(tasks.At(currentTask));
}