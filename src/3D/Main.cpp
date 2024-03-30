#include <Geometry/Pyramid.hpp>
#include <Geometry/Cuboid.hpp>
#include <SDL2.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @return Status
int main(void) {
    try {
        SDL2Renderer renderer = SDL2Renderer("3D viewer", 800, 800);
        Cuboid<num_t> cuboid = Cuboid<num_t>(CreateVector<num_t>(0, 0, 0), CreateVector<num_t>(2, 2, 2));
        Pyramid<num_t> triangle = Pyramid<num_t>(CreateVector<num_t>(0, 0, 0), CreateVector<num_t>(2, 2, 2));
        const Matrix<num_t> axis = CreateVector<num_t>(1, 1, 1).Normalize();
        num_t angle = 0;
        num_t div = 10;
        bool pause = false;
        while (true) {
            angle += pause ? 0 : pi / (180 * div);
            if (!renderer.DrawShape<num_t>(cuboid, angle, axis, 0xff0000ff)) throw std::runtime_error("Failed to render Cuboid");
            if (!renderer.DrawShape<num_t>(triangle, angle, axis, 0x00ff00ff)) throw std::runtime_error("Failed to render Cuboid");
            if (!renderer.Update()) throw std::runtime_error("Failed to update UI");
            const Event event = renderer.GetEvent();
            if (event.type == Event::Type::Quit) break;
            else if (event.type == Event::Type::KeyPressed) {
                switch (event.data.key) {
                    case ' ': {
                        pause = !pause;
                        break;
                    }
                    case 'w': {
                        div--;
                        if (FloatsEqual<num_t>(div, 0)) div = 1;
                        break;
                    }
                    case 's': {
                        div++;
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