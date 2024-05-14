#include <Geometry/Pyramid.hpp>
#include <Geometry/Cuboid.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @return Status
int main(void) {
    try {
        SDL2Renderer renderer = SDL2Renderer("3D viewer", 800, 800);
        Cuboid<num_t> cuboid = Cuboid<num_t>(CreateVector<num_t>(0, 0, 0), CreateVector<num_t>(2, 2, 2));
        Pyramid<num_t> triangle = Pyramid<num_t>(CreateVector<num_t>(0, 0, 0), CreateVector<num_t>(2, 2, 2));
        Matrix<num_t> axis = CreateVector<num_t>(0, 1, 0);
        num_t angle = 0;
        while (true) {
            renderer.Fill(0x000000ff);
            renderer.DrawShape<num_t>(cuboid, axis.Normalize() * angle, 0xff0000ff);
            renderer.DrawShape<num_t>(triangle, axis.Normalize() * angle, 0x00ff00ff);
            if (!renderer.Update()) throw std::runtime_error("Failed to update UI");
            const Event event = renderer.GetEvent();
            if (event.type == Event::Type::Quit) break;
            else if (event.type == Event::Type::KeyPressed) {
                switch (event.data.key) {
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
                        angle += pi / 180;
                        break;
                    }
                    case 'e': {
                        angle -= pi / 180;
                        break;
                    }
                    case '1': {
                        axis = CreateVector<num_t>(1, 0, 0);
                        break;
                    }
                    case '2': {
                        axis = CreateVector<num_t>(0, 1, 0);
                        break;
                    }
                    case '3': {
                        axis = CreateVector<num_t>(0, 0, 1);
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