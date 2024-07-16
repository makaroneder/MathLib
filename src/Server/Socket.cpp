#include "Socket.hpp"
#include <Host.hpp>
#include <unistd.h>
#include <netdb.h>

Socket::Socket(void) : Socket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {
    if (handle == -1) Panic("Failed to create socket");
}
Socket::Socket(int handle_) {
    handle = handle_;
}
Socket::~Socket(void) {
    close(handle);
}
bool Socket::Bind(int port) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    return bind(handle, (sockaddr*)&addr, sizeof(sockaddr_in)) == 0;
}
Socket Socket::GetConnection(void) {
    if (listen(handle, 1) != 0) Panic("Failed to get connection");
    sockaddr_in addr;
    socklen_t size = sizeof(sockaddr_in);
    return Socket(accept(handle, (sockaddr*)&addr, &size));
}
ssize_t Socket::Write(String str) {
    return send(handle, str.GetValue(), str.GetSize(), 0);
}
ssize_t Socket::Read(String& str) const {
    char buff[1025] = { '\0', };
    ssize_t ret = recv(handle, buff, 1024, 0);
    str = buff;
    return ret;
}
int Socket::GetHandle(void) const {
    return handle;
}