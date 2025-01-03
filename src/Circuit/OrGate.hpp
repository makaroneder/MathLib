#ifndef OrGate_H
#define OrGate_H
#include "OnePinCircuitElement.hpp"

struct OrGate : OnePinCircuitElement {
    OrGate(const MathLib::Array<CircuitElementConnection>& children);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual MathLib::String ToString(const MathLib::String& padding = "") const override;

    protected:
    virtual MathLib::Expected<bool> EvaluateInternal(void) const override;
};

#endif