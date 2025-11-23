#include "Printable.hpp"

namespace MathLib {
    String ToString(const Printable& printable) {
        return printable.ToString();
    }
    String ToString(const Sequence<char>& string) {
        return CollectionToString(string);
    }
}