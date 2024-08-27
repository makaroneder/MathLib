#ifndef PS2_H
#define PS2_H
#include <stdint.h>

bool SendPS2Data(uint8_t value, bool second);
void InitPS2(void);

#endif