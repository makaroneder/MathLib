#include "BytePairEncodingSubstitute.hpp"
#include "../../../String.hpp"

namespace MathLib {
    BytePairEncodingSubstitute::BytePairEncodingSubstitute(void) : name(SIZE_MAX), a(SIZE_MAX), b(SIZE_MAX) {
        EmptyBenchmark
    }
    BytePairEncodingSubstitute::BytePairEncodingSubstitute(size_t name, size_t a, size_t b) : name(name), a(a), b(b) {
        EmptyBenchmark
    }
    bool BytePairEncodingSubstitute::Save(Writable& file) const {
        StartAndReturnFromBenchmark(file.Write<size_t>(name) && file.Write<size_t>(a) && file.Write<size_t>(b));
    }
    bool BytePairEncodingSubstitute::Load(Readable& file) {
        StartAndReturnFromBenchmark(file.Read<size_t>(name) && file.Read<size_t>(a) && file.Read<size_t>(b));
    }
    String BytePairEncodingSubstitute::ToString(const Sequence<char>& padding) const {
        StartAndReturnFromBenchmark(CollectionToString(padding) + MathLib::ToString(name, 10) + " = (" + MathLib::ToString(a, 10) + ", " + MathLib::ToString(b, 10) + ')');
    }
    bool BytePairEncodingSubstitute::operator==(const BytePairEncodingSubstitute& other) const {
        StartAndReturnFromBenchmark(name == other.name && a == other.a && b == other.b);
    }
    bool BytePairEncodingSubstitute::operator!=(const BytePairEncodingSubstitute& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}