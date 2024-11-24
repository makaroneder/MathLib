#ifndef OnePinCircuitElement_H
#define OnePinCircuitElement_H
#include "CircuitElement.hpp"

struct OnePinCircuitElement : CircuitElement {
    OnePinCircuitElement(void);
    OnePinCircuitElement(const MathLib::Array<CircuitElementConnection>& children);
    virtual MathLib::Expected<MathLib::Bitmap> Evaluate(void) const override;

    protected:
    virtual MathLib::Expected<bool> EvaluateInternal(void) const = 0;
};

#endif