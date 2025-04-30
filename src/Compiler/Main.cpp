#include "NodeType.hpp"
#include "Compiler.hpp"
#include "Toolchain/Toolchain.hpp"
#include "Toolchain/IdentityEvaluator.hpp"
#include "Toolchain/Lexer/DigitLexerRule.hpp"
#include "Toolchain/Parser/MiddleParserLayer.hpp"
#include "Toolchain/Parser/KeywordParserLayer.hpp"
#include "Toolchain/Lexer/WhitespaceLexerRule.hpp"
#include "Toolchain/Lexer/IdentifierLexerRule.hpp"
#include "Toolchain/Lexer/SingleCharLexerRule.hpp"
#include "Toolchain/Parser/IdentityParserLayer.hpp"
#include "Toolchain/Parser/UnwrapperParserLayer.hpp"
#include <Libc/HostFileSystem.hpp>
#include <FunctionT.hpp>
#include <iostream>

// TODO: Optimizer

enum class TokenType {
    Comma,
    Digit,
    Identifier,
    Addition,
    Multiplication,
    ParenthesesStart,
    ParenthesesEnd,
    BracesStart,
    BracesEnd,
};
struct FunctionParserLayer : ParserLayer {
    [[nodiscard]] virtual Node Parse(const MathLib::Function<Node>& root, const MathLib::Function<Node>& next, const MathLib::Sequence<Token>& tokens, size_t& i) const override {
        const Token token = tokens.At(i);
        if (!token.CheckType((size_t)TokenType::Identifier) || token.GetValue() != "func") return next();
        i++;
        const Token type = tokens.At(i++);
        if (!type.CheckType((size_t)TokenType::Identifier)) return Node();
        const Token name = tokens.At(i++);
        if (!name.CheckType((size_t)TokenType::Identifier)) return Node();
        const Node args = root();
        const Node body = root();
        return Node((size_t)NodeType::FunctionDefinition, name.GetValue(), MathLib::MakeArray<Node>(
            Node((size_t)NodeType::Identifier, type.GetValue(), MathLib::Array<Node>()), args, body
        ));
    }
};
struct VariableParserLayer : ParserLayer {
    [[nodiscard]] virtual Node Parse(const MathLib::Function<Node>&, const MathLib::Function<Node>& next, const MathLib::Sequence<Token>& tokens, size_t& i) const override {
        const Token token = tokens.At(i);
        if (!token.CheckType((size_t)TokenType::Identifier) || token.GetValue() != "let") return next();
        i++;
        const Token type = tokens.At(i++);
        if (!type.CheckType((size_t)TokenType::Identifier)) return Node();
        const Token name = tokens.At(i++);
        if (!name.CheckType((size_t)TokenType::Identifier)) return Node();
        return Node((size_t)NodeType::VariableDefinition, name.GetValue(), MathLib::MakeArray<Node>(
            Node((size_t)NodeType::Identifier, type.GetValue(), MathLib::Array<Node>())
        ));
    }
};
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        Toolchain toolchain = Toolchain(new Lexer(MathLib::MakeArray<LexerRule*>(
            new WhitespaceLexerRule(SIZE_MAX),
            new SingleCharLexerRule((size_t)TokenType::Comma, ","_M),
            new DigitLexerRule((size_t)TokenType::Digit),
            new IdentifierLexerRule((size_t)TokenType::Identifier),
            new SingleCharLexerRule((size_t)TokenType::ParenthesesStart, '('_M),
            new SingleCharLexerRule((size_t)TokenType::ParenthesesEnd, ')'_M),
            new SingleCharLexerRule((size_t)TokenType::BracesStart, '{'_M),
            new SingleCharLexerRule((size_t)TokenType::BracesEnd, '}'_M),
            new SingleCharLexerRule((size_t)TokenType::Multiplication, "*/"_M),
            new SingleCharLexerRule((size_t)TokenType::Addition, "+-"_M)
        )), new Parser(MathLib::MakeArray<ParserLayer*>(
            new MiddleParserLayer((size_t)NodeType::Comma, (size_t)TokenType::Comma),
            new MiddleParserLayer((size_t)NodeType::Addition, (size_t)TokenType::Addition),
            new MiddleParserLayer((size_t)NodeType::Multiplication, (size_t)TokenType::Multiplication),
            new UnwrapperParserLayer((size_t)TokenType::ParenthesesStart, (size_t)TokenType::ParenthesesEnd),
            new UnwrapperParserLayer((size_t)TokenType::BracesStart, (size_t)TokenType::BracesEnd),
            new KeywordParserLayer((size_t)NodeType::Return, Token((size_t)TokenType::Identifier, "return"_M)),
            new VariableParserLayer(),
            new FunctionParserLayer(),
            new IdentityParserLayer((size_t)NodeType::Identifier, (size_t)TokenType::Identifier),
            new IdentityParserLayer((size_t)NodeType::Digit, (size_t)TokenType::Digit)
        )), new IdentityEvaluator());
        toolchain.LoadInput("func uint64 Main() { 1 }"_M);
        Compiler compiler;
        MathLib::HostFileSystem fs;
        MathLib::File file = fs.Open("t.asm"_M, MathLib::OpenMode::Write);
        if (!file.Puts(toolchain.Evaluate<MathLib::String>(compiler))) MathLib::Panic("Failed to compile data");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}