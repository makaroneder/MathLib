#define SDL_MAIN_HANDLED
#include "Lens.hpp"
#include <SDL2.cpp>
#include <iostream>

template <typename T>
MathLib::Matrix<T> Refract(const MathLib::Matrix<T>& direction, const MathLib::Matrix<T>& normal, T n1, T n2) {
    const T n = n1 / n2;
    const T cosI = -normal.Dot(direction);
    const T sinT2 = n * n * (1 - cosI * cosI);
    if (sinT2 > 1) return MathLib::CreateVector<T>(0, 0, 0);
    return direction * n + normal * (n * cosI - MathLib::Sqrt(1 - sinT2));
}
template <typename T>
MathLib::Ray<T> IntersectionToRay(const MathLib::Matrix<T>& dir, const MathLib::Matrix<T>& intersection, const MathLib::Matrix<T>& otherPosition, const T& refractionIndex1, const T& refractionIndex2, bool isInsideLens) {
    const MathLib::Matrix<T> normal = (intersection - otherPosition).Normalize();
    T n1 = refractionIndex1;
    T n2 = refractionIndex2;
    if (isInsideLens) MathLib::Swap<T>(n1, n2);
    return MathLib::Ray<T>(intersection, Refract<T>(dir, isInsideLens ? -normal : normal, n1, n2).Normalize());
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::SDL2Renderer renderer = MathLib::SDL2Renderer("Ray tracing", 800, 800);
        MathLib::Ray<MathLib::num_t> ray = MathLib::Ray<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(-3, 2, 0), MathLib::CreateVector<MathLib::num_t>(1, -1, 0));
        const MathLib::num_t airRefractionIndex = 1;
        bool isInsideLens = false;
        Lens<MathLib::num_t> lens[] = {
            Lens<MathLib::num_t>(MathLib::CreateVector<MathLib::num_t>(0, 0, 0), 1.5, 1),
        };
        MathLib::Array<MathLib::matrix_t> points = MathLib::MakeArrayFromSingle<MathLib::matrix_t>(ray.position);
        while (true) {
            MathLib::num_t closestIntersection = MathLib::MakeInf();
            size_t len = SIZE_MAX;
            for (size_t i = 0; i < SizeOfArray(lens); i++) {
                const MathLib::Expected<MathLib::num_t> intersection = lens[i].GetIntersection(ray);
                if (intersection.HasValue() && intersection.Get() < closestIntersection) {
                    closestIntersection = intersection.Get();
                    len = i;
                }
            }
            if (MathLib::IsInf(closestIntersection)) break;
            ray = IntersectionToRay<MathLib::num_t>(ray.GetDirection(), ray.At(closestIntersection), lens[len].position, airRefractionIndex, lens[len].refractionIndex, isInsideLens);
            if (!points.Add(ray.position)) MathLib::Panic("Failed to add point");
            isInsideLens = !isInsideLens;
            if (renderer.GetEvent().type == MathLib::Event::Type::Quit) break;
        }
        for (size_t i = 0; i < SizeOfArray(lens); i++)
            renderer.DrawCircle2D<MathLib::num_t>(lens[i].position, lens[i].radius, 0xff0000ff);
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