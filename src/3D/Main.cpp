#include <Geometry/Pyramid.hpp>
#include <Geometry/Cuboid.hpp>
#include <MathLib.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2Renderer renderer = MathLib::SDL2Renderer("3D viewer", 800, 800);
        MathLib::Cuboid<MathLib::num_t> cuboid = MathLib::Cuboid<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(0, 0, 0), MathLib::CreateVector<MathLib::num_t>(2, 2, 2));
        MathLib::Pyramid<MathLib::num_t> triangle = MathLib::Pyramid<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(0, 0, 0), MathLib::CreateVector<MathLib::num_t>(2, 2, 2));
        MathLib::Matrix<MathLib::num_t> axis = MathLib::CreateVector<MathLib::num_t>(0, 1, 0);
        MathLib::num_t angle = 0;
        while (true) {
            renderer.Fill(0);
            renderer.DrawShape<MathLib::num_t>(cuboid, axis.Normalize() * angle, 0xff0000ff);
            renderer.DrawShape<MathLib::num_t>(triangle, axis.Normalize() * angle, 0x00ff00ff);
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) {
                switch (event.key) {
                    case 'w': {
                        GetZ(renderer.position)++;
                        break;
                    }
                    case 's': {
                        GetZ(renderer.position)--;
                        break;
                    }
                    case 'd': {
                        GetX(renderer.position)++;
                        break;
                    }
                    case 'a': {
                        GetX(renderer.position)--;
                        break;
                    }
                    case 'q': {
                        angle += MathLib::pi / 180;
                        break;
                    }
                    case 'e': {
                        angle -= MathLib::pi / 180;
                        break;
                    }
                    case '1': {
                        axis = MathLib::CreateVector<MathLib::num_t>(1, 0, 0);
                        break;
                    }
                    case '2': {
                        axis = MathLib::CreateVector<MathLib::num_t>(0, 1, 0);
                        break;
                    }
                    case '3': {
                        axis = MathLib::CreateVector<MathLib::num_t>(0, 0, 1);
                        break;
                    }
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