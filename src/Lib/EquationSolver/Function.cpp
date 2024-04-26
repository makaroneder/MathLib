#include "Function.hpp"

Function::Function(std::string n, std::vector<Variable> args, Node* b, std::string d, std::string c) {
    name = n;
    arguments = args;
    body = b;
    domain = d;
    codomain = c;
}