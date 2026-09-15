#include "Task.hpp"
#include "SystemCallFileSystem.hpp"
#include <ModuleEntryData.hpp>
#include <Logger.hpp>

void Main(int argc, char** argv, MathLib::FileSystem& fs);
extern "C" void _start(const ModuleEntryData* data) {
    SystemCallFileSystem fs;
    MathLib::logger = new MathLib::File(fs, data->outputFile);
    if (!MathLib::logger) MathLib::Panic("Failed to create logger");
    // TODO: argc, argv
    Main(0, nullptr, fs);
    delete MathLib::logger;
    Exit();
}