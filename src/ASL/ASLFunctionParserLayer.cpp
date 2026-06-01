#include "ASLFunctionParserLayer.hpp"
#include "TokenType.hpp"

MathLib::ParserNode ASLFunctionParserLayer::Parse(const MathLib::Function<MathLib::ParserNode>& root, const MathLib::Function<MathLib::ParserNode>& next, const MathLib::Sequence<MathLib::Token>& tokens, size_t& i) const {
    const size_t size = tokens.GetSize();
    const MathLib::ParserNode name = next();
    if (name.GetType() != (size_t)TokenType::Identifier || i >= size) return name;
    MathLib::ParserNode args = MathLib::ParserNode((size_t)TokenType::None);
    bool hasArgs = false;
    if (tokens.AtUnsafe(i).CheckType((size_t)TokenType::ParenthesesStart)) {
        hasArgs = true;
        i++;
        args = root();
        if (i >= size || !tokens.AtUnsafe(i++).CheckType((size_t)TokenType::ParenthesesEnd)) return MathLib::ParserNode();
    }
    if (i >= size || !tokens.AtUnsafe(i).CheckType((size_t)TokenType::BracketsStart)) {
        if (args.GetType() == (size_t)TokenType::None) return name;
        return MathLib::ParserNode((size_t)TokenType::Function, name.GetData(), MathLib::MakeArray<MathLib::ParserNode>(args));
    }
    i++;
    MathLib::Array<MathLib::ParserNode> body;
    while (true) {
        if (i >= size) return MathLib::ParserNode();
        if (tokens.AtUnsafe(i).CheckType((size_t)TokenType::BracketsEnd)) break;
        if (!body.Add(root())) return MathLib::ParserNode();
    }
    i++;
    if (!hasArgs) return MathLib::ParserNode((size_t)TokenType::Function, name.GetData(), MathLib::MakeArray<MathLib::ParserNode>(MathLib::ParserNode((size_t)TokenType::FunctionBody, ""_M, body)));
    return MathLib::ParserNode((size_t)TokenType::Function, name.GetData(), MathLib::MakeArray<MathLib::ParserNode>(args, MathLib::ParserNode((size_t)TokenType::FunctionBody, ""_M, body)));
}