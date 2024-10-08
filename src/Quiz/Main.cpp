#include "Question.hpp"
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
        if (argc < 2) Panic(String("Usage: ") + argv[0] + " <input files>");
        HostFileSystem fs;
        const size_t retryCount = 3;
        num_t solved = 0;
        bool running = true;
        for (int i = 1; i < argc && running; i++) {
            const Question question = Question(Preproces(fs, argv[i]));
            std::cout << question.equation;
            for (size_t j = 0; j < retryCount && running; j++) {
                std::cout << "> ";
                std::string tmp;
                std::cin >> tmp;
                if (tmp == "exit") running = false;
                else if (FloatsEqual<num_t>(StringToNumber(tmp), question.solution, 0.01)) {
                    solved++;
                    break;
                }
                else std::cout << "Invalid answer\n";
            }
        }
        if (argc != 1) std::cout << "You solved " << (solved / (argc - 1) * 100) << "% of equations (" << solved << "/" << (argc - 1) << ")\n" << std::flush;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}