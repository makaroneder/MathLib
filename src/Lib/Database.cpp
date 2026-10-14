#include "Database.hpp"
#include "Interfaces/Sequence/ByteArray.hpp"

namespace MathLib {
    bool Database::DefaultOverwrite(const Sequence<char>& path, const void* buffer, size_t size, size_t position) {
        ByteArray data = Read(path);
        return data.WritePositionedBuffer(buffer, size, position) && CreateRecord(path, data);
    }
}