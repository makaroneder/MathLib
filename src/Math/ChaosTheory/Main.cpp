#include "LorenzSystem.hpp"
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Chaos theory", 800, 800);
        const MathLib::matrix_t epsMatrix = MathLib::CreateVector<MathLib::num_t>(MathLib::eps, MathLib::eps, MathLib::eps);
        const MathLib::matrix_t startPos = MathLib::CreateVector<MathLib::num_t>(0, 1, 1.05);
        const MathLib::matrix_t params = MathLib::CreateVector<MathLib::num_t>(10, 28, 8.0 / 3);
        LorenzSystem<MathLib::num_t> systems[] = {
            LorenzSystem<MathLib::num_t>(startPos, params),
            LorenzSystem<MathLib::num_t>(startPos + epsMatrix, params + epsMatrix),
        };
        renderer.pointMultiplier = 10;
        const MathLib::num_t dt = 0.01;
        while (true) {
            for (size_t i = 0; i < SizeOfArray(systems); i++)
                systems[i].Update(dt);
            renderer.SetPixel<MathLib::num_t>(systems[0].GetData(), 0xff0000ff);
            renderer.SetPixel<MathLib::num_t>(systems[1].GetData(), 0x00ff00ff);
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            if (renderer.GetEvent().type == MathLib::Event::Type::Quit) break;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}