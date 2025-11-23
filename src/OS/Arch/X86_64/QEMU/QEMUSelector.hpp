#ifdef __x86_64__
#ifndef QEMUSelector_H
#define QEMUSelector_H
#include <stdint.h>

enum class QEMUSelector : uint16_t {
    Signature = 0x0000,
    ID,
    Directory = 0x0019,
};

#endif
#endif