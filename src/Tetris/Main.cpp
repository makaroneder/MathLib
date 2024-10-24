#define SDL_MAIN_HANDLED
#include <Geometry/Cuboid.hpp>
#include <MathLib.hpp>
#include <SDL2.cpp>
#include <iostream>

template <typename T>
struct Tetromino : LineShape<T> {
    Tetromino(const Matrix<T>& pos, const Bitmap& shape, size_t width) : LineShape<T>(pos), shape(shape), width(width) {}
    virtual bool CollidesWith(const Shape<T>& other) const override {
        // TODO:
        (void)other;
        return false;
    }
    virtual Array<Line<T>> ToLines(const Matrix<T>& rotation) const override {
        Array<Line<T>> ret;
        const size_t height = shape.GetSize() / width;
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                const Expected<bool> tmp = shape.Get(y * width + x);
                if (!tmp.HasValue()) return Array<Line<num_t>>();
                else if (tmp.Get()) {
                    const Array<Line<num_t>> tmp = Cuboid<num_t>(this->position + CreateVector<num_t>(x * cubeWidth, y * cubeWidth, 0), CreateVector<num_t>(cubeWidth, cubeWidth, 0.01)).ToLines(CreateVector<num_t>(0, 0, 0));
                    for (const Line<num_t>& line : tmp)
                        if (!ret.Add(Line<num_t>(RotateVector<num_t>(line.start, this->position, rotation), RotateVector<num_t>(line.end, this->position, rotation)))) return Array<Line<num_t>>();
                }
            }
        }
        return ret;
    }

    private:
    static constexpr num_t cubeWidth = 0.5;
    Bitmap shape;
    size_t width;
};

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        #ifdef Debug
        const Test test = TestSelf();
        const size_t tests = test.GetRecordCount();
        const size_t passed = test.GetPassed();
        std::cout << test << passed << "/" << tests << " tests passed" << std::endl;
        if (passed != tests) Panic("Some tests failed");
        #endif
        SDL2Renderer renderer = SDL2Renderer("Tetris", 800, 800);
        Tetromino<num_t> shape = Tetromino<num_t>(CreateVector<num_t>(0, 0, 0), Bitmap(std::vector<uint8_t> {
            0b00000011,
            0b00000001,
            0b00000001,
        }), 8);
        Second<num_t> prevTime = Second<num_t>(GetTime());
        while (true) {
            const Second<num_t> currTime = Second<num_t>(GetTime());
            const Second<num_t> dt = currTime - prevTime;
            prevTime = currTime;
            renderer.Fill(0);
            renderer.DrawShape<num_t>(shape, CreateVector<num_t>(0, 0, 0), UINT32_MAX);
            if (!renderer.Update()) Panic("Failed to update UI");
            const Event event = renderer.GetEvent();
            if (event.type == Event::Type::Quit) break;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}