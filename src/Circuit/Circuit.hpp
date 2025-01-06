#ifndef Circuit_H
#define Circuit_H
#include "CircuitElement.hpp"

struct Circuit : CircuitElement {
    Circuit(void);
    Circuit(const MathLib::Array<CircuitElementConnection>& children);
    [[nodiscard]] virtual MathLib::Expected<MathLib::Bitmap> Evaluate(void) const override;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::String& padding = "") const override;
};

#endif