#include "Context.hpp"
#include <Compiler/Parser/RightBinaryParserLayer.hpp>
#include <Compiler/Parser/LeftBinaryParserLayer.hpp>
#include <Compiler/Parser/UnwrapperParserLayer.hpp>
#include <Compiler/Parser/IdentityParserLayer.hpp>
#include <Compiler/Lexer/IdentifierLexerRule.hpp>
#include <Compiler/Lexer/SingleCharLexerRule.hpp>
#include <Compiler/Lexer/WhitespaceLexerRule.hpp>
#include <Compiler/Lexer/StringLexerRule.hpp>
#include <Compiler/IdentityEvaluator.hpp>
#include <Interfaces/Comparable.hpp>
#include <Interfaces/Printable.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Compiler/Toolchain.hpp>
#include <iostream>

enum class TokenType : uint8_t {
    ParenthesesStart,
    ParenthesesEnd,
    Variable,
    Comma,
    Assignment,
    TypeDeclaration,
    Abstraction,
    Application,
};
[[nodiscard]] Term NodeToTerm(const MathLib::ParserNode& node) {
    switch ((TokenType)node.GetType()) {
        case TokenType::Variable: return Term(node.GetData());
        case TokenType::Comma:
        case TokenType::Assignment:
        case TokenType::TypeDeclaration:
        case TokenType::Abstraction:
        case TokenType::Application: {
            Term::Type type = Term::Type::None;
            switch ((TokenType)node.GetType()) {
                case TokenType::Comma: {
                    type = Term::Type::Comma;
                    break;
                }
                case TokenType::Assignment: {
                    type = Term::Type::Assignment;
                    break;
                }
                case TokenType::TypeDeclaration: {
                    type = Term::Type::TypeDeclaration;
                    break;
                }
                case TokenType::Abstraction: {
                    type = node.GetData() == "->" ? Term::Type::Abstraction : Term::Type::TypeAbstraction;
                    break;
                }
                case TokenType::Application: {
                    type = node.GetData() == "." ? Term::Type::Application : Term::Type::TypeApplication;
                    break;
                }
                default: break;
            }
            return Term(type, NodeToTerm(node.At(0)), NodeToTerm(node.At(1)));
        }
        default: return Term();
    }
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
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesStart, '('_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesEnd, ')'_M),
            new MathLib::IdentifierLexerRule((size_t)TokenType::Variable, true),
            new MathLib::StringLexerRule((size_t)TokenType::Abstraction, MathLib::MakeArray<MathLib::String>("->", "=>")),
            new MathLib::StringLexerRule((size_t)TokenType::Application, MathLib::MakeArray<MathLib::String>('.', '^')),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Comma, ','_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::TypeDeclaration, ':'_M),
            new MathLib::SingleCharLexerRule((size_t)TokenType::Assignment, '='_M)
        )), new MathLib::Parser(MathLib::MakeArray<MathLib::ParserLayer*>(
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Comma, (size_t)TokenType::Comma),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Assignment, (size_t)TokenType::Assignment),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::TypeDeclaration, (size_t)TokenType::TypeDeclaration),
            new MathLib::RightBinaryParserLayer((size_t)TokenType::Abstraction, (size_t)TokenType::Abstraction),
            new MathLib::LeftBinaryParserLayer((size_t)TokenType::Application, (size_t)TokenType::Application),
            new MathLib::UnwrapperParserLayer((size_t)TokenType::ParenthesesStart, (size_t)TokenType::ParenthesesEnd),
            new MathLib::IdentityParserLayer((size_t)TokenType::Variable, (size_t)TokenType::Variable)
        )), new MathLib::IdentityEvaluator());
        MathLib::HostFileSystem fs;
        toolchain.LoadInput(fs.Open(MathLib::String(argv[1]), MathLib::OpenMode::Read).ReadUntil('\0'));
        Context context;
        if (!context.TypeCheck(NodeToTerm(toolchain.GetNode()))) MathLib::Panic("Failed to type check the program");
        std::cout << context << std::endl;
        // TODO: Pattern matching
        /*
        add : Nat -> Nat -> Nat = (a : Nat) -> {
            0 -> a,
            (b : Nat, succ . b) -> succ . (add . a . b)
        }
        */
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}