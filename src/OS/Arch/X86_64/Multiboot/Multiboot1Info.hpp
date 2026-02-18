#ifdef __x86_64__
#ifndef Multiboot1Info_H
#define Multiboot1Info_H
#include "Multiboot.hpp"

struct Multiboot1AoutSymbolTable {
    uint32_t tabSize;
    uint32_t strSize;
    uint32_t address;
    uint32_t reserved;
};
struct Multiboot1ELFSectionHeaderTable {
    uint32_t count;
    uint32_t size;
    uint32_t address;
    uint32_t sectionIndex;
};
struct Multiboot1Info {
    bool hasMemoryInfo : 1;
    bool hasBootDevice : 1;
    bool hasCommandLine : 1;
    bool hasModules : 1;
    bool hasAoutInfo : 1;
    bool hasELFInfo : 1;
    bool hasMemoryMap : 1;
    bool hasDriveInfo : 1;
    bool hasConfigTable : 1;
    bool hasBootloaderName : 1;
    bool hasAPM : 1;
    bool hasVBE : 1;
    bool hasFramebuffer : 1;
    uint32_t reserved : 19;
    uint32_t lowerMem;
    uint32_t upperMem;
    uint32_t bootDevice;
    uint32_t commandLine;
    uint32_t moduleCount;
    uint32_t moduleAddress;
    union {
        Multiboot1AoutSymbolTable aout;
        Multiboot1ELFSectionHeaderTable elf;
    };
    uint32_t memoryMapLength;
    uint32_t memoryMapAddress;
    uint32_t drivesLength;
    uint32_t drivesAddress;
    uint32_t configTable;
    uint32_t bootloaderName;
    uint32_t apm;
    uint32_t vbeControlInfo;
    uint32_t vbeModeInfo;
    uint16_t vbeMode;
    uint16_t vbeInterfaceSegment;
    uint16_t vbeInterfaceOffset;
    uint16_t vbeInterfaceLength;
    MultibootFramebuffer framebuffer;
    union {
        struct {
            uint32_t paletteAddress;
            uint16_t paletteColors;
        };
        struct {
            uint8_t redFieldPos;
            uint8_t redMaskSize;
            uint8_t greenFieldPos;
            uint8_t greenMaskSize;
            uint8_t blueFieldPos;
            uint8_t blueMaskSize;
        };
    };
};
struct Multiboot1MemoryMapEntry {
    uint32_t size;
    uint64_t address;
    uint64_t length;
    MultibootMemoryMapEntryType type;
} __attribute__((packed));
struct Multiboot1Module {
    uint32_t start;
    uint32_t end;
    uint32_t path;
    uint32_t reserved;
};

#endif
#endif