#ifndef CircuitElementConnection_H
#define CircuitElementConnection_H
#include <Interfaces/Printable.hpp>
#include <Expected.hpp>

struct CircuitElementConnection : MathLib::Printable {
    size_t index;
    size_t pin;

    CircuitElementConnection(void);
    CircuitElementConnection(size_t index, size_t pin);
    [[nodiscard]] MathLib::Expected<bool> Evaluate(void) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
};

#endif