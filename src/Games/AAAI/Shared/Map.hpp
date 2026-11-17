#ifndef Map_H
#define Map_H
#include "Block.hpp"
#include <Renderer.hpp>
#include <Interfaces/Dictionary.hpp>
#include <Image/Aseprite/Aseprite.hpp>

void SaveMap(MathLib::FileSystem& fs, const MathLib::String& outputPath, const MathLib::Array<Block>& blocks);
void LoadMap(MathLib::FileSystem& fs, const MathLib::String& outputPath, MathLib::Array<Block>& blocks);
void DrawMap(MathLib::Renderer& renderer, size_t cellSize, ssize_t offsetX, ssize_t offsetY, const MathLib::Array<Block>& blocks, MathLib::FileSystem& fs, const MathLib::String& path, MathLib::Dictionary<MathLib::String, MathLib::Aseprite>& cachedTextures);

#endif