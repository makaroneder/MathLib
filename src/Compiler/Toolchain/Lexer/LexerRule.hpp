#ifndef LexerRule_H
#define LexerRule_H
#include <Interfaces/Sequence.hpp>

struct LexerRule : MathLib::Allocatable {
    LexerRule(size_t type);
    [[nodiscard]] size_t GetType(void) const;
    virtual void Match(const MathLib::Sequence<char>& str, size_t& i) const = 0;

    private:
    size_t type;
};

#endif