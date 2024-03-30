#if 0
#include "Adder.hpp"
#else
#include "LogicGates.hpp"
#endif

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 3) throw std::runtime_error("No input file provided");
        #ifdef Debug
        const clock_t start = clock();
        #else
        srand(time(nullptr));
        #endif
        const size_t printStatements = 10;
        const size_t limit = std::stoull(argv[2]);
        State<num_t> state;
        if (access(argv[1], F_OK) == -1) {
            state = GetDefaultState<num_t>();
            state.neuralNetwork.Random(0, 1);
        }
        else if (!state.LoadFromPath(argv[1])) throw std::runtime_error("Failed to load neural network");
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
            if (diff.GetCount() == 0) throw std::runtime_error("Failed to update neural network");
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
        const size_t bits = state.trainingDataInput.GetWidth() / 2;
        const size_t n = (1 << bits);
        for (size_t y = 0; y < n; y++) {
            for (size_t x = 0; x < n; x++) {
                for (size_t i = 0; i < bits; i++) {
                    state.neuralNetwork.GetInput().At(i, 0) = (y >> i) & 1;
                    state.neuralNetwork.GetInput().At(i + bits, 0) = (x >> i) & 1;
                }
                state.Forward();
                size_t z = 0;
                for (size_t i = 0; i < state.trainingDataOutput.GetWidth(); i++) z |= (state.neuralNetwork.GetOutput().At(i, 0) > 0.5) << i;
                std::cout << '[' << y << ", " << x << "] => " << z << '\n';
            }
        }
        if (!state.SaveFromPath(argv[1])) throw std::runtime_error("Failed to save neural network");
        #ifdef Debug
        std::cout << (num_t)(clock() - start) / CLOCKS_PER_SEC << std::endl;
        #endif
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}