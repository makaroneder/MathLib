#ifndef ConverseNonImplicationGate_H
#define ConverseNonImplicationGate_H
#include "../Gate.hpp"

struct ConverseNonImplicationGate : Gate {
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;
};

#endif