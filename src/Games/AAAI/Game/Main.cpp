#include "../Shared/Map.hpp"
#include <FileSystem/FileSystem.hpp>
#include <Image/Animation.hpp>
#include <WindowManager.hpp>
#include <Defer.hpp>
#include <iostream>

void Main(int, char**, MathLib::FileSystem& fs, MathLib::WindowManager& windowManager) {
    const MathLib::String inputPath = "Data/AAAI/";
    const MathLib::String mapPath = "bin/Map.bin";
    MathLib::Aseprite ui;
    if (!ui.LoadFromPath(fs, inputPath + "UI.aseprite") || ui.IsEmpty()) MathLib::Panic("Failed to load UI texture");
    MathLib::Aseprite playerIdle;
    if (!playerIdle.LoadFromPath(fs, inputPath + "PlayerIdle.aseprite") || playerIdle.IsEmpty()) MathLib::Panic("Failed to load player idle texture");
    MathLib::Aseprite playerMovement;
    if (!playerMovement.LoadFromPath(fs, inputPath + "PlayerMovement.aseprite") || playerMovement.IsEmpty()) MathLib::Panic("Failed to load player movement animation");
    MathLib::Animation playerMovementAnimation = MathLib::Animation(&playerMovement);
    MathLib::Renderer* renderer = windowManager.MakeRenderer("AAAI", ui.GetWidth(), ui.GetHeight());
    if (!renderer) MathLib::Panic("Failed to create renderer");
    defer(delete renderer);
    const size_t centerX = ui.GetWidth() / 2;
    const size_t centerY = ui.GetHeight() / 2;
    const ssize_t cellSize = 32;
    const float speedX = 10;
    const float speedY = 10;
    int8_t moveX = 0;
    int8_t moveY = 0;
    float offsetX = 0;
    float offsetY = 0;
    MathLib::Array<Block> blocks;
    LoadMap(fs, mapPath, blocks);
    MathLib::Dictionary<MathLib::String, MathLib::Aseprite> cachedTextures;
    MathLib::num_t prevTime = MathLib::GetTime();
    while (true) {
        const MathLib::num_t time = MathLib::GetTime();
        const MathLib::num_t deltaTime = time - prevTime;
        std::cout << "FPS: " << 1 / deltaTime << std::endl;
        prevTime = time;
        if (!renderer->CopyFromBuffer(ui.AtUnsafe(0))) MathLib::Panic("Failed to draw UI");
        DrawMap(*renderer, cellSize, offsetX, offsetY, blocks, fs, inputPath, cachedTextures);
        if (moveX || moveY) {
            offsetX += speedX * moveX * deltaTime;
            offsetY += speedY * moveY * deltaTime;
            playerMovementAnimation.Draw(*renderer, centerX, centerY);
            playerMovementAnimation.Update(deltaTime);
        }
        else {
            renderer->DrawImage(playerIdle.AtUnsafe(0), centerX, centerY);
            playerMovementAnimation.Reset();
        }
        if (!renderer->Update()) MathLib::Panic("Failed to update UI");
        const MathLib::Event event = renderer->GetEvent();
        if (event.type == MathLib::Event::Type::Quit) break;
        else if (event.type == MathLib::Event::Type::KeyPressed) switch (event.key) {
            case 'w': {
                moveY = event.pressed ? -1 : 0;
                break;
            }
            case 's': {
                moveY = event.pressed ? 1 : 0;
                break;
            }
            case 'a': {
                moveX = event.pressed ? -1 : 0;
                break;
            }
            case 'd': {
                moveX = event.pressed ? 1 : 0;
                break;
            }
            default: break;
        }
    }
}