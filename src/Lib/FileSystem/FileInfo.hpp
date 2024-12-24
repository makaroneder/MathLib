#ifndef MathLib_FileSystem_FileInfo_H
#define MathLib_FileSystem_FileInfo_H
#include "../Interfaces/Printable.hpp"

namespace MathLib {
    struct FileInfo : Printable {
        enum class Type {
            Unknown,
            File,
            Directory,
        } type;
        static constexpr const char* typeStr[] = {
            "Unknown",
            "File",
            "Directory",
        };
        String path;

        FileInfo(Type type = Type::Unknown, const String& path = "");
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        virtual String ToString(const String& padding = "") const override;
    };
}

#endif