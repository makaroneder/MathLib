#ifndef MathLib_Emulator_AML_FieldFlags_H
#define MathLib_Emulator_AML_FieldFlags_H
#include <stdint.h>

namespace MathLib {
    union AMLFieldFlags {
        enum class AccessType {
            Any = 0,
            Byte,
            Word,
            DWord,
            QWord,
            Buffer,
        };
        enum class UpdateRule {
            Preserve = 0,
            FillWith1,
            FillWith0,
        };
        uint8_t value;
        struct {
            uint8_t accessType : 4;
            bool lock : 1;
            uint8_t updateRule : 2;
            bool reserved : 1;
        } __attribute__((packed));

        AMLFieldFlags(uint8_t value = 0);
        [[nodiscard]] bool operator==(const AMLFieldFlags& other) const;
        [[nodiscard]] bool operator!=(const AMLFieldFlags& other) const;
    } __attribute__((packed));
}

#endif