#include "Path.hpp"
#include "../String.hpp"

namespace MathLib {
    size_t Path::GetSize(void) const {
        StartAndReturnFromBenchmark(vertices.GetSize());
    }
    bool Path::Add(const size_t& value) {
        StartAndReturnFromBenchmark(vertices.Add(value));
    }
    bool Path::Reset(void) {
        StartAndReturnFromBenchmark(vertices.Reset());
    }
    size_t* Path::GetValue(void) {
        StartAndReturnFromBenchmark(vertices.GetValue());
    }
    const size_t* Path::GetValue(void) const {
        StartAndReturnFromBenchmark(vertices.GetValue());
    }
    size_t Path::GetEnd(void) const {
        StartAndReturnFromBenchmark(vertices.At(vertices.GetSize() - 1));
    }
    Edge Path::GetEdge(void) const {
        StartAndReturnFromBenchmark(Edge(vertices.At(0), GetEnd()));
    }
    bool Path::IsCyclic(void) const {
        StartAndReturnFromBenchmark(vertices.HasDuplicate());
    }
    MathLib::String Path::ToString(const MathLib::Sequence<char>& padding) const {
        StartBenchmark
        const size_t size = vertices.GetSize();
        MathLib::String ret = MathLib::CollectionToString(padding);
        for (size_t i = 0; i < size; i++) {
            if (i) ret += " -> ";
            ret += MathLib::ToString(vertices.At(i), 10);
        }
        ReturnFromBenchmark(ret + ';');
    }
    bool Path::operator==(const Path& other) const {
        StartAndReturnFromBenchmark(vertices == other.vertices);
    }
    bool Path::operator!=(const Path& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}