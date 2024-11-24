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
                if (*string->str) LogString(MathLib::String("Command line: ") + string->str + '\n');
                break;
            }
            case Multiboot2Tag::Type::BootloaderName: {
                LogString(MathLib::String("Bootloader: ") + ((const Multiboot2TagString*)tag)->str + '\n');
                break;
            }
            case Multiboot2Tag::Type::BaseLoadAddr: {
                LogString(MathLib::String("Load address: 0x") + MathLib::ToString(((const Multiboot2TagLoadBaseAddr*)tag)->address, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::BasicMemoryInfo: {
                const Multiboot2TagMemoryInfo* memoryInfo = (const Multiboot2TagMemoryInfo*)tag;
                LogString(MathLib::String("Lower memory size: ") + MathLib::ToString(memoryInfo->lowerMemory) + "KB\n");
                LogString(MathLib::String("Upper memory size: ") + MathLib::ToString(memoryInfo->upperMemory) + "KB\n");
                break;
            }
            case Multiboot2Tag::Type::BIOSBootDevice: {
                const Multiboot2TagBIOSBootDevice* dev = (const Multiboot2TagBIOSBootDevice*)tag;
                LogString("BIOS boot device: {\n");
                LogString(MathLib::String("\tID: 0x") + MathLib::ToString(dev->id, 16) + '\n');
                LogString(MathLib::String("\tPartition: 0x") + MathLib::ToString(dev->partition, 16) + '\n');
                LogString(MathLib::String("\tSub partition: 0x") + MathLib::ToString(dev->subPartition, 16) + '\n');
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
                    renderer = new KernelRenderer(framebuffer->framebuffer.width, framebuffer->framebuffer.height, (uint32_t*)framebuffer->framebuffer.address, MathLib::Color(framebuffer->redFieldPosition, framebuffer->greenFieldPosition, framebuffer->blueFieldPosition, 0));
                break;
            }
            case Multiboot2Tag::Type::MemoryMap: {
                const Multiboot2TagMemoryMap* map = (const Multiboot2TagMemoryMap*)tag;
                const size_t entries = (tag->size - sizeof(Multiboot2TagMemoryMap)) / map->entrySize;
                LogString("Memory map: {\n");
                LogString(MathLib::String("\tEntry size: ") + MathLib::ToString(map->entrySize) + '\n');
                LogString(MathLib::String("\tEntry version: 0x") + MathLib::ToString(map->entryVersion, 16) + '\n');
                for (size_t i = 0; i < entries; i++) {
                    const Multiboot2MemoryMapEntry* entry = (const Multiboot2MemoryMapEntry*)((uintptr_t)map->entries + map->entrySize * i);
                    LogString(MathLib::String("\tEntry ") + MathLib::ToString(i) + ": {\n");
                    LogString(MathLib::String("\t\tAddress: 0x") + MathLib::ToString(entry->address, 16) + '\n');
                    LogString(MathLib::String("\t\tSize: ") + MathLib::ToString(entry->length) + '\n');
                    LogString(MathLib::String("\t\tType: ") + (entry->type < MultibootMemoryMapEntryType::TypeCount ? multibootMemoryMapEntryTypeStr[(uint32_t)entry->type] : MathLib::String("Unknown (0x") + MathLib::ToString((uint32_t)entry->type, 16) + ')') + '\n');
                    LogString("\t}\n");
                    if (entry->type == MultibootMemoryMapEntryType::Available)
                        rangeMemoryManager.AddRegion(MathLib::Interval<uintptr_t>(entry->address, entry->address + entry->length));
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
                LogString(MathLib::String("\tPath: ") + module->path + '\n');
                LogString(MathLib::String("\tStart: 0x") + MathLib::ToString(module->start, 16) + '\n');
                LogString(MathLib::String("\tEnd: 0x") + MathLib::ToString(module->end, 16) + '\n');
                LogString("}\n");
                break;
            }
            case Multiboot2Tag::Type::EFISystemTable32: {
                LogString(MathLib::String("EFI system table 32 address: 0x") + MathLib::ToString(((const Multiboot2TagEFI<uint32_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::EFISystemTable64: {
                LogString(MathLib::String("EFI system table 64 address: 0x") + MathLib::ToString(((const Multiboot2TagEFI<uint64_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::EFIImageHandle32: {
                LogString(MathLib::String("EFI image handle 32 address: 0x") + MathLib::ToString(((const Multiboot2TagEFI<uint32_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::EFIImageHandle64: {
                LogString(MathLib::String("EFI image handle 64 address: 0x") + MathLib::ToString(((const Multiboot2TagEFI<uint64_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::EFIBootServicesNotTerminated: MathLib::Panic("EFI boot services were not terminated");
            case Multiboot2Tag::Type::ELFSections: {
                const Multiboot2TagELF* elf = (const Multiboot2TagELF*)tag;
                LogString("ELF info: {\n");
                LogString(MathLib::String("\tEntries: ") + MathLib::ToString(elf->count) + '\n');
                LogString(MathLib::String("\tEntry size: ") + MathLib::ToString(elf->entrySize) + '\n');
                LogString(MathLib::String("\tSection index: 0x") + MathLib::ToString(elf->sectionIndex, 16) + '\n');
                LogString(MathLib::String("\tSections: '") + elf->sections + "'\n");
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
                LogString(MathLib::String("\tMode: 0x") + MathLib::ToString(vbe->mode, 16) + '\n');
                LogString(MathLib::String("\tInterface segment: 0x") + MathLib::ToString(vbe->interfaceSegment, 16) + '\n');
                LogString(MathLib::String("\tInterface offset: 0x") + MathLib::ToString(vbe->interfaceOffset, 16) + '\n');
                LogString(MathLib::String("\tInterface length: ") + MathLib::ToString(vbe->interfaceLength) + '\n');
                LogString(MathLib::String("\tSignature: ") + signature + '\n');
                LogString(MathLib::String("\tVersion: 0x") + MathLib::ToString(vbe->info.version, 16) + '\n');
                LogString(MathLib::String("\tOEM segment: 0x") + MathLib::ToString(vbe->info.oemSegment, 16) + '\n');
                LogString(MathLib::String("\tOEM offset: 0x") + MathLib::ToString(vbe->info.oemOffset, 16) + '\n');
                LogString(MathLib::String("\tCapabilities: 0x") + MathLib::ToString(vbe->info.capabilities, 16) + '\n');
                LogString(MathLib::String("\tVideo mode segment: 0x") + MathLib::ToString(vbe->info.videoModeSegment, 16) + '\n');
                LogString(MathLib::String("\tVideo mode offset: 0x") + MathLib::ToString(vbe->info.videoModeOffset, 16) + '\n');
                LogString(MathLib::String("\tMemory: ") + MathLib::ToString(vbe->info.memory * 64) + "KB\n");
                LogString(MathLib::String("\tWindow A: 0x") + MathLib::ToString(vbe->modeInfo.windowA, 16) + '\n');
                LogString(MathLib::String("\tWindow B: 0x") + MathLib::ToString(vbe->modeInfo.windowB, 16) + '\n');
                LogString(MathLib::String("\tGranularity: 0x") + MathLib::ToString(vbe->modeInfo.granularity, 16) + '\n');
                LogString(MathLib::String("\tWindow size: ") + MathLib::ToString(vbe->modeInfo.windowSize) + '\n');
                LogString(MathLib::String("\tSegment A: 0x") + MathLib::ToString(vbe->modeInfo.segmentA, 16) + '\n');
                LogString(MathLib::String("\tSegment B: 0x") + MathLib::ToString(vbe->modeInfo.segmentB, 16) + '\n');
                LogString(MathLib::String("\tWindow function: 0x") + MathLib::ToString(vbe->modeInfo.windowFunction, 16) + '\n');
                LogString(MathLib::String("\tPitch: ") + MathLib::ToString(vbe->modeInfo.pitch) + '\n');
                LogString(MathLib::String("\tWidth: ") + MathLib::ToString(vbe->modeInfo.width) + '\n');
                LogString(MathLib::String("\tHeight: ") + MathLib::ToString(vbe->modeInfo.height) + '\n');
                LogString(MathLib::String("\tW char: 0x") + MathLib::ToString(vbe->modeInfo.wChr, 16) + '\n');
                LogString(MathLib::String("\tY char: 0x") + MathLib::ToString(vbe->modeInfo.yChr, 16) + '\n');
                LogString(MathLib::String("\tPlanes: ") + MathLib::ToString(vbe->modeInfo.planes) + '\n');
                LogString(MathLib::String("\tDepth: ") + MathLib::ToString(vbe->modeInfo.bitsPerPixel) + '\n');
                LogString(MathLib::String("\tBanks: ") + MathLib::ToString(vbe->modeInfo.banks) + '\n');
                LogString(MathLib::String("\tMemory model: 0x") + MathLib::ToString(vbe->modeInfo.memoryModel, 16) + '\n');
                LogString(MathLib::String("\tBank size: ") + MathLib::ToString(vbe->modeInfo.bankSize) + '\n');
                LogString(MathLib::String("\tImage pages: ") + MathLib::ToString(vbe->modeInfo.imagePages) + '\n');
                LogString(MathLib::String("\tRed mask: 0x") + MathLib::ToString(vbe->modeInfo.redMask, 16) + '\n');
                LogString(MathLib::String("\tRed position: ") + MathLib::ToString(vbe->modeInfo.redPosition) + '\n');
                LogString(MathLib::String("\tBlue mask: 0x") + MathLib::ToString(vbe->modeInfo.blueMask, 16) + '\n');
                LogString(MathLib::String("\tBlue position: ") + MathLib::ToString(vbe->modeInfo.bluePosition) + '\n');
                LogString(MathLib::String("\tGreen mask: 0x") + MathLib::ToString(vbe->modeInfo.greenMask, 16) + '\n');
                LogString(MathLib::String("\tGreen position: ") + MathLib::ToString(vbe->modeInfo.greenPosition) + '\n');
                LogString(MathLib::String("\tReserved mask: 0x") + MathLib::ToString(vbe->modeInfo.reservedMask, 16) + '\n');
                LogString(MathLib::String("\tReserved position: ") + MathLib::ToString(vbe->modeInfo.reservedPosition) + '\n');
                LogString(MathLib::String("\tDirect color attributes: 0x") + MathLib::ToString(vbe->modeInfo.directColorAttributes, 16) + '\n');
                LogString(MathLib::String("\tFramebuffer: 0x") + MathLib::ToString(vbe->modeInfo.framebuffer, 16) + '\n');
                LogString(MathLib::String("\tOff screen memory offset: 0x") + MathLib::ToString(vbe->modeInfo.offScreenMemoryOffset, 16) + '\n');
                LogString(MathLib::String("\tOff screen memory size: ") + MathLib::ToString(vbe->modeInfo.offScreenMemorySize) + '\n');
                LogString("}\n");
                break;
            }
            default: {
                LogString(MathLib::String("Unknown multiboot2 tag: ") + Multiboot2Tag::typeStr[(uint32_t)tag->type] + " (" + MathLib::ToString((uint32_t)tag->type) + ")\n");
                break;
            }
        }
    }
    return rsdp;
}

#endif