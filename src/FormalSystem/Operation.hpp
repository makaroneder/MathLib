#ifndef Operation_H
#define Operation_H
#include "Formula.hpp"

struct Operation {
    using Func = MathLib::Function<MathLib::Array<Formula>, const MathLib::Sequence<Formula>&>;
    MathLib::String name;
    Func* func;

    Operation(const MathLib::Sequence<char>& name, Func* func);
    ~Operation(void);
};

#endif