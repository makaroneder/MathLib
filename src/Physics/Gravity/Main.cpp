#define SDL_MAIN_HANDLED
#include "Body.hpp"
#include <Physics/SIUnits.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2Renderer renderer = MathLib::SDL2Renderer("Gravity simulation", 800, 800);
        MathLib::Second<MathLib::num_t> prevTime = MathLib::Second<MathLib::num_t>(MathLib::GetTime());
        bool pause = false;
        Body<MathLib::num_t> bodies[] = {
            Body<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(1, -1, 0), false, 10, MathLib::CreateVector<MathLib::num_t>(-1, 0, 0)),
            Body<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(-1, 1, 0), false, 10, MathLib::CreateVector<MathLib::num_t>(1, 0, 0)),
        };
        while (true) {
            const MathLib::Second<MathLib::num_t> currTime = MathLib::Second<MathLib::num_t>(MathLib::GetTime());
            const MathLib::Second<MathLib::num_t> dt = currTime - prevTime;
            prevTime = currTime;
            if (!pause) {
                for (size_t i = 0; i < SizeOfArray(bodies) - 1; i++)
                    for (size_t j = i + 1; j < SizeOfArray(bodies); j++)
                        bodies[i].UpdateGravity(bodies[j]);
                for (size_t i = 0; i < SizeOfArray(bodies); i++) bodies[i].Update(dt);
            }
            renderer.Fill(0);
            for (size_t i = 0; i < SizeOfArray(bodies); i++) renderer.DrawShape<MathLib::num_t>(bodies[i], MathLib::CreateVector<MathLib::num_t>(0, 0, 0), 0xff0000ff);
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::KeyPressed && event.pressed && event.key == ' ') pause = !pause;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}