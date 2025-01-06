#ifndef NotGate_H
#define NotGate_H
#include "OnePinCircuitElement.hpp"

struct NotGate : OnePinCircuitElement {
    NotGate(const CircuitElementConnection& child);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override;

    protected:
    [[nodiscard]] virtual MathLib::Expected<bool> EvaluateInternal(void) const override;
};


#endif