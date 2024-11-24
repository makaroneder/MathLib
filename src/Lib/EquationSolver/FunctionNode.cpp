#include "FunctionNode.hpp"

namespace MathLib {
    FunctionNode::FunctionNode(void) {}
    FunctionNode::FunctionNode(const String& name, const Array<Variable>& args, Node* body, const String& dataType) : name(name), arguments(args), body(body), dataType(dataType) {}
}