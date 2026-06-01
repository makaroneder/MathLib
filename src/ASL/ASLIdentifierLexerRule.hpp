#ifndef ASLIdentifierLexerRule_H
#define ASLIdentifierLexerRule_H
#include <Compiler/Lexer/LexerRule.hpp>

struct ASLIdentifierLexerRule : MathLib::LexerRule {
    ASLIdentifierLexerRule(size_t type);
    virtual void Match(const MathLib::Sequence<char>& str, size_t& i) const override;
};

#endif