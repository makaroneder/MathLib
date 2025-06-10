#ifndef MathLib_Compiler_Evaluator_H
#define MathLib_Compiler_Evaluator_H
#include "Parser/ParserNode.hpp"

namespace MathLib {
    template <typename T>
    struct Evaluator : Allocatable {
        [[nodiscard]] virtual T Evaluate(const ParserNode& node) = 0;
    };
}

#endif