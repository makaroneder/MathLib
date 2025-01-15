#include "Music.hpp"
#include <Libc/HostFileSystem.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <output file>");
        MathLib::HostFileSystem fs;
        const Music<MathLib::num_t> music = Music<MathLib::num_t>(44100, 32767, std::vector<MusicNote<MathLib::num_t>> {
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::E3, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::D2, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::C1, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::D2, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::E3, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::E3, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::E3, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::D2, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::D2, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::D2, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::E3, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::E3, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::E3, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::E3, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::D2, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::C1, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::D2, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::E3, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::E3, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::E3, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::E3, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::D2, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::D2, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::E3, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::D2, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
            MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::C1, 0.4), MusicNote<MathLib::num_t>(MusicNote<MathLib::num_t>::Type::None, 0.5),
        });
        if (!music.SaveFromPath(fs, argv[1])) MathLib::Panic("Failed to save music to output file");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}