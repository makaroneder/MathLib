#ifndef Memory_H
#define Memory_H
#include "Gate.hpp"

struct Memory : Gate {
    Memory(size_t size);
    Memory(const MathLib::Sequence<bool>& memory);
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;

    private:
    MathLib::Bitmap memory;
};


#endif