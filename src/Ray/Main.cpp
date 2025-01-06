#define SDL_MAIN_HANDLED
#include "Sphere.hpp"
#include <MathLib.hpp>
#include <Libc/HostFunction.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2Renderer renderer = MathLib::SDL2Renderer("Ray tracing", 800, 800);
        MathLib::Threads threads;
        bool quit = false;
        for (MathLib::num_t t = -4; t <= 4 && !quit; t += 0.01) {
            MathLib::HostFunction<void, MathLib::Interval<ssize_t>> function = MathLib::HostFunction<void, MathLib::Interval<ssize_t>>(nullptr, [&renderer, &quit, t](const void*, MathLib::Interval<ssize_t> interval) {
                const MathLib::num_t start = (MathLib::num_t)interval.GetMin() / 100;
                const MathLib::num_t end = (MathLib::num_t)interval.GetMax() / 100;
                Sphere<MathLib::num_t> sphere = Sphere<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(t, 0, 0), 1);
                const MathLib::matrix_t rayStart = MathLib::CreateVector<MathLib::num_t>(0, 0, -2);
                renderer.Fill(0);
                for (MathLib::num_t y = start; y <= end && !quit; y += 0.1) {
                    for (MathLib::num_t x = -4; x <= 4 && !quit; x += 0.1) {
                        const MathLib::matrix_t pos = MathLib::CreateVector<MathLib::num_t>(x, y, 0);
                        const MathLib::Ray<MathLib::num_t> ray = MathLib::Ray<MathLib::num_t>(MathLib::Line<MathLib::num_t>(rayStart, pos));
                        if (sphere.HasIntersections(ray)) renderer.SetPixel<MathLib::num_t>(pos, UINT32_MAX);
                        if (renderer.GetEvent().type == MathLib::Event::Type::Quit) quit = true;
                    }
                }
            });
            threads.RunAndJoin(function, MathLib::Interval<ssize_t>(-400, 400));
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
        }
        while (renderer.GetEvent().type != MathLib::Event::Type::Quit)
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}