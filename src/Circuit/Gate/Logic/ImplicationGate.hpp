#ifndef ImplicationGate_H
#define ImplicationGate_H
#include "../Gate.hpp"

struct ImplicationGate : Gate {
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;
};

#endif