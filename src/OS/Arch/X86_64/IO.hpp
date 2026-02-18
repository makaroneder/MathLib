#ifdef __x86_64__
#ifndef IO_H
#define IO_H
#include <stdint.h>

template <typename T>
void WritePort(uint16_t port, T value);
template <typename T>
[[nodiscard]] T ReadPort(uint16_t port);
void IOWait(void);

#endif
#endif