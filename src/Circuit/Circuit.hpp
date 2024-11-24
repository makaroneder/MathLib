#ifndef Circuit_H
#define Circuit_H
#include "CircuitElement.hpp"

struct Circuit : CircuitElement {
    Circuit(const MathLib::Array<CircuitElementConnection>& children);
    virtual MathLib::Expected<MathLib::Bitmap> Evaluate(void) const override;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual MathLib::String ToString(const MathLib::String& padding = "") const override;
};

#endif