#define SDL_MAIN_HANDLED
#include "Block.hpp"
#include <Image/Aseprite/Aseprite.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Collision.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::HostFileSystem fs;
        const MathLib::String path = "src/TestPrograms/Chomp/";
        MathLib::Aseprite backgrounds;
        if (!backgrounds.LoadFromPath(fs, path + "Background.aseprite")) MathLib::Panic("Failed to load background image");
        backgrounds.Resize(2, 2);
        size_t background = 0;
        const size_t width = backgrounds.GetWidth();
        const size_t height = backgrounds.GetHeight();
        const size_t centerX = width / 2;
        const size_t centerY = height / 2;
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Chomp", width, height);
        MathLib::Aseprite block;
        if (!block.LoadFromPath(fs, path + "Block.aseprite")) MathLib::Panic("Failed to load block image");
        block.Resize(2, 2);
        MathLib::Aseprite finalBlock;
        if (!finalBlock.LoadFromPath(fs, path + "FinalBlock.aseprite")) MathLib::Panic("Failed to load final block image");
        finalBlock.Resize(2, 2);
        const size_t blockWidth = block.AtUnsafe(block.GetSize() - 1).GetWidth();
        const size_t blockHeight = block.AtUnsafe(block.GetSize() - 1).GetHeight();
        const size_t cellsX = 8;
        const size_t cellsY = 4;
        MathLib::Matrix<Block> centers = ResetBlocks(centerX, centerY, blockWidth, blockHeight, cellsX, cellsY);
        ssize_t positionX = 0;
        ssize_t positionY = 0;
        ssize_t stackDiff = 3;
        const size_t speedX = 10;
        const size_t speedY = 10;
        bool update = true;
        MathLib::num_t prevTime = MathLib::GetTime();
        while (true) {
            const MathLib::num_t time = MathLib::GetTime();
            const MathLib::num_t deltaTime = time - prevTime;
            prevTime = time;
            std::cout << "FPS: " << 1 / deltaTime << std::endl;
            if (update) {
                if (!renderer.CopyFromBuffer(backgrounds.AtUnsafe(background))) MathLib::Panic("Failed to draw background");
                for (const Block& center : centers) switch (center.state) {
                    case Block::State::Normal: {
                        renderer.DrawStackedImage(block, center.centerX + positionX, center.centerY + positionY, stackDiff);
                        break;
                    }
                    case Block::State::Final: {
                        renderer.DrawStackedImage(finalBlock, center.centerX + positionX, center.centerY + positionY, stackDiff);
                        break;
                    }
                    default: break;
                }
                update = false;
            }
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            if (event.type == MathLib::Event::Type::MousePressed && event.pressed) {
                size_t blockX = SIZE_MAX;
                size_t blockY = SIZE_MAX;
                for (size_t y = 0; y < centers.GetHeight(); y++) {
                    for (size_t x = 0; x < centers.GetWidth(); x++) {
                        const Block center = centers.AtUnsafe(x, y);
                        if (!MathLib::CheckBoxCollision(event.mouseX, event.mouseY, center.centerX + positionX, center.centerY + positionY, blockWidth, blockHeight, 1, 1)) continue;
                        blockX = x;    
                        blockY = y;
                        break;
                    }
                    if (blockX != SIZE_MAX) break;
                }
                if (blockX != SIZE_MAX) {
                    bool reset = false;
                    for (size_t y = 0; y <= blockY; y++) {
                        for (size_t x = blockX; x < centers.GetWidth(); x++) {
                            if (centers.AtUnsafe(x, y).state == Block::State::Final) {
                                reset = true;
                                break;
                            }
                            centers.AtUnsafe(x, y).state = Block::State::Dead;
                        }
                        if (reset) break;
                    }
                    if (reset) centers = ResetBlocks(centerX, centerY, blockWidth, blockHeight, cellsX, cellsY);
                    update = true;
                }
            }
            if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) switch (event.key) {
                case ' ': {
                    background = (background + 1) % backgrounds.GetSize();
                    update = true;
                    break;
                }
                case 'r': {
                    positionX = positionY = 0;
                    stackDiff = 3;
                    update = true;
                    break;
                }
                case 'w': {
                    positionY -= speedY;
                    update = true;
                    break;
                }
                case 's': {
                    positionY += speedY;
                    update = true;
                    break;
                }
                case 'a': {
                    positionX -= speedX;
                    update = true;
                    break;
                }
                case 'd': {
                    positionX += speedX;
                    update = true;
                    break;
                }
                case 'e': {
                    stackDiff++;
                    update = true;
                    break;
                }
                case 'q': {
                    stackDiff--;
                    update = true;
                    break;
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