#include "ParserNode.hpp"
#include "../../String.hpp"

namespace MathLib {
    ParserNode::ParserNode(void) : value(), children(), type(SIZE_MAX) {}
    ParserNode::ParserNode(size_t type, const Sequence<char>& value, const Sequence<ParserNode>& children) : value(CollectionToString(value)), children(CollectionToArray<ParserNode>(children)), type(type) {}
    size_t ParserNode::GetType(void) const {
        return type;
    }
    String ParserNode::GetData(void) const {
        return value;
    }
    void ParserNode::Replace(const ParserNode& element, const ParserNode& replacement) {
        if (*this == element) {
            value = replacement.value;
            children = replacement.children;
            type = replacement.type;
        }
        else for (ParserNode& child : children) child.Replace(element, replacement);
    }
    void ParserNode::ReplaceValue(const Sequence<char>& element, const Sequence<char>& replacement) {
        if (value == element) value = CollectionToString(replacement);
        for (ParserNode& child : children) child.ReplaceValue(element, replacement);
    }
    void ParserNode::ModifyValue(const Function<String, const Sequence<char>&>& func) {
        value = func(value);
        for (ParserNode& child : children) child.ModifyValue(func);
    }
    bool ParserNode::Add(const ParserNode& val) {
        return children.Add(val);
    }
    bool ParserNode::Reset(void) {
        return children.Reset();
    }
    ParserNode* ParserNode::GetValue(void) {
        return children.GetValue();
    }
    const ParserNode* ParserNode::GetValue(void) const {
        return children.GetValue();
    }
    size_t ParserNode::GetSize(void) const {
        return children.GetSize();
    }
    String ParserNode::ToString(const Sequence<char>& padding) const {
        const String padd = CollectionToString(padding);
        if (children.IsEmpty()) return padd + Formatter<size_t>::ToString(type) + ": " + value;
        const String padd2 = padd + "\t\t";
        String ret = padd + Formatter<size_t>::ToString(type) + ": {\n" + padd + "\tValue: " + value + '\n' + padd + "\tChildren: {\n";
        for (const ParserNode& child : children) ret += child.ToString(padd2) + '\n';
        return ret + padd + "\t}\n" + padd + '}';
    }
    bool ParserNode::operator==(const ParserNode& other) const {
        return type == other.type && value == other.value && children == other.children;
    }
    bool ParserNode::operator!=(const ParserNode& other) const {
        return !(*this == other);
    }
}