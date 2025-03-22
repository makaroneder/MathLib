#ifndef OnePinCircuitElement_H
#define OnePinCircuitElement_H
#include "CircuitElement.hpp"

struct OnePinCircuitElement : CircuitElement {
    OnePinCircuitElement(void);
    OnePinCircuitElement(const MathLib::Sequence<CircuitElementConnection>& children);
    [[nodiscard]] virtual MathLib::Expected<MathLib::Bitmap> Evaluate(void) const override;

    protected:
    [[nodiscard]] virtual MathLib::Expected<bool> EvaluateInternal(void) const = 0;
};

#endif