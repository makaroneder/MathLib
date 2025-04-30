#ifndef ExclusiveOrGate_H
#define ExclusiveOrGate_H
#include "../Gate.hpp"

struct ExclusiveOrGate : Gate {
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;
};

#endif