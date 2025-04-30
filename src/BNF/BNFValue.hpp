#ifndef BNFValue_H
#define BNFValue_H
#include <Interval.hpp>
#include <Graph/Tree.hpp>

struct BNFVariable;
struct BNFValue : MathLib::Printable {
    enum class Type : uint8_t {
        Constant,
        Variable,
        Array,
        Or,
        Optional,
        KleeneStar,
        KleenePlus,
        Range,
    };
    
    BNFValue(void);
    BNFValue(const MathLib::Interval<char>& range);
    BNFValue(const MathLib::Sequence<char>& value, bool constant);
    BNFValue(const MathLib::Array<BNFValue>& children, bool array);
    BNFValue(Type type, const BNFValue& child);
    [[nodiscard]] bool IsConstant(void) const;
    [[nodiscard]] MathLib::String GetValue(void) const;
    [[nodiscard]] bool Add(const BNFValue& child);
    [[nodiscard]] bool Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables) const;
    [[nodiscard]] MathLib::Tree<MathLib::String> BuildAST(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables) const;
    [[nodiscard]] BNFValue Substitute(const BNFVariable& variable) const;
    [[nodiscard]] BNFValue Substitute(const MathLib::Sequence<BNFVariable>& variables) const;
    [[nodiscard]] BNFValue Simplify(void) const;
    [[nodiscard]] size_t GetDependenceCount(const MathLib::Sequence<char>& variable, const MathLib::Sequence<BNFVariable>& variables) const;
    [[nodiscard]] size_t GetDataSize(size_t variableSize) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] bool operator==(const BNFValue& other) const;
    [[nodiscard]] bool operator!=(const BNFValue& other) const;

    private:
    MathLib::String value;
    MathLib::Array<BNFValue> children;
    Type type;

    [[nodiscard]] bool Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<BNFVariable>& variables, size_t& i, MathLib::Tree<MathLib::String>& tree) const;
};

#endif