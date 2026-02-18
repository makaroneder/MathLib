#ifndef MathLib_Compiler_Toolchain_H
#define MathLib_Compiler_Toolchain_H
#include "Evaluator.hpp"
#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"

namespace MathLib {
    struct Toolchain : Allocatable {
        Toolchain(Lexer* lexer, Parser* parser, Evaluator<ParserNode>* optimizer);
        virtual ~Toolchain(void) override;
        void LoadInput(const Sequence<char>& input);
        [[nodiscard]] ParserNode GetNode(void) const;
        template <typename T>
        [[nodiscard]] T Evaluate(Evaluator<T>& evaluator) {
            return evaluator.Evaluate(GetNode());
        }

        private:
        Lexer* lexer;
        Parser* parser;
        Evaluator<ParserNode>* optimizer;
    };
}

#endif