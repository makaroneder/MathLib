#include "Record.hpp"

namespace MathLib {
    Record::Record(const String& expression, bool passed) : expression(expression), passed(passed) {}
    String Record::ToString(const String& padding) const {
        return padding + expression + " [" + (passed ? "passed" : "failed") + ']';
    }
}