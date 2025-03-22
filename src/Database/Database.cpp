#include "Database.hpp"

Database::Database(void) : signature(expectedSignature) {}
bool Database::IsValid(void) const {
    return signature == expectedSignature;
}
MathLib::Expected<DatabaseEntry> Database::GetEntry(const MathLib::Sequence<char>& name) const {
    for (const DatabaseEntry& entry : entries)
        if (entry.GetName() == name) return MathLib::Expected<DatabaseEntry>(entry);
    return MathLib::Expected<DatabaseEntry>();
}
bool Database::UpdateEntry(const DatabaseEntry& entry) {
    for (DatabaseEntry& tmp : entries) {
        if (tmp.GetName() == entry.GetName()) {
            tmp = entry;
            return true;
        }
    }
    return entries.Add(entry);
}
bool Database::Save(MathLib::Writable& file) const {
    if (!file.Write<uint8_t>(signature) || !file.Write<size_t>(entries.GetSize())) return false;
    for (const DatabaseEntry& entry : entries)
        if (!entry.Save(file)) return false;
    return true;
}
bool Database::Load(MathLib::Readable& file) {
    size_t size;
    if (!file.Read<uint8_t>(signature) || !file.Read<size_t>(size)) return false;
    entries = MathLib::Array<DatabaseEntry>(size);
    for (DatabaseEntry& entry : entries)
        if (!entry.Load(file)) return false;
    return true;
}
MathLib::String Database::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding) + "{\n";
    for (const DatabaseEntry& entry : entries) ret += entry.ToString(MathLib::CollectionToString(padding) + '\t') + '\n';
    return ret + padding + '}';
}