#ifndef Rope_H
#define Rope_H
#include "Particle.hpp"
#include <functional>

template <typename T>
struct RopeSegment {
    Kilogram<T> mass;
    Matrix<T> length;
    bool fixed;

    RopeSegment(Kilogram<T> m, Matrix<T> len, bool f) {
        mass = m;
        length = len;
        fixed = f;
    }
};
template <typename T>
struct Rope : Shape<T> {
    static constexpr size_t jakobsenIterations = 50;
    Rope(Matrix<T> start, std::vector<RopeSegment<T>> segments) {
        const Matrix<T> save = start;
        for (const RopeSegment<T>& segment : segments) {
            particles.push_back(Particle<T>(start, segment.mass, segment.fixed));
            start += segment.length;
        }
        expectedDistance = (save - start).GetLength();
    }
    constexpr void Update(Second<T> time, std::function<Matrix<T>(Particle<T>)> force) {
        for (Particle<T>& particle : particles) {
            particle.Update(time, force(particle));
            if (!particle.IsFixed()) particle.SetPosition((particle.position * 2) - particle.GetPreviousPosition() + particle.GetNewPosition(time));
        }
        for (size_t iteration = 0; iteration < jakobsenIterations; iteration++) {
            for (size_t i = 1; i < particles.size(); i++) {
                Particle<T>& p1 = particles.at(i - 1);
                Particle<T>& p2 = particles.at(i);
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
    virtual std::vector<Line<T>> ToLines(T angle, Matrix<T> axis) const override {
        std::vector<Line<T>> lines;
        Matrix<T> prev = particles.at(0).position;
        for (size_t i = 1; i < particles.size(); i++) {
            lines.push_back(Line<T>(RotateVector<T>(prev, particles.at(0).position, angle, axis), RotateVector<T>(particles.at(i).position, particles.at(0).position, angle, axis)));
            prev = lines.back().end;
        }
        return lines;
    }

    private:
    std::vector<Particle<T>> particles;
    T expectedDistance;
};

#endif