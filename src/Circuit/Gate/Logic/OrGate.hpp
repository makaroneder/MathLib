#ifndef OrGate_H
#define OrGate_H
#include "../Gate.hpp"

struct OrGate : Gate {
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;
};

#endif