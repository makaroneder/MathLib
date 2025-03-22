#include <Interfaces/ByteDeviceSequence.hpp>
#include <Libc/HostFileSystem.hpp>
#include <Graph/Tree.hpp>
#include <String.hpp>
#include <iostream>

MathLib::Tree<size_t> GenerateHeaderTreeInternal(MathLib::FileSystem& fileSystem, const MathLib::Sequence<char>& basePath, const MathLib::Sequence<char>& path, MathLib::Array<MathLib::Tree<size_t>>& visited) {
    MathLib::Tree<size_t> ret = MathLib::Tree<size_t>(path, 0);
    for (const MathLib::Tree<size_t>& x : visited)
        if (x.name == path) return x;
    MathLib::File file = fileSystem.Open(path, MathLib::OpenMode::Read);
    const MathLib::ByteDeviceSequence<char> sequence = MathLib::ByteDeviceSequence<char>(file);
    const size_t size = file.GetSize();
    size_t i = 0;
    while (i < size) {
        i = sequence.Find("#include"_M, i);
        if (i == SIZE_MAX) continue;
        i += 8;
        MathLib::SkipWhiteSpace(sequence, i);
        char end;
        switch (sequence.At(i++)) {
            case '"': {
                end = '"';
                break;
            }
            case '<': {
                end = '>';
                break;
            }
            default: return MathLib::Tree<size_t>();
        }
        const size_t endIndex = sequence.Find(end, i);
        if (endIndex == SIZE_MAX) return MathLib::Tree<size_t>();
        const MathLib::Array<MathLib::String> pathSplit = MathLib::Split(MathLib::SubString(sequence, i, endIndex - i), '/'_M, false);
        MathLib::Array<MathLib::String> basePathSplit = MathLib::Split(basePath, '/'_M, false);
        i = endIndex + 1;
        for (const MathLib::Sequence<char>& str : pathSplit) {
            if (str == "."_M) continue;
            else if (str == ".."_M) {
                MathLib::Array<MathLib::String> tmp = MathLib::Array<MathLib::String>(basePathSplit.GetSize() - 1);
                for (size_t i = 0; i < tmp.GetSize(); i++) tmp.At(i) = basePathSplit.At(i);
                basePathSplit = tmp;
            }
            else if (!basePathSplit.Add(MathLib::CollectionToString(str))) return MathLib::Tree<size_t>();
        }
        MathLib::String bp = "";
        MathLib::String p = "";
        for (size_t i = 0; i < basePathSplit.GetSize(); i++) {
            if (i + 1 == basePathSplit.GetSize()) p = basePathSplit.At(i);
            else bp += basePathSplit.At(i) + '/';
        }
        if (!ret.Add(end == '>' ? MathLib::Tree<size_t>(p, 0) : GenerateHeaderTreeInternal(fileSystem, MathLib::SubString(bp, 0, bp.GetSize() - 1), bp + p, visited))) return MathLib::Tree<size_t>();
    }
    const size_t tmp = ret.children.GetSize();
    ret.data = tmp;
    for (size_t i = 0; i < tmp; i++)
        ret.data += ret.children.At(i).data;
    return visited.Add(ret) ? ret : MathLib::Tree<size_t>();
}
MathLib::Tree<size_t> GenerateHeaderTree(MathLib::FileSystem& fileSystem, const MathLib::Sequence<char>& basePath, const MathLib::Sequence<char>& path) {
    MathLib::Array<MathLib::Tree<size_t>> visited;
    return GenerateHeaderTreeInternal(fileSystem, basePath, path, visited);
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic("Usage: "_M + argv[0] + " <file>");
        MathLib::HostFileSystem fs;
        MathLib::Array<MathLib::String> split = MathLib::Split(MathLib::String(argv[1]), '/'_M, false);
        MathLib::String basePath = "";
        for (size_t i = 0; i < split.GetSize() - 1; i++) basePath += split.At(i) + '/';
        const MathLib::num_t tmp = MathLib::GetTime();
        std::cout << GenerateHeaderTree(fs, MathLib::SubString(basePath, 0, basePath.GetSize() - 1), MathLib::String(argv[1])) << std::endl;
        std::cout << "Time: " << MathLib::GetTime() - tmp << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}