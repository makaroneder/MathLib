#include "TLS/TLS.hpp"
#include <FileSystem/FileSystem.hpp>
#include <Libc/HostSocket.hpp>
#include <iostream>

void Main(int, char**, MathLib::FileSystem&) {
    const MathLib::String host = "tls-v1-2.badssl.com";
    MathLib::HostSocket socket = true;
    if (!socket.ConnectToHost(host, 1012)) MathLib::Panic("Failed to connect to website");
    // if (!socket.Puts("GET / HTTP/1.1\r\nHost: "_M + host + "\r\nConnection: close\r\n\r\n")) MathLib::Panic("Failed to send GET request");
    // std::cout << socket.ReadUntil('\0') << std::endl;
    TLS tls = socket;
    if (!tls.PerformHandshake(host)) MathLib::Panic("Failed to perform TLS handshake");
}