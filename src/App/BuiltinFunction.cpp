#include "BuiltinFunction.hpp"

BuiltinFunction::BuiltinFunction(std::string n, std::function<Node*(std::vector<Node*>)> func) {
    name = n;
    function = func;
}