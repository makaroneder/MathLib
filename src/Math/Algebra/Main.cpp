#include <Compiler/Parser/UnwrapperParserLayer.hpp>
#include <Compiler/Parser/IdentityParserLayer.hpp>
#include <Compiler/Lexer/IdentifierLexerRule.hpp>
#include <Compiler/Lexer/WhitespaceLexerRule.hpp>
#include <Compiler/Lexer/SingleCharLexerRule.hpp>
#include <Compiler/Parser/BinaryParserLayer.hpp>
#include <Compiler/Lexer/StringLexerRule.hpp>
#include <Compiler/IdentityEvaluator.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Compiler/Toolchain.hpp>
#include <iostream>

enum class TokenType : uint8_t {
    ParenthesesStart,
    ParenthesesEnd,
    Comma,
    Definition,
    Identifier,
    Membership,
    Separator,
    And,
    Implication,
    Equality,
};
enum class NodeType : uint8_t {
    TruthValue,
    Comma,
    Definition,
    Identifier,
    Function,
    Membership,
    And,
    Implication,
    Equality,
};
struct FunctionParserLayer : MathLib::ParserLayer {
    [[nodiscard]] virtual MathLib::ParserNode Parse(const MathLib::Function<MathLib::ParserNode>& root, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const override {
        if (tokens.GetSize() <= i + 2) return next();
        const MathLib::Token name = tokens.At(i);
        if (!name.CheckType((size_t)TokenType::Identifier) || !tokens.At(i + 1).CheckType((size_t)TokenType::ParenthesesStart)) return next();
        i += 2;
        MathLib::ParserNode params = root();
        if (!tokens.At(i++).CheckType((size_t)TokenType::ParenthesesEnd)) return MathLib::ParserNode();
        return MathLib::ParserNode((size_t)NodeType::Function, name.GetValue(), MathLib::MakeArray<MathLib::ParserNode>(params));
    }
};
MathLib::String NodeToStringInternal(const MathLib::ParserNode& node, bool root) {
    switch ((NodeType)node.GetType()) {
        case NodeType::Identifier: return node.GetData();
        case NodeType::Function: return node.GetData() + '(' + NodeToStringInternal(node.At(0), true) + ')';
        case NodeType::Membership: return NodeToStringInternal(node.At(0), false) + ' ' + node.GetData() + ' ' + NodeToStringInternal(node.At(1), false);
        case NodeType::Comma: {
            const MathLib::String ret = NodeToStringInternal(node.At(0), true) + node.GetData() + ' ' + NodeToStringInternal(node.At(1), true);
            return root ? ret : '('_M + ret + ')';
        }
        case NodeType::Equality: {
            const MathLib::String ret = NodeToStringInternal(node.At(0), node.At(0).GetType() != (size_t)NodeType::Implication && node.At(0).GetType() != (size_t)NodeType::And) + ' ' + node.GetData() + ' ' + NodeToStringInternal(node.At(1), node.At(1).GetType() != (size_t)NodeType::Implication && node.At(1).GetType() != (size_t)NodeType::And);
            return root ? ret : '('_M + ret + ')';
        }
        case NodeType::And: {
            const MathLib::String ret = NodeToStringInternal(node.At(0), node.At(0).GetType() != (size_t)NodeType::Implication) + ' ' + node.GetData() + ' ' + NodeToStringInternal(node.At(1), node.At(1).GetType() != (size_t)NodeType::Implication);
            return root ? ret : '('_M + ret + ')';
        }
        case NodeType::Implication: {
            const MathLib::String ret = NodeToStringInternal(node.At(0), true) + ' ' + node.GetData() + ' ' + NodeToStringInternal(node.At(1), true);
            return root ? ret : '('_M + ret + ')';
        }
        case NodeType::Definition: {
            const MathLib::String ret = NodeToStringInternal(node.At(0), true) + ' ' + node.GetData() + ' ' + NodeToStringInternal(node.At(1), true);
            return root ? ret : '('_M + ret + ')';
        }
        default: return "";
    }
}
MathLib::String NodeToString(const MathLib::ParserNode& node) {
    return NodeToStringInternal(node, true);
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::Toolchain toolchain = MathLib::Toolchain(new MathLib::Lexer(MathLib::MakeArray<MathLib::LexerRule*>(
            new MathLib::WhitespaceLexerRule(SIZE_MAX),
            new MathLib::StringLexerRule((size_t)TokenType::Implication, "=>"_M),
            new MathLib::StringLexerRule((size_t)TokenType::And, "&&"_M),
            new MathLib::StringLexerRule((size_t)TokenType::Equality, "=="_M),
            new MathLib::StringLexerRule((size_t)TokenType::Separator, "::"_M),
            new MathLib::StringLexerRule((size_t)TokenType::Membership, ":="_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesStart, '('_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesEnd, ')'_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Comma, ','_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Membership, ':'_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Definition, '='_M),
            new MathLib::IdentifierLexerRule((size_t)TokenType::Identifier, true)
        )), new MathLib::Parser(MathLib::MakeArray<MathLib::ParserLayer*>(
            new MathLib::BinaryParserLayer((size_t)NodeType::Comma, (size_t)TokenType::Comma),
            new MathLib::BinaryParserLayer((size_t)NodeType::Definition, (size_t)TokenType::Definition),
            new MathLib::BinaryParserLayer((size_t)NodeType::Implication, (size_t)TokenType::Implication),
            new MathLib::BinaryParserLayer((size_t)NodeType::And, (size_t)TokenType::And),
            new MathLib::BinaryParserLayer((size_t)NodeType::Equality, (size_t)TokenType::Equality),
            new MathLib::BinaryParserLayer((size_t)NodeType::Membership, (size_t)TokenType::Membership),
            new FunctionParserLayer(),
            new MathLib::IdentityParserLayer((size_t)NodeType::Identifier, (size_t)TokenType::Identifier),
            new MathLib::UnwrapperParserLayer((size_t)TokenType::ParenthesesStart, (size_t)TokenType::ParenthesesEnd)
        )), new MathLib::IdentityEvaluator());
        MathLib::HostFileSystem fs;
        toolchain.LoadInput(fs.Open("src/TestPrograms/Algebra/2.txt"_M, MathLib::OpenMode::Read).ReadUntil('\0'));
        std::cout << (toolchain.GetNode()) << std::endl;
        // std::cout << NodeToString(toolchain.GetNode()) << std::endl;
        // TODO: Check "proof" blocks
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}