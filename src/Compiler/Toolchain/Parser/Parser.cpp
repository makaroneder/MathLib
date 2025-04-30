#include "Parser.hpp"
#include <FunctionT.hpp>

Parser::Parser(const MathLib::Sequence<ParserLayer*>& layers) : layers(MathLib::CollectionToArray<ParserLayer*>(layers)), tokens(), i(0) {}
Parser::~Parser(void) {
    for (ParserLayer*& layer : layers) delete layer;
}
void Parser::LoadInput(const MathLib::Sequence<Token>& input) {
    tokens = MathLib::CollectionToArray<Token>(input);
    i = 0;
}
Node Parser::Parse(void) {
    return Parse(0);
}
Node Parser::Parse(size_t layer) {
    return layer != layers.GetSize() ? layers.At(layer)->Parse(MathLib::MakeFunctionT<Node>(nullptr, [this](const void*) -> Node {
        return Parse();
    }), MathLib::MakeFunctionT<Node>(nullptr, [this, layer](const void*) -> Node {
        return Parse(layer + 1);
    }), tokens, i) : Node();
}