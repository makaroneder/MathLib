#include "LogicGate.hpp"

namespace MathLib {
    bool NotGate::Invoke(bool x) const {
        return !x;
    }
    #define MakeLogicGate(name, op1, op2)           \
    bool name##Gate::Invoke(bool x, bool y) const { \
        return op2(x op1 y);                        \
    }
    MakeLogicGate(And, &,);
    MakeLogicGate(Or, |,);
    MakeLogicGate(Xor, ^,);
    MakeLogicGate(Nand, &, !);
    MakeLogicGate(Nor, |, !);
    MakeLogicGate(Xnor, ^, !);
    #undef MakeLogicGate
}