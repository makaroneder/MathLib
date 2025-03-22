#ifndef MathLib_Emulator_MathArch_H
#define MathLib_Emulator_MathArch_H
#include "MathArchOpcode.hpp"
#include "../StepEmulator.hpp"

namespace MathLib {
    struct MathArch : StepEmulator, Printable {
        MathArch(const Sequence<uint8_t>& memory);
        [[nodiscard]] virtual bool Step(void) override;
        [[nodiscard]] virtual Register GetPC(void) const override;
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] size_t EncodeOneByteInstruction(size_t i, MathArchOpcode opcode);
        [[nodiscard]] size_t EncodeConstInstruction(size_t i, MathArchOpcode opcode, uint64_t value);

        private:
        template <typename T>
        [[nodiscard]] Expected<T> Fetch(void) {
            StartBenchmark
            const Expected<T> ret = ReadPositioned<T>(pc.value);
            if (ret.HasValue()) pc.value += sizeof(T);
            ReturnFromBenchmark(ret);
        }
        template <typename T>
        [[nodiscard]] bool Push(T value) {
            StartBenchmark
            sp.value -= sizeof(T);
            ReturnFromBenchmark(WritePositioned<T>(value, sp.value))
        }
        template <typename T>
        [[nodiscard]] Expected<T> Pop(void) {
            StartBenchmark
            const Expected<T> ret = ReadPositioned<T>(sp.value);
            if (ret.HasValue()) sp.value += sizeof(T);
            ReturnFromBenchmark(ret);
        }

        Register pc;
        Register sp;
        Register a;
    };
}

#endif