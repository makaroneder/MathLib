#include "Variable.hpp"

Variable::Variable(std::string n, Node* val) : name(n), value(val) {}
Variable::Variable(std::string n, std::string val) : name(n), value((val.at(0) == '(') ? new Node(Node::Type::ComplexConstant, val) : new Node(Node::Type::Constant, val)) {}