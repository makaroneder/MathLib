#ifndef MathLib_Automaton_FiniteStateMachine_H
#define MathLib_Automaton_FiniteStateMachine_H
#include "FiniteStateMachineState.hpp"

namespace MathLib {
    template <typename T>
    struct FiniteStateMachine {
        FiniteStateMachine(const Array<FiniteStateMachineState<T>>& states) : states(states), currentState(0) {
            EmptyBenchmark
        }
        Expected<size_t> Run(const Collection<T>& input) const {
            StartBenchmark
            size_t state = currentState;
            for (const T& x : input) {
                const Expected<size_t> tmp = states.At(state).Run(x);
                if (!tmp.HasValue() || tmp.Get() >= states.GetSize()) ReturnFromBenchmark(Expected<size_t>());
                state = tmp.Get();
            }
            ReturnFromBenchmark(states.At(state).IsAccepting() ? Expected<size_t>(state) : Expected<size_t>());
        }
        bool Update(const Collection<T>& input) {
            StartBenchmark
            const Expected<size_t> tmp = Run(input);
            if (!tmp.HasValue()) ReturnFromBenchmark(false);
            currentState = tmp.Get();
            ReturnFromBenchmark(true);
        }

        private:
        Array<FiniteStateMachineState<T>> states;
        size_t currentState;
    };
}

#endif