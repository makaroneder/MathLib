#include "BuiltinFunction.hpp"

BuiltinFunction::BuiltinFunction(void) {}
BuiltinFunction::BuiltinFunction(const String& n, const BuiltinFunctionPointer& func) : name(n), function(func) {}