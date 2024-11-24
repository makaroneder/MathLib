#ifndef Emulator_MathArch_H
#define Emulator_MathArch_H
#include "MathArchOpcode.hpp"
#include "../Emulator.hpp"
#include "../Register.hpp"

namespace MathLib {
    struct MathArch : Emulator {
        MathArch(const Array<uint8_t>& mem);
        virtual bool Run(void) override;
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        virtual String ToString(const String& padding = "") const override;
        size_t EncodeOneByteInstruction(size_t i, MathArchOpcode opcode);
        size_t EncodeConstInstruction(size_t i, MathArchOpcode opcode, uint64_t value);

        private:
        bool Step(void);
        template <typename T>
        Expected<T> Fetch(void) {
            const Expected<T> ret = ReadPositioned<T>(pc.value);
            if (ret.HasValue()) pc.value += sizeof(T);
            return ret;
        }
        template <typename T>
        bool Push(T value) {
            sp.value -= sizeof(T);
            return WritePositioned<T>(value, sp.value);
        }
        template <typename T>
        Expected<T> Pop(void) {
            const Expected<T> ret = ReadPositioned<T>(sp.value);
            if (ret.HasValue()) sp.value += sizeof(T);
            return ret;
        }

        Register pc;
        Register sp;
        Register a;
    };
}

#endif