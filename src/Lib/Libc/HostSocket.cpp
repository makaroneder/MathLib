#ifndef Freestanding
#if __has_include("arpa/inet.h") && __has_include("unistd.h") && __has_include("netdb.h")
#include "HostSocket.hpp"
#include "../Host.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

namespace MathLib {
    HostSocket::HostSocket(bool tcp) : handle(socket(AF_INET, SOCK_STREAM, tcp ? IPPROTO_TCP : IPPROTO_UDP)) {
        if (handle == -1) Panic("Failed to create socket");
    }
    HostSocket::HostSocket(int handle) : handle(handle) {
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
    HostSocket* HostSocket::GetConnection(void) {
        if (listen(handle, 1) != 0) return nullptr;
        sockaddr_in addr;
        socklen_t size = sizeof(sockaddr_in);
        return new HostSocket(accept(handle, (sockaddr*)&addr, &size));
    }
    bool HostSocket::ConnectToIPv4(const MathLib::String& ip, size_t port) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        return inet_pton(addr.sin_family, ip.GetValue(), &addr.sin_addr) == 1 && !connect(handle, (sockaddr*)&addr, sizeof(sockaddr_in));
    }
    bool HostSocket::ConnectToHost(const MathLib::String& host, size_t port) {
        addrinfo hints;
        hints.ai_family = PF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = 0;
        hints.ai_protocol = hints.ai_addrlen = 0;
        hints.ai_addr = nullptr;
        hints.ai_next = nullptr;
        hints.ai_canonname = nullptr;
        addrinfo* infos;
        if (getaddrinfo(host.GetValue(), nullptr, &hints, &infos)) return false;
        port = htons(port);
        for (addrinfo* info = infos; true; info = info->ai_next) {
            sockaddr* addr = nullptr;
            size_t size = 0;
            if (info) switch (info->ai_family) {
                case AF_INET: {
                    size = sizeof(sockaddr_in);
                    addr = info->ai_addr;
                    ((sockaddr_in*)addr)->sin_port = port;
                    break;
                }
                case AF_INET6: {
                    size = sizeof(sockaddr_in6);
                    addr = info->ai_addr;
                    ((sockaddr_in6*)addr)->sin6_port = port;
                    break;
                }
                default: break;
            }
            if (!addr || connect(handle, addr, size)) continue;
            freeaddrinfo(infos);
            return info;
        }
    }
    int HostSocket::GetHandle(void) const {
        return handle;
    }
}

#endif
#endif