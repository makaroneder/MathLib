#include "Music.hpp"
#include <MathLib.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        #ifdef Debug
        const Test test = TestSelf();
        const size_t tests = test.GetRecordCount();
        const size_t passed = test.GetPassed();
        std::cout << test << passed << "/" << tests << " tests passed" << std::endl;
        if (passed != tests) Panic("Some tests failed");
        #endif
        if (argc < 2) Panic(String("Usage: ") + argv[0] + " <output file>");
        HostFileSystem fs;
        Music<num_t> music = Music<num_t>(44100, 32767, std::vector<MusicNote<num_t>> {
            MusicNote<num_t>(MusicNote<num_t>::Type::E3, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::D2, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::C1, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::D2, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::E3, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::E3, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::E3, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::D2, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::D2, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::D2, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::E3, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::E3, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::E3, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::E3, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::D2, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::C1, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::D2, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::E3, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::E3, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::E3, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::E3, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::D2, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::D2, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::E3, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::D2, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
            MusicNote<num_t>(MusicNote<num_t>::Type::C1, 0.4), MusicNote<num_t>(MusicNote<num_t>::Type::None, 0.5),
        });
        if (!music.SaveFromPath(fs, argv[1])) Panic("Failed to save music to output file");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}