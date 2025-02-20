#define SDL_MAIN_HANDLED
#include "Lens.hpp"
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Ray tracing", 800, 800);
        MathLib::Ray<MathLib::num_t> ray = MathLib::Ray<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(-3, 2, 0), MathLib::CreateVector<MathLib::num_t>(1, -1, 0));
        const MathLib::num_t airRefractionIndex = 1;
        bool isInsideLens = false;
        Lens<MathLib::num_t> lens[] = {
            Lens<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(0, 0, 0), 1.5, 1),
        };
        MathLib::Array<MathLib::matrix_t> points = MathLib::MakeArray<MathLib::matrix_t>(ray.position);
        while (true) {
            MathLib::num_t closestIntersection = MathLib::infinity;
            size_t len = SIZE_MAX;
            for (size_t i = 0; i < SizeOfArray(lens); i++) {
                const MathLib::Expected<MathLib::num_t> intersection = lens[i].GetIntersection(ray);
                if (intersection.HasValue() && intersection.Get() < closestIntersection) {
                    closestIntersection = intersection.Get();
                    len = i;
                }
            }
            if (MathLib::IsInf(closestIntersection)) break;
            ray = lens[len].IntersectionToRay(ray.GetDirection(), ray.At(closestIntersection), airRefractionIndex, isInsideLens);
            if (!points.Add(ray.position)) MathLib::Panic("Failed to add point");
            isInsideLens = !isInsideLens;
            if (renderer.GetEvent().type == MathLib::Event::Type::Quit) break;
        }
        renderer.Fill(0);
        for (size_t i = 0; i < SizeOfArray(lens); i++)
            lens[i].Draw(renderer, 0xff0000ff);
        for (size_t i = 1; i < points.GetSize(); i++)
            renderer.DrawLine<MathLib::num_t>(MathLib::Line<MathLib::num_t>(points.At(i - 1), points.At(i)), UINT32_MAX);
        renderer.DrawShape<MathLib::num_t>(ray, MathLib::CreateVector<MathLib::num_t>(0, 0, 0), 0x00ff00ff);
        while (true) {
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            if (renderer.GetEvent().type == MathLib::Event::Type::Quit) break;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}