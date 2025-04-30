#include "Record.hpp"

namespace MathLib {
    Record::Record(const Sequence<char>& expression, bool passed) : expression(CollectionToString(expression)), passed(passed) {
        EmptyBenchmark
    }
    String Record::ToString(const Sequence<char>& padding) const {
        StartAndReturnFromBenchmark(CollectionToString(padding) + expression + " [" + (passed ? "passed" : "failed") + ']');
    }
    bool Record::operator==(const Record& other) const {
        StartAndReturnFromBenchmark(passed == other.passed && expression == other.expression);
    }
    bool Record::operator!=(const Record& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}