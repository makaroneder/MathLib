#ifdef __x86_64__
#include "../../../KernelRenderer.hpp"
#include "Multiboot1.hpp"
#include "../VGA.hpp"
#include <Logger.hpp>
#include <String.hpp>

void InitMultiboot1(Multiboot1Info* info) {
    LogString("Multiboot1 signature detected\n");
    if (info->hasBootloaderName) LogString("Bootloader: "_M + (const char*)(uintptr_t)info->bootloaderName + '\n');
    if (info->hasAPM) LogString(((MultibootAPM*)(uintptr_t)info->apm)->ToString());
    if (info->hasConfigTable) LogString("Config table: 0x"_M + MathLib::ToString(info->configTable, 16) + '\n');
    if (info->hasDriveInfo) {
        LogString("Drives length: "_M + MathLib::ToString(info->drivesLength) + '\n');
        LogString("Drives address: 0x"_M + MathLib::ToString(info->drivesAddress, 16) + '\n');
    }
    if (info->hasMemoryInfo) {
        LogString("Lower memory size: "_M + MathLib::ToString(info->lowerMem) + "KB\n");
        LogString("Upper memory size: "_M + MathLib::ToString(info->upperMem) + "KB\n");
    }
    if (info->hasVBE) {
        LogString("VBE: {\n");
        LogString("\tControl info: 0x"_M + MathLib::ToString(info->vbeControlInfo) + '\n');
        LogString("\tMode info: 0x"_M + MathLib::ToString(info->vbeModeInfo) + '\n');
        LogString("\tMode: 0x"_M + MathLib::ToString(info->vbeMode) + '\n');
        LogString("\tInterface segment: 0x"_M + MathLib::ToString(info->vbeInterfaceSegment) + '\n');
        LogString("\tInterface offset: 0x"_M + MathLib::ToString(info->vbeInterfaceOffset) + '\n');
        LogString("\tInterface length: 0x"_M + MathLib::ToString(info->vbeInterfaceLength) + '\n');
        LogString("}\n");
    }
    if (info->hasELFInfo) {
        LogString("ELF info: {\n");
        LogString("\tEntries: "_M + MathLib::ToString(info->elf.count) + '\n');
        LogString("\tSize: "_M + MathLib::ToString(info->elf.size) + '\n');
        LogString("\tSection index: 0x"_M + MathLib::ToString(info->elf.sectionIndex, 16) + '\n');
        LogString("\tAdress: 0x"_M + MathLib::ToString(info->elf.address, 16) + '\n');
        LogString("}\n");
    }
    else if (info->hasAoutInfo) {
        LogString("a.out info: {\n");
        LogString("\tTab size: "_M + MathLib::ToString(info->aout.tabSize) + '\n');
        LogString("\tString size: "_M + MathLib::ToString(info->aout.strSize) + '\n');
        LogString("\tAdress: 0x"_M + MathLib::ToString(info->aout.address, 16) + '\n');
        LogString("\tReserved: 0x"_M + MathLib::ToString(info->aout.reserved, 16) + '\n');
        LogString("}\n");
    }
    if (info->hasBootDevice) LogString("Boot device: 0x"_M + MathLib::ToString(info->bootDevice, 16) + '\n');
    if (info->hasCommandLine && *(const char*)(uintptr_t)info->commandLine) LogString("Command line: "_M + (const char*)(uintptr_t)info->commandLine + '\n');
    if (info->hasModules) {
        LogString("Modules: {\n");
        for (uint32_t i = 0; i < info->moduleCount; i++) {
            Multiboot1Module* module = (Multiboot1Module*)(uintptr_t)info->moduleAddress + i;
            LogString("\tEntry "_M + MathLib::ToString(i) + ": {\n");
            LogString("\t\tPath: "_M + module->path + '\n');
            LogString("\t\tStart: 0x"_M + MathLib::ToString(module->start, 16) + '\n');
            LogString("\t\tEnd: 0x"_M + MathLib::ToString(module->end, 16) + '\n');
            LogString("\t}\n");
        }
        LogString("}\n");
    }
    if (info->hasFramebuffer) {
        LogString(info->framebuffer.ToString());
        if (info->framebuffer.type == MultibootFramebuffer::Type::RGB && info->framebuffer.bitsPerPixel == sizeof(uint32_t) * 8)
            renderer = new KernelRenderer(info->framebuffer.width, info->framebuffer.height, (uint32_t*)info->framebuffer.address, MathLib::Color(info->redFieldPos, info->greenFieldPos, info->blueFieldPos, 0));
        else if (info->framebuffer.type == MultibootFramebuffer::Type::EGA && info->framebuffer.bitsPerPixel == sizeof(uint16_t) * 8)
            textUI = new VGA(VGA::colors[(uint8_t)VGA::Color::Black], VGA::colors[(uint8_t)VGA::Color::Gray], (uint8_t*)info->framebuffer.address, info->framebuffer.width, info->framebuffer.height);
    }
    if (info->hasMemoryMap) {
        const size_t entries = info->memoryMapLength / sizeof(Multiboot1MemoryMapEntry);
        for (size_t i = 0; i < entries; i++) {
            const Multiboot1MemoryMapEntry* entry = (const Multiboot1MemoryMapEntry*)((uintptr_t)info->memoryMapAddress + i * sizeof(Multiboot1MemoryMapEntry));
            LogString("Memory map entry "_M + MathLib::ToString(i) + ": {\n");
            LogString("\tAddress: 0x"_M + MathLib::ToString(entry->address, 16) + '\n');
            LogString("\tSize: "_M + MathLib::ToString(entry->length) + '\n');
            LogString("\tType: "_M + (entry->type < MultibootMemoryMapEntryType::TypeCount ? multibootMemoryMapEntryTypeStr[(uint32_t)entry->type] : "Unknown (0x"_M + MathLib::ToString((uint32_t)entry->type, 16) + ')') + '\n');
            LogString("}\n");
            // TODO: Add region
        }
    }
}

#endif