#include "MathFS.hpp"
#include "../../String.hpp"
#include "MathFSBootSector.hpp"
#include "MathFSFileHeader.hpp"

namespace MathLib {
    MathFS::MathFS(ByteDevice& disk) : PhysicalFileSystem(disk), root(0), files(Array<MathFSFile>()) {
        StartBenchmark
        const Expected<MathFSBootSector> bootSector = disk.ReadPositioned<MathFSBootSector>(0);
        if (!bootSector.HasValue()) Panic("Failed to read MathFS boot sector");
        if (bootSector.Get().IsValid()) root = bootSector.Get().root;
        EndBenchmark
    }
    bool MathFS::IsValid(void) const {
        StartBenchmark
        ReturnFromBenchmark(root);
    }
    bool MathFS::Create(void) {
        StartBenchmark
        root = 1;
        ReturnFromBenchmark(disk.WritePositioned<MathFSBootSector>(MathFSBootSector(root), 0) && disk.WritePositioned<MathFSFileHeader>(MathFSFileHeader(MathFSFileHeader::Type::Directory, root + 1, 0, ""), root * 512));
    }
    size_t MathFS::OpenInternal(const String& path, OpenMode mode) {
        StartBenchmark
        MathFSFileHeader prev = MathFSFileHeader();
        disk.ReadPositioned<MathFSFileHeader>(prev, root * 512);
        const Array<String> split = Split(path, "/", false);
        for (const String& name : split) {
            bool found = false;
            if (prev.type != MathFSFileHeader::Type::Directory) ReturnFromBenchmark(SIZE_MAX);
            for (size_t i = 0; i < prev.size && !found; i += sizeof(MathFSFileHeader)) {
                MathFSFileHeader header = MathFSFileHeader();
                disk.ReadPositioned<MathFSFileHeader>(header, prev.lba * 512 + i);
                if (String(header.name) == name) {
                    prev = header;
                    found = true;
                }
                else if (header.type == MathFSFileHeader::Type::LastEntry) break;
            }
            if (!found) {
                if (mode == OpenMode::Write) {
                    // TODO:
                }
                ReturnFromBenchmark(SIZE_MAX);
            }
        }
        ReturnFromBenchmark(files.Add(MathFSFile(prev, mode == OpenMode::Read ? false : true)) ? files.GetSize() - 1 : SIZE_MAX);
    }
    bool MathFS::Close(size_t file) {
        // TODO:
        (void)file;
        StartBenchmark
        ReturnFromBenchmark(false);
    }
    size_t MathFS::Read(size_t file, void* buffer, size_t size, size_t position) {
        // TODO:
        (void)file;
        (void)buffer;
        (void)size;
        (void)position;
        StartBenchmark
        ReturnFromBenchmark(0);
    }
    size_t MathFS::Write(size_t file, const void* buffer, size_t size, size_t position) {
        // TODO:
        (void)file;
        (void)buffer;
        (void)size;
        (void)position;
        StartBenchmark
        ReturnFromBenchmark(0);
    }
    size_t MathFS::GetSize(size_t file) {
        // TODO:
        (void)file;
        StartBenchmark
        ReturnFromBenchmark(0);
    }
    Array<FileInfo> MathFS::ReadDirectory(const String& path) {
        // TODO:
        (void)path;
        StartBenchmark
        ReturnFromBenchmark(Array<FileInfo>());
    }
}