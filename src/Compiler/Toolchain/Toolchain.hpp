#ifndef Toolchain_H
#define Toolchain_H
#include "Evaluator.hpp"
#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"

struct Toolchain {
    Toolchain(Lexer* lexer, Parser* parser, Evaluator<Node>* optimizer);
    ~Toolchain(void);
    void LoadInput(const MathLib::Sequence<char>& input);
    template <typename T>
    [[nodiscard]] T Evaluate(Evaluator<T>& evaluator) {
        return evaluator.Evaluate(optimizer->Evaluate(parser->Parse()));
    }

    private:
    Lexer* lexer;
    Parser* parser;
    Evaluator<Node>* optimizer;
};

#endif