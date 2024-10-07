#ifndef String_H
#define String_H
#include "Typedefs.hpp"

String Erase(String str, size_t pos, size_t len);
size_t Find(String str, String delim);
String SubString(String str, size_t pos, size_t len);
Array<String> Split(String str, String delim, bool preserveDelim);
String ToString(size_t x, size_t base, size_t size = 0);
String BoolToString(bool x);

#endif