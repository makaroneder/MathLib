#define SDL_MAIN_HANDLED
#include <Image/Aseprite/Aseprite.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Image/Animation.hpp>
#include <CommandLine.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
        const size_t width = MathLib::StringToNumber(cmdLine.GetEntry("width"_M).Get("No width specified"));
        const size_t height = MathLib::StringToNumber(cmdLine.GetEntry("height"_M).Get("No height specified"));
        const MathLib::num_t speed = MathLib::StringToNumber(cmdLine.GetEntry("speed"_M).Get("No speed specified"));
        const MathLib::num_t skipDuration = MathLib::StringToNumber(cmdLine.GetEntry("skipDuration"_M).Get("No skip duration specified"));
        const size_t multX = MathLib::StringToNumber(cmdLine.GetEntry("multX"_M).Get("No X multiplier specified"));
        const size_t multY = MathLib::StringToNumber(cmdLine.GetEntry("multY"_M).Get("No Y multiplier specified"));
        const MathLib::String path = cmdLine.GetEntry("path"_M).Get("No path specified");

        const size_t centerX = width / 2;
        const size_t centerY = height / 2;
        MathLib::SDL2 sdl2;
        MathLib::SDL2Renderer renderer = sdl2.MakeRenderer("Video player", width, height);
        MathLib::HostFileSystem fs;
        MathLib::Aseprite video;
        if (!video.LoadFromPath(fs, path)) MathLib::Panic("Failed to load video");
        video.Resize(multX, multY);
        MathLib::Animation animation = &video;
        MathLib::num_t prevTime = MathLib::GetTime();
        bool running = true;
        while (true) {
            const MathLib::num_t time = MathLib::GetTime();
            const MathLib::num_t deltaTime = time - prevTime;
            prevTime = time;
            std::cout << "FPS: " << 1 / deltaTime << std::endl;
            if (running) animation.Update(deltaTime * speed);
            renderer.Fill(0);
            animation.Draw(renderer, centerX, centerY);
            if (!renderer.Update()) MathLib::Panic("Failed to update UI");
            const MathLib::Event event = renderer.GetEvent();
            if (event.type == MathLib::Event::Type::Quit) break;
            else if (event.type == MathLib::Event::Type::KeyPressed && event.pressed) {
                switch (event.key) {
                    case ' ': {
                        running = !running;
                        break;
                    }
                    case 'a': {
                        animation.Reverse(skipDuration);
                        break;
                    }
                    case 'd': {
                        animation.Update(skipDuration);
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