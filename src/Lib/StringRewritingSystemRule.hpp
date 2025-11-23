#ifndef MathLib_StringRewritingSystemRule_H
#define MathLib_StringRewritingSystemRule_H
#include "Typedefs.hpp"

namespace MathLib {
    template <typename T>
    struct StringRewritingSystemRule {
        Array<T> input;
        Array<T> output;
    
        StringRewritingSystemRule(void) : input(), output() {}
        StringRewritingSystemRule(const Sequence<T>& input, const Sequence<T>& output) : input(CollectionToArray<T>(input)), output(CollectionToArray<T>(output)) {}
    };
}

#endif