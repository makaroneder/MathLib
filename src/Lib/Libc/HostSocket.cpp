#ifndef Freestanding
#if __has_include("arpa/inet.h") && __has_include("unistd.h") && __has_include("netdb.h")
#include "HostSocket.hpp"
#include "../Host.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

namespace MathLib {
    HostSocket::HostSocket(void) : HostSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {
        EmptyBenchmark
    }
    HostSocket::HostSocket(int handle) : handle(handle) {
        StartBenchmark
        if (handle == -1) Panic("Invalid socket handle provided");
        EndBenchmark
    }
    HostSocket::~HostSocket(void) {
        StartBenchmark
        if (close(handle)) Panic("Failed to close socket");
        EndBenchmark
    }
    size_t HostSocket::ReadSizedBuffer(void* buffer, size_t size) {
        StartBenchmark
        const ssize_t tmp = recv(handle, buffer, size, 0);
        ReturnFromBenchmark(tmp < 0 ? 0 : tmp);
    }
    size_t HostSocket::WriteSizedBuffer(const void* buffer, size_t size) {
        StartBenchmark
        const ssize_t tmp = send(handle, buffer, size, 0);
        ReturnFromBenchmark(tmp < 0 ? 0 : tmp);
    }
    bool HostSocket::Skip(size_t size) {
        StartAndReturnFromBenchmark(DefaultSkip(size));
    }
    bool HostSocket::Bind(size_t port) {
        StartBenchmark
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(port);
        ReturnFromBenchmark(!bind(handle, (const sockaddr*)&addr, sizeof(sockaddr_in)));
    }
    Socket* HostSocket::GetConnection(void) {
        StartBenchmark
        if (listen(handle, 1) != 0) ReturnFromBenchmark(nullptr);
        sockaddr_in addr;
        socklen_t size = sizeof(sockaddr_in);
        ReturnFromBenchmark(new HostSocket(accept(handle, (sockaddr*)&addr, &size)));
    }
    bool HostSocket::Connect(const IPv4& ip, size_t port) {
        StartBenchmark
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        ReturnFromBenchmark(inet_pton(addr.sin_family, ip.ToString().GetValue(), &addr.sin_addr) == 1 && !connect(handle, (sockaddr*)&addr, sizeof(sockaddr_in)));
    }
    int HostSocket::GetHandle(void) const {
        StartAndReturnFromBenchmark(handle);
    }
}

#endif
#endif