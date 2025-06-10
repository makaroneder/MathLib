#include "Parser.hpp"
#include "../../FunctionT.hpp"

namespace MathLib {
    Parser::Parser(const Sequence<ParserLayer*>& layers) : layers(CollectionToArray<ParserLayer*>(layers)), tokens(), i(0) {
        EmptyBenchmark
    }
    Parser::~Parser(void) {
        StartBenchmark
        for (ParserLayer*& layer : layers) delete layer;
        EndBenchmark
    }
    void Parser::LoadInput(const Sequence<Token>& input) {
        StartBenchmark
        tokens = CollectionToArray<Token>(input);
        i = 0;
        EndBenchmark
    }
    ParserNode Parser::Parse(void) {
        StartAndReturnFromBenchmark(Parse(0));
    }
    ParserNode Parser::Parse(size_t layer) {
        StartAndReturnFromBenchmark(layer != layers.GetSize() ? layers.At(layer)->Parse(MakeFunctionT<ParserNode>(nullptr, [this](const void*) -> ParserNode {
            return Parse();
        }), MakeFunctionT<ParserNode>(nullptr, [this, layer](const void*) -> ParserNode {
            return Parse(layer + 1);
        }), tokens, i) : ParserNode());
    }
}