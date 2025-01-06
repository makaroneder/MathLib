#include "Printable.hpp"

namespace MathLib {
    String ToString(const Printable& printable) {
        StartBenchmark
        ReturnFromBenchmark(printable.ToString());
    }
    String ToString(const String& string) {
        StartBenchmark
        ReturnFromBenchmark(string);
    }
}