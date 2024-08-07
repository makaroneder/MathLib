#ifndef String_H
#define String_H
#include "Typedefs.hpp"

String Erase(String str, size_t pos, size_t len);
size_t Find(String str, String delim);
String SubString(String str, size_t pos, size_t len);
Array<String> Split(String str, String delim);
String ToString(size_t x, size_t base);

#endif