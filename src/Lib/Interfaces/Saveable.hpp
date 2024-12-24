#ifndef MathLib_Interfaces_Saveable_H
#define MathLib_Interfaces_Saveable_H
#include "ByteDevice.hpp"
#include "../FileSystem/FileSystem.hpp"

namespace MathLib {
    /// @brief Interface for saveable structures
    struct Saveable : Allocatable {
        /// @brief Saves data
        /// @param file File to save data into
        /// @return Status
        virtual bool Save(Writeable& file) const = 0;
        /// @brief Loads data
        /// @param file File to load data from
        /// @return Status
        virtual bool Load(Readable& file) = 0;
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
}

#endif