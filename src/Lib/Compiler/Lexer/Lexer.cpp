#include "Lexer.hpp"
#include "../../Interfaces/Sequence/SubSequence.hpp"

namespace MathLib {
    Lexer::Lexer(const Sequence<LexerRule*>& rules) : input(), rules(CollectionToArray<LexerRule*>(rules)), i(0) {}
    Lexer::~Lexer(void) {
        for (LexerRule*& rule : rules) delete rule;
    }
    void Lexer::LoadInput(const Sequence<char>& str) {
        input = CollectionToString(str);
        i = 0;
    }
    Expected<Token> Lexer::LexSingle(void) {
        if (i >= input.GetSize()) return Expected<Token>();
        for (const LexerRule* const& rule : rules) {
            const size_t save = i;
            rule->Match(input, i);
            if (save != i) {
                const size_t type = rule->GetType();
                if (type != SIZE_MAX) return Token(type, SubSequence<char>(input, Interval<size_t>(save, i)));
            }
        }
        return Expected<Token>();
    }
    Array<Token> Lexer::Lex(void) {
        const size_t size = input.GetSize();
        Array<Token> ret;
        while (i < size) {
            const Expected<Token> tmp = LexSingle();
            if (!tmp.HasValue() || !ret.Add(tmp.Get())) return Array<Token>();
        }
        return ret;
    }
}