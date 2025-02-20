#define SDL_MAIN_HANDLED
#include "FreeParticleWaveFunction.hpp"
#include <Math/FourierTransform.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Quantum simulation", 800, 800);
        renderer.fillGapsInFunctions = false;
        const MathLib::num_t a = 1;
        const FreeParticleWaveFunction<MathLib::num_t> wave = FreeParticleWaveFunction<MathLib::num_t>(new MathLib::HostFunction<MathLib::complex_t, MathLib::num_t>([&a] (MathLib::num_t k) -> MathLib::complex_t {
            return MathLib::Pow(a / MathLib::pi, 1 / 4) * MathLib::Exp(-a * k * k / 2);
        }), 1, 50, MathLib::IntegrationType::SimpsonRule);
        MathLib::num_t t = 0;
        bool update = true;
        while (true) {
            if (update) {
                MathLib::num_t time = MathLib::GetTime();
                renderer.Fill(0);
                renderer.DrawAxis<MathLib::num_t>(0xffffffff, 0x808080ff, 1);
                renderer.DrawFunction<MathLib::num_t>(renderer.GenerateMultiFunction<MathLib::num_t>(MathLib::HostFunction<MathLib::Array<MathLib::num_t>, MathLib::num_t>([&wave, &t] (MathLib::num_t x) -> MathLib::Array<MathLib::num_t> {
                    return MathLib::MakeArray<MathLib::num_t>(wave(x, t).GetLengthSquared());
                })), 0xff0000ff);
                update = false;
                std::cout << MathLib::GetTime() - time << std::endl;
            }
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::KeyPressed && event.pressed && event.key == ' ') {
                update = true;
                t += 1e33;
            }
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}