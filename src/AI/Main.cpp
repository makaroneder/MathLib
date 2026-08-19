#include "NeuralNetwork.hpp"
#include "NeuralNetworkLayer.hpp"
#include "NeuralNetworkNeuron.hpp"
#include "NeuralNetworkConnection.hpp"
#include <Interfaces/IdentityFunction.hpp>
#include <iostream>

template <typename T>
size_t TestNeuralNetwork(const NeuralNetwork<T>& network, const MathLib::Function<bool, bool, bool>& func) {
    size_t failed = 0;
    for (uint8_t y = 0; y < 2; y++) {
        for (uint8_t x = 0; x < 2; x++) {
            const T prediction = network.Evaluate(MathLib::Matrix<DualNumber<T>>(2, 1, MathLib::MakeArray<DualNumber<T>>(x, y))).At(0, 0).real;
            const bool result = func(x, y);
            const bool correct = MathLib::Abs((T)result - prediction) < MathLib::Abs((T)!result - prediction);
            const MathLib::String tmp = MathLib::ToString(prediction);
            const size_t size = tmp.GetSize();
            std::cout << "f("_M + MathLib::ToString(x, 10) + ", " + MathLib::ToString(y, 10) + ") = " << tmp;
            for (size_t i = 0; i + size < 9; i++) std::cout << ' ';
            std::cout << "[" << (correct ? "PASSED" : "FAILED") << ']' << std::endl;
            failed += !correct;
        }
    }
    std::cout << "Failed: " << failed << std::endl;
    return failed;
}
int main(int, char**) {
    try {
        const auto& identity = MathLib::IdentityFunction<MathLib::Matrix<dual_t>, MathLib::Matrix<dual_t>>();
        const auto& sigmoid = MathLib::MakeFunctionT<MathLib::Matrix<dual_t>, MathLib::Matrix<dual_t>>([](MathLib::Matrix<dual_t> input) -> MathLib::Matrix<dual_t> {
            for (dual_t& x : input) x = x.Sigmoid();
            return input;
        });
        const auto& relu = MathLib::MakeFunctionT<MathLib::Matrix<dual_t>, MathLib::Matrix<dual_t>>([](MathLib::Matrix<dual_t> input) -> MathLib::Matrix<dual_t> {
            for (dual_t& x : input) x = x <= dual_t() ? 0 : x;
            return input;
        });
        const auto& func = MathLib::MakeFunctionT<bool, bool, bool>([](bool x, bool y) -> bool {
            return !x ^ y;
        });
        MathLib::Array<Point<MathLib::Matrix<dual_t>>> data = 4;
        for (uint8_t y = 0; y < 2; y++) {
            for (uint8_t x = 0; x < 2; x++) {
                data.At(y * 2 + x) = Point<MathLib::Matrix<dual_t>>(
                    MathLib::Matrix<dual_t>(2, 1, MathLib::MakeArray<dual_t>(x, y)),
                    MathLib::Matrix<dual_t>(1, 1, MathLib::MakeArray<dual_t>(func(x, y)))
                );
            }
        }
        const NeuralNetworkConnection<MathLib::num_t> neuron = NeuralNetworkConnection<MathLib::num_t>(
            new NeuralNetworkLayer<MathLib::num_t>(NeuralNetworkLayerMerge::SideBySide,
                new NeuralNetworkNeuron<MathLib::num_t>(
                    2, 1, 1, sigmoid
                ), new NeuralNetworkNeuron<MathLib::num_t>(
                    2, 1, 1, sigmoid
                )
            ), new NeuralNetworkNeuron<MathLib::num_t>(
                2, 1, 1, relu
            )
        );
        NeuralNetwork<MathLib::num_t> network = neuron.Copy();
        for (size_t i = 0; i < 10000; i++) {
            if (!(i % 500)) {
                std::cout << "Epoch: " << i << std::endl;
                const bool passed = !TestNeuralNetwork<MathLib::num_t>(network, func);
                std::cout << std::endl;
                if (passed) break;
            }
            network.TrainMeanSquaredError(data, 1e-1);
        }
        TestNeuralNetwork<MathLib::num_t>(network, func);
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}