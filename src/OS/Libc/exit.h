#ifndef Exit_H
#define Exit_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void abort(void);
void exit(int status);

#ifdef __cplusplus
}
#endif
#endif