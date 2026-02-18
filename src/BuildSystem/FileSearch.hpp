#ifndef FileSearch_H
#define FileSearch_H
#include <Typedefs.hpp>

struct FileSearch {
    MathLib::String directory;
    MathLib::String extension;

    FileSearch(const MathLib::Sequence<char>& directory, const MathLib::Sequence<char>& extension);
    [[nodiscard]] bool operator==(const FileSearch& other) const;
    [[nodiscard]] bool operator!=(const FileSearch& other) const;
};

#endif