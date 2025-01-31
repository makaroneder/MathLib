#include "DatabaseEntryField.hpp"

DatabaseEntryField::DatabaseEntryField(void) {}
DatabaseEntryField::DatabaseEntryField(const MathLib::String& name, const MathLib::String& value) : name(name), value(value) {}
MathLib::String DatabaseEntryField::GetName(void) const {
    return name;
}
bool DatabaseEntryField::Save(MathLib::Writable& file) const {
    return file.Puts(name) && file.Write<char>('\0') && file.Puts(value) && file.Write<char>('\0');
}
bool DatabaseEntryField::Load(MathLib::Readable& file) {
    name = file.ReadUntil('\0');
    value = file.ReadUntil('\0');
    return true;
}
MathLib::String DatabaseEntryField::ToString(const MathLib::String& padding) const {
    return padding + name + ": " + value;
}