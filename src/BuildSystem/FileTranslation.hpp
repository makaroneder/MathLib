#ifndef FileTranslation_H
#define FileTranslation_H
#include <Typedefs.hpp>

struct FileTranslation {
    MathLib::String inputFormat;
    MathLib::String outputFormat;
    MathLib::String sources;

    FileTranslation(const MathLib::String& inputFormat, const MathLib::String& outputFormat, const MathLib::String& sources);
};


#endif