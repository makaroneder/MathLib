#ifndef NotOrGate_H
#define NotOrGate_H
#include "../Gate.hpp"

struct NotOrGate : Gate {
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;
};

#endif