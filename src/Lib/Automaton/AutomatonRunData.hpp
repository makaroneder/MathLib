#ifndef MathLib_Automaton_RunData_H
#define MathLib_Automaton_RunData_H
#include "Tape.hpp"
#include "../Expected.hpp"

namespace MathLib {
    template <typename Input, typename Output, typename State>
    struct AutomatonRunData {
        State state;
        Expected<Output> output;
        Tape<Input> input;
        Array<Tape<Input>> tapes;

        AutomatonRunData(void) : state(), output(), input(), tapes() {
            EndBenchmark
        }
        AutomatonRunData(const State& state, const Output& output, const Tape<Input>& input, const Array<Tape<Input>>& tapes) : state(state), output(output), input(input), tapes(tapes) {
            EmptyBenchmark
        }
        AutomatonRunData(const State& state, const Tape<Input>& input, const Array<Tape<Input>>& tapes) : state(state), output(), input(input), tapes(tapes) {
            EmptyBenchmark
        }
        bool operator==(const AutomatonRunData<Input, Output, State>& other) const {
            StartAndReturnFromBenchmark(!((output.HasValue() != other.output.HasValue()) || (output.HasValue() && output.Get() != other.output.Get())) && state == other.state && input == other.input && tapes == other.tapes);
        }
        bool operator!=(const AutomatonRunData<Input, Output, State>& other) const {
            StartAndReturnFromBenchmark(!(*this == other));
        }
    };
}

#endif