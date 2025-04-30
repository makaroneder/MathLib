#include "Database.hpp"

bool Database::Save(MathLib::Writable& file) const {
    if (!file.Write<size_t>(entries.GetSize())) return false;
    for (const DatabaseEntry& entry : entries)
        if (!entry.Save(file)) return false;
    return true;
}
bool Database::Load(MathLib::Readable& file) {
    size_t size;
    if (!file.Read<size_t>(size)) return false;
    entries = MathLib::Array<DatabaseEntry>(size);
    for (DatabaseEntry& entry : entries)
        if (!entry.Load(file)) return false;
    return true;
}