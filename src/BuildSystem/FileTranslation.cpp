#include "FileTranslation.hpp"

FileTranslation::FileTranslation(const MathLib::Sequence<char>& inputFormat, const MathLib::Sequence<char>& outputFormat, const MathLib::Sequence<char>& sources) : inputFormat(MathLib::CollectionToString(inputFormat)), outputFormat(MathLib::CollectionToString(outputFormat)), sources(MathLib::CollectionToString(sources)) {}