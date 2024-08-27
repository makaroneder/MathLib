#ifndef Multiboot1Info_H
#define Multiboot1Info_H
#include <stdint.h>

enum class Multiboot1InfoFlags : uint8_t {
    MemoryInfo = 0,
    BootDevice,
    CommandLine,
    Modules,
    Aout,
    ELF,
    MemoryMap,
    DriveInfo,
    ConfigTable,
    BootloaderName,
    APM,
    VBE,
    Framebuffer,
};
struct Multiboot1AoutSymbolTable {
    uint32_t tabSize;
    uint32_t strSize;
    uint32_t address;
    uint32_t reserved;
};
struct Multiboot1ELFSectionHeaderTable {
    uint32_t num;
    uint32_t size;
    uint32_t address;
    uint32_t shndx;
};
struct Multiboot1Info {
    enum class FramebufferType : uint8_t {
        Indexed = 0,
        RGB,
        EGA,
    };
    uint32_t flags;
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
    uint64_t framebufferAddress;
    uint32_t framebufferPitch;
    uint32_t framebufferWidth;
    uint32_t framebufferHeight;
    uint8_t framebufferBitsPerPixel;
    FramebufferType framebufferType;
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

#endif