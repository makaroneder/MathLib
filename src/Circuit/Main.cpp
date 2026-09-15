#include "Circuit.hpp"
#include "Gate/Clock.hpp"
#include "Gate/Memory.hpp"
#include "Gate/Logic/ConstantGate.hpp"
#include <iostream>

void Main(int, char**, MathLib::FileSystem&) {
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
}