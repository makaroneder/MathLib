#include "TokenType.hpp"
#include "Optimizer.hpp"
#include "ASLCommaParserLayer.hpp"
#include "ASLFunctionParserLayer.hpp"
#include "ASLIdentifierLexerRule.hpp"
#include <Compiler/Lexer/MultiLineCommentLexerRule.hpp>
#include <Compiler/Parser/PostfixUnaryParserLayer.hpp>
#include <Compiler/Parser/ArrayAccessParserLayer.hpp>
#include <Compiler/Parser/LeftBinaryParserLayer.hpp>
#include <Compiler/Parser/UnwrapperParserLayer.hpp>
#include <Compiler/Parser/IdentityParserLayer.hpp>
#include <Compiler/Lexer/StringMatchLexerRule.hpp>
#include <Compiler/Lexer/SingleCharLexerRule.hpp>
#include <Compiler/Lexer/IdentifierLexerRule.hpp>
#include <Compiler/Lexer/WhitespaceLexerRule.hpp>
#include <Compiler/Lexer/HexDigitLexerRule.hpp>
#include <Compiler/Parser/UnaryParserLayer.hpp>
#include <Compiler/Lexer/CommentLexerRule.hpp>
#include <Compiler/Lexer/StringLexerRule.hpp>
#include <Compiler/Lexer/DigitLexerRule.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Compiler/Toolchain.hpp>
#include <iostream>

