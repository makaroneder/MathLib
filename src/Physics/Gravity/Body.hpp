#ifndef Body_H
#define Body_H
#include <Physics/Particle.hpp>
#include <Renderer.hpp>

template <typename T>
struct Body : MathLib::Particle<T> {
    Body(const MathLib::Matrix<T>& position, bool fixed, T mass) : MathLib::Particle<T>(position, fixed), mass(mass) {}
    void UpdateGravity(Body<T>& other) {
        const MathLib::Matrix<T> dist = this->position - other.position;
        const T distLen = MathLib::Sqrt(dist.GetLengthSquared() + MathLib::eps * MathLib::eps) + MathLib::eps;
        if (!this->IsFixed()) this->Accelerate(-dist.Normalize() * constantOfGravitation * other.mass / distLen);
        if (!other.IsFixed()) other.Accelerate(dist.Normalize() * constantOfGravitation * mass / distLen);
    }
    void Draw(MathLib::Renderer& renderer, uint32_t color) const {
        renderer.FillCircle2D<T>(this->prevPosition, 0.25, color);
    }

    private:
    static constexpr T constantOfGravitation = 1;

    T mass;
};

#endif