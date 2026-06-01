#include "string.h"
#include "stdlib.h"
#include <stdbool.h>

void* memcpy(void* dst, const void* src, size_t size) {
    const uint8_t* src8 = (const uint8_t*)src;
    uint8_t* dst8 = (uint8_t*)dst;
    for (size_t i = 0; i < size; i++) dst8[i] = src8[i];
    return dst;
}
void* memset(void* dst, int value, size_t size) {
    uint8_t* dst8 = (uint8_t*)dst;
    for (size_t i = 0; i < size; i++) dst8[i] = value;
    return dst;
}
void* memmove(void* dst, const void* src, size_t size) {
    uint8_t buff[size];
    return memcpy(dst, memcpy(buff, src, size), size);
}

size_t strlen(const char* str) {
    size_t ret = 0;
    while (str[ret]) ret++;
    return ret;
}
char* strcat(char* dst, const char* src) {
    const size_t len1 = strlen(dst);
    const size_t len2 = strlen(src);
    memcpy(dst + len1, src, len2 + 1);
    return dst;
}
char* strchr(const char* str, int target) {
    for (size_t i = 0; true; i++) {
        if (str[i] == target) return (char*)&str[i];
        if (!str[i]) return NULL;
    }
}
char* strrchr(const char* str, int target) {
    char* ret = NULL;
    for (size_t i = 0; true; i++) {
        if (str[i] == target) ret = (char*)&str[i];
        if (!str[i]) break;
    }
    return ret;
}
char* strstr(const char* str, const char* target) {
    const size_t len1 = strlen(str);
    const size_t len2 = strlen(target);
    for (size_t i = 0; i < len1 - len2; i++) {
        bool found = true;
        for (size_t j = 0; j < len2; j++) {
            if (str[i + j] == target[j]) continue;
            found = false;
            break;
        }
        if (found) return (char*)&str[i];
    }
    return NULL;
}
char* strcpy(char* dst, const char* src) {
    return (char*)memcpy(dst, src, strlen(src) + 1);
}
char* strncpy(char* dst, const char* src, size_t size) {
    size_t i = 0;
    for (; i < size && src[i]; i++) dst[i] = src[i];
    for (; i < size; i++) dst[i] = '\0';
    return dst;
}
int strcmp(const char* a, const char* b) {
    if (a == b) return 0;
    for (size_t i = 0; true; i++) {
        const int ret = a[i] - b[i];
        if (ret) return ret;
        if (!a[i]) return 0;
    }
}
int strncmp(const char* a, const char* b, size_t size) {
    if (a == b) return 0;
    for (size_t i = 0; i < size; i++) {
        const int ret = a[i] - b[i];
        if (ret) return ret;
        if (!a[i]) break;
    }
    return 0;
}
char* strdup(const char* str) {
    const size_t size = strlen(str) + 1;
    char* ret = (char*)malloc(size);
    if (!ret) return NULL;
    return (char*)memcpy(ret, str, size);
}
char* strndup(const char* str, size_t size) {
    size_t s = strlen(str) + 1;
    const bool add0 = size < s;
    if (add0) s = size;
    char* ret = (char*)malloc(s + add0);
    if (!ret) return NULL;
    memcpy(ret, str, s);
    if (add0) ret[s] = '\0';
    return ret;
}