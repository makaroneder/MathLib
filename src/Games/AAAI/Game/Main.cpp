#include "../Shared/Map.hpp"
#include <FileSystem/FileSystem.hpp>
#include <WindowManager.hpp>
#include <Defer.hpp>
#include <iostream>

void Main(int, char**, MathLib::FileSystem& fs, MathLib::WindowManager& windowManager) {
    const MathLib::String inputPath = "Data/AAAI/";
    const MathLib::String mapPath = "bin/Map.bin";
    MathLib::Aseprite ui;
    if (!ui.LoadFromPath(fs, inputPath + "UI.aseprite")) MathLib::Panic("Failed to load UI texture");
    MathLib::Renderer* renderer = windowManager.MakeRenderer("AAAI", ui.GetWidth(), ui.GetHeight());
    if (!renderer) MathLib::Panic("Failed to create renderer");
    defer(delete renderer);
    const size_t cellSize = 32;
    ssize_t offsetX = 0;
    ssize_t offsetY = 0;
    MathLib::Array<Block> blocks;
    LoadMap(fs, mapPath, blocks);
    MathLib::Dictionary<MathLib::String, MathLib::Aseprite> cachedTextures;
    MathLib::num_t prevTime = MathLib::GetTime();
    while (true) {
        const MathLib::num_t time = MathLib::GetTime();
        std::cout << "FPS: " << 1 / (time - prevTime) << std::endl;
        prevTime = time;
        if (!renderer->CopyFromBuffer(ui.At(0))) MathLib::Panic("Failed to draw UI");
        DrawMap(*renderer, cellSize, offsetX, offsetY, blocks, fs, inputPath, cachedTextures);
        if (!renderer->Update()) MathLib::Panic("Failed to update UI");
        const MathLib::Event event = renderer->GetEvent();
        if (event.type == MathLib::Event::Type::Quit) break;
    }
}