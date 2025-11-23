#include "NodeType.hpp"
#include "Compiler.hpp"
#include "Optimizer.hpp"
#include <Compiler/Parser/LeftBinaryParserLayer.hpp>
#include <Compiler/Parser/UnwrapperParserLayer.hpp>
#include <Compiler/Parser/IdentityParserLayer.hpp>
#include <Compiler/Parser/KeywordParserLayer.hpp>
#include <Compiler/Lexer/WhitespaceLexerRule.hpp>
#include <Compiler/Lexer/IdentifierLexerRule.hpp>
#include <Compiler/Lexer/SingleCharLexerRule.hpp>
#include <Compiler/Lexer/DigitLexerRule.hpp>
#include <Compiler/IdentityEvaluator.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Compiler/Toolchain.hpp>
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
struct VariableParserLayer : MathLib::ParserLayer {
    [[nodiscard]] virtual MathLib::ParserNode Parse(const MathLib::Function<MathLib::ParserNode>&, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const override {
        const MathLib::Token token = tokens.At(i);
        if (!token.CheckType((size_t)TokenType::Identifier) || token.GetValue() != "let") return next();
        i++;
        const MathLib::Token type = tokens.At(i++);
        if (!type.CheckType((size_t)TokenType::Identifier)) return MathLib::ParserNode();
        const MathLib::Token name = tokens.At(i++);
        if (!name.CheckType((size_t)TokenType::Identifier)) return MathLib::ParserNode();
        return MathLib::ParserNode((size_t)NodeType::VariableDefinition, name.GetValue(), MathLib::MakeArray<MathLib::ParserNode>(
            MathLib::ParserNode((size_t)NodeType::Identifier, type.GetValue(), MathLib::Array<MathLib::ParserNode>())
        ));
    }
};
struct FunctionDefinitionParserLayer : MathLib::ParserLayer {
    [[nodiscard]] virtual MathLib::ParserNode Parse(const MathLib::Function<MathLib::ParserNode>& root, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const override {
        const MathLib::Token token = tokens.At(i);
        if (!token.CheckType((size_t)TokenType::Identifier) || token.GetValue() != "func") return next();
        i++;
        const MathLib::Token type = tokens.At(i++);
        if (!type.CheckType((size_t)TokenType::Identifier)) return MathLib::ParserNode();
        const MathLib::Token name = tokens.At(i++);
        if (!name.CheckType((size_t)TokenType::Identifier)) return MathLib::ParserNode();
        const MathLib::ParserNode args = root();
        if (!tokens.At(i).CheckType((size_t)TokenType::BracesStart)) {
            i--;
            return next();
        }
        i++;
        MathLib::ParserNode body;
        if (!tokens.At(i).CheckType((size_t)TokenType::BracesEnd)) {
            body = root();
            if (!tokens.At(i++).CheckType((size_t)TokenType::BracesEnd)) return MathLib::ParserNode();
        }
        else i++;
        return MathLib::ParserNode((size_t)NodeType::FunctionDefinition, name.GetValue(), MathLib::MakeArray<MathLib::ParserNode>(
            MathLib::ParserNode((size_t)NodeType::Identifier, type.GetValue(), MathLib::Array<MathLib::ParserNode>()), args, body
        ));
    }
};
struct FunctionCallParserLayer : MathLib::ParserLayer {
    [[nodiscard]] virtual MathLib::ParserNode Parse(const MathLib::Function<MathLib::ParserNode>& root, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const override {
        const MathLib::Token name = tokens.At(i);
        if (!name.CheckType((size_t)TokenType::Identifier)) return next();
        i++;
        if (!tokens.At(i).CheckType((size_t)TokenType::ParenthesesStart)) {
            i--;
            return next();
        }
        i++;
        MathLib::ParserNode args;
        if (!tokens.At(i).CheckType((size_t)TokenType::ParenthesesEnd)) {
            args = root();
            if (!tokens.At(i++).CheckType((size_t)TokenType::ParenthesesEnd)) return MathLib::ParserNode();
        }
        else i++;
        return MathLib::ParserNode((size_t)NodeType::FunctionCall, name.GetValue(), MathLib::MakeArray<MathLib::ParserNode>(args));
    }
};
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        #if 0
        MathLib::Evaluator<MathLib::ParserNode>* optimizer = new MathLib::IdentityEvaluator();
        #else
        MathLib::Evaluator<MathLib::ParserNode>* optimizer = new Optimizer();
        #endif
        MathLib::Toolchain toolchain = MathLib::Toolchain(new MathLib::Lexer(MathLib::MakeArray<MathLib::LexerRule*>(
            new MathLib::WhitespaceLexerRule(SIZE_MAX),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Comma, ","_M),
            new MathLib::DigitLexerRule((size_t)TokenType::Digit),
            new MathLib::IdentifierLexerRule((size_t)TokenType::Identifier, false),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesStart, '('_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesEnd, ')'_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::BracesStart, '{'_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::BracesEnd, '}'_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Multiplication, "*/"_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Addition, "+-"_M)
        )), new MathLib::Parser(MathLib::MakeArray<MathLib::ParserLayer*>(
            new MathLib::LeftBinaryParserLayer((size_t)NodeType::Comma, (size_t)TokenType::Comma),
            new MathLib::LeftBinaryParserLayer((size_t)NodeType::Addition, (size_t)TokenType::Addition),
            new MathLib::LeftBinaryParserLayer((size_t)NodeType::Multiplication, (size_t)TokenType::Multiplication),
            new MathLib::UnwrapperParserLayer((size_t)TokenType::ParenthesesStart, (size_t)TokenType::ParenthesesEnd),
            new MathLib::UnwrapperParserLayer((size_t)TokenType::BracesStart, (size_t)TokenType::BracesEnd),
            new MathLib::KeywordParserLayer((size_t)NodeType::Return, MathLib::Token((size_t)TokenType::Identifier, "return"_M)),
            new VariableParserLayer(),
            new FunctionDefinitionParserLayer(),
            new FunctionCallParserLayer(),
            new MathLib::IdentityParserLayer((size_t)NodeType::Identifier, (size_t)TokenType::Identifier),
            new MathLib::IdentityParserLayer((size_t)NodeType::Digit, (size_t)TokenType::Digit)
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