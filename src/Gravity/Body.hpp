#ifndef Body_H
#define Body_H
#include <Physics/Particle.hpp>

template <typename T>
struct Body : MathLib::Particle<T> {
    Body(const MathLib::Matrix<T>& position, bool fixed, T mass, const MathLib::Matrix<T>& acceleration) : MathLib::Particle<T>(position, fixed), acceleration(acceleration), mass(mass) {}
    void UpdateGravity(Body<T>& other) {
        const MathLib::Matrix<T> dist = this->position - other.position;
        if (!this->IsFixed()) this->Accelerate(-dist.Normalize() * constantOfGravitation * other.mass / dist.GetLengthSquared());
        if (!other.IsFixed()) other.Accelerate(dist.Normalize() * constantOfGravitation * mass / dist.GetLengthSquared());
    }
    virtual void Update(const MathLib::Second<T>& dt) override {
        this->Accelerate(acceleration);
        MathLib::Particle<T>::Update(dt);
    }

    private:
    static constexpr T constantOfGravitation = 1;

    MathLib::Matrix<T> acceleration;
    T mass;
};

#endif