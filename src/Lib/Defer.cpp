#include "Defer.hpp"

namespace MathLib {
    Defer::Defer(const Function<void>* function) : function(function) {}
    Defer::~Defer(void) {
        if (!function) return;
        (*function)();
        delete function;
    }
}