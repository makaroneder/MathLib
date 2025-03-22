#ifndef ConstGate_H
#define ConstGate_H
#include "OnePinCircuitElement.hpp"

struct ConstGate : OnePinCircuitElement {
    ConstGate(bool value);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    void Flip(void);

    protected:
    [[nodiscard]] virtual MathLib::Expected<bool> EvaluateInternal(void) const override;

    private:
    bool value;
};

#endif