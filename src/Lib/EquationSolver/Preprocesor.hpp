#ifndef Preprocesor_H
#define Preprocesor_H
#include "../FileSystems/FileSystem.hpp"

/// @brief Reads file from given path
/// @param path Path to the file to read
/// @return Contents of the specified file
String Preproces(FileSystem& fileSystem, const String& path);

#endif