#include "../../../KernelRenderer.hpp"
#include "Multiboot1.hpp"
#include <Logger.hpp>
#include <String.hpp>

void InitMultiboot1(Multiboot1Info* info) {
    LogString("Multiboot1 signature detected\n");
    if (info->flags & 1 << (uint8_t)Multiboot1InfoFlags::MemoryInfo) {
        LogString(String("Lower memory size: ") + ToString(info->lowerMem) + "KB\n");
        LogString(String("Upper memory size: ") + ToString(info->upperMem) + "KB\n");
    }
    if (info->flags & 1 << (uint8_t)Multiboot1InfoFlags::BootDevice) LogString(String("Boot device: 0x") + ToString(info->bootDevice, 16) + '\n');
    if (info->flags & 1 << (uint8_t)Multiboot1InfoFlags::CommandLine && *(const char*)(uintptr_t)info->commandLine) LogString(String("Command line: ") + (const char*)(uintptr_t)info->commandLine + '\n');
    if (info->flags & 1 << (uint8_t)Multiboot1InfoFlags::Modules) {
        LogString(String("Module count: ") + ToString(info->moduleCount) + '\n');
        LogString(String("Module address: 0x") + ToString(info->moduleAddress, 16) + '\n');
    }
    if (info->flags & 1 << (uint8_t)Multiboot1InfoFlags::Framebuffer) {
        String type;
        switch (info->framebufferType) {
            case Multiboot1Info::FramebufferType::Indexed: {
                type = "Indexed";
                break;
            }
            case Multiboot1Info::FramebufferType::RGB: {
                type = "RGB";
                break;
            }
            case Multiboot1Info::FramebufferType::EGA: {
                type = "EGA";
                break;
            }
            default: type = String("Unknown (0x") + ToString((uint8_t)info->framebufferType, 16) + ')';
        }
        LogString("Framebuffer: {\n");
        LogString(String("\tAddress: 0x") + ToString(info->framebufferAddress, 16) + '\n');
        LogString(String("\tWidth: ") + ToString(info->framebufferWidth) + '\n');
        LogString(String("\tHeight: ") + ToString(info->framebufferHeight) + '\n');
        LogString(String("\tDepth: ") + ToString(info->framebufferBitsPerPixel) + '\n');
        LogString(String("\tPitch: ") + ToString(info->framebufferPitch) + '\n');
        LogString(String("\tType: ") + type + '\n');
        LogString("}\n");
        if (info->framebufferType == Multiboot1Info::FramebufferType::RGB && info->framebufferBitsPerPixel == 32)
            renderer = new KernelRenderer(info->framebufferWidth, info->framebufferHeight, (uint32_t*)info->framebufferAddress, Color(info->redFieldPos, info->greenFieldPos, info->blueFieldPos, 0));
    }
}