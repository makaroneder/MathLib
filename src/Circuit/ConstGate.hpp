#ifndef ConstGate_H
#define ConstGate_H
#include "OnePinCircuitElement.hpp"

struct ConstGate : OnePinCircuitElement {
    ConstGate(bool value);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual MathLib::String ToString(const MathLib::String& padding = "") const override;
    void Flip(void);

    protected:
    virtual MathLib::Expected<bool> EvaluateInternal(void) const override;

    private:
    bool value;
};

#endif