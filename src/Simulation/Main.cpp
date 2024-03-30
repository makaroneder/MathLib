#include "Rope.hpp"
#include <SDL2.hpp>
#include <iostream>

int main(void) {
    try {
        SDL2Renderer renderer = SDL2Renderer("Physics simulation", 800, 800);
        const Matrix<num_t> baseSpeed = CreateVector<num_t>(0, -1, 0);
        const Second<num_t> time = Second<num_t>(0.01);
        Rope<num_t> rope = Rope<num_t>(CreateVector<num_t>(0, 2, 0), CreateVector<num_t>(1, 0, 0), { baseSpeed, baseSpeed / 5, baseSpeed / 10, });
        while (true) {
            rope.Update(time);
            if (!renderer.DrawShape<num_t>(rope, 0, CreateVector<num_t>(0, 0, 0), 0xff0000ff)) throw std::runtime_error("Failed to draw rope");
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