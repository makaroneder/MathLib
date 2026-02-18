#include "Parser.hpp"
#include "../../FunctionT.hpp"

namespace MathLib {
    Parser::Parser(const Sequence<ParserLayer*>& layers) : layers(CollectionToArray<ParserLayer*>(layers)), tokens(), i(0) {}
    Parser::~Parser(void) {
        for (ParserLayer*& layer : layers) delete layer;
    }
    void Parser::LoadInput(const Sequence<Token>& input) {
        tokens = CollectionToArray<Token>(input);
        i = 0;
    }
    ParserNode Parser::Parse(void) {
        return Parse(0);
    }
    ParserNode Parser::Parse(size_t layer) {
        return layer != layers.GetSize() ? layers.At(layer)->Parse(MakeFunctionT<ParserNode>([this](void) -> ParserNode {
            return Parse();
        }), MakeFunctionT<ParserNode>([this, layer](void) -> ParserNode {
            return Parse(layer + 1);
        }), tokens, i) : ParserNode();
    }
}