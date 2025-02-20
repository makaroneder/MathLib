#include "ConstGate.hpp"
#include "Circuit.hpp"
#include "AndGate.hpp"
#include "XorGate.hpp"
#include "OrGate.hpp"
#include <iostream>

// TODO: Nor, Nand, Xnor

#define MakeElement(name, element)                                                      \
    if (!elements.Add(element)) MathLib::Panic("Failed to add element to the circuit"); \
    const size_t name = elements.GetSize() - 1

[[nodiscard]] size_t Create1BitAdder(CircuitElementConnection a, CircuitElementConnection b, CircuitElementConnection carryIn) {
    MakeElement(xor1, new XorGate(MathLib::MakeArray<CircuitElementConnection>(a, b)));
    MakeElement(and1, new AndGate(MathLib::MakeArray<CircuitElementConnection>(a, b)));
    MakeElement(sum, new XorGate(MathLib::MakeArray<CircuitElementConnection>(CircuitElementConnection(xor1, 0), carryIn)));
    MakeElement(and2, new AndGate(MathLib::MakeArray<CircuitElementConnection>(CircuitElementConnection(xor1, 0), carryIn)));
    MakeElement(carryOut, new OrGate(MathLib::MakeArray<CircuitElementConnection>(CircuitElementConnection(and1, 0), CircuitElementConnection(and2, 0))));
    MakeElement(ret, new Circuit(MathLib::MakeArray<CircuitElementConnection>(
        CircuitElementConnection(sum, 0),
        CircuitElementConnection(carryOut, 0)
    )));
    return ret;
}
[[nodiscard]] MathLib::Expected<Circuit> CreateAdder(uint8_t bits, size_t a, size_t b, const CircuitElementConnection& carryIn, const CircuitElementConnection& low, const CircuitElementConnection& high) {
    MathLib::Array<CircuitElementConnection> adders;
    for (uint8_t i = 0; i < bits; i++) {
        const size_t element = Create1BitAdder(a & (1 << i) ? high : low, b & (1 << i) ? high : low, i ? CircuitElementConnection(adders.At(i - 1).index, 1) : carryIn);
        if (!adders.Add(CircuitElementConnection(element, 0)) || !(i + 1 == bits && !adders.Add(CircuitElementConnection(element, 1)))) return MathLib::Expected<Circuit>();
    }
    return MathLib::Expected<Circuit>(Circuit(adders));
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
        const Circuit circuit = CreateAdder(bits, a, b, carryIn, CircuitElementConnection(low, 0), CircuitElementConnection(high, 0)).Get("Failed to create adder");
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