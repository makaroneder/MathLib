#include "Database.hpp"
#include <Libc/HostFileSystem.hpp>
#include <CommandLine.hpp>
#include <String.hpp>
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
        MathLib::HostFileSystem fs;
        const MathLib::String path = cmdLine.GetEntry("program"_M).Get("No database specified");
        Database database;
        if (!database.LoadFromPath(fs, path) || !database.IsValid()) database = Database();
        while (true) {
            const MathLib::Array<MathLib::String> split = MathLib::Split(ReadLine(), " "_M, false);
            if (split.At(0) == "exit") break;
            else if (split.At(0) == "help") {
                std::cout << "exit: closes the program" << std::endl;
                std::cout << "help: prints this message" << std::endl;
                std::cout << "read: prints database" << std::endl;
                std::cout << "read A: prints database entry A" << std::endl;
                std::cout << "read A B: prints database entry A field B" << std::endl;
                std::cout << "write: save database" << std::endl;
                std::cout << "write A: writes data to database entry A" << std::endl;
                std::cout << "write A B: writes data to database entry A field B" << std::endl;
            }
            else if (split.At(0) == "read") {
                if (split.GetSize() > 1) {
                    const MathLib::Expected<DatabaseEntry> entry = database.GetEntry(split.At(1));
                    if (!entry.HasValue()) std::cout << "Entry not found" << std::endl;
                    else if (split.GetSize() > 2) {
                        const MathLib::Expected<DatabaseEntryField> field = entry.Get().GetField(split.At(2));
                        std::cout << (field.HasValue() ? field.Get().ToString() : "Field not found") << std::endl;
                    }
                    else std::cout << entry.Get() << std::endl;
                }
                else std::cout << "Database: " << database << std::endl;
            }
            else if (split.At(0) == "write") {
                if (split.GetSize() > 1) {
                    const MathLib::Expected<DatabaseEntry> entry = database.GetEntry(split.At(1));
                    DatabaseEntry tmpEntry = entry.HasValue() ? entry.Get() : DatabaseEntry(split.At(1));
                    if (split.GetSize() > 2) {
                        if (!tmpEntry.UpdateField(DatabaseEntryField(split.At(2), ReadLine()))) MathLib::Panic("Failed to update field");
                    }
                    else while (true) {
                        MathLib::String name = ReadLine();
                        size_t i = name.Find(' ');
                        const MathLib::String tmp = i == SIZE_MAX ? name : MathLib::SubString(name, 0, i);
                        if (tmp == "exit") break;
                        else if (!tmpEntry.UpdateField(DatabaseEntryField(tmp, MathLib::SubString(name, i + 1, name.GetSize() - i - 1)))) MathLib::Panic("Failed to update field");
                    }
                    if (!database.UpdateEntry(tmpEntry)) MathLib::Panic("Failed to update entry");
                }
                else if (!database.SaveFromPath(fs, path)) MathLib::Panic("Failed to save database");
            }
            else std::cout << "Command '" << split.At(0) << "' not found" << std::endl;
        }
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}