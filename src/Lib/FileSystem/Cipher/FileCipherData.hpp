#ifndef MathLib_FileSystem_Cipher_FileData_H
#define MathLib_FileSystem_Cipher_FileData_H
#include "../../Typedefs.hpp"

namespace MathLib {
    struct FileCipherData {
        enum class Type : uint8_t {
            Invalid,
            Normal,
            Skip,
        };
        String name;
        size_t size;
        Type type;

        FileCipherData(void);
        FileCipherData(Type type, const Sequence<char>& name, size_t size);
    };
}

#endif