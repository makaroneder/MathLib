#ifndef MathLib_Emulator_MOS6502_H
#define MathLib_Emulator_MOS6502_H
#include "../../Interfaces/Function.hpp"
#include "../StepEmulator.hpp"
#include "MOS6502Flags.hpp"

namespace MathLib {
    struct MOS6502 : StepEmulator, Printable {
        MOS6502(const Sequence<uint8_t>& memory);
        [[nodiscard]] virtual bool Reset(void) override;
        [[nodiscard]] virtual bool Step(void) override;
        [[nodiscard]] virtual Register GetPC(void) const override;
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        [[nodiscard]] MathLib::Expected<uint16_t> GetInstructionOperandAddress(uint8_t addressingMode);

        template <typename T>
        [[nodiscard]] Expected<T> Fetch(void) {
            const Expected<T> ret = ReadPositioned<T>(pc);
            if (ret.HasValue()) pc += sizeof(T);
            return ret;
        }

        uint16_t pc;
        uint8_t sp;
        uint8_t a;
        uint8_t x;
        uint8_t y;
        MOS6502Flags flags;
    };
}

#endif