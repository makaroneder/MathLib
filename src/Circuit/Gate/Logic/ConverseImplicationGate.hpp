#ifndef ConverseImplicationGate_H
#define ConverseImplicationGate_H
#include "../Gate.hpp"

struct ConverseImplicationGate : Gate {
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;
};

#endif