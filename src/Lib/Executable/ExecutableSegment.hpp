#ifndef MathLib_Executable_Segment_H
#define MathLib_Executable_Segment_H
#include "../Interfaces/Printable.hpp"

namespace MathLib {
    struct ExecutableSegment : Printable {
        Array<uint8_t> data;
        uint64_t address;

        ExecutableSegment(void);
        ExecutableSegment(uint64_t address, const Array<uint8_t>& data);
        ExecutableSegment(uint64_t start, uint64_t end);
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
    };
}

#endif