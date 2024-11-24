#ifdef __x86_64__
#include "SelectorErrorCode.hpp"
#include <String.hpp>

MathLib::String SelectorErrorCode::ToString(const MathLib::String& padding) const {
    MathLib::String ret = padding + (external ? "External" : "Internal") + '\n' + padding;
    switch ((Table)table) {
        case Table::GDT: {
            ret += "GDT";
            break;
        }
        case Table::IDT1:
        case Table::IDT2: {
            ret += "IDT";
            break;
        }
        case Table::LDT: {
            ret += "LDT";
            break;
        }
    }
    return ret + padding + "\nIndex: 0x" + MathLib::ToString(index, 16);
}

#endif