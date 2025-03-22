#ifndef BNF_H
#define BNF_H
#include "BNFVariable.hpp"
#include <Interfaces/Saveable.hpp>

struct BNF : MathLib::Printable, MathLib::Saveable {
    MathLib::Array<BNFVariable> variables;

    BNF(void);
    BNF(const MathLib::Array<BNFVariable>& variables);
    [[nodiscard]] bool Match(const MathLib::Sequence<char>& str, const MathLib::Sequence<char>& variable) const;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override;
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override;
};

#endif