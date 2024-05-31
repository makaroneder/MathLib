#include "BuiltinFunction.hpp"

BuiltinFunction::BuiltinFunction(void) {}
BuiltinFunction::BuiltinFunction(String n, std::function<Node*(Array<const Node*>)> func) : name(n), function(func) {}