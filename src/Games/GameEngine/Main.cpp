#define SDL_MAIN_HANDLED
#include <Geometry/Ray.hpp>
#include <SDL2.cpp>
#include <iostream>

template <typename T>
T GetIntersections(const MathLib::Matrix<T>& position, const T& angle, const MathLib::Sequence<MathLib::Matrix<T>>& objects) {
    const size_t size = objects.GetSize();
    T ret = MathLib::infinity;
    for (size_t i = 0; i < size; i++) {
        const T x = MathLib::Abs(GetX(objects.At(i)) - GetX(position));
        const T y = MathLib::Abs(GetY(objects.At(i)) - GetY(position));
        if (MathLib::FloatsEqual<T>(angle, MathLib::InversedTan2(y, x))) {
            const T tmp = x * x + y * y;
            if (tmp < ret) ret = tmp;
        }
    }
    return MathLib::Sqrt(ret);
}
template <typename T>
void Draw(MathLib::Renderer& renderer, const T& angle, const T& fov, const MathLib::Matrix<T>& player, const MathLib::Sequence<MathLib::Matrix<T>>& objects) {
    const size_t width = renderer.GetWidth();
    const size_t height = renderer.GetHeight();
    for (size_t i = 0; i < width; i++) {
        const T theta = angle - fov / 2 + fov * i / width;
        const T dist = GetIntersections<T>(player, theta, objects);
        if (!MathLib::IsNaN(dist)) {
            const size_t draw = height * (1 - 1 / dist) / 2;
            for (size_t y = 0; y < height; y++)
            renderer.At(i, y) = UINT32_MAX * (y >= draw && (y <= height - draw));
        }
    }
}

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Game engine", 800, 800);
        MathLib::matrix_t player = MathLib::CreateVector<MathLib::num_t>(0, 0, 0);
        const MathLib::Array<MathLib::matrix_t> objects = MathLib::MakeArray<MathLib::matrix_t>(
            MathLib::CreateVector<MathLib::num_t>(0, 1, 0),
            MathLib::CreateVector<MathLib::num_t>(0.1, 2, 0)
        );
        const MathLib::num_t fov = MathLib::pi / 3;
        MathLib::num_t angle = 1.523;
        MathLib::num_t prev = MathLib::GetTime();
        while (true) {
            const MathLib::num_t curr = MathLib::GetTime();
            const MathLib::num_t deltaTime = curr - prev;
            prev = curr;
            renderer.Fill(0);
            Draw<MathLib::num_t>(renderer, angle, fov, player, objects);
            (void)renderer.Update();
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) {
                switch (event.key) {
                    case 'a': {
                        angle -= deltaTime;
                        break;
                    }
                    case 'd': {
                        angle += deltaTime;
                        break;
                    }
                    case 'w': {
                        GetX(player) += deltaTime * MathLib::Cos<MathLib::num_t>(angle);
                        GetY(player) += deltaTime * MathLib::Sin(angle);
                        break;
                    }
                    case 's': {
                        GetX(player) -= deltaTime * MathLib::Cos<MathLib::num_t>(angle);
                        GetY(player) -= deltaTime * MathLib::Sin(angle);
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