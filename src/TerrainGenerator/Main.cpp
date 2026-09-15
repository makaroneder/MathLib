#include "Option.hpp"
#include <WindowManager.hpp>
#include <FileSystem/FileSystem.hpp>
#include <iostream>

size_t SelectBlock(Block* map, size_t blocks) {
    MathLib::Array<size_t> index;
    size_t minOptions = SIZE_MAX;
    for (size_t i = 0; i < blocks; i++) {
        const size_t options = map[i].GetOptionCount();
        if (!options || minOptions < options) continue;
        if (options == minOptions) {
            if (!index.Add(i)) return SIZE_MAX;
            continue;
        }
        index = MathLib::MakeArray<size_t>(i);
        minOptions = options;
    }
    return minOptions == SIZE_MAX ? SIZE_MAX : MathLib::AtRandom<size_t>(index);
}
void Draw(MathLib::Renderer& renderer, const Block* map, const Option* options, size_t blockWidth, size_t blockHeight, size_t blockDrawWidth, size_t blockDrawHeight) {
    renderer.Fill(0);
    for (size_t y = 0; y < blockHeight; y++) {
        for (size_t x = 0; x < blockWidth; x++) {
            const uint8_t option = map[y * blockWidth + x].Get();
            if (option == UINT8_MAX) continue;
            renderer.SetRectangle((0.5 + x) * blockDrawWidth, (0.5 + y) * blockDrawHeight, blockDrawWidth, blockDrawHeight, options[option].color);
        }
    }
}
void Main(int, char**, MathLib::FileSystem&, MathLib::WindowManager& windowManager) {
    srand(time(nullptr));
    const size_t width = 800;
    const size_t height = 800;
    const size_t blockWidth = 50;
    const size_t blockHeight = 50;
    const size_t blockDrawWidth = width / blockWidth;
    const size_t blockDrawHeight = height / blockHeight;
    const size_t blocks = blockWidth * blockHeight;
    MathLib::Renderer* renderer = windowManager.MakeRenderer("Terrain generator", width, height);
    if (!renderer) MathLib::Panic("Failed to create renderer");
    Block map[blocks];
    Option options[] = {
        Option(
            0x00ff00ff,
            Block(MathLib::MakeArray<uint8_t>(0, 1)),
            Block(MathLib::MakeArray<uint8_t>(0, 1)),
            Block(MathLib::MakeArray<uint8_t>(0, 1)),
            Block(MathLib::MakeArray<uint8_t>(0, 1))
        ),
        Option(
            0xffff00ff,
            Block(MathLib::MakeArray<uint8_t>(0, 1, 2)),
            Block(MathLib::MakeArray<uint8_t>(0, 1, 2)),
            Block(MathLib::MakeArray<uint8_t>(0, 1, 2)),
            Block(MathLib::MakeArray<uint8_t>(0, 1, 2))
        ),
        Option(
            0x0000ffff,
            Block(MathLib::MakeArray<uint8_t>(1, 2)),
            Block(MathLib::MakeArray<uint8_t>(1, 2)),
            Block(MathLib::MakeArray<uint8_t>(1, 2)),
            Block(MathLib::MakeArray<uint8_t>(1, 2))
        ),
    };
    // TODO: Constraints with distance greater than 1
    for (size_t i = 0; i < blocks; i++) map[i] = Block(SizeOfArray(options));
    while (true) {
        const size_t block = SelectBlock(map, blocks);
        if (block == SIZE_MAX) break;
        const Constraint& constraint = options[map[block].GetRandomOption()];
        const size_t x = block % blockWidth;
        const size_t y = block / blockWidth;
        if (x && !map[block - 1].Intersect(constraint.left)) MathLib::Panic("Failed to apply constraints");
        if (x + 1 < blockWidth && !map[block + 1].Intersect(constraint.right)) MathLib::Panic("Failed to apply constraints");
        if (y && !map[block - blockWidth].Intersect(constraint.upper)) MathLib::Panic("Failed to apply constraints");
        if (y + 1 < blockHeight && !map[block + blockWidth].Intersect(constraint.lower)) MathLib::Panic("Failed to apply constraints");
    }
    Draw(*renderer, map, options, blockWidth, blockHeight, blockDrawWidth, blockDrawHeight);
    while (true) {
        if (!renderer->Update()) MathLib::Panic("Failed to update UI");
        if (renderer->GetEvent().type == MathLib::Event::Type::Quit) break;
    }
}