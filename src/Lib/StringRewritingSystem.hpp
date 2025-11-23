#ifndef MathLib_StringRewritingSystem_H
#define MathLib_StringRewritingSystem_H
#include "StringRewritingSystemRule.hpp"

namespace MathLib {
    template <typename T>
    struct StringRewritingSystem {
        StringRewritingSystem(const Array<StringRewritingSystemRule<T>>& rules) : rules(rules) {}
        Array<T> Rewrite(const Sequence<T>& sequence) {
            Array<T> ret = CollectionToArray<T>(sequence);
            while (true) {
                bool found = false;
                for (const StringRewritingSystemRule<T>& rule : rules) {
                    const size_t start = ret.Find(rule.input);
                    if (start == SIZE_MAX) continue;
                    const size_t end = start + rule.input.GetSize();
                    Array<T> tmp = Array<T>(start + ret.GetSize() - end + rule.output.GetSize());
                    for (size_t i = 0; i < start; i++) tmp.At(i) = ret.At(i);
                    for (size_t i = 0; i < rule.output.GetSize(); i++) tmp.At(i + start) = rule.output.At(i);
                    for (size_t i = end; i < ret.GetSize(); i++) tmp.At(i + start + rule.output.GetSize() - end) = ret.At(i);
                    ret = tmp;
                    found = true;
                    break;
                }
                if (!found) break;
            }
            return ret;
        }
    
        private:
        Array<StringRewritingSystemRule<T>> rules;
    };
}

#endif