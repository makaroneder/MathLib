#ifndef MathLib_Pair_H
#define MathLib_Pair_H
#include "Typedefs.hpp"

namespace MathLib {
    template <typename A, typename B>
    struct Pair : Allocatable {
        A first;
        B second;

        Pair(void) {
            EmptyBenchmark
        }
        Pair(const A& first, const B& second) : first(first), second(second) {
            EmptyBenchmark
        }
        bool operator==(const Pair<A, B>& other) const {
            return first == other.first && second == other.second;
        }
        bool operator!=(const Pair<A, B>& other) const {
            return first != other.first || second != other.second;
        }
    };
}

#endif