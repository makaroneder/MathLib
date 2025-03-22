#include "FiniteStateMachine.hpp"

FiniteStateMachine::FiniteStateMachine(void) : MathLib::Automaton<uint8_t, uint8_t, uint8_t>(0, 0) {}
MathLib::Set<MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>> FiniteStateMachine::MakeRunData(const uint8_t& state, const MathLib::Tape<uint8_t>& inputTape, const MathLib::Array<MathLib::Tape<uint8_t>>& tapes) const {
    MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t> ret = MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>(state, inputTape, tapes);
    if (inputTape.IsOutside()) {
        if (state) return MathLib::Set<MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>>();
        ret.output = state;
    }
    return MathLib::Set<MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>>(ret);
}
MathLib::Set<MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>> FiniteStateMachine::RunSingle(const uint8_t& state, MathLib::Tape<uint8_t> inputTape, MathLib::Array<MathLib::Tape<uint8_t>> tapes) const {
    if (state && state != 1) return MathLib::Set<MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>>();
    if (inputTape.IsOutside()) return MakeRunData(state, inputTape, tapes);
    const uint8_t input = inputTape.Pop();
    if (input && input != 1) return MathLib::Set<MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>>();
    return MakeRunData(input ? state : !state, inputTape, tapes);
}