#include "DirectoryEntry.hpp"

bool DirectoryEntry::IsValid(void) const {
    return length && date.IsValid();
}
String DirectoryEntry::GetName(void) const {
    String ret;
    for (uint8_t i = 0; i < nameLength && name[i] != ';'; i++) ret += name[i];
    return ret;
}