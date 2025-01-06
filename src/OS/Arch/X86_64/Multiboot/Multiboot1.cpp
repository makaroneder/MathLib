#ifdef __x86_64__
#include "../../../KernelRenderer.hpp"
#include "Multiboot1.hpp"
#include "../VGA.hpp"
#include <Logger.hpp>
#include <String.hpp>

void InitMultiboot1(Multiboot1Info* info) {
    LogString("Multiboot1 signature detected\n");
    if (info->hasBootloaderName) LogString(MathLib::String("Bootloader: ") + (const char*)(uintptr_t)info->bootloaderName + '\n');
    if (info->hasAPM) LogString(((MultibootAPM*)(uintptr_t)info->apm)->ToString());
    if (info->hasConfigTable) LogString(MathLib::String("Config table: 0x") + MathLib::ToString(info->configTable, 16) + '\n');
    if (info->hasDriveInfo) {
        LogString(MathLib::String("Drives length: ") + MathLib::ToString(info->drivesLength) + '\n');
        LogString(MathLib::String("Drives address: 0x") + MathLib::ToString(info->drivesAddress, 16) + '\n');
    }
    if (info->hasMemoryInfo) {
        LogString(MathLib::String("Lower memory size: ") + MathLib::ToString(info->lowerMem) + "KB\n");
        LogString(MathLib::String("Upper memory size: ") + MathLib::ToString(info->upperMem) + "KB\n");
    }
    if (info->hasVBE) {
        LogString("VBE: {\n");
        LogString(MathLib::String("\tControl info: 0x") + MathLib::ToString(info->vbeControlInfo) + '\n');
        LogString(MathLib::String("\tMode info: 0x") + MathLib::ToString(info->vbeModeInfo) + '\n');
        LogString(MathLib::String("\tMode: 0x") + MathLib::ToString(info->vbeMode) + '\n');
        LogString(MathLib::String("\tInterface segment: 0x") + MathLib::ToString(info->vbeInterfaceSegment) + '\n');
        LogString(MathLib::String("\tInterface offset: 0x") + MathLib::ToString(info->vbeInterfaceOffset) + '\n');
        LogString(MathLib::String("\tInterface length: 0x") + MathLib::ToString(info->vbeInterfaceLength) + '\n');
        LogString("}\n");
    }
    if (info->hasELFInfo) {
        LogString("ELF info: {\n");
        LogString(MathLib::String("\tEntries: ") + MathLib::ToString(info->elf.count) + '\n');
        LogString(MathLib::String("\tSize: ") + MathLib::ToString(info->elf.size) + '\n');
        LogString(MathLib::String("\tSection index: 0x") + MathLib::ToString(info->elf.sectionIndex, 16) + '\n');
        LogString(MathLib::String("\tAdress: 0x") + MathLib::ToString(info->elf.address, 16) + '\n');
        LogString("}\n");
    }
    else if (info->hasAoutInfo) {
        LogString("a.out info: {\n");
        LogString(MathLib::String("\tTab size: ") + MathLib::ToString(info->aout.tabSize) + '\n');
        LogString(MathLib::String("\tString size: ") + MathLib::ToString(info->aout.strSize) + '\n');
        LogString(MathLib::String("\tAdress: 0x") + MathLib::ToString(info->aout.address, 16) + '\n');
        LogString(MathLib::String("\tReserved: 0x") + MathLib::ToString(info->aout.reserved, 16) + '\n');
        LogString("}\n");
    }
    if (info->hasBootDevice) LogString(MathLib::String("Boot device: 0x") + MathLib::ToString(info->bootDevice, 16) + '\n');
    if (info->hasCommandLine && *(const char*)(uintptr_t)info->commandLine) LogString(MathLib::String("Command line: ") + (const char*)(uintptr_t)info->commandLine + '\n');
    if (info->hasModules) {
        LogString("Modules: {\n");
        for (uint32_t i = 0; i < info->moduleCount; i++) {
            Multiboot1Module* module = (Multiboot1Module*)(uintptr_t)info->moduleAddress + i;
            LogString(MathLib::String("\tEntry ") + MathLib::ToString(i) + ": {\n");
            LogString(MathLib::String("\t\tPath: ") + module->path + '\n');
            LogString(MathLib::String("\t\tStart: 0x") + MathLib::ToString(module->start, 16) + '\n');
            LogString(MathLib::String("\t\tEnd: 0x") + MathLib::ToString(module->end, 16) + '\n');
            LogString("\t}\n");
        }
        LogString("}\n");
    }
    if (info->hasFramebuffer) {
        LogString(info->framebuffer.ToString());
        if (info->framebuffer.type == MultibootFramebuffer::Type::RGB && info->framebuffer.bitsPerPixel == sizeof(uint32_t) * 8)
            renderer = new KernelRenderer(info->framebuffer.width, info->framebuffer.height, (uint32_t*)info->framebuffer.address, MathLib::Color(info->redFieldPos, info->greenFieldPos, info->blueFieldPos, 0));
        else if (info->framebuffer.type == MultibootFramebuffer::Type::EGA && info->framebuffer.bitsPerPixel == sizeof(uint16_t) * 2)
            textUI = new VGA(VGA::colors[(uint8_t)VGA::Color::Black], VGA::colors[(uint8_t)VGA::Color::Gray], (uint8_t*)info->framebuffer.address, info->framebuffer.width, info->framebuffer.height);
    }
    if (info->hasMemoryMap) {
        const size_t entries = info->memoryMapLength / sizeof(Multiboot1MemoryMapEntry);
        for (size_t i = 0; i < entries; i++) {
            const Multiboot1MemoryMapEntry* entry = (const Multiboot1MemoryMapEntry*)((uintptr_t)info->memoryMapAddress + i * sizeof(Multiboot1MemoryMapEntry));
            LogString(MathLib::String("Memory map entry ") + MathLib::ToString(i) + ": {\n");
            LogString(MathLib::String("\tAddress: 0x") + MathLib::ToString(entry->address, 16) + '\n');
            LogString(MathLib::String("\tSize: ") + MathLib::ToString(entry->length) + '\n');
            LogString(MathLib::String("\tType: ") + (entry->type < MultibootMemoryMapEntryType::TypeCount ? multibootMemoryMapEntryTypeStr[(uint32_t)entry->type] : MathLib::String("Unknown (0x") + MathLib::ToString((uint32_t)entry->type, 16) + ')') + '\n');
            LogString("}\n");
            // TODO: Add region
        }
    }
}

#endif