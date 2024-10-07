#include "ISO9660DirectoryEntry.hpp"

bool ISO9660DirectoryEntry::IsValid(void) const {
    return length && date.IsValid();
}
String ISO9660DirectoryEntry::GetName(void) const {
    String ret;
    for (uint8_t i = 0; i < nameLength && name[i] != ';'; i++) ret += name[i];
    return ret;
}