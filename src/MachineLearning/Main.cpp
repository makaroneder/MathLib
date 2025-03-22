#define DefaultML 2
#if DefaultML == 0
#include "Adder.hpp"
#elif DefaultML == 1
#include "LogicGates.hpp"
#elif DefaultML == 2
#include "Multiplier.hpp"
#endif
#include <Libc/HostFileSystem.hpp>
#include <iostream>
#include <unistd.h>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        #ifdef Debug
        const MathLib::num_t start = MathLib::GetTime();
        #else
        srand(time(nullptr));
        #endif
        if (argc < 3) MathLib::Panic("Usage: "_M + argv[0] + " <file> <iteration count>");
        MathLib::HostFileSystem fs;
        const size_t printStatements = 10;
        const size_t limit = (size_t)MathLib::StringToNumber(MathLib::String(argv[2]));
        NeuralNetworkState<MathLib::num_t> state;
        if (access(argv[1], F_OK) == -1) {
            state = GetDefaultState();
            state.neuralNetwork.Random(0, 1);
        }
        else if (!state.LoadFromPath(fs, MathLib::String(argv[1]))) MathLib::Panic("Failed to load neural network");
        #ifdef Debug
        std::cout << "Start data:\n";
        std::cout << "Cost = " << state.Cost() << std::endl;
        std::cout << "Neural network:\n" << state.neuralNetwork << std::endl;
        #endif
        for (size_t i = 0; i < limit; i++) {
            #ifdef UseFiniteDiff
            const MathLib::NeuralNetwork<MathLib::num_t> diff = state.FiniteDiff();
            #else
            const MathLib::NeuralNetwork<MathLib::num_t> diff = state.Backprop().Get("Backprop algorithm failed");
            #endif
            state.Learn(diff);
            #ifdef Debug
            if (!(i % (limit / printStatements))) {
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
        std::cout << StateToString(state) << std::endl;
        if (!state.SaveFromPath(fs, MathLib::String(argv[1]))) MathLib::Panic("Failed to save neural network");
        #ifdef Debug
        std::cout << "Time: " << MathLib::GetTime() - start << " second(s)" << std::endl;
        #endif
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}