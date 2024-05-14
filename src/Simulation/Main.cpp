#include "Rope.hpp"
#include <SDL2.cpp>
#include <iostream>

int main(void) {
    try {
        SDL2Renderer renderer = SDL2Renderer("Physics simulation", 800, 800);
        const Kilogram<num_t> baseMass = Kilogram<num_t>(1);
        const Matrix<num_t> gravity = CreateVector<num_t>(0, -9.81, 0);
        Rope<num_t> rope = Rope<num_t>(CreateVector<num_t>(0, 3, 0), {
            RopeSegment<num_t>(baseMass, CreateVector<num_t>(0, -1, 0), true),
            RopeSegment<num_t>(Kilogram<num_t>(baseMass * 5), CreateVector<num_t>(2, -1, 0), false),
            RopeSegment<num_t>(Kilogram<num_t>(baseMass * 10), CreateVector<num_t>(0, 0, 0), false),
        });
        Second<num_t> prevTime = Second<num_t>(0);
        const num_t bottom = GetY(renderer.GetStart<num_t>());
        while (true) {
            const Second<num_t> currTime = Second<num_t>((num_t)clock() / CLOCKS_PER_SEC);
            const Second<num_t> time = currTime - prevTime;
            prevTime = currTime;
            rope.Update(time / 10, [gravity, bottom](Particle<num_t> particle) -> Matrix<num_t> {
                return gravity * particle.GetMass().GetValue();
            });
            renderer.Fill(0x000000ff);
            renderer.DrawShape<num_t>(rope, CreateVector<num_t>(0, 0, 0), 0xff0000ff);
            if (!renderer.Update()) throw std::runtime_error("Failed to update UI");
            if (renderer.GetEvent().type == Event::Type::Quit) break;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}