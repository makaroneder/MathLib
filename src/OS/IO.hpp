#ifndef IO_H
#define IO_H
#include <stdint.h>

uint8_t ReadPort8(uint16_t port);
void WritePort8(uint16_t port, uint8_t val);

#endif