#ifndef AndGate_H
#define AndGate_H
#include "../Gate.hpp"

struct AndGate : Gate {
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;
};

#endif