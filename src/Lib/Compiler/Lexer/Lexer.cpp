#include "Lexer.hpp"
#include "../../Interfaces/SubSequence.hpp"

namespace MathLib {
    Lexer::Lexer(const Sequence<LexerRule*>& rules) : input(), rules(CollectionToArray<LexerRule*>(rules)), i(0) {
        EmptyBenchmark
    }
    Lexer::~Lexer(void) {
        StartBenchmark
        for (LexerRule*& rule : rules) delete rule;
        EndBenchmark
    }
    void Lexer::LoadInput(const Sequence<char>& str) {
        StartBenchmark
        input = CollectionToString(str);
        i = 0;
        EndBenchmark
    }
    Expected<Token> Lexer::LexSingle(void) {
        StartBenchmark
        if (i >= input.GetSize()) ReturnFromBenchmark(Expected<Token>());
        for (const LexerRule* const& rule : rules) {
            const size_t save = i;
            rule->Match(input, i);
            if (save != i) {
                const size_t type = rule->GetType();
                if (type != SIZE_MAX) ReturnFromBenchmark(Token(type, SubSequence<char>(input, Interval<size_t>(save, i))));
            }
        }
        ReturnFromBenchmark(Expected<Token>());
    }
    Array<Token> Lexer::Lex(void) {
        StartBenchmark
        const size_t size = input.GetSize();
        Array<Token> ret;
        while (i < size) {
            const Expected<Token> tmp = LexSingle();
            if (!tmp.HasValue() || !ret.Add(tmp.Get())) ReturnFromBenchmark(Array<Token>());
        }
        ReturnFromBenchmark(ret);
    }
}