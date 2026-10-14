#ifndef GitCommit_H
#define GitCommit_H
#include <Interfaces/Sequence/ByteArray.hpp>
#include <Interfaces/Dictionary.hpp>
#include <Interfaces/Saveable.hpp>

struct GitCommit : MathLib::Saveable, MathLib::Printable {
    static constexpr const char* commitStart = "commit ";
    MathLib::String treeHash;

    [[nodiscard]] MathLib::String GetHash(void) const;
    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override;
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override;
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;

    private:
    [[nodiscard]] MathLib::ByteArray SaveInternal(void) const;
};

#endif