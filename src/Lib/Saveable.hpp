#ifndef Saveable_H
#define Saveable_H
#include "File.hpp"

/// @brief Interface for saveable structures
struct Saveable : Allocatable {
    /// @brief Saves data
    /// @param file File to save data into
    /// @return Status
    virtual bool Save(ByteDevice& file) const = 0;
    /// @brief Loads data
    /// @param file File to load data from
    /// @return Status
    virtual bool Load(ByteDevice& file) = 0;
    /// @brief Saves data
    /// @param fileSystem File system to save data into
    /// @param path Path to the file
    /// @return Status
    bool SaveFromPath(FileSystem& fileSystem, const String& path) const;
    /// @brief Loads data
    /// @param fileSystem File system to load data from
    /// @param path Path to the file
    /// @return Status
    bool LoadFromPath(FileSystem& fileSystem, const String& path);
};

#endif