#define SDL_MAIN_HANDLED
#include "Box.hpp"
#include <Image/Aseprite/Aseprite.hpp>
#include <Libc/HostFileSystem.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::HostFileSystem fs;
        const MathLib::String path = "src/TestPrograms/FactorySimulator/";
        MathLib::Aseprite background;
        if (!background.LoadFromPath(fs, path + "Background.aseprite")) MathLib::Panic("Failed to load background image");
        MathLib::Aseprite miner;
        if (!miner.LoadFromPath(fs, path + "Miner.aseprite")) MathLib::Panic("Failed to load machines");

        const size_t width = background.GetWidth();
        const size_t height = background.GetHeight();
        const size_t boxWidth = 32;
        const size_t boxHeight = 32;
        const size_t boxCountX = width / boxWidth;
        const size_t boxCountY = height / boxHeight - 2;
        const size_t centerX = width / 2;
        const size_t centerY = height / 2 - boxHeight;

        size_t drawMode = 0;
        MathLib::Matrix<Box> world = MathLib::Matrix<Box>(boxCountX, boxCountY);
        world.AtUnsafe(boxCountX / 2, boxCountY / 2).machine = &miner;
        (void)world.AtUnsafe(boxCountX / 2, boxCountY / 2).materials.SetUnsafe(0, true);

        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Factory simulator", width, height);
        MathLib::num_t prevTime = MathLib::GetTime();
        while (true) {
            const MathLib::num_t time = MathLib::GetTime();
            const MathLib::num_t deltaTime = time - prevTime;
            prevTime = time;
            std::cout << "FPS: " << 1 / deltaTime << std::endl;
            if (!renderer.CopyFromBuffer(background.AtUnsafe(0))) MathLib::Panic("Failed to draw background");
            if (drawMode) for (size_t y = 0; y < boxCountY; y++) {
                for (size_t x = 0; x < boxCountX; x++) {
                    if (!world.AtUnsafe(x, y).ContainsMaterial(drawMode - 1)) continue;
                    renderer.FillRectangle(x * boxWidth + boxWidth / 2, y * boxHeight + boxHeight / 2, boxWidth, boxHeight, 0x00ff00ff);
                }
            }
            else for (size_t y = 0; y < boxCountY; y++) {
                for (size_t x = 0; x < boxCountX; x++) {
                    world.AtUnsafe(x, y).machine.Draw(renderer, x * boxWidth + boxWidth / 2, y * boxHeight + boxHeight / 2);
                    world.AtUnsafe(x, y).machine.Update(deltaTime);
                }
            }
            renderer.DrawGrid(centerX, centerY, boxWidth, boxHeight, boxCountX, boxCountY, UINT32_MAX);
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) {
                switch (event.key) {
                    case ' ': {
                        drawMode = (drawMode + 1) % (Box::materialCount + 1);
                        break;
                    }
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