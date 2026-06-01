#ifndef GitCommit_H
#define GitCommit_H
#include <Interfaces/Dictionary.hpp>
#include <Interfaces/Saveable.hpp>

struct GitCommit : MathLib::Saveable, MathLib::Printable {
    static constexpr const char* commitStart = "commit ";
    MathLib::String treeHash;

    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override;
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
};

#endif