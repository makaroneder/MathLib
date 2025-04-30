#include "Edge.hpp"
#include "../String.hpp"

namespace MathLib {
    Edge::Edge(void) : from(SIZE_MAX), to(SIZE_MAX) {
        EmptyBenchmark
    }
    Edge::Edge(size_t from, size_t to) : from(from), to(to) {
        EmptyBenchmark
    }
    String Edge::ToString(const Sequence<char>& padding) const {
        StartAndReturnFromBenchmark(CollectionToString(padding) + MathLib::ToString(from, 10) + " -> " + MathLib::ToString(to, 10) + ';');
    }
    bool Edge::operator==(const Edge& other) const {
        StartAndReturnFromBenchmark(from == other.from && to == other.to);
    }
    bool Edge::operator!=(const Edge& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}