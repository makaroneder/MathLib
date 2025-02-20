#ifndef WaitingTask_H
#define WaitingTask_H
#include "Task.hpp"

template <typename T>
struct WaitingTask : Task<T> {
    WaitingTask(const MathLib::Second<T>& duration) : current(0), duration(duration) {}
    virtual T Update(MathLib::Renderer&, const MathLib::Second<T>& deltaTime) override {
        if (current.GetValue() >= duration.GetValue()) return 1;
        current += deltaTime;
        return (current / duration).GetValue();
    }

    private:
    MathLib::Second<T> current;
    MathLib::Second<T> duration;
};

#endif