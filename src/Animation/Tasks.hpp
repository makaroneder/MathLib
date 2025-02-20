#ifndef Tasks_H
#define Tasks_H
#include "Task.hpp"

template <typename T>
struct Tasks : Task<T> {
    Tasks(MathLib::Array<Task<T>*> tasks) : tasks(tasks), current(0) {}
    virtual ~Tasks(void) override {
        for (Task<T>*& task : tasks) delete task;
    }
    virtual T Update(MathLib::Renderer& renderer, const MathLib::Second<T>& deltaTime) override {
        if (current == tasks.GetSize()) return 1;
        if (tasks.At(current)->Done(renderer, deltaTime)) current++;
        return (T)current / tasks.GetSize();
    }

    private:
    MathLib::Array<Task<T>*> tasks;
    size_t current;
};

#endif