#ifndef FileSearch_H
#define FileSearch_H
#include <Typedefs.hpp>

struct FileSearch {
    MathLib::String directory;
    MathLib::String extension;

    FileSearch(const MathLib::String& directory, const MathLib::String& extension);
};

#endif