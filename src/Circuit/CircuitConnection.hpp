#ifndef CircuitConnection_H
#define CircuitConnection_H
#include <Interfaces/Printable.hpp>

struct CircuitConnection : MathLib::Printable {
    size_t element;
    size_t bit;

    CircuitConnection(void);
    CircuitConnection(size_t element, size_t bit);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] bool operator==(const CircuitConnection& other) const;
    [[nodiscard]] bool operator!=(const CircuitConnection& other) const;
};

#endif