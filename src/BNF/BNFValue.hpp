#ifndef BNFValue_H
#define BNFValue_H
#include <Interfaces/Printable.hpp>

struct BNFVariable;
struct BNFValue : MathLib::Printable {
    enum class Type : uint8_t {
        Constant,
        Variable,
        Array,
        Or,
    };
    MathLib::String value;
    MathLib::Array<BNFValue> children;
    Type type;

    BNFValue(void);
    BNFValue(const MathLib::Sequence<char>& value, bool constant);
    BNFValue(const MathLib::Array<BNFValue>& children, bool array);
    [[nodiscard]] bool Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables) const;
    [[nodiscard]] BNFValue Substitute(const MathLib::Sequence<BNFVariable>& variables) const;
    [[nodiscard]] bool IsDependentOn(const MathLib::Sequence<char>& variable) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    private:
    [[nodiscard]] bool Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables, size_t& i) const;
};

#endif