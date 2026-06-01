#ifndef MathLib_Executable_ELF_Symbol_H
#define MathLib_Executable_ELF_Symbol_H
#include "../../Interfaces/Formatter.hpp"

namespace MathLib {
    struct ELFSymbol {
        enum class Type : uint8_t {
            None = 0x00,
            Data,
            Code,
            Section,
            FileName,
            Common,
            ThreadLocal,
            OSSpecificStart = 0x0a,
            GNUIndirectCode = OSSpecificStart,
            OSSpecificEnd = 0x0c,
            CPUSpecificStart,
            CPUSpecificEnd = 0x0f,
        };
        enum class Bind : uint8_t {
            Local = 0x00,
            Global,
            Weak,
            OSSpecificStart = 0x0a,
            GNUUnique = OSSpecificStart,
            OSSpecificEnd = 0x0c,
            CPUSpecificStart,
            CPUSpecificEnd = 0x0f,
        };
        enum class Visibility : uint8_t {
            Default = 0x00,
            CPUSpecific,
            Hidden,
            Protected,
        };
        uint32_t name;
        uint8_t type : 4;
        uint8_t bind : 4;
        uint8_t visibility : 2;
        uint8_t reserved : 6;
        uint16_t section;
        uint64_t value;
        uint64_t size;

        [[nodiscard]] bool operator==(const ELFSymbol& other) const;
        [[nodiscard]] bool operator!=(const ELFSymbol& other) const;
    } __attribute__((packed));
    MakeFormatter(ELFSymbol, self, padding, {
        // TODO: Convert type, bind and visibility to string
        const String padd = CollectionToString(padding);
        return "{\n"_M +
            padd + "\tName: 0x" + MathLib::ToString(self.name, 16) + '\n' +
            padd + "\tType: 0x" + MathLib::ToString(self.type, 16) + '\n' +
            padd + "\tBind: 0x" + MathLib::ToString(self.bind, 16) + '\n' +
            padd + "\tVisibility: 0x" + MathLib::ToString(self.visibility, 16) + '\n' +
            padd + "\tReserved: 0x" + MathLib::ToString(self.reserved, 16) + '\n' +
            padd + "\tSection: 0x" + MathLib::ToString(self.section, 16) + '\n' +
            padd + "\tValue: 0x" + MathLib::ToString(self.value, 16) + '\n' +
            padd + "\tSize: " + MathLib::ToString(self.size, 10) + '\n' +
        padd + '}';
    });
}

#endif