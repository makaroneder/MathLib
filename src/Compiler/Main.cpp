#include "TokenType.hpp"
#include "LetParserLayer.hpp"
#include "ScopeParserLayer.hpp"
#include "ReturnParserLayer.hpp"
#include "FunctionParserLayer.hpp"
#include <Compiler/Parser/RightBinaryParserLayer.hpp>
#include <Compiler/Parser/UnwrapperParserLayer.hpp>
#include <Compiler/Parser/IdentityParserLayer.hpp>
#include <Compiler/Lexer/WhitespaceLexerRule.hpp>
#include <Compiler/Lexer/IdentifierLexerRule.hpp>
#include <Compiler/Lexer/SingleCharLexerRule.hpp>
#include <Interfaces/IdentityFunction.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Compiler/Toolchain.hpp>
#include <iostream>

MathLib::String NodeToStringInternal(const MathLib::ParserNode& node, const MathLib::String& padding) {
    switch ((TokenType)node.GetType()) {
        case TokenType::Identifier: return node.GetData();
        case TokenType::Comma: return NodeToStringInternal(node.At(0), padding) + ", " + NodeToStringInternal(node.At(1), padding);
        case TokenType::Scope: {
            const MathLib::String padd = padding + '\t';
            MathLib::String ret = "{\n";
            for (const MathLib::ParserNode& child : node) ret += padd + NodeToStringInternal(child, padd) + ";\n";
            return ret + padding + '}';
        }
        case TokenType::Function: return "func "_M + NodeToStringInternal(node.At(0), padding) + ' ' + node.GetData() + '(' + NodeToStringInternal(node.At(1), padding) + ") " + NodeToStringInternal(node.At(2), padding);
        case TokenType::Return: return "return "_M + NodeToStringInternal(node.At(0), padding);
        case TokenType::Let: return node.GetData() + ' ' + NodeToStringInternal(node.At(0), padding) + ' ' + NodeToStringInternal(node.At(1), padding);
        default: return "";
    }
}
MathLib::String NodeToString(const MathLib::ParserNode& node) {
    if (node.GetType() != (size_t)TokenType::Scope) return "";
    const size_t size = node.GetSize();
    MathLib::String ret;
    for (size_t i = 0; i < size; i++) {
        if (i) ret += '\n';
        ret += NodeToStringInternal(node.At(i), ""_M) + ';';
    }
    return ret;
}
int main(int, char**) {
    try {
        const MathLib::IdentityFunction<MathLib::ParserNode, MathLib::ParserNode> optimizer;
        MathLib::Toolchain toolchain = MathLib::Toolchain(
            new MathLib::Lexer(MathLib::MakeArray<MathLib::LexerRule*>(
                new MathLib::WhitespaceLexerRule(SIZE_MAX),
                new MathLib::IdentifierLexerRule((size_t)TokenType::Identifier, false),
                new MathLib::SingleCharLexerRule((size_t)TokenType::Semicolon, ';'_M),
                new MathLib::SingleCharLexerRule((size_t)TokenType::Comma, ','_M),
                new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesStart, '('_M),
                new MathLib::SingleCharLexerRule((size_t)TokenType::ParenthesesEnd, ')'_M),
                new MathLib::SingleCharLexerRule((size_t)TokenType::BracketsStart, '{'_M),
                new MathLib::SingleCharLexerRule((size_t)TokenType::BracketsEnd, '}'_M)
            )), new MathLib::Parser(MathLib::MakeArray<MathLib::ParserLayer*>(
                new FunctionParserLayer(),
                new ReturnParserLayer(),
                new LetParserLayer(),
                new ScopeParserLayer(),
                new MathLib::RightBinaryParserLayer((size_t)TokenType::Comma, (size_t)TokenType::Comma),
                new MathLib::IdentityParserLayer((size_t)TokenType::Identifier, (size_t)TokenType::Identifier),
                new MathLib::UnwrapperParserLayer((size_t)TokenType::ParenthesesStart, (size_t)TokenType::ParenthesesEnd)
            )), optimizer
        );
        MathLib::HostFileSystem fs;
        toolchain.LoadInput('{'_M + fs.Open("src/TestPrograms/Compiler/Main.txt"_M, MathLib::OpenMode::Read).ReadUntil('\0') + '}');
        std::cout << NodeToString(toolchain.GetNode()) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}