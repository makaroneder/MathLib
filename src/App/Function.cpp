#include "Function.hpp"

Function::Function(std::string n, std::vector<Variable> args, Node* b) {
    name = n;
    arguments = args;
    body = b;
}