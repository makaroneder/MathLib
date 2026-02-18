#include "Edge.hpp"
#include "../String.hpp"

namespace MathLib {
    Edge::Edge(void) : from(SIZE_MAX), to(SIZE_MAX) {}
    Edge::Edge(size_t from, size_t to) : from(from), to(to) {}
    String Edge::ToString(const Sequence<char>& padding) const {
        return CollectionToString(padding) + Formatter<size_t>::ToString(from) + " -> " + Formatter<size_t>::ToString(to) + ';';
    }
    bool Edge::operator==(const Edge& other) const {
        return from == other.from && to == other.to;
    }
    bool Edge::operator!=(const Edge& other) const {
        return !(*this == other);
    }
}