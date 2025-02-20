#include "MovingCuboid.hpp"
#include "Tasks.hpp"
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Animation", 800, 800);
        Task<MathLib::num_t>* task = new Tasks<MathLib::num_t>(MathLib::MakeArray<Task<MathLib::num_t>*>(
            new MovingCuboid<MathLib::num_t>(2, MathLib::Cuboid<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(-4, 4, 0), MathLib::CreateVector<MathLib::num_t>(1, 1, 0.01)), MathLib::CreateVector<MathLib::num_t>(4, -4, 0)),
            new MovingCuboid<MathLib::num_t>(2, MathLib::Cuboid<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(4, -4, 0), MathLib::CreateVector<MathLib::num_t>(1, 1, 0.01)), MathLib::CreateVector<MathLib::num_t>(-4, 4, 0))
        ));
        MathLib::Second<MathLib::num_t> prev = MathLib::GetTime();
        bool pause = false;
        while (true) {
            const MathLib::Second<MathLib::num_t> curr = MathLib::GetTime();
            const MathLib::Second<MathLib::num_t> deltaTime = curr - prev;
            prev = curr;
            if (!pause) {
                renderer.Fill(0);
                if (task->Done(renderer, deltaTime)) break;
            }
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::KeyPressed && event.pressed && event.key == ' ') pause = !pause;
        }
        delete task;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}