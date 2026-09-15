#include <Libc/HostFileSystem.hpp>
#include <Libc/HostLogger.hpp>
#include <Logger.hpp>
#include <iostream>

void Main(int argc, char** argv, MathLib::FileSystem& fs);
int main(int argc, char** argv) {
    try {
        MathLib::logger = new MathLib::HostLogger(std::cout);
        if (!MathLib::logger) MathLib::Panic("Failed to create logger");
        MathLib::HostFileSystem fs;
        Main(argc, argv, fs);
        delete MathLib::logger;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& exception) {
        if (MathLib::logger) delete MathLib::logger;
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }
}