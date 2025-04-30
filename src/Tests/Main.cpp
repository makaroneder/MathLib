#include <Tests/Test.hpp>
#include <Host.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        const MathLib::num_t start = MathLib::GetTime();
        const MathLib::Test test = MathLib::TestSelf();
        std::cout << test << std::endl;
        std::cout << "Time: " << MathLib::GetTime() - start << std::endl;
        if (!test.Passed()) MathLib::Panic("Some tests failed");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}