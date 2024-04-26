#ifndef Object_H
#define Object_H
#include <Renderer.hpp>
#include <Geometry/Cuboid.hpp>
#include <Physics/SIUnits.hpp>

/// @brief Physics object
/// @tparam T Type of number
template <typename T>
struct Object {
    /// @brief Creates a new object
    /// @param shape Shape of the object
    /// @param vel Velocity of the object
    /// @param c Color of the object
    Object(Cuboid<T> shape = Cuboid<T>(), MetrePerSecond<T> vel = MetrePerSecond<T>(), uint32_t c = 0) {
        cuboid = shape;
        velocity = vel;
        color = c;
    }
    /// @brief Draws the object
    /// @param renderer Renderer to draw the object with
    void Draw(Renderer& renderer) const {
        renderer.DrawShape<T>(cuboid, 0, CreateVector<T>(0, 0, 0), color);
    }
    /// @brief Shape of the object
    Cuboid<T> cuboid;
    /// @brief Velocity of the object
    MetrePerSecond<T> velocity;
    /// @brief Color of the object
    uint32_t color;
};

#endif