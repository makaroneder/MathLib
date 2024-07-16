#ifndef Socket_Shared_H
#define Socket_Shared_H
#include <Typedefs.hpp>

struct Socket : Allocatable {
    Socket(void);
    Socket(int handle_);
    ~Socket(void);
    bool Bind(int port);
    Socket GetConnection(void);
    ssize_t Write(String str);
    ssize_t Read(String& str) const;
    int GetHandle(void) const;

    private:
    int handle;
};

#endif