#include "ConstGate.hpp"
#include "Circuit.hpp"
#include "AndGate.hpp"
#include "XorGate.hpp"
#include "OrGate.hpp"
#include <MathLib.hpp>
#include <iostream>

// TODO: Nor, Nand, Xnor

#define MakeElement(name, element)              \
    elements.Add(element);                      \
    const size_t name = elements.GetSize() - 1

size_t Create1BitAdder(CircuitElementConnection a, CircuitElementConnection b, CircuitElementConnection carryIn) {
    MakeElement(xor1, new XorGate(std::vector<CircuitElementConnection> { a, b, }));
    MakeElement(and1, new AndGate(std::vector<CircuitElementConnection> { a, b, }));
    MakeElement(sum, new XorGate(std::vector<CircuitElementConnection> { CircuitElementConnection(xor1, 0), carryIn, }));
    MakeElement(and2, new AndGate(std::vector<CircuitElementConnection> { CircuitElementConnection(xor1, 0), carryIn, }));
    MakeElement(carryOut, new OrGate(std::vector<CircuitElementConnection> { CircuitElementConnection(and1, 0), CircuitElementConnection(and2, 0), }));
    MakeElement(ret, new Circuit(std::vector<CircuitElementConnection> {
        CircuitElementConnection(sum, 0),
        CircuitElementConnection(carryOut, 0),
    }));
    return ret;
}
Circuit CreateAdder(uint8_t bits, size_t a, size_t b, const CircuitElementConnection& carryIn, const CircuitElementConnection& low, const CircuitElementConnection& high) {
    MathLib::Array<CircuitElementConnection> adders;
    for (uint8_t i = 0; i < bits; i++) {
        const size_t element = Create1BitAdder(a & (1 << i) ? high : low, b & (1 << i) ? high : low, i ? CircuitElementConnection(adders.At(i - 1).index, 1) : carryIn);
        adders.Add(CircuitElementConnection(element, 0));
        if (i + 1 == bits) adders.Add(CircuitElementConnection(element, 1));
    }
    return Circuit(adders);
}

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MakeElement(high, new ConstGate(true));
        MakeElement(low, new ConstGate(false));
        const uint8_t bits = 4;
        const size_t a = 0b1111;
        const size_t b = 0b0001;
        const CircuitElementConnection carryIn = CircuitElementConnection(low, 0);
        const Circuit circuit = CreateAdder(bits, a, b, carryIn, CircuitElementConnection(low, 0), CircuitElementConnection(high, 0));
        std::cout << circuit << std::endl;
        const MathLib::Bitmap bitmap = circuit.Evaluate().Get("Failed to run circuit");
        for (size_t i = 0; i < bitmap.GetSize(); i++) std::cout << ConstGate(bitmap.Get(i).Get("Failed to run circuit")) << std::endl;
        for (CircuitElement*& element : elements) delete element;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}