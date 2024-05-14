#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 3) throw std::runtime_error("No input files provided");
        SDL2Renderer renderer = SDL2Renderer("Complex Animation", 800, 800);
        auto func = [](complex_t z) -> complex_t {
            return std::pow(Factorial(z), Factorial(z));
        };
        std::vector<complex_t> domain;
        for (num_t y = -4; y < 4; y += 1 / renderer.pointMultiplier) {
            if (FloatsEqual<num_t>(y, std::round(y)))
                for (num_t x = -4; x < 4; x += 1 / renderer.pointMultiplier) domain.push_back(complex_t(x, y));
            else for (num_t x = -4; x < 4; x++) domain.push_back(complex_t(x, y));
        }
        const num_t iter = 100;
        std::vector<complex_t> directions;
        for (const complex_t& z : domain) directions.push_back((func(z) - z) / iter);
        for (num_t i = 0; i <= iter; i++) {
            renderer.Fill(0x000000ff);
            renderer.DrawAxis(0xffffffff, 0x808080ff);
            for (size_t j = 0; j < domain.size(); j++) {
                const complex_t dir = domain.at(j) + directions.at(j) * i;
                renderer.SetPixel<num_t>(CreateVector<num_t>(dir.real(), dir.imag(), 0), 0xff0000ff);
            }
            if (!renderer.Update()) throw std::runtime_error("Failed to update renderer");
            if (!renderer.SaveFromPath(std::string(argv[1]) + "/Stage" + std::to_string((size_t)i) + ".ppm")) throw std::runtime_error("Failed to save frame");
        }
        if (system(("ffmpeg -hide_banner -y -framerate 30 -i " + std::string(argv[1]) + "/Stage%d.ppm -c:v libx264 -r 30 " + argv[2]).c_str())) throw std::runtime_error("Failed to generate video");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}