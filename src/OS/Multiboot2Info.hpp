#ifndef Multiboot2Info_H
#define Multiboot2Info_H
#include <stdint.h>

enum class Multiboot2TagType : uint32_t {
    FinalTag = 0,
    CMDLine,
    BootloaderName,
    Module,
    BasicMemoryInfo,
    BIOSBootDevice,
    MemoryMap,
    VBE,
    Framebuffer,
    ELFSections,
    APM,
    EFISystemTable32,
    EFISystemTable64,
    SMBIOS,
    OldACPI,
    NewACPI,
    Network,
    EFIMemoryMap,
    EFIBootServicesNotTerminated,
    EFIImageHandle32,
    EFIImageHandle64,
    BaseLoadAddr,
    End,
};
static constexpr const char* multiboot2TagNames[(uint32_t)Multiboot2TagType::End] = {
    "Final tag",
    "Command line",
    "Bootloader name",
    "Module",
    "Memory info",
    "BIOS boot device",
    "Memory map",
    "VBE",
    "Framebuffer",
    "ELF sections",
    "APM",
    "EFI system table 32",
    "EFI system table 64",
    "SMBIOS",
    "ACPI 1.0",
    "ACPI 2.0",
    "Network",
    "EFI memory map",
    "EFI boot services not terminated",
    "EFI image handle 32",
    "EFI image handle 64",
    "Base load address",
};

struct Multiboot2Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};
struct Multiboot2MemoryMapEntry {
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t zero;
};
struct Multiboot2Tag {
    uint32_t type;
    uint32_t size;
};
struct Multiboot2Info {
    uint32_t size;
    uint32_t resv;
    Multiboot2Tag tags[0];
};
struct Multiboot2TagString : Multiboot2Tag {
    char str[0];
};
struct Multiboot2TagModule : Multiboot2Tag {
    uint32_t start;
    uint32_t end;
    char path[0];
};
struct Multiboot2TagMemoryInfo : Multiboot2Tag {
    uint32_t lowerMem;
    uint32_t upperMem;
};
struct Multiboot2TagBIOSBootDevice : Multiboot2Tag {
    uint32_t id;
    uint32_t partition;
    uint32_t subPartition;
};
struct Multiboot2TagMemoryMap : Multiboot2Tag {
    uint32_t entrySize;
    uint32_t entryVersion;
    Multiboot2MemoryMapEntry entries[0];  
};
struct Multiboot2VBEInfo {
    char signature[4];
    uint16_t version;
    uint16_t oemStr[2];
    uint8_t capabilities[4];
    uint16_t videoModePtr[2];
    uint16_t memory;
    uint8_t resv[492];
} __attribute__((packed));
struct Multiboot2VBEModeInfo {
    uint16_t attr;
	uint8_t windowA;
	uint8_t windowB;
	uint16_t granularity;
	uint16_t windowSize;
	uint16_t segA;
	uint16_t segB;
	uint32_t winFunc;
	uint16_t pitch;
	uint16_t width;
	uint16_t height;
	uint8_t wChr;
	uint8_t yChr;
	uint8_t planes;
	uint8_t bpp;
	uint8_t banks;
	uint8_t memoryModel;
	uint8_t bankSize;
	uint8_t imagePages;
	uint8_t resv1;
	uint8_t redMask;
	uint8_t redPos;
	uint8_t greenMask;
	uint8_t greenPos;
	uint8_t blueMask;
	uint8_t bluePos;
	uint8_t resvMask;
	uint8_t resvPos;
	uint8_t directColorAttr;
	uint32_t framebuffer;
	uint32_t offScreenMemoryOff;
	uint32_t offScreenMemorySize;
	uint8_t resv0[206];
} __attribute__((packed));
struct Multiboot2TagVBE : Multiboot2Tag {
    uint16_t vbeMode;
    uint16_t vbeInterfaceSeg;
    uint16_t vbeInterfaceOff;
    uint16_t vbeInterfaceLen;
    Multiboot2VBEInfo vbeInfo;
    Multiboot2VBEModeInfo vbeModeInfo;
};
struct Multiboot2TagFramebuffer : Multiboot2Tag {
    uint64_t addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t bpp;
    uint8_t type;
    uint16_t resv;
    union {
        struct {
            uint16_t paletteColors;
            Multiboot2Color palette[0];
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
struct Multiboot2TagELF : Multiboot2Tag {
    uint32_t num;
    uint32_t entrySize;
    uint32_t shndx;
    char sections[0];
};
struct Multiboot2TagAPM : Multiboot2Tag {
    uint16_t version;
    uint16_t cseg;
    uint32_t offset;
    uint16_t cseg16;
    uint16_t dseg;
    uint16_t flags;
    uint16_t csegLen;
    uint16_t cseg16Len;
    uint16_t dsegLen;
};
template<typename T>
struct Multiboot2TagEFI : Multiboot2Tag {
    T ptr;
};
struct Multiboot2TagSMBIOS : Multiboot2Tag {
    uint8_t major;
    uint8_t minor;
    uint8_t resv[6];
    uint8_t tables[0];
};
struct Multiboot2TagACPI : Multiboot2Tag {
    uint8_t rsdp[0];
};
struct Multiboot2TagNetwork : Multiboot2Tag {
    uint8_t dhcPack[0];
};
struct Multiboot2TagEFIMemoryMap : Multiboot2Tag {
    uint32_t descrSize;
    uint32_t descrVers;
    uint8_t map[0];
};
struct Multiboot2TagLoadBaseAddr : Multiboot2Tag {
    uint32_t addr;
};

#endif