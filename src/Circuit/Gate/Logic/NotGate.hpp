#ifndef NotGate_H
#define NotGate_H
#include "../Gate.hpp"

struct NotGate : Gate {
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;
};

#endif