#include "Block.hpp"
#include <Image/Aseprite/Aseprite.hpp>
#include <FileSystem/FileSystem.hpp>
#include <Interfaces/Dictionary.hpp>
#include <WindowManager.hpp>
#include <Font/PSF1.hpp>
#include <Defer.hpp>
#include <Grid.hpp>
#include <iostream>

MathLib::Array<MathLib::String> GetFileList(MathLib::FileSystem& fs, const MathLib::String path) {
    const MathLib::Array<MathLib::FileInfo> fileInfos = fs.ReadDirectory(path);
    MathLib::Array<MathLib::String> fileList;
    for (const MathLib::FileInfo& fileInfo : fileInfos)
        if (fileInfo.type == MathLib::FileInfo::Type::File && !fileList.Add(fileInfo.path)) MathLib::Panic("Failed to read directory");
    return fileList;
}
void Main(int, char**, MathLib::FileSystem& fs, MathLib::WindowManager& windowManager) {
    // TODO: Layers
    // TODO: Output
    const MathLib::String inputPath = "Data/MapGenerator/";
    const MathLib::String fontPath = "Data/Fonts/zap-light16.psf";
    const MathLib::String outputPath = "bin/";
    const size_t cellSize = 16;
    const size_t uiHeight = 192;
    const size_t width = 800;
    const size_t height = 800;
    const size_t drawCellX = ((width + cellSize - 1) / cellSize);
    const size_t drawCellY = ((height - uiHeight + cellSize - 1) / cellSize);
    const size_t centerX = width / 2;
    const size_t centerY = (height - uiHeight) / 2;
    MathLib::Renderer* renderer = windowManager.MakeRenderer("MapGenerator", width, height);
    if (!renderer) MathLib::Panic("Failed to create renderer");
    defer(delete renderer);
    MathLib::PSF1 font;
    if (!font.LoadFromPath(fs, fontPath)) MathLib::Panic("Failed to load path");
    MathLib::Array<MathLib::String> fileList = GetFileList(fs, inputPath);
    size_t currentFile = 0;
    ssize_t offsetX = 0;
    ssize_t offsetY = 0;
    MathLib::Array<Block> blocks;
    bool drawGrid = true;
    while (true) {
        renderer->Fill(0);
        if (drawGrid) renderer->DrawGrid(centerX, centerY, cellSize, cellSize, drawCellX, drawCellY, UINT32_MAX);
        else renderer->DrawLinePararellToOX(0, width, height - uiHeight, UINT32_MAX);
        if (true) {
            MathLib::Dictionary<MathLib::String, MathLib::Aseprite> cachedTextures;
            for (const Block& block : blocks) {
                const ssize_t x = (block.x - offsetX) * cellSize + cellSize / 2;
                if (x < 0 || (size_t)x >= width) continue;
                const ssize_t y = (block.y - offsetY) * cellSize + cellSize / 2;
                if (y < 0 || (size_t)y >= height - uiHeight) continue;
                MathLib::Aseprite texture;
                const MathLib::Expected<MathLib::Aseprite> tmp = cachedTextures.Get(block.file);
                if (tmp.HasValue()) texture = tmp.Get();
                else {
                    if (!texture.LoadFromPath(fs, inputPath + block.file)) MathLib::Panic("Failed to load texture");
                    if (!cachedTextures.Add(block.file, texture)) MathLib::Panic("Failed to cache texture");
                }
                renderer->DrawImage(texture.At(0), x, y);
            }
        }
        if (!fileList.IsEmpty()) {
            const size_t size = fileList.GetSize();
            for (size_t i = 0; i < size; i++)
                renderer->PutsFromLeft(fileList.AtUnsafe(i), font, 0, height - uiHeight + i * font.GetHeight(), i == currentFile ? 0xffff00ff : UINT32_MAX, 0);
        }
        if (!renderer->Update()) MathLib::Panic("Failed to update UI");
        const MathLib::Event event = renderer->GetEvent();
        if (event.type == MathLib::Event::Type::Quit) break;
        if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) switch (event.key) {
            case 'r': {
                fileList = GetFileList(fs, inputPath);
                break;
            }
            case 'e': {
                if (!fileList.IsEmpty()) currentFile = (currentFile + 1) % fileList.GetSize();
                break;
            }
            case 'g': {
                drawGrid = !drawGrid;
                break;
            }
            case 'w': {
                offsetY--;
                break;
            }
            case 's': {
                offsetY++;
                break;
            }
            case 'a': {
                offsetX--;
                break;
            }
            case 'd': {
                offsetX++;
                break;
            }
            default: break;
        }
        if (event.type == MathLib::Event::Type::MousePressed && event.pressed && !fileList.IsEmpty() && event.mouseY <= height - uiHeight) {
            if (!blocks.Add(Block(fileList.AtUnsafe(currentFile), event.mouseX / 16 + offsetX, event.mouseY / 16 + offsetY))) MathLib::Panic("Failed to register texture");
        }
    }
}