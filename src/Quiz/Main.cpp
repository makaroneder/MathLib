#include "Question.hpp"
#include <EquationSolver/Preprocesor.hpp>
#include <iostream>
#include <vector>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        const size_t retryCount = 3;
        num_t solved = 0;
        bool running = true;
        for (int i = 1; i < argc && running; i++) {
            const Question question = Question(Preproces(argv[i]));
            std::cout << question.equation;
            for (size_t j = 0; j < retryCount && running; j++) {
                std::cout << "> ";
                std::string tmp;
                std::cin >> tmp;
                if (tmp == "exit") running = false;
                else if (FloatsEqual<num_t>(std::stold(tmp), question.solution, 0.01)) {
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