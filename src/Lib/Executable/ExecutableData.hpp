#ifndef MathLib_Executable_Data_H
#define MathLib_Executable_Data_H
#include "ExecutableSegment.hpp"

namespace MathLib {
    struct ExecutableData : ExecutableSegment {
        uint64_t entry;

        ExecutableData(void);
        ExecutableData(uint64_t address, const Array<uint8_t>& data, uint64_t entry);
        ExecutableData(uint64_t start, uint64_t end, uint64_t entry);
        [[nodiscard]] bool LoadSegment(const ExecutableSegment& segment);
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
    };
}

#endif