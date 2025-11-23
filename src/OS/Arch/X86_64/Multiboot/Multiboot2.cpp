#ifdef __x86_64__
#include "../../../KernelRenderer.hpp"
#include "../../../Memory.hpp"
#include "Multiboot2.hpp"
#include "../VGA.hpp"
#include <Allocator/RegionAllocator.hpp>
#include <Memory.hpp>
#include <Logger.hpp>
#include <String.hpp>

RSDP* InitMultiboot2(Multiboot2Info* info) {
    RSDP* rsdp = nullptr;
    LogString("Multiboot2 signature detected\n");
    for (Multiboot2Tag* tag = info->tags; tag->type != Multiboot2Tag::Type::FinalTag; tag = (Multiboot2Tag*)((uint8_t*)tag + ((tag->size + 7) & ~7))) {
        switch (tag->type) {
            case Multiboot2Tag::Type::CMDLine: {
                const Multiboot2TagString* string = (const Multiboot2TagString*)tag;
                if (*string->str) LogString("Command line: "_M + string->str + '\n');
                break;
            }
            case Multiboot2Tag::Type::BootloaderName: {
                LogString("Bootloader: "_M + ((const Multiboot2TagString*)tag)->str + '\n');
                break;
            }
            case Multiboot2Tag::Type::BaseLoadAddr: {
                LogString("Load address: 0x"_M + MathLib::ToString(((const Multiboot2TagLoadBaseAddr*)tag)->address, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::BasicMemoryInfo: {
                const Multiboot2TagMemoryInfo* memoryInfo = (const Multiboot2TagMemoryInfo*)tag;
                LogString("Lower memory size: "_M + MathLib::ToString(memoryInfo->lowerMemory) + "KB\n");
                LogString("Upper memory size: "_M + MathLib::ToString(memoryInfo->upperMemory) + "KB\n");
                break;
            }
            case Multiboot2Tag::Type::BIOSBootDevice: {
                const Multiboot2TagBIOSBootDevice* dev = (const Multiboot2TagBIOSBootDevice*)tag;
                LogString("BIOS boot device: {\n");
                LogString("\tID: 0x"_M + MathLib::ToString(dev->id, 16) + '\n');
                LogString("\tPartition: 0x"_M + MathLib::ToString(dev->partition, 16) + '\n');
                LogString("\tSub partition: 0x"_M + MathLib::ToString(dev->subPartition, 16) + '\n');
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
                switch (framebuffer->framebuffer.type) {
                    case MultibootFramebuffer::Type::Indexed: {
                        LogString("Indexed framebuffer palette: {\n");
                        for (uint32_t i = 0; i < framebuffer->paletteColors; i++) {
                            const Multiboot2Color color = framebuffer->palette[i];
                            LogChar('\t');
                            LogString(MathLib::ToString(i, 10));
                            LogString(": 0x");
                            LogString(MathLib::ToString(color.red, 16, 2));
                            LogString(MathLib::ToString(color.green, 16, 2));
                            LogString(MathLib::ToString(color.blue, 16, 2));
                            LogChar('\n');
                        }
                        LogString("}\n");
                        break;
                    }
                    case MultibootFramebuffer::Type::RGB: {
                        LogString("RGB framebuffer mask: {\n");
                        LogString("\tRed field position: "_M + MathLib::ToString(framebuffer->redFieldPosition, 10) + '\n');
                        LogString("\tGreen field position: "_M + MathLib::ToString(framebuffer->greenFieldPosition, 10) + '\n');
                        LogString("\tBlue field position: "_M + MathLib::ToString(framebuffer->blueFieldPosition, 10) + '\n');
                        LogString("\tRed mask size: "_M + MathLib::ToString(framebuffer->redMaskSize) + '\n');
                        LogString("\tGreen mask size: "_M + MathLib::ToString(framebuffer->greenMaskSize) + '\n');
                        LogString("\tBlue mask size: "_M + MathLib::ToString(framebuffer->blueMaskSize) + '\n');
                        LogString("}\n");
                        renderer = new KernelRenderer(framebuffer->framebuffer.width, framebuffer->framebuffer.height, (uint32_t*)framebuffer->framebuffer.address, MathLib::Color(framebuffer->redFieldPosition, framebuffer->greenFieldPosition, framebuffer->blueFieldPosition, 0));
                        if (!renderer) return nullptr;
                        break;
                    }
                    case MultibootFramebuffer::Type::EGA: {
                        textUI = new VGA(VGA::colors[(uint8_t)VGA::Color::Black], VGA::colors[(uint8_t)VGA::Color::Gray], (uint8_t*)framebuffer->framebuffer.address, framebuffer->framebuffer.width, framebuffer->framebuffer.height);
                        if (!textUI) return nullptr;
                        break;
                    }
                    default: break;
                }
                break;
            }
            case Multiboot2Tag::Type::MemoryMap: {
                const Multiboot2TagMemoryMap* map = (const Multiboot2TagMemoryMap*)tag;
                const size_t entries = (tag->size - sizeof(Multiboot2TagMemoryMap)) / map->entrySize;
                LogString("Memory map: {\n");
                LogString("\tEntry size: "_M + MathLib::ToString(map->entrySize) + '\n');
                LogString("\tEntry version: 0x"_M + MathLib::ToString(map->entryVersion, 16) + '\n');
                for (size_t i = 0; i < entries; i++) {
                    const Multiboot2MemoryMapEntry* entry = (const Multiboot2MemoryMapEntry*)((uintptr_t)map->entries + map->entrySize * i);
                    LogString("\tEntry "_M + MathLib::ToString(i) + ": {\n");
                    LogString("\t\tAddress: 0x"_M + MathLib::ToString(entry->address, 16) + '\n');
                    LogString("\t\tSize: "_M + MathLib::ToString(entry->length) + '\n');
                    LogString("\t\tType: "_M + (entry->type < MultibootMemoryMapEntryType::TypeCount ? multibootMemoryMapEntryTypeStr[(uint32_t)entry->type] : "Unknown (0x"_M + MathLib::ToString((uint32_t)entry->type, 16) + ')') + '\n');
                    LogString("\t}\n");
                    if (entry->type == MultibootMemoryMapEntryType::Available) {
                        uintptr_t buffer = entry->address;
                        size_t size = entry->length;
                        if (!buffer && size > 4096) {
                            buffer += 4096;
                            size -= 4096;
                        }
                        if (buffer && size && !allocator.AddAllocator(new MathLib::RegionAllocator((void*)buffer, size))) return nullptr;
                    }
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
                LogString("\tPath: "_M + module->path + '\n');
                LogString("\tStart: 0x"_M + MathLib::ToString(module->start, 16) + '\n');
                LogString("\tEnd: 0x"_M + MathLib::ToString(module->end, 16) + '\n');
                LogString("}\n");
                break;
            }
            case Multiboot2Tag::Type::EFISystemTable32: {
                LogString("EFI system table 32 address: 0x"_M + MathLib::ToString(((const Multiboot2TagEFI<uint32_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::EFISystemTable64: {
                LogString("EFI system table 64 address: 0x"_M + MathLib::ToString(((const Multiboot2TagEFI<uint64_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::EFIImageHandle32: {
                LogString("EFI image handle 32 address: 0x"_M + MathLib::ToString(((const Multiboot2TagEFI<uint32_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::EFIImageHandle64: {
                LogString("EFI image handle 64 address: 0x"_M + MathLib::ToString(((const Multiboot2TagEFI<uint64_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2Tag::Type::EFIBootServicesNotTerminated: MathLib::Panic("EFI boot services were not terminated");
            case Multiboot2Tag::Type::ELFSections: {
                const Multiboot2TagELF* elf = (const Multiboot2TagELF*)tag;
                LogString("ELF info: {\n");
                LogString("\tEntries: "_M + MathLib::ToString(elf->count) + '\n');
                LogString("\tEntry size: "_M + MathLib::ToString(elf->entrySize) + '\n');
                LogString("\tSection index: 0x"_M + MathLib::ToString(elf->sectionIndex, 16) + '\n');
                LogString("\tSections: '"_M + elf->sections + "'\n");
                LogString("}\n");
                break;
            }
            case Multiboot2Tag::Type::VBE: {
                const Multiboot2TagVBE* vbe = (const Multiboot2TagVBE*)tag;
                const size_t size = SizeOfArray(vbe->info.signature);
                char signature[size + 1];
                MathLib::MemoryCopy(vbe->info.signature, signature, size);
                signature[size] = '\0';
                LogString("VBE: {\n");
                LogString("\tMode: 0x"_M + MathLib::ToString(vbe->mode, 16) + '\n');
                LogString("\tInterface segment: 0x"_M + MathLib::ToString(vbe->interfaceSegment, 16) + '\n');
                LogString("\tInterface offset: 0x"_M + MathLib::ToString(vbe->interfaceOffset, 16) + '\n');
                LogString("\tInterface length: "_M + MathLib::ToString(vbe->interfaceLength) + '\n');
                LogString("\tSignature: "_M + signature + '\n');
                LogString("\tVersion: 0x"_M + MathLib::ToString(vbe->info.version, 16) + '\n');
                LogString("\tOEM segment: 0x"_M + MathLib::ToString(vbe->info.oemSegment, 16) + '\n');
                LogString("\tOEM offset: 0x"_M + MathLib::ToString(vbe->info.oemOffset, 16) + '\n');
                LogString("\tCapabilities: 0x"_M + MathLib::ToString(vbe->info.capabilities, 16) + '\n');
                LogString("\tVideo mode segment: 0x"_M + MathLib::ToString(vbe->info.videoModeSegment, 16) + '\n');
                LogString("\tVideo mode offset: 0x"_M + MathLib::ToString(vbe->info.videoModeOffset, 16) + '\n');
                LogString("\tMemory: "_M + MathLib::ToString(vbe->info.memory * 64) + "KB\n");
                LogString("\tWindow A: 0x"_M + MathLib::ToString(vbe->modeInfo.windowA, 16) + '\n');
                LogString("\tWindow B: 0x"_M + MathLib::ToString(vbe->modeInfo.windowB, 16) + '\n');
                LogString("\tGranularity: 0x"_M + MathLib::ToString(vbe->modeInfo.granularity, 16) + '\n');
                LogString("\tWindow size: "_M + MathLib::ToString(vbe->modeInfo.windowSize) + '\n');
                LogString("\tSegment A: 0x"_M + MathLib::ToString(vbe->modeInfo.segmentA, 16) + '\n');
                LogString("\tSegment B: 0x"_M + MathLib::ToString(vbe->modeInfo.segmentB, 16) + '\n');
                LogString("\tWindow function: 0x"_M + MathLib::ToString(vbe->modeInfo.windowFunction, 16) + '\n');
                LogString("\tPitch: "_M + MathLib::ToString(vbe->modeInfo.pitch) + '\n');
                LogString("\tWidth: "_M + MathLib::ToString(vbe->modeInfo.width) + '\n');
                LogString("\tHeight: "_M + MathLib::ToString(vbe->modeInfo.height) + '\n');
                LogString("\tW char: 0x"_M + MathLib::ToString(vbe->modeInfo.wChr, 16) + '\n');
                LogString("\tY char: 0x"_M + MathLib::ToString(vbe->modeInfo.yChr, 16) + '\n');
                LogString("\tPlanes: "_M + MathLib::ToString(vbe->modeInfo.planes) + '\n');
                LogString("\tDepth: "_M + MathLib::ToString(vbe->modeInfo.bitsPerPixel) + '\n');
                LogString("\tBanks: "_M + MathLib::ToString(vbe->modeInfo.banks) + '\n');
                LogString("\tMemory model: 0x"_M + MathLib::ToString(vbe->modeInfo.memoryModel, 16) + '\n');
                LogString("\tBank size: "_M + MathLib::ToString(vbe->modeInfo.bankSize) + '\n');
                LogString("\tImage pages: "_M + MathLib::ToString(vbe->modeInfo.imagePages) + '\n');
                LogString("\tRed mask: 0x"_M + MathLib::ToString(vbe->modeInfo.redMask, 16) + '\n');
                LogString("\tRed position: "_M + MathLib::ToString(vbe->modeInfo.redPosition) + '\n');
                LogString("\tBlue mask: 0x"_M + MathLib::ToString(vbe->modeInfo.blueMask, 16) + '\n');
                LogString("\tBlue position: "_M + MathLib::ToString(vbe->modeInfo.bluePosition) + '\n');
                LogString("\tGreen mask: 0x"_M + MathLib::ToString(vbe->modeInfo.greenMask, 16) + '\n');
                LogString("\tGreen position: "_M + MathLib::ToString(vbe->modeInfo.greenPosition) + '\n');
                LogString("\tReserved mask: 0x"_M + MathLib::ToString(vbe->modeInfo.reservedMask, 16) + '\n');
                LogString("\tReserved position: "_M + MathLib::ToString(vbe->modeInfo.reservedPosition) + '\n');
                LogString("\tDirect color attributes: 0x"_M + MathLib::ToString(vbe->modeInfo.directColorAttributes, 16) + '\n');
                LogString("\tFramebuffer: 0x"_M + MathLib::ToString(vbe->modeInfo.framebuffer, 16) + '\n');
                LogString("\tOff screen memory offset: 0x"_M + MathLib::ToString(vbe->modeInfo.offScreenMemoryOffset, 16) + '\n');
                LogString("\tOff screen memory size: "_M + MathLib::ToString(vbe->modeInfo.offScreenMemorySize) + '\n');
                LogString("}\n");
                break;
            }
            default: {
                LogString("Unknown multiboot2 tag: "_M + Multiboot2Tag::typeStr[(uint32_t)tag->type] + " (" + MathLib::ToString((uint32_t)tag->type) + ")\n");
                break;
            }
        }
    }
    return rsdp;
}

#endif