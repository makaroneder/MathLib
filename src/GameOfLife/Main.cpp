#define SDL_MAIN_HANDLED
#include "LifeAutomaton.hpp"
#include <DummyRenderer.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Game of life", 800, 800);
        LifeAutomaton game = LifeAutomaton(10, 10, LifeAutomatonRule(LifeAutomatonRule::life));
        std::cout << game << std::endl;
        game.Set(1, 2, true);
        game.Set(2, 2, true);
        game.Set(3, 2, true);
        game.Set(1, 5, true);
        game.Set(2, 5, true);
        game.Set(3, 5, true);
        bool pause = false;
        while (true) {
            if (!pause) {
                renderer.Fill(0);
                MathLib::DummyRenderer dummy = MathLib::DummyRenderer(0, 0);
                dummy.pixels = game.ToImage(0x000000ff, UINT32_MAX).Resize(10, 10).pixels;
                if (!renderer.DrawImage<MathLib::num_t>(dummy, MathLib::CreateVector<MathLib::num_t>(0, 0, 0))) MathLib::Panic("Failed to render game UI");
                game = game.Update();
            }
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) {
                switch (event.key) {
                    case ' ': {
                        pause = !pause;
                        break;
                    }
                    default: break;
                }
            }
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}