#ifndef HTTPHeader_H
#define HTTPHeader_H
#include <Interfaces/Printable.hpp>

struct HTTPHeader : MathLib::Printable {
    MathLib::String name;
    MathLib::String value;

    HTTPHeader(const MathLib::Sequence<char>& name = ""_M, const MathLib::Sequence<char>& value = ""_M);
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
};

#endif