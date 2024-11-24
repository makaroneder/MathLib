#ifndef Freestanding
#if __has_include("unistd.h") && __has_include("netdb.h")
#include "HostSocket.hpp"
#include "../Host.hpp"
#include <unistd.h>
#include <netdb.h>

namespace MathLib {
    HostSocket::HostSocket(void) : HostSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {
        if (handle == -1) Panic("Failed to create socket");
    }
    HostSocket::HostSocket(int handle_) {
        handle = handle_;
    }
    HostSocket::~HostSocket(void) {
        if (close(handle)) Panic("Failed to close socket");
    }
    size_t HostSocket::ReadSizedBuffer(void* buffer, size_t size) {
        const ssize_t tmp = recv(handle, buffer, size, 0);
        return tmp < 0 ? 0 : tmp;
    }
    size_t HostSocket::WriteSizedBuffer(const void* buffer, size_t size) {
        const ssize_t tmp = send(handle, buffer, size, 0);
        return tmp < 0 ? 0 : tmp;
    }
    bool HostSocket::Bind(int port) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(port);
        return !bind(handle, (sockaddr*)&addr, sizeof(sockaddr_in));
    }
    Socket* HostSocket::GetConnection(void) {
        if (listen(handle, 1) != 0) return nullptr;
        sockaddr_in addr;
        socklen_t size = sizeof(sockaddr_in);
        return new HostSocket(accept(handle, (sockaddr*)&addr, &size));
    }
    int HostSocket::GetHandle(void) const {
        return handle;
    }
}

#endif
#endif