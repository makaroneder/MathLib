#include "stdio.h"
#include "exit.h"
#include "string.h"
#include "stdlib.h"
#include "SystemCall.h"
#include <SystemCalls.h>
#include <ReadWriteSystemCall.h>
#include <OpenSystemCall.h>

FILE* stdout = NULL;
FILE* stdin = NULL;
FILE* stderr = NULL;

FILE* fopen(const char* path, const char* mode) {
    OpenSystemCall data;
    data.path = path;
    data.create = mode[0] != 'r';
    const uint64_t index = PerformSystemCall(OpenFileSystemCall, &data);
    if (index == UINT64_MAX) return NULL;
    FILE* ret = (FILE*)malloc(sizeof(FILE));
    if (!ret) return NULL;
    ret->index = index;
    ret->position = 0;
    if (mode[0] == 'a') (void)fseek(ret, 0, SEEK_END);
    return ret;
}
int fclose(FILE* file) {
    if (!file) return 1;
    const bool ret = !PerformSystemCall(CloseFileSystemCall, &file->index);
    file->index = UINT64_MAX;
    return ret;
}
int fflush(FILE*) {
    return 0;
}
int fseek(FILE* file, long offset, int mode) {
    if (!file) return -1;
    const size_t size = PerformSystemCall(GetSizeOfFileSystemCall, &file->index);
    switch (mode) {
        case SEEK_SET: {
            if (offset < 0 || (uint64_t)offset > size) return -1;
            file->position = offset;
            return file->position;
        }
        case SEEK_CUR: {
            if (file->position + offset > size || file->position < (uint64_t)-offset) return -1;
            file->position += offset;
            return file->position;
        }
        case SEEK_END: {
            if (offset < 0 || size < (uint64_t)offset) return -1;
            file->position = size - offset;
            return file->position;
        }
        default: return -1;
    }
}
void rewind(FILE* file) {
    (void)fseek(file, 0, SEEK_SET);
}
long ftell(FILE* file) {
    return file ? file->position : 0;
}
size_t fread(void* buff, size_t elementSize, size_t count, FILE* file) {
    ReadWriteSystemCall data;
    data.file = file->index;
    data.address = (uint64_t)buff;
    data.size = elementSize * count;
    data.position = file->position;
    const size_t ret = PerformSystemCall(ReadFileSystemCall, &data);
    file->position += ret;
    return ret;
}
size_t fwrite(const void* buff, size_t elementSize, size_t count, FILE* file) {
    ReadWriteSystemCall data;
    data.file = file->index;
    data.address = (uint64_t)buff;
    data.size = elementSize * count;
    data.position = file->position;
    const size_t ret = PerformSystemCall(WriteFileSystemCall, &data);
    file->position += ret;
    return ret;
}
int fputc(int chr, FILE* file) {
    char buff[] = { (char)chr, };
    return fwrite(buff, sizeof(char), 1, file);
}
int fputs(const char* str, FILE* file) {
    return fwrite(str, sizeof(char), strlen(str), file);
}
int putc(int chr, FILE* file) {
    return fputc(chr, file);
}
int putchar(int chr) {
    return fputc(chr, stdout);
}
int puts(const char* str) {
    return fputs(str, stdout);
}

int remove(const char* path) {
    // TODO:
    (void)path;
    abort();
    return 0;
}
int rename(const char* oldName, const char* newName) {
    // TODO:
    (void)oldName;
    (void)newName;
    abort();
    return 0;
}
void setbuf(FILE* file, char*) {
    // TODO:
    (void)file;
    abort();
}

int printf(const char* str, ...) {
    va_list args;
    va_start(args, str);
    const int ret = vfprintf(stdout, str, args);
    va_end(args);
    return ret;
}
int fprintf(FILE* file, const char* str, ...) {
    va_list args;
    va_start(args, str);
    const int ret = vfprintf(file, str, args);
    va_end(args);
    return ret;
}
int sprintf(char* buff, const char* str, ...) {
    va_list args;
    va_start(args, str);
    const int ret = vsprintf(buff, str, args);
    va_end(args);
    return ret;
}
int snprintf(char* buff, size_t size, const char* str, ...) {
    va_list args;
    va_start(args, str);
    const int ret = vsnprintf(buff, size, str, args);
    va_end(args);
    return ret;
}
int vprintf(const char* str, va_list args) {
    return vfprintf(stdout, str, args);
}
int vfprintf(FILE* file, const char* str, va_list args) {
    // TODO:
    (void)file;
    (void)str;
    (void)args;
    abort();
    return 0;
}
int vsprintf(char* buff, const char* str, va_list args) {
    return vsnprintf(buff, SIZE_MAX, str, args);
}
int vsnprintf(char* buff, size_t size, const char* str, va_list args) {
    // TODO:
    (void)buff;
    (void)size;
    (void)str;
    (void)args;
    abort();
    return 0;
}

int scanf(const char* str, ...) {
    va_list args;
    va_start(args, str);
    const int ret = vfscanf(stdin, str, args);
    va_end(args);
    return ret;
}
int fscanf(FILE* file, const char* str, ...) {
    va_list args;
    va_start(args, str);
    const int ret = vfscanf(file, str, args);
    va_end(args);
    return ret;
}
int sscanf(const char* input, const char* str, ...) {
    va_list args;
    va_start(args, str);
    const int ret = vsscanf(input, str, args);
    va_end(args);
    return ret;
}
int vscanf(const char* str, va_list args) {
    return vfscanf(stdin, str, args);
}
int vsscanf(const char* input, const char* str, va_list args) {
    // TODO:
    (void)input;
    (void)str;
    (void)args;
    abort();
    return 0;
}
int vfscanf(FILE* file, const char* str, va_list args) {
    // TODO:
    (void)file;
    (void)str;
    (void)args;
    abort();
    return 0;
}