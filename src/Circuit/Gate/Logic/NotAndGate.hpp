#ifndef NotAndGate_H
#define NotAndGate_H
#include "../Gate.hpp"

struct NotAndGate : Gate {
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;
};

#endif