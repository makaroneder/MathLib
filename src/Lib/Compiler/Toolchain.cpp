#include "Toolchain.hpp"

namespace MathLib {
    Toolchain::Toolchain(Lexer* lexer, Parser* parser, Evaluator<ParserNode>* optimizer) : lexer(lexer), parser(parser), optimizer(optimizer) {
        EmptyBenchmark
    }
    Toolchain::~Toolchain(void) {
        StartBenchmark
        delete lexer;
        delete parser;
        delete optimizer;
        EndBenchmark
    }
    void Toolchain::LoadInput(const MathLib::Sequence<char>& input) {
        StartBenchmark
        lexer->LoadInput(input);
        parser->LoadInput(lexer->Lex());
        EndBenchmark
    }
    ParserNode Toolchain::GetNode(void) const {
        StartAndReturnFromBenchmark(optimizer->Evaluate(parser->Parse()));
    }
}