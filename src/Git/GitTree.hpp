#ifndef GitTree_H
#define GitTree_H
#include <Interfaces/Dictionary.hpp>
#include <Interfaces/Saveable.hpp>

struct GitTree : MathLib::Saveable, MathLib::Printable {
    static constexpr const char* treeStart = "tree ";
    static constexpr const char* fileMode = "100644 ";
    MathLib::Dictionary<MathLib::String, MathLib::String> elements;

    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override;
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
};

#endif