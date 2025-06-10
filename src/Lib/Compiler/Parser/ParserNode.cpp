#include "ParserNode.hpp"
#include "../../String.hpp"

namespace MathLib {
    ParserNode::ParserNode(void) : value(), children(), type(SIZE_MAX) {
        EmptyBenchmark
    }
    ParserNode::ParserNode(size_t type, const Sequence<char>& value, const Sequence<ParserNode>& children) : value(CollectionToString(value)), children(CollectionToArray<ParserNode>(children)), type(type) {
        EmptyBenchmark
    }
    size_t ParserNode::GetType(void) const {
        StartAndReturnFromBenchmark(type);
    }
    MathLib::String ParserNode::GetData(void) const {
        StartAndReturnFromBenchmark(value);
    }
    ParserNode ParserNode::At(size_t index) const {
        StartAndReturnFromBenchmark(children.At(index));
    }
    ParserNode& ParserNode::At(size_t index) {
        StartAndReturnFromBenchmark(children.At(index));
    }
    bool ParserNode::Add(const ParserNode& val) {
        StartAndReturnFromBenchmark(children.Add(val));
    }
    bool ParserNode::Reset(void) {
        StartAndReturnFromBenchmark(children.Reset());
    }
    const ParserNode* ParserNode::GetValue(void) const {
        StartAndReturnFromBenchmark(children.GetValue());
    }
    size_t ParserNode::GetSize(void) const {
        StartAndReturnFromBenchmark(children.GetSize());
    }
    MathLib::String ParserNode::ToString(const MathLib::Sequence<char>& padding) const {
        StartBenchmark
        const MathLib::String padd = MathLib::CollectionToString(padding);
        if (children.IsEmpty()) ReturnFromBenchmark(padd + MathLib::ToString(type, 10) + ": " + value);
        const MathLib::String padd2 = padd + "\t\t";
        MathLib::String ret = padd + MathLib::ToString(type, 10) + ": {\n" + padd + "\tValue: " + value + '\n' + padd + "\tChildren: {\n";
        for (const ParserNode& child : children)
            ret += child.ToString(padd2) + '\n';
        ReturnFromBenchmark(ret + padd + "\t}\n" + padd + '}');
    }
    bool ParserNode::operator==(const ParserNode& other) const {
        StartAndReturnFromBenchmark(type == other.type && value == other.value && children == other.children);
    }
    bool ParserNode::operator!=(const ParserNode& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}