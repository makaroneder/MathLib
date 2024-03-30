#include "Variable.hpp"

Variable::Variable(std::string n, Node* val) {
    name = n;
    value = val;
}
Variable::Variable(std::string n, std::string val) {
    name = n;
    if (val.at(0) == '(') value = new Node(Node::Type::ComplexConstant, val);
    else value = new Node(Node::Type::Constant, val);
}