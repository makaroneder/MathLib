#ifndef Gate_H
#define Gate_H
#include <Bitmap.hpp>

struct Gate : MathLib::Allocatable {
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) = 0;
};

#endif