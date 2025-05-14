#include "NodeType.hpp"
#include "Compiler.hpp"
#include "Optimizer.hpp"
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
#include <CommandLine.hpp>
#include <FunctionT.hpp>
#include <iostream>

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
struct FunctionDefinitionParserLayer : ParserLayer {
    [[nodiscard]] virtual Node Parse(const MathLib::Function<Node>& root, const MathLib::Function<Node>& next, const MathLib::Sequence<Token>& tokens, size_t& i) const override {
        const Token token = tokens.At(i);
        if (!token.CheckType((size_t)TokenType::Identifier) || token.GetValue() != "func") return next();
        i++;
        const Token type = tokens.At(i++);
        if (!type.CheckType((size_t)TokenType::Identifier)) return Node();
        const Token name = tokens.At(i++);
        if (!name.CheckType((size_t)TokenType::Identifier)) return Node();
        const Node args = root();
        if (!tokens.At(i).CheckType((size_t)TokenType::BracesStart)) {
            i--;
            return next();
        }
        i++;
        Node body;
        if (!tokens.At(i).CheckType((size_t)TokenType::BracesEnd)) {
            body = root();
            if (!tokens.At(i++).CheckType((size_t)TokenType::BracesEnd)) return Node();
        }
        else i++;
        return Node((size_t)NodeType::FunctionDefinition, name.GetValue(), MathLib::MakeArray<Node>(
            Node((size_t)NodeType::Identifier, type.GetValue(), MathLib::Array<Node>()), args, body
        ));
    }
};
struct FunctionCallParserLayer : ParserLayer {
    [[nodiscard]] virtual Node Parse(const MathLib::Function<Node>& root, const MathLib::Function<Node>& next, const MathLib::Sequence<Token>& tokens, size_t& i) const override {
        const Token name = tokens.At(i);
        if (!name.CheckType((size_t)TokenType::Identifier)) return next();
        i++;
        if (!tokens.At(i).CheckType((size_t)TokenType::ParenthesesStart)) {
            i--;
            return next();
        }
        i++;
        Node args;
        if (!tokens.At(i).CheckType((size_t)TokenType::ParenthesesEnd)) {
            args = root();
            if (!tokens.At(i++).CheckType((size_t)TokenType::ParenthesesEnd)) return Node();
        }
        else i++;
        return Node((size_t)NodeType::FunctionCall, name.GetValue(), MathLib::MakeArray<Node>(args));
    }
};
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        #if 0
        Evaluator<Node>* optimizer = new IdentityEvaluator();
        #else
        Evaluator<Node>* optimizer = new Optimizer();
        #endif
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
            new FunctionDefinitionParserLayer(),
            new FunctionCallParserLayer(),
            new IdentityParserLayer((size_t)NodeType::Identifier, (size_t)TokenType::Identifier),
            new IdentityParserLayer((size_t)NodeType::Digit, (size_t)TokenType::Digit)
        )), optimizer);
        MathLib::HostFileSystem fs;
        const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
        toolchain.LoadInput(fs.Open(cmdLine.GetEntry("input"_M).Get("No input provided"), MathLib::OpenMode::Read).ReadUntil('\0'));
        Compiler compiler;
        if (!fs.Open(cmdLine.GetEntry("output"_M).Get("No output provided"), MathLib::OpenMode::Write).Puts(toolchain.Evaluate<MathLib::String>(compiler))) MathLib::Panic("Failed to compile data");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}