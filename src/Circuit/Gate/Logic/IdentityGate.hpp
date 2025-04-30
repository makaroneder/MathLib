#ifndef IdentityGate_H
#define IdentityGate_H
#include "../Gate.hpp"

struct IdentityGate : Gate {
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;
};

#endif