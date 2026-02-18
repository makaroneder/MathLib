#ifndef MathLib_Emulator_AML_H
#define MathLib_Emulator_AML_H
#include "AMLOpcode.hpp"
#include "AMLObject.hpp"
#include "../Emulator.hpp"
#include "../../ACPI/DSDT.hpp"

namespace MathLib {
    struct AML : Emulator {
        AML(const Sequence<uint8_t>& memory);
        AML(const DSDT* dsdt);
        [[nodiscard]] virtual bool Reset(void) override;
        [[nodiscard]] virtual bool Run(void) override;
        [[nodiscard]] AMLObject GetRoot(void) const;

        private:
        [[nodiscard]] bool Run(AMLObject& object);
        [[nodiscard]] Expected<AMLOpcode> ParseOpcode(void);
        [[nodiscard]] Expected<uint32_t> ParsePackageLength(void);
        [[nodiscard]] Expected<String> ParseNameSegment(void);
        [[nodiscard]] Expected<String> ParseString(void);
        [[nodiscard]] Array<uint64_t> ParseComputetionalData(void);
        template <typename T>
        [[nodiscard]] Expected<T> Fetch(void) {
            const Expected<T> ret = ReadPositioned<T>(pc);
            if (ret.HasValue()) pc += sizeof(T);
            return ret;
        }

        size_t pc;
        AMLObject root;
    };
}

#endif