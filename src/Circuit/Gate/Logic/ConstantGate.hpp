#ifndef ConstantGate_H
#define ConstantGate_H
#include "../Gate.hpp"

struct ConstantGate : Gate {
    ConstantGate(const MathLib::Sequence<bool>& value);
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;

    MathLib::Bitmap value;
};

#endif