MathLib::String NodeToString(const MathLib::ParserNode& node, const MathLib::String& padding) {
    // TODO: Handle parentheses
    switch ((TokenType)node.GetType()) {
        case TokenType::String:
        case TokenType::Number:
        case TokenType::Identifier: return node.GetData();
        case TokenType::Comma: return NodeToString(node.At(0), padding) + ", " + NodeToString(node.At(1), padding);
        case TokenType::Function: {
            MathLib::String ret = node.GetData();
            for (uint8_t i = 0; i < node.GetSize(); i++) {
                const MathLib::String str = NodeToString(node.At(i), padding);
                if (node.At(i).GetType() == (size_t)TokenType::FunctionBody) ret += str;
                else ret += '('_M + str + ')';
            }
            return ret;
        }
        case TokenType::FunctionBody: {
            MathLib::String ret = " {\n";
            for (const MathLib::ParserNode& child : node) ret += padding + '\t' + NodeToString(child, padding + '\t') + '\n';
            return ret + padding + '}';
        }
        case TokenType::ArrayAccess: return NodeToString(node.At(0), padding) + '[' + NodeToString(node.At(1), padding) + ']';
        case TokenType::LessThanEqual: return NodeToString(node.At(0), padding) + " <= " + NodeToString(node.At(1), padding);
        case TokenType::LessThan: return NodeToString(node.At(0), padding) + " < " + NodeToString(node.At(1), padding);
        case TokenType::GreaterThanEqual: return NodeToString(node.At(0), padding) + " >= " + NodeToString(node.At(1), padding);
        case TokenType::GreaterThan: return NodeToString(node.At(0), padding) + " > " + NodeToString(node.At(1), padding);
        case TokenType::Equal: return NodeToString(node.At(0), padding) + " == " + NodeToString(node.At(1), padding);
        case TokenType::NotEqual: return NodeToString(node.At(0), padding) + " != " + NodeToString(node.At(1), padding);
        case TokenType::LeftShift: return NodeToString(node.At(0), padding) + " << " + NodeToString(node.At(1), padding);
        case TokenType::RightShift: return NodeToString(node.At(0), padding) + " >> " + NodeToString(node.At(1), padding);
        case TokenType::LogicalAnd: return NodeToString(node.At(0), padding) + " && " + NodeToString(node.At(1), padding);
        case TokenType::LogicalOr: return NodeToString(node.At(0), padding) + " || " + NodeToString(node.At(1), padding);
        case TokenType::BitwiseAnd: return NodeToString(node.At(0), padding) + " & " + NodeToString(node.At(1), padding);
        case TokenType::BitwiseOr: return NodeToString(node.At(0), padding) + " | " + NodeToString(node.At(1), padding);
        case TokenType::BitwiseXor: return NodeToString(node.At(0), padding) + " ^ " + NodeToString(node.At(1), padding);
        case TokenType::Addition: return NodeToString(node.At(0), padding) + " + " + NodeToString(node.At(1), padding);
        case TokenType::Subtraction: return NodeToString(node.At(0), padding) + " - " + NodeToString(node.At(1), padding);
        case TokenType::Multiplication: return NodeToString(node.At(0), padding) + " * " + NodeToString(node.At(1), padding);
        case TokenType::Division: return NodeToString(node.At(0), padding) + " / " + NodeToString(node.At(1), padding);
        case TokenType::Modulo: return NodeToString(node.At(0), padding) + " % " + NodeToString(node.At(1), padding);
        case TokenType::Assignment: return NodeToString(node.At(0), padding) + " = " + NodeToString(node.At(1), padding);
        case TokenType::AssignmentAnd: return NodeToString(node.At(0), padding) + " &= " + NodeToString(node.At(1), padding);
        case TokenType::AssignmentOr: return NodeToString(node.At(0), padding) + " |= " + NodeToString(node.At(1), padding);
        case TokenType::AssignmentXor: return NodeToString(node.At(0), padding) + " ^= " + NodeToString(node.At(1), padding);
        case TokenType::AssignmentLeftShift: return NodeToString(node.At(0), padding) + " <<= " + NodeToString(node.At(1), padding);
        case TokenType::AssignmentRightShift: return NodeToString(node.At(0), padding) + " >>= " + NodeToString(node.At(1), padding);
        case TokenType::AssignmentAddition: return NodeToString(node.At(0), padding) + " += " + NodeToString(node.At(1), padding);
        case TokenType::AssignmentSubtraction: return NodeToString(node.At(0), padding) + " -= " + NodeToString(node.At(1), padding);
        case TokenType::AssignmentMultiplication: return NodeToString(node.At(0), padding) + " *= " + NodeToString(node.At(1), padding);
        case TokenType::AssignmentDivision: return NodeToString(node.At(0), padding) + " /= " + NodeToString(node.At(1), padding);
        case TokenType::AssignmentModulo: return NodeToString(node.At(0), padding) + " %= " + NodeToString(node.At(1), padding);
        case TokenType::LogicalNot: return '!'_M + NodeToString(node.At(0), padding);
        case TokenType::BitwiseNot: return '~'_M + NodeToString(node.At(0), padding);
        case TokenType::Increment: return NodeToString(node.At(0), padding) + "++";
        case TokenType::Decrement: return NodeToString(node.At(0), padding) + "--";
        default: return "";
    }
}
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <input file>");
        const Optimizer optimizer;
        MathLib::Toolchain toolchain = MathLib::Toolchain(new MathLib::Lexer(MathLib::MakeArray<MathLib::LexerRule*>(
            new MathLib::WhitespaceLexerRule(SIZE_MAX),
            new MathLib::CommentLexerRule(SIZE_MAX),
            new MathLib::MultiLineCommentLexerRule(SIZE_MAX),
            new MathLib::SingleCharLexerRule(SIZE_MAX, ';'_M),
            new MathLib::StringLexerRule((size_t)TokenType::String),
            new MathLib::HexDigitLexerRule((size_t)TokenType::Number),
            new MathLib::DigitLexerRule((size_t)TokenType::Number),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Comma, ','_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesStart, '('_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesEnd, ')'_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::BracketsStart, '{'_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::BracketsEnd, '}'_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::BracesStart, '['_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::BracesEnd, ']'_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::Equal, "=="_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::NotEqual, "!="_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Assignment, "="_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::AssignmentAnd, "&="_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::AssignmentOr, "|="_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::AssignmentXor, "^="_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::AssignmentLeftShift, "<<="_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::AssignmentRightShift, ">>="_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::AssignmentAddition, "+="_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::AssignmentSubtraction, "-="_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::AssignmentMultiplication, "*="_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::AssignmentDivision, "/="_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::AssignmentModulo, "%="_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::Increment, "++"_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::Decrement, "--"_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::LeftShift, "<<"_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::RightShift, ">>"_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::LogicalAnd, "&&"_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::LogicalOr, "||"_M),
            new ASLIdentifierLexerRule((size_t)TokenType::Identifier),
            new MathLib::SingleCharLexerRule((size_t)TokenType::BitwiseAnd, "&"_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::BitwiseOr, "|"_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::BitwiseXor, "^"_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::Addition, "+"_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::Subtraction, "-"_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::Multiplication, "*"_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::Division, "/"_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::Modulo, "%"_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::LessThanEqual, "<="_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::LessThan, "<"_M),
            new MathLib::StringMatchLexerRule((size_t)TokenType::GreaterThanEqual, ">="_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::GreaterThan, ">"_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::LogicalNot, "!"_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::BitwiseNot, "~"_M)
        )), new MathLib::Parser(MathLib::MakeArray<MathLib::ParserLayer*>(
            new ASLCommaParserLayer(),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Assignment, (size_t)TokenType::Assignment),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::AssignmentAnd, (size_t)TokenType::AssignmentAnd),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::AssignmentOr, (size_t)TokenType::AssignmentOr),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::AssignmentXor, (size_t)TokenType::AssignmentXor),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::AssignmentLeftShift, (size_t)TokenType::AssignmentLeftShift),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::AssignmentRightShift, (size_t)TokenType::AssignmentRightShift),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::AssignmentAddition, (size_t)TokenType::AssignmentAddition),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::AssignmentSubtraction, (size_t)TokenType::AssignmentSubtraction),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::AssignmentMultiplication, (size_t)TokenType::AssignmentMultiplication),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::AssignmentDivision, (size_t)TokenType::AssignmentDivision),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::AssignmentModulo, (size_t)TokenType::AssignmentModulo),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::LogicalOr, (size_t)TokenType::LogicalOr),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::LogicalAnd, (size_t)TokenType::LogicalAnd),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::BitwiseOr, (size_t)TokenType::BitwiseOr),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::BitwiseXor, (size_t)TokenType::BitwiseXor),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::BitwiseAnd, (size_t)TokenType::BitwiseAnd),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Equal, (size_t)TokenType::Equal),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::NotEqual, (size_t)TokenType::NotEqual),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::LessThanEqual, (size_t)TokenType::LessThanEqual),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::LessThan, (size_t)TokenType::LessThan),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::GreaterThanEqual, (size_t)TokenType::GreaterThanEqual),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::GreaterThan, (size_t)TokenType::GreaterThan),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::LeftShift, (size_t)TokenType::LeftShift),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::RightShift, (size_t)TokenType::RightShift),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Addition, (size_t)TokenType::Addition),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Subtraction, (size_t)TokenType::Subtraction),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Multiplication, (size_t)TokenType::Multiplication),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Division, (size_t)TokenType::Division),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Modulo, (size_t)TokenType::Modulo),
            new MathLib::UnaryParserLayer((size_t)TokenType::LogicalNot, (size_t)TokenType::LogicalNot),
            new MathLib::UnaryParserLayer((size_t)TokenType::BitwiseNot, (size_t)TokenType::BitwiseNot),
            new MathLib::PostfixUnaryParserLayer((size_t)TokenType::Increment, (size_t)TokenType::Increment),
            new MathLib::PostfixUnaryParserLayer((size_t)TokenType::Decrement, (size_t)TokenType::Decrement),
            new MathLib::ArrayAccessParserLayer((size_t)TokenType::ArrayAccess, (size_t)TokenType::BracesStart, (size_t)TokenType::BracesEnd),
            new ASLFunctionParserLayer(),
            new MathLib::UnwrapperParserLayer((size_t)TokenType::ParenthesesStart, (size_t)TokenType::ParenthesesEnd),
            new MathLib::IdentityParserLayer((size_t)TokenType::Identifier, (size_t)TokenType::Identifier),
            new MathLib::IdentityParserLayer((size_t)TokenType::Number, (size_t)TokenType::Number),
            new MathLib::IdentityParserLayer((size_t)TokenType::String, (size_t)TokenType::String),
            new MathLib::IdentityParserLayer((size_t)TokenType::Identifier, (size_t)TokenType::BitwiseXor)
        )), optimizer);
        MathLib::HostFileSystem fs;
        toolchain.LoadInput(fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read).ReadUntil('\0'));
        std::cout << NodeToString(toolchain.GetNode(), ""_M) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}