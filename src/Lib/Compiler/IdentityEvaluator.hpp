#ifndef MathLib_Compiler_IdentityEvaluator_H
#define MathLib_Compiler_IdentityEvaluator_H
#include "Evaluator.hpp"

namespace MathLib {
    struct IdentityEvaluator : Evaluator<ParserNode> {
        [[nodiscard]] virtual ParserNode Evaluate(const ParserNode& node) override;
    };
}

#endif