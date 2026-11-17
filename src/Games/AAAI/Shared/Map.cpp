#include "Map.hpp"

void SaveMap(MathLib::FileSystem& fs, const MathLib::String& outputPath, const MathLib::Array<Block>& blocks) {
    MathLib::File file = fs.Open(outputPath, MathLib::OpenMode::Write);
    if (!file.Write<size_t>(blocks.GetSize())) MathLib::Panic("Failed to save block count");
    for (const Block& block : blocks)
        if (!block.Save(file)) MathLib::Panic("Failed to save block");
}
void LoadMap(MathLib::FileSystem& fs, const MathLib::String& outputPath, MathLib::Array<Block>& blocks) {
    MathLib::File file = fs.Open(outputPath, MathLib::OpenMode::Read);
    if (!file.Exists()) return;
    size_t blockCount;
    if (!file.Read<size_t>(blockCount)) MathLib::Panic("Failed to load block count");
    blocks = blockCount;
    for (Block& block : blocks)
        if (!block.Load(file)) MathLib::Panic("Failed to load block");
}
void DrawMap(MathLib::Renderer& renderer, size_t cellSize, ssize_t offsetX, ssize_t offsetY, const MathLib::Array<Block>& blocks, MathLib::FileSystem& fs, const MathLib::String& path, MathLib::Dictionary<MathLib::String, MathLib::Aseprite>& cachedTextures) {
    for (const Block& block : blocks) {
        if (block.path.IsEmpty()) continue;
        const ssize_t x = (block.x - offsetX) * cellSize;
        const ssize_t y = (block.y - offsetY) * cellSize + cellSize;
        MathLib::Aseprite texture;
        const MathLib::Expected<MathLib::Aseprite> tmp = cachedTextures.Get(block.path);
        if (tmp.HasValue()) texture = tmp.Get();
        else {
            if (!texture.LoadFromPath(fs, path + block.path)) MathLib::Panic("Failed to load texture");
            if (!cachedTextures.Add(block.path, texture)) MathLib::Panic("Failed to cache texture");
        }
        if (texture.IsEmpty()) MathLib::Panic("Empty texture provided");
        renderer.DrawImage(texture.AtUnsafe(0), x + texture.AtUnsafe(0).GetWidth() / 2, y - texture.AtUnsafe(0).GetHeight() / 2);
    }
}