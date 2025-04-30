#include "Toolchain.hpp"

Toolchain::Toolchain(Lexer* lexer, Parser* parser, Evaluator<Node>* optimizer) : lexer(lexer), parser(parser), optimizer(optimizer) {}
Toolchain::~Toolchain(void) {
    delete lexer;
    delete parser;
    delete optimizer;
}
void Toolchain::LoadInput(const MathLib::Sequence<char>& input) {
    lexer->LoadInput(input);
    parser->LoadInput(lexer->Lex());
}