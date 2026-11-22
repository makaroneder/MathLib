#include "Map.hpp"
#include <Interfaces/ComparisionFunction.hpp>

void SaveMap(MathLib::FileSystem& fs, const MathLib::String& outputPath, MathLib::Array<Block>& blocks) {
    MathLib::File file = fs.Open(outputPath, MathLib::OpenMode::Write);
    if (!blocks.CocktailShakerSort(MathLib::ComparisionFunction<Block>(MathLib::ComparisionFunctionType::GreaterThan))) MathLib::Panic("Failed to sort map");
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
void DrawMap(MathLib::Renderer& renderer, ssize_t cellSize, ssize_t offsetX, ssize_t offsetY, const MathLib::Array<Block>& blocks, MathLib::FileSystem& fs, const MathLib::String& path, MathLib::Dictionary<MathLib::String, MathLib::Aseprite>& cachedTextures) {
    const MathLib::ComparisionFunction<Block> compare = MathLib::ComparisionFunction<Block>(MathLib::ComparisionFunctionType::GreaterThan);
    const ssize_t minBlocksX = offsetX / cellSize;
    const ssize_t minBlocksY = offsetY / cellSize;
    const ssize_t maxBlocksX = (renderer.GetWidth() + offsetX + cellSize - 1) / cellSize;
    const ssize_t maxBlocksY = (renderer.GetHeight() + offsetY + cellSize - 1) / cellSize;
    for (ssize_t y = minBlocksY; y < maxBlocksY; y++) {
        for (ssize_t x = minBlocksX; x < maxBlocksX; x++) {
            const size_t i = blocks.BinarySearch(compare, Block(x, y));
            if (i == SIZE_MAX) continue;
            MathLib::Aseprite texture;
            const MathLib::Expected<MathLib::Aseprite> tmp = cachedTextures.Get(blocks.AtUnsafe(i).path);
            if (tmp.HasValue()) texture = tmp.Get();
            else {
                if (!texture.LoadFromPath(fs, path + blocks.AtUnsafe(i).path)) MathLib::Panic("Failed to load texture");
                if (!cachedTextures.Add(blocks.AtUnsafe(i).path, texture)) MathLib::Panic("Failed to cache texture");
            }
            if (texture.IsEmpty()) MathLib::Panic("Empty texture provided");
            renderer.DrawImage(texture.AtUnsafe(0), x * cellSize - offsetX + texture.AtUnsafe(0).GetWidth() / 2, y * cellSize + cellSize - offsetY - texture.AtUnsafe(0).GetHeight() / 2);
        }
    }
}