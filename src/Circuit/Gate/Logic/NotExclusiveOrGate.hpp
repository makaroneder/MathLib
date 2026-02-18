#ifndef NotExclusiveOrGate_H
#define NotExclusiveOrGate_H
#include "../Gate.hpp"

struct NotExclusiveOrGate : Gate {
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;
};

#endif