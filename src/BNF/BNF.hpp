#ifndef BNF_H
#define BNF_H
#include "BNFVariable.hpp"
#include <Interfaces/Saveable.hpp>

struct BNF : MathLib::Printable, MathLib::Saveable {
    BNF(void);
    BNF(const MathLib::Array<BNFVariable>& variables);
    [[nodiscard]] bool Add(const BNFVariable& variable);
    [[nodiscard]] bool Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<char>& variable) const;
    [[nodiscard]] MathLib::Tree<MathLib::String> BuildAST(const MathLib::Sequence<char>& str, const MathLib::Sequence<char>& variable) const;
    [[nodiscard]] size_t GetDependenceCount(const MathLib::Sequence<char>& variable, bool checkVariables) const;
    [[nodiscard]] BNF Simplify(const MathLib::Sequence<char>& variable) const;
    [[nodiscard]] BNFValue Substitute(const MathLib::Sequence<char>& variable) const;
    [[nodiscard]] size_t GetDataSize(const MathLib::Sequence<char>& variable, size_t variableSize) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override;
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override;

    private:
    MathLib::Array<BNFVariable> variables;
};

#endif