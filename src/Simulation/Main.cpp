#include "Rope.hpp"
#include <MathLib.hpp>
#include <SDL2.cpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        #ifdef Debug
        const Test test = TestSelf();
        const size_t tests = test.GetRecordCount();
        const size_t passed = test.GetPassed();
        std::cout << test << passed << "/" << tests << " tests passed" << std::endl;
        if (passed != tests) Panic("Some tests failed");
        #endif
        SDL2Renderer renderer = SDL2Renderer("Physics simulation", 800, 800);
        Array<Matrix<num_t>> points;
        for (num_t i = 0; i <= 5; i += 0.1) points.Add(CreateVector<num_t>(i, 3, 0));
        Rope<num_t> rope = Rope<num_t>(CreateVector<num_t>(0, -9.81, 0), points);
        Second<num_t> prevTime = Second<num_t>(GetTime());
        while (true) {
            const Second<num_t> currTime = Second<num_t>(GetTime());
            const Second<num_t> dt = currTime - prevTime;
            prevTime = currTime;
            rope.Update(dt);
            renderer.Fill(0);
            renderer.DrawShape<num_t>(rope, CreateVector<num_t>(0, 0, 0), 0xff0000ff);
            if (!renderer.Update()) Panic("Failed to update UI");
            if (renderer.GetEvent().type == Event::Type::Quit) break;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}