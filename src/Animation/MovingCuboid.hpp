#ifndef MovingCuboid_H
#define MovingCuboid_H
#include "MovingObject.hpp"
#include <Geometry/Cuboid.hpp>

template <typename T>
struct MovingCuboid : MovingObject<T> {
    MovingCuboid(const MathLib::Second<T>& duration, const MathLib::Cuboid<T>& cuboid, const MathLib::Matrix<T>& end) : MovingObject<T>(duration, MathLib::Line<T>(cuboid.position, end)), cuboid(cuboid) {}
    virtual T Update(MathLib::Renderer& renderer, const MathLib::Second<T>& deltaTime) override {
        const T ret = MovingObject<T>::Update(renderer, deltaTime);
        cuboid.position = this->GetPosition();
        renderer.DrawShape<T>(cuboid, MathLib::CreateVector<T>(0, 0, 0), UINT32_MAX);
        return ret;
    }

    private:
    MathLib::Cuboid<T> cuboid;
};

#endif