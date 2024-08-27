#ifndef Multiboot2Info_H
#define Multiboot2Info_H
#include <stdint.h>

struct Multiboot2Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};
struct Multiboot2MemoryMapEntry {
    enum class Type : uint32_t {
        Available = 1,
        Reserved,
        ACPIReclaimable,
        NVS,
        BadRAM,
    };
    uint64_t address;
    uint64_t length;
    Type type;
    uint32_t reserved;
};
struct Multiboot2Tag {
    enum class Type : uint32_t {
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
    static constexpr const char* typeStr[(uint32_t)Type::End] = {
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
    Type type;
    uint32_t size;
};
struct Multiboot2Info {
    uint32_t size;
    uint32_t reserved;
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
    uint32_t lowerMemory;
    uint32_t upperMemory;
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
    uint32_t oem;
    uint32_t capabilities;
    uint16_t videoModeOffset;
    uint16_t videoModeSegment;
    uint16_t memory;
    uint32_t resv[123];
} __attribute__((packed));
struct Multiboot2VBEModeInfo {
    uint16_t attributes;
    uint8_t windowA;
    uint8_t windowB;
    uint16_t granularity;
    uint16_t windowSize;
    uint16_t segmentA;
    uint16_t segmentB;
    uint32_t windowFunction;
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint8_t wChr;
    uint8_t yChr;
    uint8_t planes;
    uint8_t bitsPerPixel;
    uint8_t banks;
    uint8_t memoryModel;
    uint8_t bankSize;
    uint8_t imagePages;
    uint8_t reserved1;
    uint8_t redMask;
    uint8_t redPosition;
    uint8_t greenMask;
    uint8_t greenPosition;
    uint8_t blueMask;
    uint8_t bluePosition;
    uint8_t reservedMask;
    uint8_t reservedPosition;
    uint8_t directColorAttributes;
    uint32_t framebuffer;
    uint32_t offScreenMemoryOffset;
    uint32_t offScreenMemorySize;
    uint16_t reserved2[103];
} __attribute__((packed));
struct Multiboot2TagVBE : Multiboot2Tag {
    uint16_t mode;
    uint16_t interfaceSegment;
    uint16_t interfaceOffset;
    uint16_t interfaceLength;
    Multiboot2VBEInfo info;
    Multiboot2VBEModeInfo modeInfo;
};
struct Multiboot2TagFramebuffer : Multiboot2Tag {
    enum class Type : uint8_t {
        Indexed = 0,
        RGB,
        EGA,
    };
    uint64_t address;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t bitsPerPixel;
    Type type;
    uint16_t reserved;
    union {
        struct {
            uint16_t paletteColors;
            Multiboot2Color palette[0];
        };
        struct {
            uint8_t redFieldPosition;
            uint8_t redMaskSize;
            uint8_t greenFieldPosition;
            uint8_t greenMaskSize;
            uint8_t blueFieldPosition;
            uint8_t blueMaskSize;
        };
    };
};
struct Multiboot2TagELF : Multiboot2Tag {
    uint32_t count;
    uint32_t entrySize;
    uint32_t sectionIndex;
    char sections[0];
};
struct Multiboot2TagAPM : Multiboot2Tag {
    uint16_t version;
    uint16_t codeSegment;
    uint32_t offset;
    uint16_t codeSegment16;
    uint16_t dataSegment;
    uint16_t flags;
    uint16_t codeSegmentLength;
    uint16_t codeSegment16Length;
    uint16_t dataSegmentLength;
};
template<typename T>
struct Multiboot2TagEFI : Multiboot2Tag {
    T ptr;
};
struct Multiboot2TagSMBIOS : Multiboot2Tag {
    uint8_t majorVersion;
    uint8_t minorVersion;
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
    uint32_t descriptorSize;
    uint32_t descriptorVersion;
    uint8_t map[0];
};
struct Multiboot2TagLoadBaseAddr : Multiboot2Tag {
    uint32_t address;
};

#endif