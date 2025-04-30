#include "Database.hpp"
#include "ClientRequest.hpp"
#include <Libc/HostFileSystem.hpp>
#include <Libc/HostSocket.hpp>
#include <CommandLine.hpp>
#include <JSON.hpp>
#include <iostream>

MathLib::String ReadLine(void) {
    std::cout << "> ";
    std::string ret = "";
    getline(std::cin, ret);
    return MathLib::String(ret.c_str());
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
        const MathLib::String type = cmdLine.GetEntry("type"_M).Get("No type specified");
        MathLib::HostFileSystem fs;
        MathLib::HostSocket socket = MathLib::HostSocket();
        const MathLib::String port = cmdLine.GetEntry("port"_M).Get("No port specified");
        if (type == "client") {
            MathLib::JSON target;
            if (!target.LoadFromPath(fs, cmdLine.GetEntry("target"_M).Get("No target file specified"))) MathLib::Panic("Failed to parse target file");
            const MathLib::IPv4 ip = MathLib::IPv4(target.Find("ip"_M).Get("No IP provided").GetValue());
            std::cout << "Connecting to " << ip << ':' << port << std::endl;
            if (!socket.Connect(ip, MathLib::StringToNumber(port))) MathLib::Panic("Failed to connect to server");
            while (true) {
                const MathLib::String command = ReadLine();
                if (command == "exit") {
                    if (!socket.Write<ClientRequest>(ClientRequest::Quit)) MathLib::Panic("Failed to send quit request");
                    break;
                }
                else if (command == "create") {
                    if (!socket.Write<ClientRequest>(ClientRequest::Create)) MathLib::Panic("Failed to send create request");
                    size_t id;
                    if (!socket.Read<size_t>(id)) MathLib::Panic("Failed to get data ID");
                    std::cout << "ID: " << id << std::endl;
                }
                else if (command == "read") {
                    if (!socket.Write<ClientRequest>(ClientRequest::Read)) MathLib::Panic("Failed to send read request");
                    if (!socket.Write<size_t>(MathLib::StringToNumber(ReadLine()))) MathLib::Panic("Failed to send read request ID");
                    size_t size;
                    if (!socket.Read<size_t>(size)) MathLib::Panic("Failed to get read request data size");
                    MathLib::File file = fs.Open(ReadLine(), MathLib::OpenMode::Write);
                    for (size_t i = 0; i < size; i++) {
                        uint8_t tmp;
                        if (!socket.Read<uint8_t>(tmp) || !file.Write<uint8_t>(tmp)) MathLib::Panic("Failed to get read request data");
                    }
                }
                else if (command == "write") {
                    if (!socket.Write<ClientRequest>(ClientRequest::Write)) MathLib::Panic("Failed to send write request");
                    if (!socket.Write<size_t>(MathLib::StringToNumber(ReadLine()))) MathLib::Panic("Failed to send write request ID");
                    MathLib::File file = fs.Open(ReadLine(), MathLib::OpenMode::Read);
                    size_t size = file.GetSize();
                    if (!socket.Read<size_t>(size)) MathLib::Panic("Failed to send write request data size");
                    for (size_t i = 0; i < size; i++) {
                        uint8_t tmp;
                        if (!file.Read<uint8_t>(tmp) || !socket.Write<uint8_t>(tmp)) MathLib::Panic("Failed to send write request data");
                    }
                }
                else if (command == "extract") {
                    if (!socket.Write<ClientRequest>(ClientRequest::Extract)) MathLib::Panic("Failed to send extract request");
                    if (!socket.Write<size_t>(MathLib::StringToNumber(ReadLine()))) MathLib::Panic("Failed to send extract request ID");
                    if (!socket.Puts(ReadLine()) || !socket.Write<char>('\0')) MathLib::Panic("Failed to send extract request path");
                }
                else if (command == "destroy") {
                    if (!socket.Write<ClientRequest>(ClientRequest::Destroy)) MathLib::Panic("Failed to send destroy request");
                }
                else std::cout << "Unknown request specified" << std::endl;
            }
        }
        else if (type == "server") {
            if (!socket.Bind(MathLib::StringToNumber(port))) MathLib::Panic("Failed to bind socket to local address");
            std::cout << "Waiting for a client to connect on port " << port << std::endl;
            MathLib::Socket* client = socket.GetConnection();
            if (!client) MathLib::Panic("Failed to get client connection");
            Database database;
            const MathLib::String path = cmdLine.GetEntry("database"_M).Get("No database file specified");
            if (!database.LoadFromPath(fs, path)) std::cout << "New database created" << std::endl;
            while (true) {
                ClientRequest request;
                if (!client->Read<ClientRequest>(request)) MathLib::Panic("Failed to get client request");
                if (request == ClientRequest::Quit) break;
                else if (request == ClientRequest::Create) {
                    std::cout << "Recieved create request" << std::endl;
                    if (!database.entries.Add(DatabaseEntry())) MathLib::Panic("Failed to create new database entry");
                    const size_t id = database.entries.GetSize() - 1;
                    std::cout << "Create request ID: " << id << std::endl;
                    if (!client->Write<size_t>(id)) MathLib::Panic("Failed to send database entry ID");
                }
                else if (request == ClientRequest::Read) {
                    std::cout << "Recieved read request" << std::endl;
                    size_t id;
                    if (!client->Read<size_t>(id)) MathLib::Panic("Failed to get read request ID");
                    std::cout << "Read request ID: " << id << std::endl;
                    if (id >= database.entries.GetSize()) MathLib::Panic("Invalid ID provided");
                    if (!database.entries.At(id).Save(*client)) MathLib::Panic("Failed to send read request data");
                }
                else if (request == ClientRequest::Write) {
                    std::cout << "Recieved write request" << std::endl;
                    size_t id;
                    if (!client->Read<size_t>(id)) MathLib::Panic("Failed to get write request ID");
                    std::cout << "Read request ID: " << id << std::endl;
                    if (id >= database.entries.GetSize()) MathLib::Panic("Invalid ID provided");
                    if (!database.entries.At(id).Load(*client)) MathLib::Panic("Failed to load write request data");
                }
                else if (request == ClientRequest::Extract) {
                    std::cout << "Recieved extract request" << std::endl;
                    size_t id;
                    if (!client->Read<size_t>(id)) MathLib::Panic("Failed to get extract request ID");
                    std::cout << "Extract request ID: " << id << std::endl;
                    if (id >= database.entries.GetSize()) MathLib::Panic("Invalid ID provided");
                    const MathLib::String path = client->ReadUntil('\0');
                    std::cout << "Extract request path: " << path << std::endl;
                    MathLib::File file = fs.Open(path, MathLib::OpenMode::Write);
                    if (!file.WriteBuffer(database.entries.At(id).data.GetValue(), database.entries.At(id).data.GetSize())) MathLib::Panic("Failed to extract specified file");
                }
                else if (request == ClientRequest::Destroy) {
                    std::cout << "Recieved destroy request" << std::endl;
                    database = Database();
                }
                else MathLib::Panic("Unknown client request");
                std::cout << "Request handled" << std::endl;
            }
            if (!database.SaveFromPath(fs, path)) MathLib::Panic("Failed to save database");
            delete client;
        }
        else MathLib::Panic("Invalid type specified");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}