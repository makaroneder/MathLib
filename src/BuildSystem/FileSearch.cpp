#include "FileSearch.hpp"

FileSearch::FileSearch(const MathLib::Sequence<char>& directory, const MathLib::Sequence<char>& extension) : directory(MathLib::CollectionToString(directory)), extension(MathLib::CollectionToString(extension)) {}
bool FileSearch::operator==(const FileSearch& other) const {
    return directory == other.directory && extension == other.extension;
}
bool FileSearch::operator!=(const FileSearch& other) const {
    return !(*this == other);
}