#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include <MathLib.hpp>
#include <iostream>

MathLib::Array<HTTPHeader> resources = std::vector<HTTPHeader> {
    HTTPHeader("/", "<h1>Amongus</h1>"),
};
[[nodiscard]] HTTPHeader* GetResource(MathLib::String name) {
    for (HTTPHeader& resource : resources)
        if (resource.name == name) return &resource;
    return nullptr;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic(MathLib::String("Usage: ") + argv[0] + " <port>");
        MathLib::HostSocket server = MathLib::HostSocket();
        if (!server.Bind(atoi(argv[1]))) MathLib::Panic("Failed to bind socket to local address");
        std::cout << "Waiting for a client to connect on http://localhost:" << argv[1] << std::endl;
        MathLib::Socket* client = server.GetConnection();
        if (!client) MathLib::Panic("Failed to get client connection");
        while (true) {
            char buff[1025] = { '\0', };
            if (!client->ReadSizedBuffer(buff, 1024)) {
                delete client;
                MathLib::Panic("Failed to read data from client");
            }
            MathLib::String msg = MathLib::String(buff);
            HTTPRequest request = HTTPRequest(msg);
            std::cout << request << std::endl;
            HTTPResponse response;
            if (request.method == "GET") {
                HTTPHeader* resource = GetResource(request.target);
                response = resource ? HTTPResponse::FromHTML(resource->value) : HTTPResponse::FromStatus(HTTPStatus::NotFound, request.target + " does not exist");
            }
            else if (request.method == "HEAD") {
                HTTPHeader* resource = GetResource(request.target);
                if (resource != nullptr) {
                    response = HTTPResponse::FromHTML(resource->value);
                    response.body = "";
                }
                else response = HTTPResponse::FromStatus(HTTPStatus::NotFound, request.target + " does not exist");
            }
            else if (request.method == "OPTIONS") {
                response = HTTPResponse::FromStatus(HTTPStatus::NoContent, "OK");
                if (!response.headers.Add(HTTPHeader("Allow", "OPTIONS, GET, HEAD"))) MathLib::Panic("Failed to add resonse header");
            }
            else response = HTTPResponse::FromStatus(HTTPStatus::NotImplemented, MathLib::String("Unknown request: ") + request.method);
            std::cout << response << std::flush;
            if (!client->Puts(response.GetRaw())) {
                delete client;
                MathLib::Panic("Failed to write data to client");
            }
        }
        delete client;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}