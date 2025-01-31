#ifndef Task_H
#define Task_H
#include <FunctionPointer.hpp>

struct Task {
    Task(void);
    Task(const MathLib::FunctionPointer<void>& function, void* stack, size_t stackSize);
    void Switch(Task& next);

    private:
    void* state;
};
[[nodiscard]] bool AddTask(const Task& task);
void Schedule(void);

#endif