#ifndef NonImplicationGate_H
#define NonImplicationGate_H
#include "../Gate.hpp"

struct NonImplicationGate : Gate {
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;
};

#endif