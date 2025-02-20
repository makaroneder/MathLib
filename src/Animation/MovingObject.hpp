#ifndef MovingObject_H
#define MovingObject_H
#include "WaitingTask.hpp"
#include <Math/LinearInterpolation.hpp>

template <typename T>
struct MovingObject : WaitingTask<T> {
    MovingObject(const MathLib::Second<T>& duration, const MathLib::Line<T>& line) : WaitingTask<T>(duration), position(line.start), line(line) {}
    virtual T Update(MathLib::Renderer& renderer, const MathLib::Second<T>& deltaTime) override {
        const T ret = WaitingTask<T>::Update(renderer, deltaTime);
        position = MathLib::Lerp<MathLib::Matrix<T>, T>(line.start, line.end, ret);
        return ret;
    }
    MathLib::Matrix<T> GetPosition(void) const {
        return position;
    }

    private:
    MathLib::Matrix<T> position;
    MathLib::Line<T> line;
};

#endif