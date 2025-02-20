#ifndef MathLib_Automaton_FiniteStateMachineState_H
#define MathLib_Automaton_FiniteStateMachineState_H
#include "FiniteStateMachineConnection.hpp"

namespace MathLib {
    template <typename T>
    struct FiniteStateMachineState {
        FiniteStateMachineState(const Array<FiniteStateMachineConnection<T>>& connections, bool accepting) : connections(connections), accepting(accepting) {
            EmptyBenchmark
        }
        bool IsAccepting(void) const {
            StartAndReturnFromBenchmark(accepting);
        }
        Expected<size_t> Run(const T& x) const {
            StartBenchmark
            for (const FiniteStateMachineConnection<T>& connection : connections) {
                const Expected<size_t> tmp = connection.GetState(x);
                if (tmp.HasValue()) ReturnFromBenchmark(tmp);
            }
            ReturnFromBenchmark(Expected<size_t>());
        }

        private:
        Array<FiniteStateMachineConnection<T>> connections;
        bool accepting;
    };
}

#endif