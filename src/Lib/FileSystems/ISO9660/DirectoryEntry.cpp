#include "DirectoryEntry.hpp"

String DirectoryEntry::GetName(void) const {
    String ret;
    for (uint8_t i = 0; i < nameLength; i++) {
        if (name[i] == ';') break;
        ret += name[i];
    }
    return ret;
}