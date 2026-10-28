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
void Save(MathLib::FileSystem& fs, const MathLib::String& outputPath, const MathLib::Array<Block>& blocks) {
    MathLib::File file = fs.Open(outputPath, MathLib::OpenMode::Write);
    if (!file.Write<size_t>(blocks.GetSize())) MathLib::Panic("Failed to save block count");
    for (const Block& block : blocks)
        if (!block.Save(file)) MathLib::Panic("Failed to save block");
}
void Load(MathLib::FileSystem& fs, const MathLib::String& outputPath, MathLib::Array<Block>& blocks) {
    MathLib::File file = fs.Open(outputPath, MathLib::OpenMode::Read);
    if (!file.Exists()) return;
    size_t blockCount;
    if (!file.Read<size_t>(blockCount)) MathLib::Panic("Failed to load block count");
    blocks = blockCount;
    for (Block& block : blocks)
        if (!block.Load(file)) MathLib::Panic("Failed to load block");
}
void Main(int, char**, MathLib::FileSystem& fs, MathLib::WindowManager& windowManager) {
    const MathLib::String inputPath = "Data/MapGenerator/";
    const MathLib::String fontPath = "Data/Fonts/zap-light16.psf";
    const MathLib::String outputPath = "bin/Map.bin";
    const size_t cellSize = 32;
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
    if (!font.LoadFromPath(fs, fontPath)) MathLib::Panic("Failed to load font");
    MathLib::Array<MathLib::String> fileList = GetFileList(fs, inputPath);
    size_t currentFile = 0;
    ssize_t offsetX = 0;
    ssize_t offsetY = 0;
    MathLib::Array<Block> blocks;
    bool drawGrid = true;
    bool update = true;
    MathLib::num_t prevTime = MathLib::GetTime();
    Load(fs, outputPath, blocks);
    while (true) {
        const MathLib::num_t time = MathLib::GetTime();
        std::cout << "FPS: " << 1 / (time - prevTime) << std::endl;
        prevTime = time;
        if (update) {
            update = false;
            renderer->Fill(0);
            if (true) {
                MathLib::Dictionary<MathLib::String, MathLib::Aseprite> cachedTextures;
                for (const Block& block : blocks) {
                    if (block.path.IsEmpty()) continue;
                    const ssize_t x = (block.x - offsetX) * cellSize + cellSize / 2;
                    if (x < 0 || (size_t)x >= width) continue;
                    const ssize_t y = (block.y - offsetY) * cellSize + cellSize / 2;
                    if (y < 0 || (size_t)y >= height - uiHeight) continue;
                    MathLib::Aseprite texture;
                    const MathLib::Expected<MathLib::Aseprite> tmp = cachedTextures.Get(block.path);
                    if (tmp.HasValue()) texture = tmp.Get();
                    else {
                        if (!texture.LoadFromPath(fs, inputPath + block.path)) MathLib::Panic("Failed to load texture");
                        if (!cachedTextures.Add(block.path, texture)) MathLib::Panic("Failed to cache texture");
                    }
                    renderer->DrawImage(texture.At(0), x, y);
                }
            }
            if (drawGrid) renderer->DrawGrid(centerX, centerY, cellSize, cellSize, drawCellX, drawCellY, UINT32_MAX);
            else renderer->DrawLinePararellToOX(0, width, height - uiHeight, UINT32_MAX);
            if (!fileList.IsEmpty()) {
                const size_t size = fileList.GetSize();
                for (size_t i = 0; i < size; i++)
                    renderer->PutsFromLeft(fileList.AtUnsafe(i), font, 0, height - uiHeight + i * font.GetHeight(), i == currentFile ? 0xffff00ff : UINT32_MAX, 0);
            }
        }
        if (!renderer->Update()) MathLib::Panic("Failed to update UI");
        const MathLib::Event event = renderer->GetEvent();
        if (event.type == MathLib::Event::Type::Quit) break;
        if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) switch (event.key) {
            case 'f': {
                Save(fs, outputPath, blocks);
                break;
            }
            case 'v': {
                Load(fs, outputPath, blocks);
                update = true;
                break;
            }
            case 'x': {
                blocks = MathLib::Array<Block>();
                update = true;
                break;
            }
            case 'r': {
                fileList = GetFileList(fs, inputPath);
                update = true;
                break;
            }
            case 'e': {
                if (!fileList.IsEmpty()) currentFile = (currentFile + 1) % fileList.GetSize();
                update = true;
                break;
            }
            case 'g': {
                drawGrid = !drawGrid;
                update = true;
                break;
            }
            case 'w': {
                offsetY--;
                update = true;
                break;
            }
            case 's': {
                offsetY++;
                update = true;
                break;
            }
            case 'a': {
                offsetX--;
                update = true;
                break;
            }
            case 'd': {
                offsetX++;
                update = true;
                break;
            }
            default: break;
        }
        if (event.type == MathLib::Event::Type::MousePressed && event.pressed && !fileList.IsEmpty() && event.mouseY <= height - uiHeight) {
            const ssize_t x = event.mouseX / cellSize + offsetX;
            const ssize_t y = event.mouseY / cellSize + offsetY;
            size_t block = SIZE_MAX;
            size_t free = SIZE_MAX;
            for (size_t i = 0; i < blocks.GetSize(); i++) {
                if (blocks.AtUnsafe(i).x != x || blocks.AtUnsafe(i).y != y) {
                    if (free == SIZE_MAX && blocks.AtUnsafe(i).path.IsEmpty()) free = i;
                    continue;
                }
                block = i;
            }
            switch (event.mouseButton) {
                case MathLib::Event::MouseButton::Left: {
                    if (block != SIZE_MAX) blocks.AtUnsafe(block).path = fileList.AtUnsafe(currentFile);
                    else if (free != SIZE_MAX) blocks.AtUnsafe(free) = Block(fileList.AtUnsafe(currentFile), x, y);
                    else if (!blocks.Add(Block(fileList.AtUnsafe(currentFile), x, y))) MathLib::Panic("Failed to register texture");
                    break;
                }
                case MathLib::Event::MouseButton::Right: {
                    if (block != SIZE_MAX) blocks.AtUnsafe(block) = Block();
                    break;
                }
                default: break;
            }
            update = true;
        }
    }
    Save(fs, outputPath, blocks);
}