#ifndef Theory_H
#define Theory_H
#include "Formula.hpp"

struct Theory : MathLib::Printable {
    MathLib::String name;
    MathLib::Array<MathLib::String> variables;
    MathLib::Array<Formula> data;

    Theory(void);
    Theory(const MathLib::Sequence<char>& name, const MathLib::Sequence<Formula>& data);
    Theory(const MathLib::Sequence<char>& name, const MathLib::Sequence<MathLib::String>& variables, const MathLib::Sequence<Formula>& data);
    [[nodiscard]] size_t GetFreeVariables(const Theory& args) const;
    [[nodiscard]] Theory Substitute(const Theory& args) const;
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
};

#endif