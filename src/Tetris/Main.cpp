#define SDL_MAIN_HANDLED
#include <Geometry/Cuboid.hpp>
#include <MathLib.hpp>
#include <SDL2.cpp>
#include <iostream>

template <typename T>
struct Tetromino : MathLib::LineShape<T> {
    Tetromino(const MathLib::Matrix<T>& pos, const MathLib::Bitmap& shape, size_t width) : MathLib::LineShape<T>(pos), shape(shape), width(width) {}
    virtual bool CollidesWith(const MathLib::Shape<T>& other) const override {
        // TODO:
        (void)other;
        return false;
    }
    virtual MathLib::Array<MathLib::Line<T>> ToLines(const MathLib::Matrix<T>& rotation) const override {
        MathLib::Array<MathLib::Line<T>> ret;
        const size_t height = shape.GetSize() / width;
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                const MathLib::Expected<bool> tmp = shape.Get(y * width + x);
                if (!tmp.HasValue()) return MathLib::Array<MathLib::Line<T>>();
                else if (tmp.Get()) {
                    const MathLib::Array<MathLib::Line<T>> tmp = MathLib::Cuboid<T>(this->position + MathLib::CreateVector<T>(x * cubeWidth, y * cubeWidth, 0), MathLib::CreateVector<T>(cubeWidth, cubeWidth, 0.01)).ToLines(MathLib::CreateVector<T>(0, 0, 0));
                    for (const MathLib::Line<T>& line : tmp)
                        if (!ret.Add(MathLib::Line<T>(MathLib::RotateVector<T>(line.start, this->position, rotation), MathLib::RotateVector<T>(line.end, this->position, rotation)))) return MathLib::Array<MathLib::Line<T>>();
                }
            }
        }
        return ret;
    }

    private:
    static constexpr T cubeWidth = 0.5;
    MathLib::Bitmap shape;
    size_t width;
};

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2Renderer renderer = MathLib::SDL2Renderer("Tetris", 800, 800);
        Tetromino<MathLib::num_t> shape = Tetromino<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(0, 0, 0), MathLib::Bitmap(std::vector<uint8_t> {
            0b00000011,
            0b00000001,
            0b00000001,
        }), 8);
        MathLib::Second<MathLib::num_t> prevTime = MathLib::Second<MathLib::num_t>(MathLib::GetTime());
        while (true) {
            const MathLib::Second<MathLib::num_t> currTime = MathLib::Second<MathLib::num_t>(MathLib::GetTime());
            const MathLib::Second<MathLib::num_t> dt = currTime - prevTime;
            prevTime = currTime;
            renderer.Fill(0);
            renderer.DrawShape<MathLib::num_t>(shape, MathLib::CreateVector<MathLib::num_t>(0, 0, 0), UINT32_MAX);
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}