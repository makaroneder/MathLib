#include "Multiboot2Renderer.hpp"
#include "ControlRegisters.hpp"
#include "ACPI/ACPI.hpp"
#include "CPUID.hpp"
#include "PIT.hpp"
#include "E9.hpp"
#include <MathLib.hpp>

extern "C" bool constructorsCalled;
__attribute__ ((constructor)) void TestConstructors(void) {
    constructorsCalled = true;
}
uint8_t fxsaveRegion[512] __attribute__((aligned(16)));
extern "C" __attribute__((noreturn)) void Main(uint32_t signature, Multiboot2Info* mbInfo) {
    if (!constructorsCalled) Panic("Failed to call global constructors");
    if (signature != 0x36d76289 || !mbInfo) Panic("Invalid bootloader signature");
    E9 e9;
    Multiboot2TagFramebuffer* framebuffer = nullptr;
    RSDP* rsdp = nullptr;
    for (Multiboot2Tag* tag = mbInfo->tags; tag->type != (uint32_t)Multiboot2TagType::FinalTag; tag = (Multiboot2Tag*)((uint8_t*)tag + ((tag->size + 7) & ~7))) {
        switch ((Multiboot2TagType)tag->type) {
            case Multiboot2TagType::CMDLine: {
                const Multiboot2TagString* string = (const Multiboot2TagString*)tag;
                if (string->str[0]) e9.Puts(String("Command line: ") + string->str + '\n');
                break;
            }
            case Multiboot2TagType::BootloaderName: {
                e9.Puts(String("Bootloader: ") + ((const Multiboot2TagString*)tag)->str + '\n');
                break;
            }
            case Multiboot2TagType::BaseLoadAddr: {
                e9.Puts(String("Load address: 0x") + ToString(((const Multiboot2TagLoadBaseAddr*)tag)->addr, 16) + '\n');
                break;
            }
            case Multiboot2TagType::BasicMemoryInfo: {
                const Multiboot2TagMemoryInfo* memoryInfo = (const Multiboot2TagMemoryInfo*)tag;
                e9.Puts(String("Lower memory size: ") + ToString(memoryInfo->lowerMem) + "KB\n");
                e9.Puts(String("Upper memory size: ") + ToString(memoryInfo->upperMem) + "KB\n");
                break;
            }
            case Multiboot2TagType::BIOSBootDevice: {
                const Multiboot2TagBIOSBootDevice* dev = (const Multiboot2TagBIOSBootDevice*)tag;
                e9.Puts("BIOS boot device:\n");
                e9.Puts(String("\tID: 0x") + ToString(dev->id, 16) + '\n');
                e9.Puts(String("\tPartition: 0x") + ToString(dev->partition, 16) + '\n');
                e9.Puts(String("\tSub partition: 0x") + ToString(dev->subPartition, 16) + '\n');
                break;
            }
            case Multiboot2TagType::OldACPI:
            case Multiboot2TagType::NewACPI: {
                rsdp = (RSDP*)((const Multiboot2TagACPI*)tag)->rsdp;
                e9.Puts("RSDP:\n");
                e9.Puts(String("\tSignature: ") + rsdp->signature + '\n');
                e9.Puts(String("\tChecksum: 0x") + ToString(rsdp->checksum, 16) + '\n');
                e9.Puts(String("\tOEM ID: ") + rsdp->oemID + '\n');
                e9.Puts(String("\tRevision: 0x") + ToString(rsdp->revision, 16) + '\n');
                e9.Puts(String("\tRSDT address: 0x") + ToString(rsdp->rsdt, 16) + '\n');
                if (rsdp->revision == 2) {
                    const RSDP2* rsdp2 = (const RSDP2*)rsdp;
                    e9.Puts(String("\tLength: 0x") + ToString(rsdp2->length, 16) + '\n');
                    e9.Puts(String("\tXSDT address: 0x") + ToString(rsdp2->xsdt, 16) + '\n');
                    e9.Puts(String("\tExtended checksum: 0x") + ToString(rsdp2->extendedChecksum, 16) + '\n');
                }
                break;
            }
            case Multiboot2TagType::Framebuffer: {
                framebuffer = (Multiboot2TagFramebuffer*)tag;
                e9.Puts("Framebuffer:\n");
                e9.Puts(String("\tAddress: 0x") + ToString(framebuffer->addr, 16) + '\n');
                e9.Puts(String("\tWidth: ") + ToString(framebuffer->width) + '\n');
                e9.Puts(String("\tHeight: ") + ToString(framebuffer->height) + '\n');
                e9.Puts(String("\tDepth: ") + ToString(framebuffer->bpp) + '\n');
                e9.Puts(String("\tPitch: ") + ToString(framebuffer->pitch) + '\n');
                break;
            }
            case Multiboot2TagType::MemoryMap: {
                const Multiboot2TagMemoryMap* map = (const Multiboot2TagMemoryMap*)tag;
                const size_t entries = (tag->size - sizeof(Multiboot2TagMemoryMap)) / map->entrySize;
                e9.Puts("Memory map:\n");
                e9.Puts(String("\tEntry size: ") + ToString(map->entrySize) + '\n');
                e9.Puts(String("\tEntry version: 0x") + ToString(map->entryVersion, 16) + '\n');
                for (size_t i = 0; i < entries; i++) {
                    const Multiboot2MemoryMapEntry* entry = (const Multiboot2MemoryMapEntry*)((uintptr_t)map->entries + map->entrySize * i);
                    e9.Puts(String("\tEntry ") + ToString(i) + ":\n");
                    e9.Puts(String("\t\tAddress: 0x") + ToString(entry->addr, 16) + "\n");
                    e9.Puts(String("\t\tSize: ") + ToString(entry->len) + "\n");
                    e9.Puts(String("\t\tType: ") + ToString(entry->type) + "\n");
                }
                break;
            }
            case Multiboot2TagType::APM: {
                const Multiboot2TagAPM* apm = (const Multiboot2TagAPM*)tag;
                e9.Puts("APM:\n");
                e9.Puts(String("\tVersion: 0x") + ToString(apm->version, 16) + '\n');
                e9.Puts(String("\t32 bit code segment: 0x") + ToString(apm->cseg, 16) + '\n');
                e9.Puts(String("\tOffset: 0x") + ToString(apm->offset, 16) + '\n');
                e9.Puts(String("\t16 bit code segment: 0x") + ToString(apm->cseg16, 16) + '\n');
                e9.Puts(String("\t32 bit data segment: 0x") + ToString(apm->dseg, 16) + '\n');
                e9.Puts(String("\tFlags: 0x") + ToString(apm->flags, 16) + '\n');
                e9.Puts(String("\t32 bit code segment length: 0x") + ToString(apm->csegLen, 16) + '\n');
                e9.Puts(String("\t16 bit code segment length: 0x") + ToString(apm->cseg16Len, 16) + '\n');
                e9.Puts(String("\t32 bit data segment length: 0x") + ToString(apm->dsegLen, 16) + '\n');
                break;
            }
            case Multiboot2TagType::Module: {
                const Multiboot2TagModule* module = (const Multiboot2TagModule*)tag;
                e9.Puts("Module:\n");
                e9.Puts(String("\tPath: ") + module->path + '\n');
                e9.Puts(String("\tStart: 0x") + ToString(module->start, 16) + '\n');
                e9.Puts(String("\tEnd: 0x") + ToString(module->end, 16) + '\n');
                break;
            }
            case Multiboot2TagType::EFISystemTable32: {
                e9.Puts(String("EFI system table 32 address: 0x") + ToString(((const Multiboot2TagEFI<uint32_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2TagType::EFISystemTable64: {
                e9.Puts(String("EFI system table 64 address: 0x") + ToString(((const Multiboot2TagEFI<uint64_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2TagType::EFIImageHandle32: {
                e9.Puts(String("EFI image handle 32 address: 0x") + ToString(((const Multiboot2TagEFI<uint32_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2TagType::EFIImageHandle64: {
                e9.Puts(String("EFI image handle 64 address: 0x") + ToString(((const Multiboot2TagEFI<uint64_t>*)tag)->ptr, 16) + '\n');
                break;
            }
            case Multiboot2TagType::EFIBootServicesNotTerminated: Panic("EFI boot services were not terminated");
            default: {
                e9.Puts(String("Unknown multiboot2 tag: ") + multiboot2TagNames[tag->type] + " (" + ToString(tag->type) + ")\n");
                break;
            }
        }
    }
    uintptr_t c;
    uintptr_t d;
    CPUID(0x1, nullptr, nullptr, &c, &d);
    bool sseEnabled = false;
    if (d & (1 << (uint8_t)CPUIDBits::DFXSR)) {
        asm volatile("fxsave %0" :: "m"(fxsaveRegion));
        if (d & (1 << (uint8_t)CPUIDBits::DFPU)) {
            SetControlRegister(0, GetControlRegister(0) & ~((1 << (uint8_t)ControlRegister0::Emulation) | (1 << (uint8_t)ControlRegister0::TaskSwitched)));
            asm volatile("fninit");
        }
        else SetControlRegister(0, GetControlRegister(0) | (1 << (uint8_t)ControlRegister0::Emulation) | (1 << (uint8_t)ControlRegister0::TaskSwitched));
        if (d & (1 << (uint8_t)CPUIDBits::DSSE)) {
            SetControlRegister(0, (GetControlRegister(0) & ~(1 << (uint8_t)ControlRegister0::Emulation)) | (1 << (uint8_t)ControlRegister0::MonitorCoProcessor));
            SetControlRegister(4, GetControlRegister(4) | (1 << (uint8_t)ControlRegister4::FXSaveOSSupport) | (1 << (uint8_t)ControlRegister4::UnmaskedSIMDFloatExceptionsOSSupport));
            sseEnabled = true;
        }
    }
    if (c & (1 << (uint8_t)CPUIDBits::CXSave)) {
        SetControlRegister(4, GetControlRegister(4) | (1 << (uint8_t)ControlRegister4::XSaveEnable));
        if (sseEnabled && c & (1 << (uint8_t)CPUIDBits::CAVX)) asm volatile(
            "xor %%rcx, %%rcx\n"
            "xgetbv\n"
            "or $0b111, %%eax\n"
            "xsetbv" ::: "rax", "rcx", "rdx"
        );
    }
    InitInterrupts(0x20, 0x08);
    mainTimer = new PIT();
    InitACPI(rsdp);
    Multiboot2Renderer renderer = Multiboot2Renderer(framebuffer);
    renderer.Fill(UINT32_MAX);
    renderer.Update();
    while (true) asm volatile("hlt");
}