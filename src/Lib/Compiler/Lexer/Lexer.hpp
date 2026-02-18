#ifndef MathLib_Compiler_Lexer_H
#define MathLib_Compiler_Lexer_H
#include "Token.hpp"
#include "LexerRule.hpp"
#include "../../Expected.hpp"

namespace MathLib {
    struct Lexer : Allocatable {
        Lexer(const Sequence<LexerRule*>& rules);
        virtual ~Lexer(void) override;
        void LoadInput(const Sequence<char>& str);
        [[nodiscard]] Expected<Token> LexSingle(void);
        [[nodiscard]] Array<Token> Lex(void);

        private:
        String input;
        Array<LexerRule*> rules;
        size_t i;
    };
}

#endif