#ifndef MathLib_FileSystem_FileInfo_H
#define MathLib_FileSystem_FileInfo_H
#include "../Interfaces/Printable.hpp"

namespace MathLib {
    struct FileInfo;
    struct FileInfo : Comparable<FileInfo>, Printable {
        enum class Type : uint8_t {
            Unknown,
            File,
            Directory,
        };
        static constexpr const char* typeStr[] = {
            "Unknown",
            "File",
            "Directory",
        };
        String path;
        Type type;

        FileInfo(Type type = Type::Unknown, const Sequence<char>& path = ""_M);
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        protected:
        [[nodiscard]] virtual bool Equals(const FileInfo& other) const override;
    };
}

#endif