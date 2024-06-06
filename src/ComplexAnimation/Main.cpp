#include <Libc/HostFileSystem.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 3) Panic("No input files provided");
        SDL2Renderer renderer = SDL2Renderer("Complex animation", 800, 800);
        HostFileSystem fs;
        auto func = [](complex_t z) -> complex_t {
            return std::pow(Factorial(z), Factorial(z));
        };
        Array<complex_t> domain;
        for (num_t y = -4; y < 4; y += 1 / renderer.pointMultiplier) {
            if (FloatsEqual<num_t>(y, Round(y)))
                for (num_t x = -4; x < 4; x += 1 / renderer.pointMultiplier) domain.Add(complex_t(x, y));
            else for (num_t x = -4; x < 4; x++) domain.Add(complex_t(x, y));
        }
        const num_t iter = 100;
        Array<complex_t> directions = Array<complex_t>(domain.GetSize());
        for (size_t i = 0; i < domain.GetSize(); i++) directions.At(i) = (func(domain.At(i)) - domain.At(i)) / iter;
        for (num_t i = 0; i <= iter; i++) {
            renderer.Fill(0x000000ff);
            renderer.DrawAxis(0xffffffff, 0x808080ff);
            for (size_t j = 0; j < domain.GetSize(); j++) {
                const complex_t dir = domain.At(j) + directions.At(j) * i;
                renderer.SetPixel<num_t>(CreateVector<num_t>(dir.real(), dir.imag(), 0), 0xff0000ff);
            }
            if (!renderer.Update()) Panic("Failed to update renderer");
            if (!renderer.SaveFromPath(fs, String(argv[1]) + "/Stage" + ToString((size_t)i) + ".ppm")) Panic("Failed to save frame");
        }
        if (system((String("ffmpeg -hide_banner -y -framerate 30 -i ") + String(argv[1]) + String("/Stage%d.ppm -c:v libx264 -r 30 ") + String(argv[2])).GetValue())) Panic("Failed to generate video");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}