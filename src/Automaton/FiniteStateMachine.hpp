#ifndef FiniteStateMachine_H
#define FiniteStateMachine_H
#include <Automaton/Automaton.hpp>

struct FiniteStateMachine : MathLib::Automaton<uint8_t, uint8_t, uint8_t> {
    FiniteStateMachine(void);

    protected:
    MathLib::Set<MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>> MakeRunData(const uint8_t& state, const MathLib::Tape<uint8_t>& inputTape, const MathLib::Array<MathLib::Tape<uint8_t>>& tapes) const;
    [[nodiscard]] virtual MathLib::Set<MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>> RunSingle(const uint8_t& state, MathLib::Tape<uint8_t> inputTape, MathLib::Array<MathLib::Tape<uint8_t>> tapes) const override;
};

#endif