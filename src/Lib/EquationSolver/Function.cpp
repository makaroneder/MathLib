#include "Function.hpp"

Function::Function(void) {}
Function::Function(String n, Array<Variable> args, Node* b, String d, String c) : name(n), arguments(args), body(b), domain(d), codomain(c) {}