#ifndef MathLib_Emulator_X86_H
#define MathLib_Emulator_X86_H
#include "../StepEmulator.hpp"
#include "X86Opcode.hpp"
#include "X86State.hpp"

namespace MathLib {
    struct X86 : StepEmulator, Printable {
        X86(const Sequence<uint8_t>& memory);
        [[nodiscard]] virtual bool Reset(void) override;
        [[nodiscard]] virtual bool Step(void) override;
        [[nodiscard]] virtual Register GetPC(void) const override;
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        X86State state;

        void Interrupt(uint8_t index);
        void UpdateFlags(uint64_t val, uint64_t a, uint64_t b);
        [[nodiscard]] uint64_t ToLinear(uint64_t segment, uint64_t offset);
        template <typename T>
        [[nodiscard]] Expected<T> Fetch(void) {
            const Expected<T> ret = ReadPositioned<T>(ToLinear(state.cs.value, state.ip.value));
            if (ret.HasValue()) state.ip.value += sizeof(T);
            return ret;
        }
        template <typename T>
        [[nodiscard]] bool Push(T value) {
            state.sp.value -= sizeof(T);
            return WritePositioned<T>(value, ToLinear(state.ss.value, state.sp.value));
        }
        template <typename T>
        [[nodiscard]] Expected<T> Pop(void) {
            const Expected<T> ret = ReadPositioned<T>(ToLinear(state.ss.value, state.sp.value));
            if (ret.HasValue()) state.sp.value += sizeof(T);
            return ret;
        }
        [[nodiscard]] Register* GetRegister(uint8_t code);
    };
}

#endif