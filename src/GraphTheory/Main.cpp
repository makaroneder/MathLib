#include <Graph/Graph.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::Graph<MathLib::num_t> graph = MathLib::Graph<MathLib::num_t>(5);
        graph.SetWeight(0, 1, 7, true);
        graph.SetWeight(0, 4, 1, true);
        graph.SetWeight(1, 2, 3, true);
        graph.SetWeight(1, 4, 8, true);
        graph.SetWeight(2, 3, 6, true);
        graph.SetWeight(2, 4, 2, true);
        graph.SetWeight(3, 4, 7, true);
        std::cout << graph << std::endl;
        std::cout << graph.DijkstrasAlgorithm(0).Get() << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}