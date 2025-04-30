#include "Node.hpp"
#include <String.hpp>

Node::Node(void) : value(), children(), type(SIZE_MAX) {}
Node::Node(size_t type, const MathLib::Sequence<char>& value, const MathLib::Sequence<Node>& children) : value(MathLib::CollectionToString(value)), children(MathLib::CollectionToArray<Node>(children)), type(type) {}
size_t Node::GetType(void) const {
    return type;
}
MathLib::String Node::GetData(void) const {
    return value;
}
Node Node::At(size_t index) const {
    return children.At(index);
}
Node& Node::At(size_t index) {
    return children.At(index);
}
bool Node::Add(const Node& val) {
    return children.Add(val);
}
bool Node::Reset(void) {
    return children.Reset();
}
const Node* Node::GetValue(void) const {
    return children.GetValue();
}
size_t Node::GetSize(void) const {
    return children.GetSize();
}
MathLib::String Node::ToString(const MathLib::Sequence<char>& padding) const {
    const MathLib::String padd = MathLib::CollectionToString(padding);
    if (children.IsEmpty()) return padd + MathLib::ToString(type, 10) + ": " + value;
    const MathLib::String padd2 = padd + "\t\t";
    MathLib::String ret = padd + MathLib::ToString(type, 10) + ": {\n" + padd + "\tValue: " + value + '\n' + padd + "\tChildren: {\n";
    for (const Node& child : children)
        ret += child.ToString(padd2) + '\n';
    return ret + padd + "\t}\n" + padd + '}';
}
bool Node::operator==(const Node& other) const {
    return type == other.type && value == other.value && children == other.children;
}
bool Node::operator!=(const Node& other) const {
    return !(*this == other);
}