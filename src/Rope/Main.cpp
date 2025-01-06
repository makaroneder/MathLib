#define SDL_MAIN_HANDLED
#include "Rope.hpp"
#include <MathLib.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2Renderer renderer = MathLib::SDL2Renderer("Physics simulation", 800, 800);
        MathLib::Array<MathLib::matrix_t> points;
        for (MathLib::num_t i = 0; i <= 5; i += 0.1)
            if (!points.Add(MathLib::CreateVector<MathLib::num_t>(i, 3, 0))) MathLib::Panic("Failed to add point to the rope");
        Rope<MathLib::num_t> rope = Rope<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(0, -9.81, 0), points);
        MathLib::Second<MathLib::num_t> prevTime = MathLib::Second<MathLib::num_t>(MathLib::GetTime());
        bool pause = false;
        while (true) {
            const MathLib::Second<MathLib::num_t> currTime = MathLib::Second<MathLib::num_t>(MathLib::GetTime());
            const MathLib::Second<MathLib::num_t> dt = currTime - prevTime;
            prevTime = currTime;
            if (!pause) rope.Update(dt);
            renderer.Fill(0);
            renderer.DrawShape<MathLib::num_t>(rope, MathLib::CreateVector<MathLib::num_t>(0, 0, 0), 0xff0000ff);
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