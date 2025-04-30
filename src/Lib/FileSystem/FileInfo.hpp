#ifndef MathLib_FileSystem_FileInfo_H
#define MathLib_FileSystem_FileInfo_H
#include "../Interfaces/Printable.hpp"

namespace MathLib {
    struct FileInfo : Printable {
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
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] bool operator==(const FileInfo& other) const;
        [[nodiscard]] bool operator!=(const FileInfo& other) const;
    };
}

#endif