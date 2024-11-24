#ifdef __x86_64__
#include "Interrupts/Exceptions/DivisionError.hpp"
#include "Multiboot/Multiboot1.hpp"
#include "Multiboot/Multiboot2.hpp"
#include "ControlRegisters.hpp"
#include "ACPI/ACPI.hpp"
#include "Scheduler.hpp"
#include "../Arch.hpp"
#include "CPUID.hpp"
#include "CMOS.hpp"
#include "MSR.hpp"
#include "PIT.hpp"
#include "E9.hpp"
#include "IO.hpp"
#include <Logger.hpp>
#include <String.hpp>

RangeMemoryManager rangeMemoryManager;
uint8_t fxsaveRegion[512] __attribute__((aligned(16)));
bool InitArch(uintptr_t signature, void* info) {
    if (E9::IsPresent()) MathLib::logger = new E9();
    LogString("Logger initialized\n");
    if (HasMSR()) {
        const MathLib::Expected<uint64_t> tmp = GetMSR(0xc0000080);
        if (!tmp.HasValue() || !SetMSR(0xc0000080, tmp.Get() | 1 << (uint8_t)EFER::NoExecuteEnable)) return false;
    }
    uintptr_t c;
    uintptr_t d;
    uintptr_t b;
    CPUID(0x1, nullptr, &b, &c, &d);
    bool sseEnabled = false;
    if (d & (1 << (uint8_t)CPUIDBits::D1FXSR)) {
        asm volatile("fxsave %0" :: "m"(fxsaveRegion));
        if (d & (1 << (uint8_t)CPUIDBits::D1FPU)) {
            const MathLib::Expected<uintptr_t> tmp = GetControlRegister(0);
            if (!tmp.HasValue()) return false;
            SetControlRegister(0, tmp.Get() & ~((1 << (uint8_t)ControlRegister0::Emulation) | (1 << (uint8_t)ControlRegister0::TaskSwitched)));
            asm volatile("fninit");
        }
        else {
            const MathLib::Expected<uintptr_t> tmp = GetControlRegister(0);
            if (!tmp.HasValue()) return false;
            SetControlRegister(0, tmp.Get() | (1 << (uint8_t)ControlRegister0::Emulation) | (1 << (uint8_t)ControlRegister0::TaskSwitched));
        }
        if (d & (1 << (uint8_t)CPUIDBits::D1SSE)) {
            MathLib::Expected<uintptr_t> tmp = GetControlRegister(0);
            if (!tmp.HasValue()) return false;
            SetControlRegister(0, (tmp.Get() & ~(1 << (uint8_t)ControlRegister0::Emulation)) | (1 << (uint8_t)ControlRegister0::MonitorCoProcessor));
            tmp = GetControlRegister(4);
            if (!tmp.HasValue()) return false;
            SetControlRegister(4, tmp.Get() | (1 << (uint8_t)ControlRegister4::FXSaveOSSupport) | (1 << (uint8_t)ControlRegister4::UnmaskedSIMDFloatExceptionsOSSupport));
            sseEnabled = true;
        }
    }
    if (c & (1 << (uint8_t)CPUIDBits::C1XSave)) {
        const MathLib::Expected<uintptr_t> tmp = GetControlRegister(4);
        if (!tmp.HasValue()) return false;
        SetControlRegister(4, tmp.Get() | (1 << (uint8_t)ControlRegister4::XSaveEnable));
        if (sseEnabled && c & (1 << (uint8_t)CPUIDBits::C1AVX)) asm volatile (
            "xor %%rcx, %%rcx\n"
            "xgetbv\n"
            "or $0b111, %%eax\n"
            "xsetbv" ::: "rax", "rcx", "rdx"
        );
    }
    CPUID(0x7, nullptr, &b, &c, &d);
    if (b & 1 << (uint8_t)CPUIDBits::B7SMEP) {
        const MathLib::Expected<uintptr_t> tmp = GetControlRegister(4);
        if (!tmp.HasValue()) return false;
        SetControlRegister(4, tmp.Get() | 1 << (uint8_t)ControlRegister4::SupervisorModeExecutionsProtectionEnable);
    }
    if (b & 1 << (uint8_t)CPUIDBits::B7SMAP) {
        const MathLib::Expected<uintptr_t> tmp = GetControlRegister(4);
        if (!tmp.HasValue()) return false;
        SetControlRegister(4, tmp.Get() | 1 << (uint8_t)ControlRegister4::SupervisorModeAccessProtectionEnable);
    }
    if (!InitInterrupts(0x20, 0x08)) return false;
    PIT* pit = new PIT();
    if (!pit) return false;
    pit->SetFunction(MathLib::FunctionPointer<void, Registers*>(nullptr, &Schedule));
    mainTimer = pit;
    RSDP* rsdp = nullptr;
    if (signature == 0x2badb002) InitMultiboot1((Multiboot1Info*)info, rangeMemoryManager);
    else if (signature == 0x36d76289) rsdp = InitMultiboot2((Multiboot2Info*)info, rangeMemoryManager);
    else LogString(MathLib::String("Unknown bootloader signature: 0x") + MathLib::ToString(signature, 16) + '\n');
    if (rangeMemoryManager.GetSize()) memoryManager = &rangeMemoryManager;
    // TODO: Create new paging structure
    if (!rsdp) rsdp = FindRSDP();
    if (!InitACPI(rsdp, true)) return false;
    const CMOSFloppyData floppyData = cmos->GetFloppyData();
    LogString(MathLib::String("CMOS master floppy data: 0x") + MathLib::ToString(floppyData.master, 16) + '\n');
    LogString(MathLib::String("CMOS slave floppy data: 0x") + MathLib::ToString(floppyData.slave, 16) + '\n');
    #ifdef TriggerDivisionError
    {
        Registers regs;
        regs.a = 0x1;
        regs.b = 0x2;
        regs.c = 0x3;
        regs.d = 0x3;
        regs.si = 0x4;
        regs.di = 0x5;
        regs.r8 = 0x6;
        regs.r9 = 0x7;
        regs.r10 = 0x8;
        regs.r11 = 0x9;
        regs.r12 = 0xa;
        regs.r13 = 0xb;
        regs.r14 = 0xc;
        regs.r15 = 0xd;
        DivisionError::Trigger(regs);
    }
    #endif
    return true;
}
[[noreturn]] void ShutdownArch(void) {
    // TODO: Perform APCI shutdown
    // Bochs shutdown
    WritePort<uint16_t>(0xb004, 0x2000);
    // QEMU shutdown
    WritePort<uint16_t>(0x0604, 0x2000);
    // VirtualBox shutdown
    WritePort<uint16_t>(0x4004, 0x3400);
    // Cloud Hypervisor shutdown
    WritePort<uint16_t>(0x0600, 0x0034);
    MathLib::Panic("It's now safe to turn off your computer");
}
[[noreturn]] void ArchPanic(void) {
    if (cmos) cmos->SetNMI(false);
    SetInterrupts(false);
    while (true) asm volatile("hlt");
}

#endif