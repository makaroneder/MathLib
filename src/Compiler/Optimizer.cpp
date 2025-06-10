#include "Optimizer.hpp"
#include "NodeType.hpp"
#include <String.hpp>

MathLib::ParserNode Optimizer::Evaluate(const MathLib::ParserNode& node) {
    MathLib::ParserNode ret = node;
    const size_t size = node.GetSize();
    for (size_t i = 0; i < size; i++) ret.At(i) = Evaluate(node.At(i));
    switch ((NodeType)ret.GetType()) {
        case NodeType::Addition: {
            const NodeType tmp = (NodeType)ret.At(0).GetType();
            if (tmp == NodeType::Digit && tmp == (NodeType)ret.At(1).GetType()) {
                const size_t a = MathLib::StringToNumber(ret.At(0).GetData(), 10);
                const size_t b = MathLib::StringToNumber(ret.At(1).GetData(), 10);
                const MathLib::String operation = ret.GetData();
                if (operation == "+") return MathLib::ParserNode((size_t)tmp, MathLib::ToString(a + b, 10), MathLib::Array<MathLib::ParserNode>());
                else if (operation == "-") return MathLib::ParserNode((size_t)tmp, MathLib::ToString(a - b, 10), MathLib::Array<MathLib::ParserNode>());
                else return MathLib::ParserNode();
            }
            break;
        }
        case NodeType::Multiplication: {
            const NodeType tmp = (NodeType)ret.At(0).GetType();
            if (tmp == NodeType::Digit && tmp == (NodeType)ret.At(1).GetType()) {
                const size_t a = MathLib::StringToNumber(ret.At(0).GetData(), 10);
                const size_t b = MathLib::StringToNumber(ret.At(1).GetData(), 10);
                const MathLib::String operation = ret.GetData();
                if (operation == "*") return MathLib::ParserNode((size_t)tmp, MathLib::ToString(a * b, 10), MathLib::Array<MathLib::ParserNode>());
                else if (operation == "/") return MathLib::ParserNode((size_t)tmp, MathLib::ToString(a / b, 10), MathLib::Array<MathLib::ParserNode>());
                else return MathLib::ParserNode();
            }
            break;
        }
        default: break;
    }
    return ret;
}