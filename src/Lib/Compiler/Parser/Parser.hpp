#ifndef MathLib_Compiler_Parser_H
#define MathLib_Compiler_Parser_H
#include "ParserLayer.hpp"

namespace MathLib {
    struct Parser : Allocatable {
        Parser(const Sequence<ParserLayer*>& layers);
        virtual ~Parser(void) override;
        void LoadInput(const Sequence<Token>& input);
        [[nodiscard]] ParserNode Parse(void);

        private:
        [[nodiscard]] ParserNode Parse(size_t layer);

        Array<ParserLayer*> layers;
        Array<Token> tokens;
        size_t i;
    };
}

#endif