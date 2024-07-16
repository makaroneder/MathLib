#if 0
#include "Adder.hpp"
#else
#include "LogicGates.hpp"
#endif
#include <Libc/HostFileSystem.hpp>
#include <MathLib.hpp>
#include <iostream>
#include <unistd.h>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 3) Panic(String("Usage: ") + argv[0] + " <file> <iteration count>");
        #ifdef Debug
        const num_t start = GetTime();
        #else
        srand(time(nullptr));
        #endif
        HostFileSystem fs;
        const size_t printStatements = 10;
        const size_t limit = (size_t)StringToNumber(argv[2]);
        NeuralNetworkState<num_t> state;
        if (access(argv[1], F_OK) == -1) {
            state = GetDefaultState<num_t>();
            state.neuralNetwork.Random(0, 1);
        }
        else if (!state.LoadFromPath(fs, argv[1])) Panic("Failed to load neural network");
        #ifdef Debug
        std::cout << "Start data:\n";
        std::cout << "Cost = " << state.Cost() << std::endl;
        std::cout << "Neural network:\n" << state.neuralNetwork << std::endl;
        #endif
        for (size_t i = 0; i < limit; i++) {
            #ifdef UseFiniteDiff
            const NeuralNetwork<num_t> diff = state.FiniteDiff();
            #else
            const NeuralNetwork<num_t> diff = state.Backprop();
            #endif
            if (diff.GetCount() == 0) Panic("Failed to update neural network");
            state.Learn(diff);
            #ifdef Debug
            if (i % (limit / printStatements) == 0) {
                std::cout << i << ":\n";
                std::cout << "Cost = " << state.Cost() << std::endl;
                std::cout << "Neural network:\n" << state.neuralNetwork << std::endl;
                std::cout << "Diff:\n" << diff << std::endl;
            }
            #endif
        }
        #ifdef Debug
        std::cout << "End data:\n";
        std::cout << "Cost = " << state.Cost() << std::endl;
        std::cout << "Neural network:\n" << state.neuralNetwork << std::endl;
        #endif
        std::cout << StateToString<num_t>(state) << std::endl;
        if (!state.SaveFromPath(fs, argv[1])) Panic("Failed to save neural network");
        #ifdef Debug
        std::cout << "Time: " << GetTime() - start << " second(s)" << std::endl;
        #endif
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}