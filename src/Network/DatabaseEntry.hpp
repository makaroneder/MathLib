#ifndef DatabaseEntry_H
#define DatabaseEntry_H
#include <Interfaces/Saveable.hpp>

struct DatabaseEntry : MathLib::Saveable {
    MathLib::Array<uint8_t> data;

    DatabaseEntry(void);
    DatabaseEntry(size_t size);
    [[nodiscard]] virtual bool Save(MathLib::Writable& file) const override;
    [[nodiscard]] virtual bool Load(MathLib::Readable& file) override;
    [[nodiscard]] bool operator==(const DatabaseEntry& other) const;
    [[nodiscard]] bool operator!=(const DatabaseEntry& other) const;
};

#endif