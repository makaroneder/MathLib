#ifndef MathLib_Automaton_H
#define MathLib_Automaton_H
#include "../Pair.hpp"
#include "../Math/Set.hpp"
#include "AutomatonRunData.hpp"
#include "../Interfaces/ReverseSequence.hpp"

namespace MathLib {
    template <typename Input, typename Output, typename State>
    struct Automaton {
        Automaton(const State& startState, const Array<Tape<Input>>& startTapes) : startState(startState), startTapes(startTapes) {
            EmptyBenchmark
        }
        [[nodiscard]] State GetState(void) const {
            StartAndReturnFromBenchmark(startState);
        }
        [[nodiscard]] Expected<Set<Output>> Run(const Sequence<Input>& input) const {
            StartAndReturnFromBenchmark(RunInternal(startState, Tape<Input>(ReverseSequence<Input>(input)), startTapes));
        }
        [[nodiscard]] Expected<bool> Recognizes(const Sequence<Array<Input>>& language) const {
            StartBenchmark
            for (size_t i = 0; i < language.GetSize(); i++) {
                const Expected<Set<Output>> tmp = Run(language.At(i));
                if (!tmp.HasValue()) ReturnFromBenchmark(Expected<bool>());
                if (tmp.Get().IsEmpty()) ReturnFromBenchmark(false);
            }
            ReturnFromBenchmark(true);
        }

        protected:
        [[nodiscard]] virtual Set<AutomatonRunData<Input, Output, State>> RunSingle(const State& state, Tape<Input> inputTape, Array<Tape<Input>> tapes) const = 0;

        private:
        [[nodiscard]] Expected<Set<Output>> RunInternal(const State& state, const Tape<Input>& input, const Array<Tape<Input>>& tapes) const {
            StartBenchmark
            const Set<AutomatonRunData<Input, Output, State>> tmp = RunSingle(state, input, tapes);
            Set<Output> ret;
            for (const AutomatonRunData<Input, Output, State>& data : tmp) {
                if (!data.output.HasValue()) {
                    const Expected<Set<Output>> tmp = RunInternal(data.state, data.input, data.tapes);
                    if (!tmp.HasValue()) return Expected<Set<Output>>();
                    ret += tmp.Get();
                }
                else if (!ret.Add(data.output.Get())) return Expected<Set<Output>>();
            }
            ReturnFromBenchmark(ret);
        }
        State startState;
        Array<Tape<Input>> startTapes;
    };
}

#endif