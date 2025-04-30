#include "Lexer.hpp"
#include <Interfaces/SubSequence.hpp>

Lexer::Lexer(const MathLib::Sequence<LexerRule*>& rules) : input(), rules(MathLib::CollectionToArray<LexerRule*>(rules)), i(0) {}
Lexer::~Lexer(void) {
    for (LexerRule*& rule : rules) delete rule;
}
void Lexer::LoadInput(const MathLib::Sequence<char>& str) {
    input = MathLib::CollectionToString(str);
    i = 0;
}
MathLib::Expected<Token> Lexer::LexSingle(void) {
    if (i >= input.GetSize()) return MathLib::Expected<Token>();
    for (const LexerRule* const& rule : rules) {
        const size_t save = i;
        rule->Match(input, i);
        if (save != i) {
            const size_t type = rule->GetType();
            if (type != SIZE_MAX) return Token(type, MathLib::SubSequence<char>(input, MathLib::Interval<size_t>(save, i)));
        }
    }
    return MathLib::Expected<Token>();
}
MathLib::Array<Token> Lexer::Lex(void) {
    const size_t size = input.GetSize();
    MathLib::Array<Token> ret;
    while (i < size) {
        const MathLib::Expected<Token> tmp = LexSingle();
        if (!tmp.HasValue() || !ret.Add(tmp.Get())) return MathLib::Array<Token>();
    }
    return ret;
}