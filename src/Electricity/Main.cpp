#include <Typedefs.hpp>
#include <Host.hpp>
#include <iostream>

struct CircuitState {
    MathLib::num_t voltage;
    MathLib::num_t current;

    CircuitState(MathLib::num_t voltage, MathLib::num_t current);
};
CircuitState::CircuitState(MathLib::num_t voltage, MathLib::num_t current) : voltage(voltage), current(current) {}
struct Circuit {
    virtual CircuitState GetState(CircuitState state) const = 0;
    virtual MathLib::num_t GetResistance(void) const = 0;
};
struct CircuitElement : Circuit {
    CircuitElement(MathLib::num_t resistance);
    virtual CircuitState GetState(CircuitState state) const override;
    virtual MathLib::num_t GetResistance(void) const override;

    private:
    MathLib::num_t resistance;
};
CircuitElement::CircuitElement(MathLib::num_t resistance) : resistance(resistance) {}
CircuitState CircuitElement::GetState(CircuitState state) const {
    return CircuitState(state.voltage - resistance * state.current, state.current);
}
MathLib::num_t CircuitElement::GetResistance(void) const {
    return resistance;
}
struct ParallelCircuit : Circuit {
    ParallelCircuit(const Circuit* a, const Circuit* b);
    virtual CircuitState GetState(CircuitState state) const override;
    virtual MathLib::num_t GetResistance(void) const override;

    private:
    const Circuit* a;
    const Circuit* b;
};
ParallelCircuit::ParallelCircuit(const Circuit* a, const Circuit* b) : a(a), b(b) {}
CircuitState ParallelCircuit::GetState(CircuitState state) const {
    const MathLib::num_t currentA = state.voltage / a->GetResistance();
    const MathLib::num_t currentB = state.voltage / b->GetResistance();
    if (!MathLib::FloatsEqual(state.current, currentA + currentB)) return CircuitState(MathLib::nan, MathLib::nan);
    const CircuitState stateA = a->GetState(CircuitState(state.voltage, currentA));
    const CircuitState stateB = b->GetState(CircuitState(state.voltage, currentB));
    if (!MathLib::FloatsEqual(stateA.voltage, stateB.voltage)) return CircuitState(MathLib::nan, MathLib::nan);
    return CircuitState(stateA.voltage, stateA.current + stateB.current);
}
MathLib::num_t ParallelCircuit::GetResistance(void) const {
    return 1 / (1 / a->GetResistance() + 1 / b->GetResistance());
}

struct SeriesCircuit : Circuit {
    SeriesCircuit(const Circuit* a, const Circuit* b);
    virtual CircuitState GetState(CircuitState state) const override;
    virtual MathLib::num_t GetResistance(void) const override;

    private:
    const Circuit* a;
    const Circuit* b;
};
SeriesCircuit::SeriesCircuit(const Circuit* a, const Circuit* b) : a(a), b(b) {}
CircuitState SeriesCircuit::GetState(CircuitState state) const {
    return b->GetState(a->GetState(state));
}
MathLib::num_t SeriesCircuit::GetResistance(void) const {
    return a->GetResistance() + b->GetResistance();
}

int main(int, char**) {
    try {
        const MathLib::num_t voltage = 5;
        const CircuitElement a = 100;
        const CircuitElement b = 1000;
        const SeriesCircuit v = SeriesCircuit(&a, &b);
        const MathLib::num_t current = voltage / v.GetResistance();
        const CircuitState state = v.GetState(CircuitState(voltage, current));
        std::cout << "I : [" << current << ", " << state.current << ']' << std::endl;
        std::cout << "V : [" << voltage << ", " << state.voltage << ']' << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}