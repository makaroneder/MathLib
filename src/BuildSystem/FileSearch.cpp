#include "FileSearch.hpp"

FileSearch::FileSearch(const MathLib::Sequence<char>& directory, const MathLib::Sequence<char>& extension) : directory(MathLib::CollectionToString(directory)), extension(MathLib::CollectionToString(extension)) {}