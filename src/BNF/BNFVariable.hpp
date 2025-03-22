#ifndef BNFVariable_H
#define BNFVariable_H
#include "BNFValue.hpp"

struct BNFVariable : MathLib::Printable {
    BNFValue value;
    MathLib::String name;

    BNFVariable(void);
    BNFVariable(const MathLib::Sequence<char>& name, const BNFValue& value);
    [[nodiscard]] bool Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables) const;
    [[nodiscard]] bool IsRecursive(void) const;
    [[nodiscard]] BNFVariable Substitute(const MathLib::Sequence<BNFVariable>& variables) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
};

#endif