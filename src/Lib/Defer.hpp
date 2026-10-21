#ifndef MathLib_Defer_H
#define MathLib_Defer_H
#include "FunctionT.hpp"

namespace MathLib {
    struct Defer : Allocatable {
        Defer(const Function<void>* function);
        virtual ~Defer(void) override;

        private:
        const Function<void>* function;
    };
    #define defer1(x, y) x##y
    #define defer2(x, y) defer1(x, y)
    #define defer3(x) defer2(x, __COUNTER__)
    #define defer(code) const MathLib::Defer defer3(_defer) = MathLib::AllocFunctionT<void>([&](void){ code; })
}

#endif