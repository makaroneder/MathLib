#ifndef Lexer_H
#define Lexer_H
#include "Token.hpp"
#include "LexerRule.hpp"
#include <Expected.hpp>

struct Lexer : MathLib::Allocatable {
    Lexer(const MathLib::Sequence<LexerRule*>& rules);
    virtual ~Lexer(void) override;
    void LoadInput(const MathLib::Sequence<char>& str);
    [[nodiscard]] MathLib::Expected<Token> LexSingle(void);
    [[nodiscard]] MathLib::Array<Token> Lex(void);

    private:
    MathLib::String input;
    MathLib::Array<LexerRule*> rules;
    size_t i;
};

#endif