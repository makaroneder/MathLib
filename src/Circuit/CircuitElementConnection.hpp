#ifndef CircuitElementConnection_H
#define CircuitElementConnection_H
#include <Interfaces/Printable.hpp>
#include <Expected.hpp>

struct CircuitElementConnection : MathLib::Printable {
    size_t index;
    size_t pin;

    CircuitElementConnection(void);
    CircuitElementConnection(size_t index, size_t pin);
    MathLib::Expected<bool> Evaluate(void) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual MathLib::String ToString(const MathLib::String& padding = "") const override;
};

#endif