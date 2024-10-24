#ifdef __x86_64__
#include "../../../KernelRenderer.hpp"
#include "Multiboot1.hpp"
#include <Logger.hpp>
#include <String.hpp>

void InitMultiboot1(Multiboot1Info* info, RangeMemoryManager& rangeMemoryManager) {
    LogString("Multiboot1 signature detected\n");
    if (info->hasBootloaderName) LogString(String("Bootloader: ") + (const char*)(uintptr_t)info->bootloaderName + '\n');
    if (info->hasAPM) LogString(((MultibootAPM*)(uintptr_t)info->apm)->ToString());
    if (info->hasConfigTable) LogString(String("Config table: 0x") + ToString(info->configTable, 16) + '\n');
    if (info->hasDriveInfo) {
        LogString(String("Drives length: ") + ToString(info->drivesLength) + '\n');
        LogString(String("Drives address: 0x") + ToString(info->drivesAddress, 16) + '\n');
    }
    if (info->hasMemoryInfo) {
        LogString(String("Lower memory size: ") + ToString(info->lowerMem) + "KB\n");
        LogString(String("Upper memory size: ") + ToString(info->upperMem) + "KB\n");
    }
    if (info->hasVBE) {
        LogString("VBE: {\n");
        LogString(String("\tControl info: 0x") + ToString(info->vbeControlInfo) + '\n');
        LogString(String("\tMode info: 0x") + ToString(info->vbeModeInfo) + '\n');
        LogString(String("\tMode: 0x") + ToString(info->vbeMode) + '\n');
        LogString(String("\tInterface segment: 0x") + ToString(info->vbeInterfaceSegment) + '\n');
        LogString(String("\tInterface offset: 0x") + ToString(info->vbeInterfaceOffset) + '\n');
        LogString(String("\tInterface length: 0x") + ToString(info->vbeInterfaceLength) + '\n');
        LogString("}\n");
    }
    if (info->hasELFInfo) {
        LogString("ELF info: {\n");
        LogString(String("\tEntries: ") + ToString(info->elf.count) + '\n');
        LogString(String("\tSize: ") + ToString(info->elf.size) + '\n');
        LogString(String("\tSection index: 0x") + ToString(info->elf.sectionIndex, 16) + '\n');
        LogString(String("\tAdress: 0x") + ToString(info->elf.address, 16) + '\n');
        LogString("}\n");
    }
    else if (info->hasAoutInfo) {
        LogString("a.out info: {\n");
        LogString(String("\tTab size: ") + ToString(info->aout.tabSize) + '\n');
        LogString(String("\tString size: ") + ToString(info->aout.strSize) + '\n');
        LogString(String("\tAdress: 0x") + ToString(info->aout.address, 16) + '\n');
        LogString(String("\tReserved: 0x") + ToString(info->aout.reserved, 16) + '\n');
        LogString("}\n");
    }
    if (info->hasBootDevice) LogString(String("Boot device: 0x") + ToString(info->bootDevice, 16) + '\n');
    if (info->hasCommandLine && *(const char*)(uintptr_t)info->commandLine) LogString(String("Command line: ") + (const char*)(uintptr_t)info->commandLine + '\n');
    if (info->hasModules) {
        LogString("Modules: {\n");
        for (uint32_t i = 0; i < info->moduleCount; i++) {
            Multiboot1Module* module = (Multiboot1Module*)(uintptr_t)info->moduleAddress + i;
            LogString(String("\tEntry ") + ToString(i) + ": {\n");
            LogString(String("\t\tPath: ") + module->path + '\n');
            LogString(String("\t\tStart: 0x") + ToString(module->start, 16) + '\n');
            LogString(String("\t\tEnd: 0x") + ToString(module->end, 16) + '\n');
            LogString("\t}\n");
        }
        LogString("}\n");
    }
    if (info->hasFramebuffer) {
        LogString(info->framebuffer.ToString());
        if (info->framebuffer.type == MultibootFramebuffer::Type::RGB && info->framebuffer.bitsPerPixel == 32)
            renderer = new KernelRenderer(info->framebuffer.width, info->framebuffer.height, (uint32_t*)info->framebuffer.address, Color(info->redFieldPos, info->greenFieldPos, info->blueFieldPos, 0));
    }
    if (info->hasMemoryMap) {
        const size_t entries = info->memoryMapLength / sizeof(Multiboot1MemoryMapEntry);
        for (size_t i = 0; i < entries; i++) {
            const Multiboot1MemoryMapEntry* entry = (const Multiboot1MemoryMapEntry*)((uintptr_t)info->memoryMapAddress + i * sizeof(Multiboot1MemoryMapEntry));
            LogString(String("Memory map entry ") + ToString(i) + ": {\n");
            LogString(String("\tAddress: 0x") + ToString(entry->address, 16) + '\n');
            LogString(String("\tSize: ") + ToString(entry->length) + '\n');
            LogString(String("\tType: ") + (entry->type < MultibootMemoryMapEntryType::TypeCount ? multibootMemoryMapEntryTypeStr[(uint32_t)entry->type] : String("Unknown (0x") + ToString((uint32_t)entry->type, 16) + ')') + '\n');
            LogString("}\n");
            if (entry->type == MultibootMemoryMapEntryType::Available)
                rangeMemoryManager.AddRegion(Interval<uintptr_t>(entry->address, entry->address + entry->length));
        }
    }
}

#endif