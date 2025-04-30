#ifndef FormalSystem_H
#define FormalSystem_H
#include "Operation.hpp"
#include "Theory.hpp"

struct FormalSystem {
    FormalSystem(const MathLib::Sequence<Operation*>& operations, const MathLib::Sequence<Theory>& axioms);
    ~FormalSystem(void);
    [[nodiscard]] bool Prove(const Theory& proof);

    private:
    MathLib::Array<Operation*> operations;
    MathLib::Array<Theory> theories;

    [[nodiscard]] MathLib::Expected<bool> IsLogicalConsequence(Formula& prev, Formula& curr, const Formula& formula) const;
    [[nodiscard]] bool IsLogicalConsequence(Formula prev, const Formula& formula) const;
};

#endif