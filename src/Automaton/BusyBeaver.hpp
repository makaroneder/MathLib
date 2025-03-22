#ifndef BusyBeaver_H
#define BusyBeaver_H
#include <Automaton/Automaton.hpp>

struct BusyBeaver : MathLib::Automaton<uint8_t, uint8_t, uint8_t> {
    BusyBeaver(void);

    protected:
    [[nodiscard]] virtual MathLib::Set<MathLib::AutomatonRunData<uint8_t, uint8_t, uint8_t>> RunSingle(const uint8_t& state, MathLib::Tape<uint8_t> inputTape, MathLib::Array<MathLib::Tape<uint8_t>> tapes) const;
};

#endif