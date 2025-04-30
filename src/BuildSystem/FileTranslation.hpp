#ifndef FileTranslation_H
#define FileTranslation_H
#include <Typedefs.hpp>

struct FileTranslation {
    MathLib::String inputFormat;
    MathLib::String outputFormat;
    MathLib::String sources;

    FileTranslation(const MathLib::Sequence<char>& inputFormat, const MathLib::Sequence<char>& outputFormat, const MathLib::Sequence<char>& sources);
    [[nodiscard]] bool operator==(const FileTranslation& other) const;
    [[nodiscard]] bool operator!=(const FileTranslation& other) const;
};


#endif