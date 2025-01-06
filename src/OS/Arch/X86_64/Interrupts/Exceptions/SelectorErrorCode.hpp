#ifdef __x86_64__
#ifndef SelectorErrorCode_H
#define SelectorErrorCode_H
#include <Typedefs.hpp>

struct SelectorErrorCode {
    enum class Table {
        GDT = 0,
        IDT1,
        LDT,
        IDT2,
    };
    bool external : 1;
    uint8_t table : 2;
    uint16_t index : 13;

    [[nodiscard]] MathLib::String ToString(const MathLib::String& padding = "") const;
} __attribute__((packed));

#endif
#endif