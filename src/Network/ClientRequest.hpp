#ifndef ClientRequest_H
#define ClientRequest_H
#include <stdint.h>

enum class ClientRequest : uint8_t {
    Quit,
    Create,
    Read,
    Write,
    Destroy,
    Extract,
};

#endif