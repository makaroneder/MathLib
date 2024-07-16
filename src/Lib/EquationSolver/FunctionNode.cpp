#include "FunctionNode.hpp"

FunctionNode::FunctionNode(void) {}
FunctionNode::FunctionNode(const String& n, const Array<Variable>& args, Node* b, const String& d, const String& c) : name(n), arguments(args), body(b), domain(d), codomain(c) {}