#define SDL_MAIN_HANDLED
#include <Allocator/RegionAllocator.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::allocator = new MathLib::RegionAllocator(MathLib::allocator, 1024 * 1024);
        if (!MathLib::allocator) MathLib::Panic("Failed to allocate allocator");
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Paint", 800, 800);
        renderer.Fill(0);
        const uint32_t colors[] = {
            UINT32_MAX,
            0x000000ff,
        };
        size_t color = 0;
        const MathLib::num_t radius = 0.05;
        MathLib::Matrix<size_t> position = MathLib::CreateVector<size_t>(SIZE_MAX, SIZE_MAX, SIZE_MAX);
        while (true) {
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::MousePressed) {
                switch (event.mouseButton) {
                    case MathLib::Event::MouseButton::None: {
                        if (position != MathLib::CreateVector<size_t>(SIZE_MAX, SIZE_MAX, SIZE_MAX)) {
                            GetX(position) = event.mouseX;
                            GetY(position) = event.mouseY;
                        }
                        break;
                    }
                    case MathLib::Event::MouseButton::Left: {
                        if (event.pressed) {
                            GetX(position) = event.mouseX;
                            GetY(position) = event.mouseY;
                        }
                        else position = MathLib::CreateVector<size_t>(SIZE_MAX, SIZE_MAX, SIZE_MAX);
                        break;
                    }
                    case MathLib::Event::MouseButton::Right: {
                        if (event.pressed) color = (color + 1) % SizeOfArray(colors);
                        break;
                    }
                    default: break;
                }
            }
            if (position != MathLib::CreateVector<size_t>(SIZE_MAX, SIZE_MAX, SIZE_MAX))
                renderer.FillCircle2D<MathLib::num_t>(renderer.IndexToPosition<MathLib::num_t>(GetX(position), GetY(position)), radius, colors[color]);
        }
        delete MathLib::allocator;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}