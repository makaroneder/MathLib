#include "Path.hpp"
#include "../String.hpp"

namespace MathLib {
    Path::Path(void) : vertices() {}
    Path::Path(const Sequence<size_t>& vertices) : vertices(CollectionToArray<size_t>(vertices)) {}
    size_t Path::GetSize(void) const {
        return vertices.GetSize();
    }
    bool Path::Add(const size_t& value) {
        return vertices.Add(value);
    }
    bool Path::Reset(void) {
        return vertices.Reset();
    }
    size_t* Path::GetValue(void) {
        return vertices.GetValue();
    }
    const size_t* Path::GetValue(void) const {
        return vertices.GetValue();
    }
    size_t Path::GetEnd(void) const {
        return vertices.At(vertices.GetSize() - 1);
    }
    Edge Path::GetEdge(void) const {
        return Edge(vertices.At(0), GetEnd());
    }
    bool Path::IsCyclic(void) const {
        return vertices.HasDuplicate();
    }
    String Path::ToString(const Sequence<char>& padding) const {
        const size_t size = vertices.GetSize();
        String ret = CollectionToString(padding);
        for (size_t i = 0; i < size; i++) {
            if (i) ret += " -> ";
            ret += Formatter<size_t>::ToString(vertices.AtUnsafe(i));
        }
        return ret + ';';
    }
}