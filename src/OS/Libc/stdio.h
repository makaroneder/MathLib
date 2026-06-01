#ifndef _STDIO_H
#define _STDIO_H
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define O_TRUNC (1 << 0)
#define O_CREAT (1 << 1)
#define O_RDWR (1 << 2)
#define O_RDONLY (1 << 3)

#define S_IWUSR (1 << 0)
#define S_IRUSR (1 << 1)

typedef struct {
    uint64_t index;
    uint64_t position;
} FILE;

extern FILE* stdout;
#define stdout stdout

extern FILE* stdin;
#define stdin stdin

extern FILE* stderr;
#define stderr stderr

FILE* fopen(const char* path, const char* mode);
int fclose(FILE* file);
int fflush(FILE* file);
int fseek(FILE* file, long offset, int mode);
void rewind(FILE* file);
long ftell(FILE* file);
size_t fread(void* buff, size_t elementSize, size_t count, FILE* file);
size_t fwrite(const void* buff, size_t elementSize, size_t count, FILE* file);
int fputc(int chr, FILE* file);
int fputs(const char* str, FILE* file);
int putc(int chr, FILE* file);
int putchar(int chr);
int puts(const char* str);

int remove(const char* path);
int rename(const char* oldName, const char* newName);
void setbuf(FILE* file, char*);

int printf(const char* str, ...);
int fprintf(FILE* file, const char* str, ...);
int sprintf(char* buff, const char* str, ...);
int snprintf(char* buff, size_t size, const char* str, ...);
int vprintf(const char* str, va_list args);
int vfprintf(FILE* file, const char* str, va_list args);
int vsprintf(char* buff, const char* str, va_list args);
int vsnprintf(char* buff, size_t size, const char* str, va_list args);

int scanf(const char* str, ...);
int fscanf(FILE* file, const char* str, ...);
int sscanf(const char* input, const char* str, ...);
int vscanf(const char* str, va_list args);
int vsscanf(const char* input, const char* str, va_list args);
int vfscanf(FILE* file, const char* str, va_list args);

#ifdef __cplusplus
}
#endif
#endif