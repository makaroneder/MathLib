#define SDL_MAIN_HANDLED
#include <Image/Aseprite/Aseprite.hpp>
#include <Libc/HostFileSystem.hpp>
#include <DummyRenderer.hpp>
#include <Image/TGA/TGA.hpp>
#include <Image/PPM/P6.hpp>
#include <CommandLine.hpp>
#include <SDL2.cpp>
#include <iostream>

[[nodiscard]] bool SetImageInterface(MathLib::Renderer& renderer, const MathLib::Sequence<char>& type) {
    if (type == "P6"_M) return renderer.SetImage<MathLib::P6>();
    else if (type == "TGA"_M) return renderer.SetImage<MathLib::TGA>();
    else if (type == "Aseprite"_M) return renderer.SetImage<MathLib::Aseprite>();
    else return false;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Image converter", 800, 800);
        MathLib::DummyRenderer dummyRenderer = MathLib::DummyRenderer(0, 0);
        MathLib::HostFileSystem fs;
        const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
        if (!SetImageInterface(dummyRenderer, cmdLine.GetEntry("inputType"_M).Get("No input type specified"))) MathLib::Panic("Invalid inputType");
        if (!dummyRenderer.LoadFromPath(fs, cmdLine.GetEntry("input"_M).Get("No input specified"))) MathLib::Panic("Failed to load image data");
        if (!SetImageInterface(dummyRenderer, cmdLine.GetEntry("outputType"_M).Get("No output type specified"))) MathLib::Panic("Invalid outputType");
        if (!dummyRenderer.SaveFromPath(fs, cmdLine.GetEntry("output"_M).Get("No output specified"))) MathLib::Panic("Failed to save image data");
        if (!renderer.DrawImage<MathLib::num_t>(dummyRenderer, MathLib::CreateVector<MathLib::num_t>(0, 0, 0))) MathLib::Panic("Failed to render image");
        while (renderer.GetEvent().type != MathLib::Event::Type::Quit)
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}