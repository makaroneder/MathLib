#include "DatabaseEntry.hpp"

DatabaseEntry::DatabaseEntry(const MathLib::Sequence<char>& name) : name(MathLib::CollectionToString(name)) {}
MathLib::String DatabaseEntry::GetName(void) const {
    return name;
}
MathLib::Expected<DatabaseEntryField> DatabaseEntry::GetField(const MathLib::Sequence<char>& name) const {
    for (const DatabaseEntryField& field : fields)
        if (field.GetName() == name) return MathLib::Expected<DatabaseEntryField>(field);
    return MathLib::Expected<DatabaseEntryField>();
}
bool DatabaseEntry::UpdateField(const DatabaseEntryField& field) {
    for (DatabaseEntryField& tmp : fields) {
        if (tmp.GetName() == field.GetName()) {
            tmp = field;
            return true;
        }
    }
    return fields.Add(field);
}
bool DatabaseEntry::Save(MathLib::Writable& file) const {
    if (!file.Puts(name) || !file.Write<char>('\0') || !file.Write<size_t>(fields.GetSize())) return false;
    for (const DatabaseEntryField& field : fields)
        if (!field.Save(file)) return false;
    return true;
}
bool DatabaseEntry::Load(MathLib::Readable& file) {
    name = file.ReadUntil('\0');
    size_t size;
    if (!file.Read<size_t>(size)) return false;
    fields = MathLib::Array<DatabaseEntryField>(size);
    for (DatabaseEntryField& field : fields)
        if (!field.Load(file)) return false;
    return true;
}
MathLib::String DatabaseEntry::ToString(const MathLib::Sequence<char>& padding) const {
    MathLib::String ret = MathLib::CollectionToString(padding) + name + ": {\n";
    for (const DatabaseEntryField& field : fields) ret += field.ToString(MathLib::CollectionToString(padding) + '\t') + '\n';
    return ret + padding + '}';
}