#include "Multiboot2Renderer.hpp"
#include "Halt.hpp"
#include "E9.hpp"
#include <MathLib.hpp>

extern "C" bool constructorsCalled; 
__attribute__ ((constructor)) void TestConstructors(void) {
    constructorsCalled = true;
}
extern "C" __attribute__((noreturn)) void Main(uint32_t signature, Multiboot2Info* mbInfo) {
    if (!constructorsCalled) Panic("Failed to call global constructors");
    if (signature != 0x36d76289 || !mbInfo) Panic("Invalid bootloader signature");
    E9 e9;
    Multiboot2TagFramebuffer* framebuffer = nullptr;
    for (Multiboot2Tag* tag = mbInfo->tags; tag->type != (uint32_t)Multiboot2TagType::End; tag = (Multiboot2Tag*)((uint8_t*)tag + ((tag->size + 7) & ~7))) {
        switch ((Multiboot2TagType)tag->type) {
            // TODO: Print memory map
            case Multiboot2TagType::CMDLine: {
                const Multiboot2TagString* string = (const Multiboot2TagString*)tag;
                if (string->str[0]) {
                    e9.Puts("String: ");
                    e9.Puts(string->str);
                    e9.Write<char>('\n');
                }
                break;
            }
            case Multiboot2TagType::BootloaderName: {
                e9.Puts("Bootloader: ");
                e9.Puts(((const Multiboot2TagString*)tag)->str);
                e9.Write<char>('\n');
                break;
            }
            case Multiboot2TagType::BaseLoadAddr: {
                e9.Puts("Load addr: 0x");
                e9.Puts(ToString(((const Multiboot2TagLoadBaseAddr*)tag)->addr, 16));
                e9.Write<char>('\n');
                break;
            }
            case Multiboot2TagType::BasicMemoryInfo: {
                const Multiboot2TagMemoryInfo* memoryInfo = (const Multiboot2TagMemoryInfo*)tag;
                e9.Puts("Lower memory size: ");
                e9.Puts(ToString(memoryInfo->lowerMem));
                e9.Puts("KB\nUpper memory size: ");
                e9.Puts(ToString(memoryInfo->upperMem));
                e9.Puts("KB\n");
                break;
            }
            case Multiboot2TagType::BIOSBootDevice: {
                const Multiboot2TagBIOSBootDevice* dev = (const Multiboot2TagBIOSBootDevice*)tag;
                e9.Puts("BIOS device id: 0x");
                e9.Puts(ToString(dev->id, 16));
                e9.Puts("\nBIOS device partition: 0x");
                e9.Puts(ToString(dev->partition, 16));
                e9.Puts("\nBIOS device sub partition: 0x");
                e9.Puts(ToString(dev->subPartition, 16));
                e9.Write<char>('\n');
                break;
            }
            case Multiboot2TagType::OldACPI:
            case Multiboot2TagType::NewACPI: {
                e9.Puts("RSDP: 0x");
                e9.Puts(ToString((uintptr_t)((const Multiboot2TagACPI*)tag)->rsdp, 16));
                e9.Write<char>('\n');
                break;
            }
            case Multiboot2TagType::Framebuffer: {
                framebuffer = (Multiboot2TagFramebuffer*)tag;
                e9.Puts("Framebuffer address: 0x");
                e9.Puts(ToString(framebuffer->addr, 16));
                e9.Puts("\nFramebuffer width: ");
                e9.Puts(ToString(framebuffer->width));
                e9.Puts("\nFramebuffer height: ");
                e9.Puts(ToString(framebuffer->height));
                e9.Puts("\nFramebuffer depth: ");
                e9.Puts(ToString(framebuffer->bpp));
                e9.Write<char>('\n');
                break;
            }
            default: {
                e9.Puts("Unknown multiboot2 tag: ");
                e9.Puts(ToString(tag->type, 10));
                e9.Write<char>('\n');
                break;
            }
        }
    }
    Multiboot2Renderer renderer = Multiboot2Renderer(framebuffer);
    renderer.Fill(UINT32_MAX);
    renderer.Update();
    Halt();
}