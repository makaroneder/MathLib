#include "Printable.hpp"

namespace MathLib {
    String ToString(const Printable& printable) {
        StartBenchmark
        ReturnFromBenchmark(printable.ToString());
    }
}