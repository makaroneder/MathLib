#define SDL_MAIN_HANDLED
#include <MathLib.hpp>
#include <SDL2.cpp>
#include <iostream>

bool SetImageInterface(MathLib::Renderer& renderer, const MathLib::String& type) {
    if (type == "P6") return renderer.SetImage<MathLib::P6>();
    else if (type == "TGA") return renderer.SetImage<MathLib::TGA>();
    else if (type == "Aseprite") return renderer.SetImage<MathLib::Aseprite>();
    else return false;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        MathLib::SDL2Renderer renderer = MathLib::SDL2Renderer("Image converter", 800, 800);
        MathLib::DummyRenderer dummyRenderer = MathLib::DummyRenderer(0, 0);
        MathLib::HostFileSystem fs;
        const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
        if (!SetImageInterface(dummyRenderer, cmdLine.GetEntry("inputType").Get("No input type specified"))) MathLib::Panic("Invalid inputType");
        if (!dummyRenderer.LoadFromPath(fs, cmdLine.GetEntry("input").Get("No input specified"))) MathLib::Panic("Failed to load image data");
        if (!SetImageInterface(dummyRenderer, cmdLine.GetEntry("outputType").Get("No output type specified"))) MathLib::Panic("Invalid outputType");
        if (!dummyRenderer.SaveFromPath(fs, cmdLine.GetEntry("output").Get("No output specified"))) MathLib::Panic("Failed to save image data");
        renderer.DrawImage<MathLib::num_t>(dummyRenderer, MathLib::CreateVector<MathLib::num_t>(0, 0, 0));
        while (renderer.GetEvent().type != MathLib::Event::Type::Quit) renderer.Update();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}