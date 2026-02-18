#ifndef BNFVariable_H
#define BNFVariable_H
#include "BNFValue.hpp"

struct BNFVariable : MathLib::Printable {
    BNFVariable(void);
    BNFVariable(const MathLib::Sequence<char>& name, const BNFValue& value);
    [[nodiscard]] bool CheckName(const MathLib::Sequence<char>& name_) const;
    [[nodiscard]] MathLib::String GetName(void) const;
    [[nodiscard]] BNFValue GetValue(void) const;
    [[nodiscard]] bool Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables) const;
    [[nodiscard]] MathLib::Tree<MathLib::String> BuildAST(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables) const;
    [[nodiscard]] size_t GetDependenceCount(const MathLib::Sequence<char>& variable, const MathLib::Sequence<BNFVariable>& variables) const;
    [[nodiscard]] bool IsRecursive(const MathLib::Sequence<BNFVariable>& variables) const;
    [[nodiscard]] BNFVariable Substitute(const BNFVariable& variable) const;
    [[nodiscard]] BNFVariable Substitute(const MathLib::Sequence<BNFVariable>& variables) const;
    [[nodiscard]] BNFVariable Simplify(void) const;
    [[nodiscard]] size_t GetDataSize(size_t variableSize) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] bool operator==(const BNFVariable& other) const;
    [[nodiscard]] bool operator!=(const BNFVariable& other) const;

    private:
    BNFValue value;
    MathLib::String name;
};

#endif