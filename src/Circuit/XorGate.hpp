#ifndef XorGate_H
#define XorGate_H
#include "OnePinCircuitElement.hpp"

struct XorGate : OnePinCircuitElement {
    XorGate(const MathLib::Array<CircuitElementConnection>& children);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override;

    protected:
    [[nodiscard]] virtual MathLib::Expected<bool> EvaluateInternal(void) const override;
};

#endif