#ifndef FileTranslation_H
#define FileTranslation_H
#include <Typedefs.hpp>

struct FileTranslation {
    String inputFormat;
    String outputFormat;
    String sources;

    FileTranslation(const String& inputFormat, const String& outputFormat, const String& sources);
};


#endif