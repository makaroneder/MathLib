#ifndef MathLib_Interfaces_Saveable_H
#define MathLib_Interfaces_Saveable_H
#include "ByteDevice.hpp"
#include "../FileSystem/FileSystem.hpp"

namespace MathLib {
    struct Saveable : Allocatable {
        [[nodiscard]] virtual bool Save(Writable& file) const = 0;
        [[nodiscard]] virtual bool Load(Readable& file) = 0;
        [[nodiscard]] bool SaveFromPath(FileSystem& fileSystem, const Sequence<char>& path) const;
        [[nodiscard]] bool LoadFromPath(FileSystem& fileSystem, const Sequence<char>& path);
        [[nodiscard]] bool SaveFromPath(FileSystem* fileSystem, const Sequence<char>& path) const;
        [[nodiscard]] bool LoadFromPath(FileSystem* fileSystem, const Sequence<char>& path);
        [[nodiscard]] bool LoadFromSequence(const Sequence<uint8_t>& sequence);
    };
}

#endif