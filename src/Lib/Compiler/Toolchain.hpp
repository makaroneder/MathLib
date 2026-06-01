#ifndef MathLib_Compiler_Toolchain_H
#define MathLib_Compiler_Toolchain_H
#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"

namespace MathLib {
    struct Toolchain : Allocatable {
        Toolchain(Lexer* lexer, Parser* parser, const Function<ParserNode, ParserNode>& optimizer);
        virtual ~Toolchain(void) override;
        void LoadInput(const Sequence<char>& input);
        Array<Token> Lex(const Sequence<char>& input);
        [[nodiscard]] ParserNode GetNode(void) const;

        private:
        Lexer* lexer;
        Parser* parser;
        const Function<ParserNode, ParserNode>& optimizer;
    };
}

#endif