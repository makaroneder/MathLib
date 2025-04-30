#ifndef Clock_H
#define Clock_H
#include "Gate.hpp"

struct Clock : Gate {
    Clock(bool value);
    virtual MathLib::Bitmap Update(const MathLib::Sequence<bool>& input) override;

    private:
    bool value;
};

#endif