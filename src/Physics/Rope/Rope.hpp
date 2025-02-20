#ifndef Rope_H
#define Rope_H
#include <Physics/Particle.hpp>
#include <Math/Quaternion.hpp>

template <typename T>
struct Rope : MathLib::LineShape<T> {
    Rope(const MathLib::Matrix<T>& gravity, const MathLib::Collection<MathLib::Matrix<T>>& segments) : MathLib::LineShape<T>(MathLib::CreateVector<T>(0, 0, 0)), gravity(gravity), particles(MathLib::Array<MathLib::Particle<T>>(segments.GetSize())), lengths(MathLib::Array<T>(segments.GetSize() - 1)) {
        particles.At(0) = MathLib::Particle<T>(segments.At(0), true);
        for (size_t i = 1; i < particles.GetSize(); i++) {
            particles.At(i) = MathLib::Particle<T>(segments.At(i), false);
            lengths.At(i - 1) = (segments.At(i) - segments.At(i - 1)).GetLength();
        }
    }
    constexpr void Update(const MathLib::Second<T>& dt) {
        for (MathLib::Particle<T>& particle : particles) {
            particle.Accelerate(gravity);
            particle.Update(dt);
        }
        for (size_t i = 1; i < particles.GetSize(); i++) {
            MathLib::Particle<T>& p1 = particles.At(i - 1);
            MathLib::Particle<T>& p2 = particles.At(i);
            const MathLib::Matrix<T> d1 = p2.prevPosition - p1.prevPosition;
            const T d2 = d1.GetLength();
            const MathLib::Matrix<T> tmp = d1 * (d2 - lengths.At(i - 1)) / (d2 * 2);
            if (p1.IsFixed() && !p2.IsFixed()) p2.position -= tmp * 2;
            else if (!p1.IsFixed() && p2.IsFixed()) p1.position += tmp * 2;
            else {
                p1.position += tmp;
                p2.position -= tmp;
            }
        }
    }
    [[nodiscard]] virtual MathLib::Array<MathLib::Line<T>> ToLines(const MathLib::Matrix<T>& rotation) const override {
        MathLib::Array<MathLib::Line<T>> lines = MathLib::Array<MathLib::Line<T>>((particles.GetSize() - 1) * 2);
        MathLib::Line<T> prev = particles.At(0).ToLines(rotation).At(0);
        for (size_t i = 1; i < particles.GetSize(); i++) {
            const MathLib::Line<T> tmp = particles.At(i).ToLines(rotation).At(0);
            lines.At((i - 1) * 2) = MathLib::Line<T>(MathLib::RotateVector<T>(prev.start + this->position, particles.At(0).GetPreviousPosition(), rotation), MathLib::RotateVector<T>(tmp.start + this->position, particles.At(0).GetPreviousPosition(), rotation));
            lines.At((i - 1) * 2 + 1) = MathLib::Line<T>(MathLib::RotateVector<T>(prev.end + this->position, particles.At(0).position, rotation), MathLib::RotateVector<T>(tmp.end + this->position, particles.At(0).position, rotation));
            prev = tmp;
        }
        return lines;
    }
    [[nodiscard]] virtual bool CollidesWith(const MathLib::Shape<T>&) const override {
        // TODO:
        return false;
    }

    private:
    MathLib::Matrix<T> gravity;
    MathLib::Array<MathLib::Particle<T>> particles;
    MathLib::Array<T> lengths;
    T expectedDistance;
};

#endif