#define SDL_MAIN_HANDLED
#include <Libc/HostFileSystem.hpp>
#include <FunctionPointer.hpp>
#include <Image/TGA/TGA.hpp>
#include <CommandLine.hpp>
#include <SDL2.cpp>
#include <iostream>

uint32_t GetPixel(const MathLib::Image& image, ssize_t x, ssize_t y) {
    return image.At(MathLib::Clamp<ssize_t>(x, 0, image.GetWidth() - 1), MathLib::Clamp<ssize_t>(y, 0, image.GetHeight() - 1));
}
template <typename T>
uint32_t Convolution(const MathLib::Matrix<T>& filterKernel, const MathLib::Image& image, size_t x0, size_t y0) {
    const size_t w = filterKernel.GetWidth();
    const size_t h = filterKernel.GetHeight();
    T ret = 0;
    for (size_t y = 0; y < h; y++)
        for (size_t x = 0; x < w; x++)
            ret += filterKernel.At(x, y) * GetPixel(image, x + x0 - w / 2, y + y0 - h / 2);
    return ret;
}
template <typename T>
MathLib::Image Convolution(const MathLib::Matrix<T>& filterKernel, const MathLib::Image& image) {
    const size_t w = image.GetWidth();
    const size_t h = image.GetHeight();
    MathLib::Image ret = MathLib::Image(w, h);
    for (size_t y = 0; y < h; y++)
        for (size_t x = 0; x < w; x++)
            ret.At(x, y) = Convolution(filterKernel, image, x, y);
    return ret;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        MathLib::SDL2 sdl2;
        const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Image processing", 800, 800);
        MathLib::HostFileSystem fs;
        MathLib::TGA tga = MathLib::TGA(0, 0);
        if (!tga.LoadFromPath(fs, cmdLine.GetEntry("input"_M).Get("No input specified"))) MathLib::Panic("Failed to load input image");
        MathLib::matrix_t kernel = MathLib::matrix_t(3, 3);
        kernel.At(1, 1) = 5;
        kernel.At(0, 1) = -1;
        kernel.At(2, 1) = -1;
        kernel.At(1, 0) = -1;
        kernel.At(1, 2) = -1;
        std::cout << kernel << std::endl;
        renderer.DrawImage<MathLib::num_t>(Convolution(kernel, tga), MathLib::CreateVector<MathLib::num_t>(0, 0, 0));
        while (renderer.GetEvent().type != MathLib::Event::Type::Quit)
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}