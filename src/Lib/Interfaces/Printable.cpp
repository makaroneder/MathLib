#include "Printable.hpp"

namespace MathLib {
    String ToString(const Printable& printable) {
        StartAndReturnFromBenchmark(printable.ToString());
    }
    String ToString(const String& string) {
        StartAndReturnFromBenchmark(string);
    }
}