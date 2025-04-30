#ifndef MathLib_Emulator_MOS6502_H
#define MathLib_Emulator_MOS6502_H
#include "../../Interfaces/Function.hpp"
#include "../StepEmulator.hpp"
#include "MOS6502Flags.hpp"

namespace MathLib {
    struct MOS6502 : StepEmulator, Printable {
        MOS6502(const Sequence<uint8_t>& memory);
        virtual void Reset(void) override;
        [[nodiscard]] virtual bool Step(void) override;
        [[nodiscard]] virtual Register GetPC(void) const override;
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        [[nodiscard]] bool OrA(uint16_t addr);

        template <typename T>
        [[nodiscard]] Expected<T> Fetch(void) {
            StartBenchmark
            const Expected<T> ret = ReadPositioned<T>(pc.value);
            if (ret.HasValue()) pc.value += sizeof(T);
            ReturnFromBenchmark(ret);
        }

        MOS6502Flags flags;
        Register pc;
        Register sp;
        Register a;
        Register x;
        Register y;
    };
}

#endif