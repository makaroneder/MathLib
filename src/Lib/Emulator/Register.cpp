#include "Register.hpp"
#include "../Host.hpp"
#include "../String.hpp"

namespace MathLib {
    Register::Register(uint64_t value) : value(value) {
        EmptyBenchmark
    }
    uint32_t Register::Get32(bool upper) const {
        StartBenchmark
        ReturnFromBenchmark(value >> (upper ? 32 : 0));
    }
    uint16_t Register::Get16(bool upper) const {
        StartBenchmark
        ReturnFromBenchmark(Get32(false) >> (upper ? 16 : 0));
    }
    uint8_t Register::Get8(bool upper) const {
        StartBenchmark
        ReturnFromBenchmark(Get16(false) >> (upper ? 8 : 0));
    }
    uint8_t Register::Get4(bool upper) const {
        StartBenchmark
        ReturnFromBenchmark(Get8(false) & (0b1111 << (upper ? 4 : 0)));
    }
    void Register::Set32(uint32_t val, bool upper) {
        StartBenchmark
        const uint8_t shift = upper ? 32 : 0;
        value = (value & ~((uint64_t)UINT32_MAX << shift)) | (uint64_t)val << shift;
        EndBenchmark
    }
    void Register::Set16(uint16_t val, bool upper) {
        StartBenchmark
        const uint8_t shift = upper ? 16 : 0;
        value = (value & ~((uint64_t)UINT16_MAX << shift)) | ((uint64_t)val << shift);
        EndBenchmark
    }
    void Register::Set8(uint8_t val, bool upper) {
        StartBenchmark
        const uint8_t shift = upper ? 8 : 0;
        value = (value & ~((uint64_t)UINT8_MAX << shift)) | ((uint64_t)val << shift);
        EndBenchmark
    }
    void Register::Clear4(bool upper) {
        StartBenchmark
        Set8(Get8(false) & (0b1111 << (upper ? 0 : 4)), false);
        EndBenchmark
    }
    String Register::ToString(const String& padding) const {
        StartBenchmark
        ReturnFromBenchmark(padding + "0x" + MathLib::ToString(value, 16));
    }
}