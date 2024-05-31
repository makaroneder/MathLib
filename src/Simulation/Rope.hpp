#ifndef Rope_H
#define Rope_H
#include "Particle.hpp"
#include <functional>

template <typename T>
struct RopeSegment {
    Kilogram<T> mass;
    Matrix<T> length;
    bool fixed;

    RopeSegment(Kilogram<T> m, Matrix<T> len, bool f) : mass(m), length(len), fixed(f) {}
};
template <typename T>
struct Rope : LineShape<T> {
    static constexpr size_t jakobsenIterations = 50;
    Rope(Matrix<T> start, Array<RopeSegment<T>> segments) : LineShape<T>(CreateVector<T>(0, 0, 0)) {
        const Matrix<T> save = start;
        for (size_t i = 0; i < segments.GetSize(); i++) {
            particles.Add(Particle<T>(start, segments.At(i).mass, segments.At(i).fixed));
            start += segments.At(i).length;
        }
        expectedDistance = (save - start).GetLength();
    }
    constexpr void Update(Second<T> time, std::function<Matrix<T>(Particle<T>)> force) {
        for (size_t i = 0; i < particles.GetSize(); i++) {
            particles.At(i).Update(time, force(particles.At(i)));
            if (!particles.At(i).IsFixed()) particles.At(i).SetPosition((particles.At(i).position * 2) - particles.At(i).GetPreviousPosition() + particles.At(i).GetNewPosition(time));
        }
        for (size_t iteration = 0; iteration < jakobsenIterations; iteration++) {
            for (size_t i = 1; i < particles.GetSize(); i++) {
                Particle<T>& p1 = particles.At(i - 1);
                Particle<T>& p2 = particles.At(i);
                const T distanceError = (p1.position - p2.position).GetLength() - expectedDistance;
                const Matrix<T> dir = (p2.position - p1.position).Normalize();
                if (p1.IsFixed() && !p2.IsFixed()) p2.position -= dir * distanceError;
                else if (p2.IsFixed() && !p1.IsFixed()) p1.position += dir * distanceError;
                else if (!p1.IsFixed() && !p2.IsFixed()) {
                    p1.position += dir * distanceError * 0.5;
                    p2.position -= dir * distanceError * 0.5;
                }
            }
        }
    }
    virtual Array<Line<T>> ToLines(Matrix<T> rotation) const override {
        Array<Line<T>> lines;
        Matrix<T> prev = particles.At(0).position + this->position;
        for (size_t i = 1; i < particles.GetSize(); i++) {
            const Line<T> tmp = Line<T>(RotateVector<T>(prev, particles.At(0).position, rotation), RotateVector<T>(particles.At(i).position, particles.At(0).position, rotation));
            lines.Add(tmp);
            prev = tmp.end + this->position;
        }
        return lines;
    }
    virtual bool CollidesWith(const Shape<T>&) const override {
        // TODO:
        return false;
    }

    private:
    Array<Particle<T>> particles;
    T expectedDistance;
};

#endif