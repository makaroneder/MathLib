#ifndef Task_H
#define Task_H
#include <Physics/SIUnits.hpp>
#include <Renderer.hpp>

template <typename T>
struct Task : MathLib::Allocatable {
    virtual T Update(MathLib::Renderer& renderer, const MathLib::Second<T>& deltaTime) = 0;
    bool Done(const T& progress) const {
        return progress >= 1;
    }
    bool Done(MathLib::Renderer& renderer, const MathLib::Second<T>& deltaTime) {
        return Done(Update(renderer, deltaTime));
    }
};

#endif