#include <FileSystem/FileSystem.hpp>
#include <Libc/HostSocket.hpp>
#include <TLS/TLS.hpp>
#include <Logger.hpp>

void Main(int, char**, MathLib::FileSystem&) {
    const MathLib::String host = "tls-v1-2.badssl.com";
    MathLib::HostSocket socket = true;
    if (!socket.ConnectToHost(host, 1012)) MathLib::Panic("Failed to connect to website");
    MathLib::TLS tls = socket;
    if (!tls.PerformHandshake(host)) MathLib::Panic("Failed to perform TLS handshake");
    if (!tls.Puts("GET / HTTP/1.1\r\nHost: "_M + host + "\r\nConnection: close\r\n\r\n")) MathLib::Panic("Failed to send GET request");
    LogString(tls.ReadUntil('\0'));
    LogChar('\n');
}