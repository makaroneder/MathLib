#define SDL_MAIN_HANDLED
#include <Image/Aseprite/Aseprite.hpp>
#include <Libc/HostFileSystem.hpp>
#include <SDL2.cpp>
#include <iostream>

struct Map {
    Map(void) {
        for (uint8_t i = 0; i < SizeOfArray(data); i++) data[i] = UINT16_MAX;
    }
    uint16_t& At(uint8_t x, uint8_t y) {
        return data[y * 13 + x];
    }
    uint16_t At(uint8_t x, uint8_t y) const {
        return data[y * 13 + x];
    }

    private:
    uint16_t data[13 * 13];
};

bool CheckPartialBoxCollision(size_t x1, size_t x2, size_t radius) {
    return (x1 < x2 + radius) && (x2 < x1 + radius);
}
bool CheckBoxCollision(size_t x1, size_t y1, size_t x2, size_t y2, size_t width1, size_t height1, size_t width2, size_t height2) {
    return CheckPartialBoxCollision(x1 * 2, x2 * 2, width1 + width2) && CheckPartialBoxCollision(y1 * 2, y2 * 2, height1 + height2);
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::HostFileSystem fs;
        const MathLib::String path = "src/TestPrograms/GovernmentSimulator/";
        MathLib::Aseprite backgrounds;
        if (!backgrounds.LoadFromPath(fs, path + "Background.aseprite")) MathLib::Panic("Failed to load background image");
        backgrounds.Resize(2, 2);
        const size_t width = backgrounds.GetWidth();
        const size_t height = backgrounds.GetHeight();
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Government simulator", width, height);
        MathLib::Aseprite assets;
        if (!assets.LoadFromPath(fs, path + "Assets.aseprite")) MathLib::Panic("Failed to load assets");
        assets.Resize(2, 2);
        MathLib::Aseprite arrow;
        if (!arrow.LoadFromPath(fs, path + "Arrow.aseprite")) MathLib::Panic("Failed to load arrow image");
        arrow.Resize(2, 2);
        const size_t arrowWidth = arrow.GetWidth();
        const size_t arrowHeight = arrow.GetHeight();
        const size_t upperArrowX = width / 2;
        const size_t upperArrowY = arrowHeight / 2;
        const size_t lowerArrowX = width / 2;
        const size_t lowerArrowY = height - arrowHeight / 2;
        const size_t leftArrowX = arrowWidth / 2;
        const size_t leftArrowY = height / 2;
        const size_t rightArrowX = width - arrowWidth / 2;
        const size_t rightArrowY = height / 2;
        uint8_t backgroundX = 1;
        uint8_t backgroundY = 1;
        Map maps[9];
        maps[backgroundY * 3 + backgroundX].At(6, 6) = 0;
        maps[backgroundY * 3 + backgroundX].At(5, 6) = 1;
        maps[backgroundY * 3 + backgroundX].At(4, 6) = 2;
        bool update = true;
        while (true) {
            if (update) {
                const uint8_t background = backgroundY * 3 + backgroundX;
                if (!renderer.CopyFromBuffer(backgrounds.At(background))) MathLib::Panic("Failed to draw background image");
                if (backgroundY) renderer.DrawImage(arrow.At(0), upperArrowX, upperArrowY);
                if (backgroundY != 2) renderer.DrawImage(arrow.At(0).RotateUpsideDown(), lowerArrowX, lowerArrowY);
                if (backgroundX) renderer.DrawImage(arrow.At(0).SwapXY(), leftArrowX, leftArrowY);
                if (backgroundX != 2) renderer.DrawImage(arrow.At(0).SwapXYAndRotateUpsideDown(), rightArrowX, rightArrowY);
                for (uint8_t y = 0; y < 13; y++) {
                    for (uint8_t x = 0; x < 13; x++) {
                        const uint16_t index = maps[background].At(x, y);
                        if (index == UINT16_MAX) continue;
                        const uint8_t ix = *((const uint8_t*)&index + 0) * 64;
                        const uint8_t iy = *((const uint8_t*)&index + 1) * 64;
                        if (!renderer.DrawPartialImage(assets.At(0), ix, ix + 64, iy, iy + 64, x * 64 + 32, y * 64 + 32)) MathLib::Panic("Failed to draw city");
                    }
                }
                update = false;
            }
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::MousePressed && event.pressed) {
                const size_t x = GetX(event.mousePosition);
                const size_t y = GetY(event.mousePosition);
                if (backgroundY && CheckBoxCollision(upperArrowX, upperArrowY, x, y, arrowWidth, arrowHeight, 1, 1)) {
                    backgroundY--;
                    update = true;
                }
                if (backgroundY != 2 && CheckBoxCollision(lowerArrowX, lowerArrowY, x, y, arrowWidth, arrowHeight, 1, 1)) {
                    backgroundY++;
                    update = true;
                }
                if (backgroundX && CheckBoxCollision(leftArrowX, leftArrowY, x, y, arrowWidth, arrowHeight, 1, 1)) {
                    backgroundX--;
                    update = true;
                }
                if (backgroundX != 2 && CheckBoxCollision(rightArrowX, rightArrowY, x, y, arrowWidth, arrowHeight, 1, 1)) {
                    backgroundX++;
                    update = true;
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