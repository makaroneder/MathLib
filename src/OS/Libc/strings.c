#include "strings.h"
#include "ctype.h"

int strcasecmp(const char* a, const char* b) {
    if (a == b) return 0;
    for (size_t i = 0; true; i++) {
        const int ret = tolower(a[i]) - tolower(b[i]);
        if (ret) return ret;
        if (!a[i]) return 0;
    }
}
int strncasecmp(const char* a, const char* b, size_t size) {
    if (a == b) return 0;
    for (size_t i = 0; i < size; i++) {
        const int ret = tolower(a[i]) - tolower(b[i]);
        if (ret) return ret;
        if (!a[i]) break;
    }
    return 0;
}