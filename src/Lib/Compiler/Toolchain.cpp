#include "Toolchain.hpp"

namespace MathLib {
    Toolchain::Toolchain(Lexer* lexer, Parser* parser, Evaluator<ParserNode>* optimizer) : lexer(lexer), parser(parser), optimizer(optimizer) {}
    Toolchain::~Toolchain(void) {
        delete lexer;
        delete parser;
        delete optimizer;
    }
    void Toolchain::LoadInput(const Sequence<char>& input) {
        lexer->LoadInput(input);
        parser->LoadInput(lexer->Lex());
    }
    ParserNode Toolchain::GetNode(void) const {
        return optimizer->Evaluate(parser->Parse());
    }
}