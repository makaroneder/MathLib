#ifndef Socket_H
#define Socket_H
#include "Readable.hpp"
#include "Writeable.hpp"

struct Socket : Readable, Writeable {
    virtual bool Bind(int port) = 0;
    virtual Socket* GetConnection(void) = 0;
};

#endif