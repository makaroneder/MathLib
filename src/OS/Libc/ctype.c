#include "ctype.h"

int toupper(int chr) {
    if (chr >= 'a' && chr <= 'z') return chr + 'A' - 'a';
    return chr;
}
int tolower(int chr) {
    if (chr >= 'A' && chr <= 'Z') return chr - 'A' + 'a';
    return chr;
}
int isspace(int chr) {
    return chr == ' ' || chr == '\f' || chr == '\n' || chr == '\r' || chr == '\t' || chr == '\v';
}