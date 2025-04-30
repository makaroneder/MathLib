#ifndef Database_H
#define Database_H
#include "DatabaseEntry.hpp"

struct Database : MathLib::Saveable {
    MathLib::Array<DatabaseEntry> entries;

    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override;
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override;
};

#endif