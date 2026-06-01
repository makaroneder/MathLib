#include "Optimizer.hpp"
#include "TokenType.hpp"

MathLib::ParserNode Optimizer::Invoke(MathLib::ParserNode node) const {
    // TODO: Handle revision
    const MathLib::ParserNode zero = MathLib::ParserNode((size_t)TokenType::Number, '0'_M);
    const MathLib::ParserNode one = MathLib::ParserNode((size_t)TokenType::Number, '1'_M);
    for (MathLib::ParserNode& child : node) child = Invoke(child);
    switch ((TokenType)node.GetType()) {
        case TokenType::Identifier: {
            const MathLib::String value = node.GetData();
            if (value == "Zero") return zero;
            else if (value == "One") return one;
            // else if (value == "Ones") return one;
            // else if (value == "Revision") return one;
            return node;
        }
        case TokenType::LessThanEqual:
        case TokenType::LessThan:
        case TokenType::GreaterThanEqual:
        case TokenType::GreaterThan:
        case TokenType::Equal:
        case TokenType::NotEqual: {
            const MathLib::ParserNode a = node.AtUnsafe(0);
            const MathLib::ParserNode b = node.AtUnsafe(1);
            if (a == b) switch ((TokenType)node.GetType()) {
                case TokenType::LessThanEqual:
                case TokenType::GreaterThanEqual:
                case TokenType::Equal: return one;
                case TokenType::LessThan:
                case TokenType::GreaterThan:
                case TokenType::NotEqual: return zero;
                default: break;
            }
            if (a.GetType() == b.GetType()) {
                switch ((TokenType)a.GetType()) {
                    case TokenType::Number: {
                        const uint64_t a64 = MathLib::StringToU64(a.GetData());
                        const uint64_t b64 = MathLib::StringToU64(b.GetData());
                        switch ((TokenType)node.GetType()) {
                            case TokenType::LessThanEqual: return a64 <= b64 ? one : zero;
                            case TokenType::LessThan: return a64 < b64 ? one : zero;
                            case TokenType::GreaterThanEqual: return a64 >= b64 ? one : zero;
                            case TokenType::GreaterThan: return a64 > b64 ? one : zero;
                            case TokenType::Equal: return a64 == b64 ? one : zero;
                            case TokenType::NotEqual: return a64 != b64 ? one : zero;
                            default: break;
                        }
                        break;
                    }
                    case TokenType::String: {
                        switch ((TokenType)node.GetType()) {
                            case TokenType::Equal: return a.GetData() == b.GetData() ? one : zero;
                            case TokenType::NotEqual: return a.GetData() != b.GetData() ? one : zero;
                            default: break;
                        }
                    }
                    default: break;
                }
            }
            return node;
        }
        case TokenType::LeftShift:
        case TokenType::RightShift:
        case TokenType::LogicalAnd:
        case TokenType::LogicalOr:
        case TokenType::BitwiseAnd:
        case TokenType::BitwiseOr:
        case TokenType::BitwiseXor:
        case TokenType::Addition:
        case TokenType::Subtraction:
        case TokenType::Multiplication:
        case TokenType::Division:
        case TokenType::Modulo: {
            // TODO: Optimize for specific cases
            // TODO: Optimize for absorbtion
            const MathLib::ParserNode a = node.AtUnsafe(0);
            const MathLib::ParserNode b = node.AtUnsafe(1);
            if (a.GetType() != b.GetType() || a.GetType() != (size_t)TokenType::Number) return node;
            const uint64_t a64 = MathLib::StringToU64(a.GetData());
            const uint64_t b64 = MathLib::StringToU64(b.GetData());
            switch ((TokenType)node.GetType()) {
                case TokenType::LeftShift: return MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(a64 << b64, 10));
                case TokenType::RightShift: return MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(a64 >> b64, 10));
                case TokenType::LogicalAnd: return MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(a64 && b64, 10));
                case TokenType::LogicalOr: return MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(a64 || b64, 10));
                case TokenType::BitwiseAnd: return MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(a64 & b64, 10));
                case TokenType::BitwiseOr: return MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(a64 | b64, 10));
                case TokenType::BitwiseXor: return MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(a64 ^ b64, 10));
                case TokenType::Addition: return MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(a64 + b64, 10));
                case TokenType::Subtraction: return MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(a64 - b64, 10));
                case TokenType::Multiplication: return MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(a64 * b64, 10));
                case TokenType::Division: return MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(a64 / b64, 10));
                case TokenType::Modulo: return MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(a64 % b64, 10));
                default: break;
            }
            return node;
        }
        // case TokenType::Assignment:
        // case TokenType::AssignmentAnd:
        // case TokenType::AssignmentOr:
        // case TokenType::AssignmentXor:
        // case TokenType::AssignmentLeftShift:
        // case TokenType::AssignmentRightShift:
        // case TokenType::AssignmentAddition:
        // case TokenType::AssignmentSubtraction:
        // case TokenType::AssignmentMultiplication:
        // case TokenType::AssignmentDivision:
        // case TokenType::AssignmentModulo:
        case TokenType::LogicalNot: return node.AtUnsafe(0).GetType() == (size_t)TokenType::Number ? MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(!MathLib::StringToU64(node.AtUnsafe(0).GetData()), 10)) : node;
        case TokenType::BitwiseNot: return node.AtUnsafe(0).GetType() == (size_t)TokenType::Number ? MathLib::ParserNode((size_t)TokenType::Number, MathLib::ToString(~MathLib::StringToU64(node.AtUnsafe(0).GetData()), 10)) : node;
        default: return node;
    }
}