#include "DatabaseEntry.hpp"

DatabaseEntry::DatabaseEntry(void) {}
DatabaseEntry::DatabaseEntry(size_t size) : data(size) {}
bool DatabaseEntry::Save(MathLib::Writable& file) const {
    const size_t size = data.GetSize();
    return file.Write<size_t>(size) && file.WriteBuffer(data.GetValue(), size);
}
bool DatabaseEntry::Load(MathLib::Readable& file) {
    size_t size;
    if (!file.Read<size_t>(size)) return false;
    data = MathLib::Array<uint8_t>(size);
    return file.ReadBuffer(data.GetValue(), size);
}
bool DatabaseEntry::operator==(const DatabaseEntry& other) const {
    return data == other.data;
}
bool DatabaseEntry::operator!=(const DatabaseEntry& other) const {
    return !(*this == other);
}