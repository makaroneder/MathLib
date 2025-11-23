#include "FunctionNode.hpp"

namespace MathLib {
    FunctionNode::FunctionNode(void) {}
    FunctionNode::FunctionNode(const Sequence<char>& name, const Sequence<Variable>& arguments, Node* body, const Sequence<char>& dataType) : name(CollectionToString(name)), arguments(CollectionToArray<Variable>(arguments)), body(body), dataType(CollectionToString(dataType)) {}
    bool FunctionNode::operator==(const FunctionNode& other) const {
        return name == other.name && dataType == other.dataType;
    }
    bool FunctionNode::operator!=(const FunctionNode& other) const {
        return !(*this == other);
    }
}