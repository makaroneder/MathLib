#include "Question.hpp"
#include <EquationSolver/Preprocesor.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Host.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <input files>");
        MathLib::HostFileSystem fs;
        const size_t retryCount = 3;
        MathLib::num_t solved = 0;
        bool running = true;
        for (int i = 1; i < argc && running; i++) {
            const Question question = Question(MathLib::Preproces(fs, MathLib::String(argv[i])));
            std::cout << question;
            for (size_t j = 0; j < retryCount && running; j++) {
                std::cout << "> ";
                std::string tmp;
                std::cin >> tmp;
                if (tmp == "exit") running = false;
                else if (question.IsSolution(MathLib::StringToNumber(MathLib::String(tmp)), 0.01)) {
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