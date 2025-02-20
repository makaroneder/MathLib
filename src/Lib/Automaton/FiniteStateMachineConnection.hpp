#ifndef MathLib_Automaton_FiniteStateMachineConnection_H
#define MathLib_Automaton_FiniteStateMachineConnection_H
#include "../Expected.hpp"

namespace MathLib {
    template <typename T>
    struct FiniteStateMachineConnection {
        FiniteStateMachineConnection(const Array<T>& input, size_t state) : input(input), state(state) {
            EmptyBenchmark
        }
        Expected<size_t> GetState(const T& x) const {
            StartAndReturnFromBenchmark(input.Contains(x) ? Expected<size_t>(state) : Expected<size_t>());
        }

        private:
        Array<T> input;
        size_t state;
    };
}

#endif