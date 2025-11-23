#ifdef __x86_64__
#include "PageEntry.hpp"

PageEntry::PageEntry(void) : present(false), writable(false), user(false), writeThrough(false), disableCache(false), accessed(false), dirty(false), pageAttributeTable(false), global(false), osSpecific1(0), address(0), osSpecific2(0), protectionKey(0), noExecute(false) {}
PageEntry::PageEntry(uintptr_t address, bool writable, bool executable) : present(true), writable(writable), user(true), writeThrough(false), disableCache(false), accessed(false), dirty(false), pageAttributeTable(false), global(false), osSpecific1(0), address(address >> 12), osSpecific2(0), protectionKey(0), noExecute(!executable) {}

#endif