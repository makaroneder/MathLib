#include "Toolchain.hpp"

namespace MathLib {
    Toolchain::Toolchain(Lexer* lexer, Parser* parser, const Function<ParserNode, ParserNode>& optimizer) : lexer(lexer), parser(parser), optimizer(optimizer) {}
    Toolchain::~Toolchain(void) {
        delete lexer;
        delete parser;
    }
    void Toolchain::LoadInput(const Sequence<char>& input) {
        parser->LoadInput(Lex(input));
    }
    Array<Token> Toolchain::Lex(const Sequence<char>& input) {
        lexer->LoadInput(input);
        return lexer->Lex();
    }
    ParserNode Toolchain::GetNode(void) const {
        return optimizer(parser->Parse());
    }
}