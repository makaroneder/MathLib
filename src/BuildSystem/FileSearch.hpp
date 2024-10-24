#ifndef FileSearch_H
#define FileSearch_H
#include <Typedefs.hpp>

struct FileSearch {
    String directory;
    String extension;

    FileSearch(const String& directory, const String& extension);
};

#endif