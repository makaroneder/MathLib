#include <MathLib.hpp>
#include <iostream>

/// @brief Converts string to number with size postfixes
/// @param size String to convert
/// @return Number
[[nodiscard]] size_t ParseSize(const MathLib::String& size) {
    switch (size.At(size.GetSize() - 1)) {
        case 'B': return MathLib::StringToNumber(MathLib::SubString(size, 0, size.GetSize() - 1));
        case 'K': return MathLib::StringToNumber(MathLib::SubString(size, 0, size.GetSize() - 1)) * 1024;
        case 'M': return MathLib::StringToNumber(MathLib::SubString(size, 0, size.GetSize() - 1)) * 1024 * 1024;
        case 'G': return MathLib::StringToNumber(MathLib::SubString(size, 0, size.GetSize() - 1)) * 1024 * 1024 * 1024;
        default: return MathLib::StringToNumber(size);
    }
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        const size_t sectorSize = 512;
        const MathLib::CommandLine cmdLine = MathLib::CommandLine(argc, (const char**)argv);
        size_t diskSize = ParseSize(cmdLine.GetEntry("diskSize").Get("Disk size not specified"));
        if (diskSize % sectorSize != 0) diskSize += sectorSize - diskSize % sectorSize;
        diskSize /= sectorSize;
        MathLib::HostFileSystem fs;
        MathLib::File file = fs.Open(cmdLine.GetEntry("output").Get("Output file not specified"), MathLib::OpenMode::Write);
        uint8_t emptySector[sectorSize] = { 0, };
        const MathLib::String diskType = cmdLine.GetEntry("diskType").Get("Disk type not specified");
        if (diskType == "MBR") {
            MathLib::MBRHeader mbr;
            mbr.entries[0] = MathLib::MBRPartitionEntry(MathLib::Interval<uint32_t>(1, diskSize--), MathLib::MBRPartitionEntry::ID::NonFileSystemData);
            if (!mbr.IsValid()) MathLib::Panic("Generated MBR is not valid");
            if (!file.Write<MathLib::MBRHeader>(mbr)) MathLib::Panic("Failed to write MBR to the disk");
        }
        else if (diskType != "None") MathLib::Panic("Invalid disk type provided");
        // TODO: Create MathFS on disk
        for (size_t i = 0; i < diskSize; i++)
            if (!file.WriteBuffer(emptySector, sectorSize)) MathLib::Panic("Failed to write data to the disk");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}