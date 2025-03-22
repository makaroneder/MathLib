#define SDL_MAIN_HANDLED
#include "LifeAutomaton.hpp"
#include <DummyRenderer.hpp>
#include <SDL2.cpp>
#include <iostream>

void MakeGameOfLifeBlinker(LifeAutomaton& game, const MathLib::Matrix<size_t>& position) {
    game.Set(GetX(position) - 1, GetY(position), true);
    game.Set(GetX(position) + 0, GetY(position), true);
    game.Set(GetX(position) + 1, GetY(position), true);
}
void MakeGameOfLifeGlider(LifeAutomaton& game, const MathLib::Matrix<size_t>& position) {
    game.Set(GetX(position) + 1, GetY(position) - 1, true);
    game.Set(GetX(position) + 0, GetY(position) + 0, true);
    game.Set(GetX(position) + 0, GetY(position) + 1, true);
    game.Set(GetX(position) + 1, GetY(position) + 1, true);
    game.Set(GetX(position) + 2, GetY(position) + 1, true);
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Game of life", 500, 500);
        LifeAutomaton game = LifeAutomaton(50, 50, MathLib::String(LifeAutomaton::life));
        std::cout << game << std::endl;
        MakeGameOfLifeBlinker(game, MathLib::CreateVector<size_t>(game.GetWidth() / 2 + 11, game.GetHeight() / 2 - 15, 0));
        MakeGameOfLifeGlider(game, MathLib::CreateVector<size_t>(game.GetWidth() / 2, game.GetHeight() / 2, 0));
        bool pause = false;
        while (true) {
            if (!pause) {
                renderer.Fill(0);
                renderer.DrawImage<MathLib::num_t>(game.ToImage(0x000000ff, UINT32_MAX).Resize(10, 10), MathLib::CreateVector<MathLib::num_t>(0, 0, 0));
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