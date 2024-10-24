#ifdef __x86_64__
#include "../../../KernelRenderer.hpp"
#include "Multiboot2.hpp"
#include <Logger.hpp>
#include <String.hpp>

RSDP* InitMultiboot2(Multiboot2Info* info, RangeMemoryManager& rangeMemoryManager) {
    RSDP* rsdp = nullptr;
    LogString("Multiboot2 signature detected\n");
    for (Multiboot2Tag* tag = info->tags; tag->type != Multiboot2Tag::Type::FinalTag; tag = (Multiboot2Tag*)((uint8_t*)tag + ((tag->size + 7) & ~7))) {
        switch (tag->type) {
            case Multiboot2Tag::Type::CMDLine: {
                const Multiboot2TagString* string = (const Multiboot2TagString*)tag;
                if (*string->str) LogString(String("Command line: ") + string->str + '\n');
                break;
            }
            case Multiboot2Tag::Type::BootloaderName: {
                LogString(String("Bootloader: ") + ((const Multiboot2TagString*)tag)->str + '\n');
                break;
            }
            case Multiboot2Tag::Type::BaseLoadAddr: {
                LogString(String("Load address: 0x") + ToString(((const Multiboot2TagLoadBaseAddr*)tag)->address, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::BasicMemoryInfo: {
                const Multiboot2TagMemoryInfo* memoryInfo = (const Multiboot2TagMemoryInfo*)tag;
                LogString(String("Lower memory size: ") + ToString(memoryInfo->lowerMemory) + "KB\n");
                LogString(String("Upper memory size: ") + ToString(memoryInfo->upperMemory) + "KB\n");
                break;
            }
            case Multiboot2Tag::Type::BIOSBootDevice: {
                const Multiboot2TagBIOSBootDevice* dev = (const Multiboot2TagBIOSBootDevice*)tag;
                LogString("BIOS boot device: {\n");
                LogString(String("\tID: 0x") + ToString(dev->id, 16) + '\n');
                LogString(String("\tPartition: 0x") + ToString(dev->partition, 16) + '\n');
                LogString(String("\tSub partition: 0x") + ToString(dev->subPartition, 16) + '\n');
                LogString("}\n");
                break;
            }
            case Multiboot2Tag::Type::OldACPI:
            case Multiboot2Tag::Type::NewACPI: {
                RSDP* tmp = (RSDP*)((const Multiboot2TagACPI*)tag)->rsdp;
                if (!rsdp || rsdp->revision < tmp->revision) rsdp = tmp;
                break;
            }
            case Multiboot2Tag::Type::Framebuffer: {
                const Multiboot2TagFramebuffer* framebuffer = (const Multiboot2TagFramebuffer*)tag;
                LogString(framebuffer->framebuffer.ToString());
                if (framebuffer->framebuffer.type == MultibootFramebuffer::Type::RGB && framebuffer->framebuffer.bitsPerPixel == 32)
                    renderer = new KernelRenderer(framebuffer->framebuffer.width, framebuffer->framebuffer.height, (uint32_t*)framebuffer->framebuffer.address, Color(framebuffer->redFieldPosition, framebuffer->greenFieldPosition, framebuffer->blueFieldPosition, 0));
                break;
            }
            case Multiboot2Tag::Type::MemoryMap: {
                const Multiboot2TagMemoryMap* map = (const Multiboot2TagMemoryMap*)tag;
                const size_t entries = (tag->size - sizeof(Multiboot2TagMemoryMap)) / map->entrySize;
                LogString("Memory map: {\n");
                LogString(String("\tEntry size: ") + ToString(map->entrySize) + '\n');
                LogString(String("\tEntry version: 0x") + ToString(map->entryVersion, 16) + '\n');
                for (size_t i = 0; i < entries; i++) {
                    const Multiboot2MemoryMapEntry* entry = (const Multiboot2MemoryMapEntry*)((uintptr_t)map->entries + map->entrySize * i);
                    LogString(String("\tEntry ") + ToString(i) + ": {\n");
                    LogString(String("\t\tAddress: 0x") + ToString(entry->address, 16) + '\n');
                    LogString(String("\t\tSize: ") + ToString(entry->length) + '\n');
                    LogString(String("\t\tType: ") + (entry->type < MultibootMemoryMapEntryType::TypeCount ? multibootMemoryMapEntryTypeStr[(uint32_t)entry->type] : String("Unknown (0x") + ToString((uint32_t)entry->type, 16) + ')') + '\n');
                    LogString("\t}\n");
                    if (entry->type == MultibootMemoryMapEntryType::Available)
                        rangeMemoryManager.AddRegion(Interval<uintptr_t>(entry->address, entry->address + entry->length));
                }
                LogString("}\n");
                break;
            }
            case Multiboot2Tag::Type::APM: {
                LogString(((const Multiboot2TagAPM*)tag)->ToString());
                break;
            }
            case Multiboot2Tag::Type::Module: {
                const Multiboot2TagModule* module = (const Multiboot2TagModule*)tag;
                LogString("Module: {\n");
                LogString(String("\tPath: ") + module->path + '\n');
                LogString(String("\tStart: 0x") + ToString(module->start, 16) + '\n');
                LogString(String("\tEnd: 0x") + ToString(module->end, 16) + '\n');
                LogString("}\n");
                break;
            }
            case Multiboot2Tag::Type::EFISystemTable32: {
                LogString(String("EFI system table 32 address: 0x") + ToString(((const Multiboot2TagEFI<uint32_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::EFISystemTable64: {
                LogString(String("EFI system table 64 address: 0x") + ToString(((const Multiboot2TagEFI<uint64_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::EFIImageHandle32: {
                LogString(String("EFI image handle 32 address: 0x") + ToString(((const Multiboot2TagEFI<uint32_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::EFIImageHandle64: {
                LogString(String("EFI image handle 64 address: 0x") + ToString(((const Multiboot2TagEFI<uint64_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::EFIBootServicesNotTerminated: Panic("EFI boot services were not terminated");
            case Multiboot2Tag::Type::ELFSections: {
                const Multiboot2TagELF* elf = (const Multiboot2TagELF*)tag;
                LogString("ELF info: {\n");
                LogString(String("\tEntries: ") + ToString(elf->count) + '\n');
                LogString(String("\tEntry size: ") + ToString(elf->entrySize) + '\n');
                LogString(String("\tSection index: 0x") + ToString(elf->sectionIndex, 16) + '\n');
                LogString(String("\tSections: '") + elf->sections + "'\n");
                LogString("}\n");
                break;
            }
            case Multiboot2Tag::Type::VBE: {
                const Multiboot2TagVBE* vbe = (const Multiboot2TagVBE*)tag;
                char signature[5];
                for (uint8_t i = 0; i < SizeOfArray(vbe->info.signature); i++)
                    signature[i] = vbe->info.signature[i];
                signature[4] = '\0';
                LogString("VBE: {\n");
                LogString(String("\tMode: 0x") + ToString(vbe->mode, 16) + '\n');
                LogString(String("\tInterface segment: 0x") + ToString(vbe->interfaceSegment, 16) + '\n');
                LogString(String("\tInterface offset: 0x") + ToString(vbe->interfaceOffset, 16) + '\n');
                LogString(String("\tInterface length: ") + ToString(vbe->interfaceLength) + '\n');
                LogString(String("\tSignature: ") + signature + '\n');
                LogString(String("\tVersion: 0x") + ToString(vbe->info.version, 16) + '\n');
                LogString(String("\tOEM segment: 0x") + ToString(vbe->info.oemSegment, 16) + '\n');
                LogString(String("\tOEM offset: 0x") + ToString(vbe->info.oemOffset, 16) + '\n');
                LogString(String("\tCapabilities: 0x") + ToString(vbe->info.capabilities, 16) + '\n');
                LogString(String("\tVideo mode segment: 0x") + ToString(vbe->info.videoModeSegment, 16) + '\n');
                LogString(String("\tVideo mode offset: 0x") + ToString(vbe->info.videoModeOffset, 16) + '\n');
                LogString(String("\tMemory: ") + ToString(vbe->info.memory * 64) + "KB\n");
                LogString(String("\tWindow A: 0x") + ToString(vbe->modeInfo.windowA, 16) + '\n');
                LogString(String("\tWindow B: 0x") + ToString(vbe->modeInfo.windowB, 16) + '\n');
                LogString(String("\tGranularity: 0x") + ToString(vbe->modeInfo.granularity, 16) + '\n');
                LogString(String("\tWindow size: ") + ToString(vbe->modeInfo.windowSize) + '\n');
                LogString(String("\tSegment A: 0x") + ToString(vbe->modeInfo.segmentA, 16) + '\n');
                LogString(String("\tSegment B: 0x") + ToString(vbe->modeInfo.segmentB, 16) + '\n');
                LogString(String("\tWindow function: 0x") + ToString(vbe->modeInfo.windowFunction, 16) + '\n');
                LogString(String("\tPitch: ") + ToString(vbe->modeInfo.pitch) + '\n');
                LogString(String("\tWidth: ") + ToString(vbe->modeInfo.width) + '\n');
                LogString(String("\tHeight: ") + ToString(vbe->modeInfo.height) + '\n');
                LogString(String("\tW char: 0x") + ToString(vbe->modeInfo.wChr, 16) + '\n');
                LogString(String("\tY char: 0x") + ToString(vbe->modeInfo.yChr, 16) + '\n');
                LogString(String("\tPlanes: ") + ToString(vbe->modeInfo.planes) + '\n');
                LogString(String("\tDepth: ") + ToString(vbe->modeInfo.bitsPerPixel) + '\n');
                LogString(String("\tBanks: ") + ToString(vbe->modeInfo.banks) + '\n');
                LogString(String("\tMemory model: 0x") + ToString(vbe->modeInfo.memoryModel, 16) + '\n');
                LogString(String("\tBank size: ") + ToString(vbe->modeInfo.bankSize) + '\n');
                LogString(String("\tImage pages: ") + ToString(vbe->modeInfo.imagePages) + '\n');
                LogString(String("\tRed mask: 0x") + ToString(vbe->modeInfo.redMask, 16) + '\n');
                LogString(String("\tRed position: ") + ToString(vbe->modeInfo.redPosition) + '\n');
                LogString(String("\tBlue mask: 0x") + ToString(vbe->modeInfo.blueMask, 16) + '\n');
                LogString(String("\tBlue position: ") + ToString(vbe->modeInfo.bluePosition) + '\n');
                LogString(String("\tGreen mask: 0x") + ToString(vbe->modeInfo.greenMask, 16) + '\n');
                LogString(String("\tGreen position: ") + ToString(vbe->modeInfo.greenPosition) + '\n');
                LogString(String("\tReserved mask: 0x") + ToString(vbe->modeInfo.reservedMask, 16) + '\n');
                LogString(String("\tReserved position: ") + ToString(vbe->modeInfo.reservedPosition) + '\n');
                LogString(String("\tDirect color attributes: 0x") + ToString(vbe->modeInfo.directColorAttributes, 16) + '\n');
                LogString(String("\tFramebuffer: 0x") + ToString(vbe->modeInfo.framebuffer, 16) + '\n');
                LogString(String("\tOff screen memory offset: 0x") + ToString(vbe->modeInfo.offScreenMemoryOffset, 16) + '\n');
                LogString(String("\tOff screen memory size: ") + ToString(vbe->modeInfo.offScreenMemorySize) + '\n');
                LogString("}\n");
                break;
            }
            default: {
                LogString(String("Unknown multiboot2 tag: ") + Multiboot2Tag::typeStr[(uint32_t)tag->type] + " (" + ToString((uint32_t)tag->type) + ")\n");
                break;
            }
        }
    }
    return rsdp;
}

#endif