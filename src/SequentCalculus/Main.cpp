#include "Sequent.hpp"
#include <Compiler/Parser/LeftBinaryParserLayer.hpp>
#include <Compiler/Parser/UnwrapperParserLayer.hpp>
#include <Compiler/Parser/IdentityParserLayer.hpp>
#include <Compiler/Lexer/IdentifierLexerRule.hpp>
#include <Compiler/Lexer/SingleCharLexerRule.hpp>
#include <Compiler/Lexer/WhitespaceLexerRule.hpp>
#include <Compiler/Parser/UnaryParserLayer.hpp>
#include <Compiler/Lexer/StringLexerRule.hpp>
#include <Compiler/IdentityEvaluator.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Compiler/Toolchain.hpp>
#include <iostream>

enum class TokenType : uint8_t {
    Comma,
    Turnstile,
    ParenthesesStart,
    ParenthesesEnd,
    Variable,
    Conjunction,
    Disjunction,
    Implication,
    Negation,
};
[[nodiscard]] Formula FromNodeInternal(const MathLib::ParserNode& node) {
    switch ((TokenType)node.GetType()) {
        case TokenType::Variable: return Formula(node.GetData());
        case TokenType::Conjunction: return Formula(Formula::Type::Conjunction, MathLib::MakeArray<Formula>(FromNodeInternal(node.At(0)), FromNodeInternal(node.At(1))));
        case TokenType::Disjunction: return Formula(Formula::Type::Disjunction, MathLib::MakeArray<Formula>(FromNodeInternal(node.At(0)), FromNodeInternal(node.At(1))));
        case TokenType::Implication: return Formula(Formula::Type::Implication, MathLib::MakeArray<Formula>(FromNodeInternal(node.At(0)), FromNodeInternal(node.At(1))));
        case TokenType::Negation: return Formula(Formula::Type::Negation, MathLib::MakeArray<Formula>(FromNodeInternal(node.At(0))));
        default: return Formula();
    }
}
[[nodiscard]] MathLib::Array<Formula> FromCommaNode(const MathLib::ParserNode& node) {
    if (node.GetType() != (size_t)TokenType::Comma) return MathLib::MakeArray<Formula>(FromNodeInternal(node));
    MathLib::Array<Formula> tmp = FromCommaNode(node.At(0));
    tmp += FromCommaNode(node.At(1));
    return tmp;
}
[[nodiscard]] Sequent FromNode(const MathLib::ParserNode& node) {
    return node.GetType() == (size_t)TokenType::Turnstile ? Sequent(FromCommaNode(node.At(0)), FromCommaNode(node.At(1))) : Sequent(MathLib::Array<Formula>(), FromCommaNode(node));
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <input file>");
        MathLib::Toolchain toolchain = MathLib::Toolchain(new MathLib::Lexer(MathLib::MakeArray<MathLib::LexerRule*>(
            new MathLib::WhitespaceLexerRule(SIZE_MAX),
            new MathLib::IdentifierLexerRule((size_t)TokenType::Variable, true),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Comma, ','_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesStart, '('_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesEnd, ')'_M),
            new MathLib::StringLexerRule((size_t)TokenType::Turnstile, "|-"_M),
            new MathLib::StringLexerRule((size_t)TokenType::Conjunction, "&&"_M),
            new MathLib::StringLexerRule((size_t)TokenType::Disjunction, "||"_M),
            new MathLib::StringLexerRule((size_t)TokenType::Implication, "->"_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Negation, '!'_M)
        )), new MathLib::Parser(MathLib::MakeArray<MathLib::ParserLayer*>(
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Turnstile, (size_t)TokenType::Turnstile),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Comma, (size_t)TokenType::Comma),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Conjunction, (size_t)TokenType::Conjunction),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Disjunction, (size_t)TokenType::Disjunction),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Implication, (size_t)TokenType::Implication),
            new MathLib::UnaryParserLayer((size_t)TokenType::Negation, (size_t)TokenType::Negation),
            new MathLib::UnwrapperParserLayer((size_t)TokenType::ParenthesesStart, (size_t)TokenType::ParenthesesEnd),
            new MathLib::IdentityParserLayer((size_t)TokenType::Variable, (size_t)TokenType::Variable)
        )), new MathLib::IdentityEvaluator());
        MathLib::HostFileSystem fs;
        toolchain.LoadInput(fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read).ReadUntil('\0'));
        const Sequent sequent = FromNode(toolchain.GetNode());
        std::cout << sequent << std::endl;
        std::cout << sequent.Prove() << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}