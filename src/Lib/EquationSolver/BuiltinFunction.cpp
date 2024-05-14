#include "BuiltinFunction.hpp"

BuiltinFunction::BuiltinFunction(std::string n, std::function<Node*(std::vector<const Node*>)> func) : name(n), function(func) {
    name = n;
    function = func;
}