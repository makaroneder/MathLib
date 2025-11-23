#ifndef MathLib_Interfaces_LogicGate_H
#define MathLib_Interfaces_LogicGate_H
#include "Function.hpp"

namespace MathLib {
    struct NotGate : Function<bool, bool> {
        [[nodiscard]] virtual bool Invoke(bool x) const override;
    };
    #define MakeLogicGate(name)                                             \
    struct name##Gate : Function<bool, bool, bool> {                        \
        [[nodiscard]] virtual bool Invoke(bool x, bool y) const override;   \
    }
    MakeLogicGate(And);
    MakeLogicGate(Or);
    MakeLogicGate(Xor);
    MakeLogicGate(Nand);
    MakeLogicGate(Nor);
    MakeLogicGate(Xnor);
    #undef MakeLogicGate
}

#endif