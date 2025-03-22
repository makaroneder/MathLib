#include "BusyBeaver.hpp"

BusyBeaver::BusyBeaver(void) : MathLib::Automaton<uint8_t, uint8_t, uint8_t>(0, 0) {}
MathLib::Set<MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>> BusyBeaver::RunSingle(const uint8_t& state, MathLib::Tape<uint8_t> inputTape, MathLib::Array<MathLib::Tape<uint8_t>> tapes) const {
    if (!IsBetween(state, 0, 2)) return MathLib::Set<MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>>();
    const uint8_t input = inputTape.Pop();
    if (input && input != 1) return MathLib::Set<MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>>();
    inputTape.Push(1);
    uint8_t newState;
    switch (state) {
        case 0: {
            if (input) {
                inputTape.Move(-1);
                newState = 2;
            }
            else {
                inputTape.Move(1);
                newState = 1;
            }
            break;
        }
        case 1: {
            if (input) {
                inputTape.Move(1);
                newState = 1;
            }
            else {
                inputTape.Move(-1);
                newState = 0;
            }
            break;
        }
        case 2: {
            if (input) {
                inputTape.Move(1);
                newState = 3;
            }
            else {
                inputTape.Move(-1);
                newState = 1;
            }
            break;
        }
    }
    MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t> ret = MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>(newState, inputTape, tapes);
    if (newState == 3) ret.output = newState;
    return MathLib::Set<MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>>(ret);
}