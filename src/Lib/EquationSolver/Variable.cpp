#include "Variable.hpp"

Variable::Variable(void) {}
Variable::Variable(String n, Node* val) : name(n), value(val) {}
Variable::Variable(String n, String val) : name(n), value((val.At(0) == '(') ? new Node(Node::Type::ComplexConstant, val) : new Node(Node::Type::Constant, val)) {}