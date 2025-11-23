#ifndef Freestanding
#if __has_include("arpa/inet.h") && __has_include("unistd.h") && __has_include("netdb.h")
#include "HostSocket.hpp"
#include "../Host.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

namespace MathLib {
    HostSocket::HostSocket(bool tcp) : handle(socket(AF_INET, SOCK_STREAM, tcp ? IPPROTO_TCP : IPPROTO_UDP)) {
        if (handle == -1) Panic("Invalid socket handle provided");
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
    bool HostSocket::Skip(size_t size) {
        return DefaultSkip(size);
    }
    bool HostSocket::Bind(size_t port) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(port);
        return !bind(handle, (const sockaddr*)&addr, sizeof(sockaddr_in));
    }
    Socket* HostSocket::GetConnection(void) {
        if (listen(handle, 1) != 0) return nullptr;
        sockaddr_in addr;
        socklen_t size = sizeof(sockaddr_in);
        return new HostSocket(accept(handle, (sockaddr*)&addr, &size));
    }
    bool HostSocket::Connect(const IPv4& ip, size_t port) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        return inet_pton(addr.sin_family, ip.ToString().GetValue(), &addr.sin_addr) == 1 && !connect(handle, (sockaddr*)&addr, sizeof(sockaddr_in));
    }
    int HostSocket::GetHandle(void) const {
        return handle;
    }
}

#endif
#endif