#include "Record.hpp"

namespace MathLib {
    Record::Record(const String& expression, bool passed) : expression(expression), passed(passed) {
        EmptyBenchmark
    }
    String Record::ToString(const String& padding) const {
        StartBenchmark
        ReturnFromBenchmark(padding + expression + " [" + (passed ? "passed" : "failed") + ']');
    }
}