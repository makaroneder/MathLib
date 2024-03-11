#include <Physics.hpp>
#include <SDL2.hpp>
#include <iostream>
#include <vector>

template <typename T>
struct Particle {
    Particle(Vector3<T> pos, Vector3<T> vel, bool f) {
        position = prevPosition = pos;
        velocity = vel;
        fixed = f;
    }
    constexpr void SetPosition(Vector3<T> pos) {
        const Vector3<T> tmp = position;
        position = pos;
        prevPosition = tmp;
    }
    constexpr Vector3<T> GetPreviousPosition(void) const {
        return prevPosition;
    }
    constexpr bool IsFixed(void) const {
        return fixed;
    }
    Vector3<T> position;
    Vector3<T> velocity;

    private:
    Vector3<T> prevPosition;
    bool fixed;
};
template <typename T>
struct Rope {
    static constexpr size_t jakobsenIterations = 50;
    Rope(Vector3<T> start, Vector3<T> end, size_t particleCount, std::vector<Vector3<T>> velocities) {
        for (T i = 0; i < particleCount; i++) {
            const T w = i / (particleCount - 1);
            particles.push_back(Particle<T>(end * w + start * (1 - w), velocities.at(i), i == 0));
        }
        expectedDistance = (start - end).GetLength();
    }
    constexpr void Update(Second<T> time) {
        for (Particle<T>& particle : particles)
            if (!particle.IsFixed()) particle.SetPosition((particle.position * 2) - particle.GetPreviousPosition() + particle.velocity * time.GetValue());
        for (size_t iteration = 0; iteration < jakobsenIterations; iteration++){
            for (size_t i = 1; i < particles.size(); i++) {
                Particle<T>& p1 = particles.at(i - 1);
                Particle<T>& p2 = particles.at(i);
                const T distanceError = (p1.position - p2.position).GetLength() - expectedDistance;
                const Vector3<T> dir = (p2.position - p1.position).Normalize();
                if (p1.IsFixed() && !p2.IsFixed()) p2.position -= dir * distanceError;
                else if (p2.IsFixed() && !p1.IsFixed()) p1.position += dir * distanceError;
                else if (!p1.IsFixed() && !p2.IsFixed()) {
                    p1.position += dir * distanceError * 0.5;
                    p2.position -= dir * distanceError * 0.5;
                }
            }
        }
    }
    constexpr std::vector<Line<T>> ToLines(void) const {
        std::vector<Line<T>> lines;
        for (size_t i = 1; i < particles.size(); i++)
            lines.push_back(Line<T>(particles.at(i - 1).position, particles.at(i).position));
        return lines;
    }

    MetrePerSecondSquared<T> acceleration;
    Vector3<T> accelerationDirection;
    private:
    std::vector<Particle<T>> particles;
    T expectedDistance;
};
int main(void) {
    try {
        SDL2Renderer renderer = SDL2Renderer("Physics Simulation", 800, 800);
        const Vector3<num_t> baseSpeed = CreateVector3<num_t>(0, -1, 0);
        const Second<num_t> time = Second<num_t>(0.01);
        Rope<num_t> rope = Rope<num_t>(CreateVector3<num_t>(0, 2, 0), CreateVector3<num_t>(1, 0, 0), 3, { baseSpeed, baseSpeed / 5, baseSpeed / 10, });
        while (true) {
            rope.Update(time);
            const std::vector<Line<num_t>> lines = rope.ToLines();
            for (const Line<num_t>& line : lines) renderer.DrawLine(line, 0xff0000ff);
            renderer.Update();
            const SDL_Event event = renderer.GetEvent();
            if (event.type == SDL_QUIT) break;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}