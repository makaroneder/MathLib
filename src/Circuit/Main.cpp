#include "Circuit.hpp"
#include "Gate/Clock.hpp"
#include "Gate/Memory.hpp"
#include "Gate/Logic/ConstantGate.hpp"
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        Circuit circuit;
        if (!circuit.SetOutput(circuit.Add(new Memory(2), MathLib::MakeArray<CircuitConnection>(
            CircuitConnection(circuit.Add(new ConstantGate(MathLib::MakeBitmap(false)), MathLib::Array<CircuitConnection>()), 0),
            CircuitConnection(circuit.Add(new Clock(false), MathLib::Array<CircuitConnection>()), 0),
            CircuitConnection(SIZE_MAX, 0)
        )))) MathLib::Panic("Failed to create circuit");
        std::cout << "Circuit: " << circuit << std::endl;
        std::cout << MathLib::ToString<bool>(circuit.Update(MathLib::MakeBitmap(true))) << std::endl;
        std::cout << MathLib::ToString<bool>(circuit.Update(MathLib::MakeBitmap(true))) << std::endl;
        std::cout << MathLib::ToString<bool>(circuit.Update(MathLib::MakeBitmap(false))) << std::endl;
        std::cout << MathLib::ToString<bool>(circuit.Update(MathLib::MakeBitmap(false))) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}