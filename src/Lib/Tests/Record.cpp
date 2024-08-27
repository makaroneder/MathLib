#include "Record.hpp"

Record::Record(const String& expr, const bool& pass) : expression(expr), passed(pass) {}
String Record::ToString(const String& padding) const {
    return padding + expression + " [" + (passed ? "passed" : "failed") + ']';
}