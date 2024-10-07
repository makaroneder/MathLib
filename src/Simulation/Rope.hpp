#ifndef Rope_H
#define Rope_H
#include <Physics/Particle.hpp>
#include <Math/Quaternion.hpp>

template <typename T>
struct Rope : LineShape<T> {
    Rope(const Matrix<T>& gravity, const Array<Matrix<T>>& segments) : LineShape<T>(CreateVector<T>(0, 0, 0)), gravity(gravity), particles(Array<Particle<T>>(segments.GetSize())), lengths(Array<T>(segments.GetSize() - 1)) {
        particles.At(0) = Particle<T>(segments.At(0), true);
        for (size_t i = 1; i < particles.GetSize(); i++) {
            particles.At(i) = Particle<T>(segments.At(i), false);
            lengths.At(i - 1) = (segments.At(i) - segments.At(i - 1)).GetLength();
        }
    }
    constexpr void Update(const Second<T>& dt) {
        for (Particle<T>& particle : particles) {
            particle.Accelerate(gravity);
            particle.Update(dt);
        }
        for (size_t i = 1; i < particles.GetSize(); i++) {
            Particle<T>& p1 = particles.At(i - 1);
            Particle<T>& p2 = particles.At(i);
            const Matrix<T> d1 = p2.prevPosition - p1.prevPosition;
            const T d2 = d1.GetLength();
            const Matrix<T> tmp = d1 * (d2 - lengths.At(i - 1)) / (d2 * 2);
            if (p1.IsFixed() && !p2.IsFixed()) p2.position -= tmp * 2;
            else if (!p1.IsFixed() && p2.IsFixed()) p1.position += tmp * 2;
            else {
                p1.position += tmp;
                p2.position -= tmp;
            }
        }
    }
    virtual Array<Line<T>> ToLines(const Matrix<T>& rotation) const override {
        Array<Line<T>> lines;
        Line<T> prev = particles.At(0).ToLines(rotation).At(0);
        for (size_t i = 1; i < particles.GetSize(); i++) {
            const Line<T> tmp = particles.At(i).ToLines(rotation).At(0);
            lines.Add(Line<T>(RotateVector<T>(prev.start + this->position, particles.At(0).GetPreviousPosition(), rotation), RotateVector<T>(tmp.start + this->position, particles.At(0).GetPreviousPosition(), rotation)));
            lines.Add(Line<T>(RotateVector<T>(prev.end + this->position, particles.At(0).position, rotation), RotateVector<T>(tmp.end + this->position, particles.At(0).position, rotation)));
            prev = tmp;
        }
        return lines;
    }
    virtual bool CollidesWith(const Shape<T>&) const override {
        // TODO:
        return false;
    }

    private:
    Matrix<T> gravity;
    Array<Particle<T>> particles;
    Array<T> lengths;
    T expectedDistance;
};

#endif