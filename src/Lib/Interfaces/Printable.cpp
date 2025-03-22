#include "Printable.hpp"

namespace MathLib {
    String ToString(const Printable& printable) {
        StartAndReturnFromBenchmark(printable.ToString());
    }
    String ToString(const Sequence<char>& string) {
        StartAndReturnFromBenchmark(CollectionToString(string));
    }
}