#include "TestCase.hpp"
#include <FileSystem/Directory.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Tests/Test.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        // TODO: Change Cipher* to Cipher&
        // TODO: CFB1 CFB8 CFB128
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <test directory>");
        MathLib::HostFileSystem fs;
        MathLib::Directory directory = MathLib::Directory(fs, MathLib::String(argv[1]));
        const MathLib::Array<MathLib::FileInfo> testCases = directory.ReadDirectory(""_M);
        MathLib::Test test;
        size_t unsupportedTests = 0;
        for (const MathLib::FileInfo& testCase : testCases) {
            MathLib::File file = directory.Open(testCase.path, MathLib::OpenMode::Read);
            const MathLib::String str = file.ReadUntil('\0');
            const TestCase tmp = TestCase(str);
            if (!tmp.IsValid()) {
                unsupportedTests++;
                std::cout << "Unsupported test: " << testCase.path << std::endl;
                continue;
            }
            const size_t size = tmp.GetTests();
            for (size_t i = 0; i < size; i++)
                if (!test.ReportRecord(MathLib::Record(testCase.path + ':' + MathLib::ToString(i, 10), tmp.IsOkay(i)))) MathLib::Panic("Failed to report test record");
        }
        std::cout << test << std::endl;
        std::cout << "Unsupported tests: " << unsupportedTests << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}