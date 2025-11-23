#define SDL_MAIN_HANDLED
#include "STL/STL.hpp"
#include "QuakeMDL/QuakeMDL.hpp"
#include <Interfaces/Sequence/TransformSequence.hpp>
#include <Geometry/WavefrontObject.hpp>
#include <Libc/HostFileSystem.hpp>
#include <CommandLine.hpp>
#include <SDL2.cpp>
#include <iostream>

template <typename T>
Model3D<T> WavefrontObjectToModel3D(const MathLib::WavefrontObject<T>& object) {
    const auto func = MathLib::MakeFunctionT<MathLib::Vector3<T>, size_t>([&object](size_t i) -> MathLib::Vector3<T> {
        return object.verticies.At(i) + object.position;
    });
    MathLib::Array<MathLib::Vector3<T>> ret;
    for (const MathLib::Array<size_t>& face : object.faces)
        if (!ret.AddSequence(MathLib::TransformSequence<MathLib::Vector3<T>, size_t>(MathLib::PolygonToTriangles<T, size_t>(face, func), func))) return Model3D<T>();
    return ret;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("3D viewer", 800, 800);
        GetZ(renderer.position) = -14;
        MathLib::HostFileSystem fs;
        const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
        const MathLib::String inputType = cmdLine.GetEntry("inputType"_M).Get("No input type specified");
        const MathLib::String input = cmdLine.GetEntry("input"_M).Get("No input specified");
        Model3D<MathLib::num_t> model;
        if (inputType == "STL") {
            STL<MathLib::num_t> stl;
            if (!stl.LoadFromPath(fs, input)) MathLib::Panic("Failed to load input file");
            model = stl;
        }
        else if (inputType == "QuakeMDL") {
            QuakeMDL<MathLib::num_t> mdl;
            if (!mdl.LoadFromPath(fs, input)) MathLib::Panic("Failed to load input file");
            model = mdl.At(0);
        }
        else if (inputType == "WavefrontObject") {
            MathLib::WavefrontObject<MathLib::num_t> obj;
            if (!obj.LoadFromPath(fs, input)) MathLib::Panic("Failed to load input file");
            model = WavefrontObjectToModel3D<MathLib::num_t>(obj);
        }
        else MathLib::Panic("Unknown input type");
        MathLib::matrix_t velocity = MathLib::matrix_t(3, 1);
        const Triangle<MathLib::num_t> triangle = Triangle<MathLib::num_t>(
            MathLib::vector3_t(0, 0, 0),
            MathLib::vector3_t(1, 1, 0),
            MathLib::vector3_t(1, 0, 0)
        );
        MathLib::num_t prevTime = MathLib::GetTime();
        while (true) {
            const MathLib::num_t time = MathLib::GetTime();
            const MathLib::num_t deltaTime = time - prevTime;
            std::cout << 1 / deltaTime << std::endl;
            prevTime = time;
            renderer.position += velocity * deltaTime;
            renderer.Fill(0);
            model.Draw(renderer, UINT32_MAX);
            // model.DrawWithBackFaceCulling(renderer, UINT32_MAX);
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            if (event.type == MathLib::Event::Type::KeyPressed) {
                switch (event.key) {
                    case 'w': {
                        GetZ(velocity) = event.pressed;
                        break;
                    }
                    case 's': {
                        GetZ(velocity) = -event.pressed;
                        break;
                    }
                    case 'a': {
                        GetX(velocity) = -event.pressed;
                        break;
                    }
                    case 'd': {
                        GetX(velocity) = event.pressed;
                        break;
                    }
                    default: break;
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