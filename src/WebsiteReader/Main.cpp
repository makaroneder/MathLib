#include "URI.hpp"
#include "HTTPS.hpp"
#include <FileSystem/FileSystem.hpp>
#include <Libc/HostSocket.hpp>
#include <TLS/TLS.hpp>
#include <Logger.hpp>
#include <Defer.hpp>

void Main(int argc, char** argv, MathLib::FileSystem&) {
    if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <url>");
    const URI uri = MathLib::String(argv[1]);
    const Protocol* const protocols[] = {
        new HTTP(), new HTTPS(),
    };
    defer(for (const Protocol* const& protocol : protocols) delete protocol);
    size_t protocol = SIZE_MAX;
    for (size_t i = 0; i < SizeOfArray(protocols); i++) {
        if (protocols[i]->GetScheme() != uri.scheme) continue;
        protocol = i;
        break;
    }
    if (protocol == SIZE_MAX) MathLib::Panic("Unknown protocol specified");
    MathLib::HostSocket socket = true;
    if (!socket.ConnectToHost(uri.host, uri.port == SIZE_MAX ? protocols[protocol]->GetDefaultPort() : uri.port)) MathLib::Panic("Failed to connect to website");
    LogString(protocols[protocol]->Read(socket, uri.host, uri.path));
}