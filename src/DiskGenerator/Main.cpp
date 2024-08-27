#include <MathLib.hpp>
#include <iostream>

/// @brief Converts string to number with size postfixes
/// @param size String to convert
/// @return Number
size_t ParseSize(const String& size) {
    switch (size.At(size.GetSize() - 1)) {
        case 'B': return StringToNumber(SubString(size, 0, size.GetSize() - 1));
        case 'K': return StringToNumber(SubString(size, 0, size.GetSize() - 1)) * 1024;
        case 'M': return StringToNumber(SubString(size, 0, size.GetSize() - 1)) * Pow(1024, 2);
        case 'G': return StringToNumber(SubString(size, 0, size.GetSize() - 1)) * Pow(1024, 3);
        default: return StringToNumber(size);
    }
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        #ifdef Debug
        const Test test = TestSelf();
        const size_t tests = test.GetRecordCount();
        const size_t passed = test.GetPassed();
        std::cout << test << passed << "/" << tests << " tests passed" << std::endl;
        if (passed != tests) Panic("Some tests failed");
        #endif
        const size_t sectorSize = 512;
        const CommandLine cmdLine = CommandLine(argc, (const char**)argv);
        size_t diskSize = ParseSize(cmdLine.GetEntry("diskSize").Get("Disk size not specified"));
        if (diskSize % sectorSize != 0) diskSize += sectorSize - diskSize % sectorSize;
        diskSize /= sectorSize;
        HostFileSystem fs;
        File file = fs.Open(cmdLine.GetEntry("output").Get("Output file not specified"), OpenMode::Write);
        uint8_t emptySector[sectorSize] = { 0, };
        const String diskType = cmdLine.GetEntry("diskType").Get("Disk type not specified");
        if (diskType == "MBR") {
            MBRHeader mbr;
            mbr.entries[0] = MBRPartitionEntry(Interval<uint32_t>(1, diskSize--), MBRPartitionEntry::ID::NonFileSystemData);
            if (!mbr.IsValid()) Panic("Generated MBR is not valid");
            if (!file.Write<MBRHeader>(mbr)) Panic("Failed to write MBR to the disk");
        }
        else if (diskType != "None") Panic("Invalid disk type provided");
        for (size_t i = 0; i < diskSize; i++)
            if (!file.WriteBuffer(emptySector, sectorSize)) Panic("Failed to write data to the disk");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}