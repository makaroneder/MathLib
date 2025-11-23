#include "Record.hpp"

namespace MathLib {
    Record::Record(const Sequence<char>& expression, bool passed) : expression(CollectionToString(expression)), passed(passed) {}
    String Record::ToString(const Sequence<char>& padding) const {
        return CollectionToString(padding) + expression + " [" + (passed ? "passed" : "failed") + ']';
    }
    bool Record::operator==(const Record& other) const {
        return passed == other.passed && expression == other.expression;
    }
    bool Record::operator!=(const Record& other) const {
        return !(*this == other);
    }
}