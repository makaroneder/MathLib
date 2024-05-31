#ifndef Saveable_H
#define Saveable_H
#include "FileSystem.hpp"

/// @brief Interface for saveable structures
struct Saveable {
    /// @brief Virtual destructor
    virtual ~Saveable(void);
    /// @brief Saves data
    /// @param fileSystem File system to save data into
    /// @param file File to save data into
    /// @return Status
    virtual bool Save(FileSystem& fileSystem, size_t file) const = 0;
    /// @brief Loads data
    /// @param fileSystem File system to load data from
    /// @param file File to load data from
    /// @return Status
    virtual bool Load(FileSystem& fileSystem, size_t file) = 0;
    /// @brief Saves data
    /// @param fileSystem File system to save data into
    /// @param path Path to the file
    /// @return Status
    bool SaveFromPath(FileSystem& fileSystem, String path) const;
    /// @brief Loads data
    /// @param fileSystem File system to load data from
    /// @param path Path to the file
    /// @return Status
    bool LoadFromPath(FileSystem& fileSystem, String path);
};

#endif