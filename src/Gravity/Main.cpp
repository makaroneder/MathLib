#include "PhysicsObject.hpp"
#include <SDL2.cpp>
#include <iostream>

int main(void) {
    try {
        SDL2Renderer renderer = SDL2Renderer("Gravity simulation", 800, 800);
        const Second<num_t> time = Second<num_t>(0.1);
        std::vector<PhysicsObject<num_t>> objects = {
            PhysicsObject<num_t>(CreateVector<num_t>(0, 0, 0), CreateVector<num_t>(0.5, 0.5, 0.01), Kilogram<num_t>(1e7), CreateVector<num_t>(0, 0, 0)),
            PhysicsObject<num_t>(CreateVector<num_t>(1, 1, 0), CreateVector<num_t>(0.1, 0.1, 0.01), Kilogram<num_t>(1e2), CreateVector<num_t>(0.2, 0.1, 0)),
        };
        while (true) {
            renderer.Fill(0x000000ff);
            for (size_t i = 0; i < objects.size(); i++) {
                renderer.DrawShape<num_t>(objects[i], CreateVector<num_t>(0, 0, 0), 0xff0000ff);
                for (size_t j = i + 1; j < objects.size(); j++) objects[i].ApplyGravity(objects[j], time);
            }
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