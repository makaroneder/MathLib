#ifndef XorGate_H
#define XorGate_H
#include "OnePinCircuitElement.hpp"

struct XorGate : OnePinCircuitElement {
    XorGate(const MathLib::Sequence<CircuitElementConnection>& children);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    protected:
    [[nodiscard]] virtual MathLib::Expected<bool> EvaluateInternal(void) const override;
};

#endif