#ifdef __x86_64__
#include "Interrupts/Exceptions/DivisionError.hpp"
#include "Paging/AlignedPageTable.hpp"
#include "Interrupts/Interrupts.hpp"
#include "SerialPort/SerialPort.hpp"
#include "Multiboot/Multiboot1.hpp"
#include "Multiboot/Multiboot2.hpp"
#include "QEMU/QEMUFileSystem.hpp"
#include "ControlRegisters.hpp"
#include "Paging/Paging.hpp"
#include "../../Memory.hpp"
#include "../../VFS.hpp"
#include "ACPI/ACPI.hpp"
#include "GDT/TSS.hpp"
#include "../Arch.hpp"
#include "CPUID.hpp"
#include "CMOS.hpp"
#include "Math.hpp"
#include "CPU.hpp"
#include "MSR.hpp"
#include "PIT.hpp"
#include "E9.hpp"
#include "IO.hpp"
#include <Logger.hpp>
#include <String.hpp>
#include <Allocator/LinearAllocator.hpp>

AlignedPageTable pml4;

void InitLogger(MathLib::Writable* logger, const char* str) {
    if (!logger) return;
    if (logger->Puts(str) && !MathLib::logger) MathLib::logger = logger;
    else delete logger;
}
bool InitArch(uintptr_t signature, void* info) {
    if (E9::IsPresent()) InitLogger(new E9(), "E9 logger initialized\n");
    do {
        const SerialPort::Port ports[] = {
            SerialPort::Port::COM1, SerialPort::Port::COM2,
            SerialPort::Port::COM3, SerialPort::Port::COM4,
            SerialPort::Port::COM5, SerialPort::Port::COM6,
            SerialPort::Port::COM7, SerialPort::Port::COM8,
        };
        char* const startMessage = (char*)"COM? initialized\n";
        for (uint8_t i = 0; i < SizeOfArray(ports); i++) {
            SerialPort* serial = new SerialPort(ports[i]);
            if (!serial) continue;
            if (!serial->Init()) {
                delete serial;
                continue;
            }
            startMessage[3] = '1' + i;
            InitLogger(serial, startMessage);
        }
    } while (false);
    InitTSS();
    if (HasMSR()) {
        const MathLib::Expected<uint64_t> tmp = GetMSR(0xc0000080);
        if (!tmp.HasValue() || !SetMSR(0xc0000080, tmp.Get() | 1 << (uint8_t)EFER::NoExecuteEnable)) return false;
    }
    if (!InitMath()) return false;
    uintptr_t maxCPUID;
    CPUID(0x00, &maxCPUID, nullptr, nullptr, nullptr);
    if (maxCPUID >= 0x07) {
        uintptr_t b;
        CPUID(0x07, nullptr, &b, nullptr, nullptr);
        if (b & 1 << (uint8_t)CPUIDBits::B7SMEP) {
            const MathLib::Expected<uintptr_t> tmp = GetControlRegister(4);
            if (!tmp.HasValue() || !SetControlRegister(4, tmp.Get() | 1 << (uint8_t)ControlRegister4::SupervisorModeExecutionsProtectionEnable)) return false;
        }
        if (b & 1 << (uint8_t)CPUIDBits::B7SMAP) {
            const MathLib::Expected<uintptr_t> tmp = GetControlRegister(4);
            if (!tmp.HasValue() || !SetControlRegister(4, tmp.Get() | 1 << (uint8_t)ControlRegister4::SupervisorModeAccessProtectionEnable)) return false;
        }
    }
    if (!InitInterrupts(0x20, 0x08)) return false;
    PIT* pit = new PIT();
    if (!pit) return false;
    mainTimer = pit;
    RSDP* rsdp = nullptr;
    if (signature == 0x2badb002) {
        if (!InitMultiboot1((Multiboot1Info*)info)) return false;
    }
    else if (signature == 0x36d76289) rsdp = InitMultiboot2((Multiboot2Info*)info);
    else LogString("Unknown bootloader signature: 0x"_M + MathLib::ToString(signature, 16) + '\n');
    do {
        PreparePaging();
        const size_t size = GetRequiredMemorySizeToMap(memorySize) + 4095;
        MathLib::LinearAllocator alloc = MathLib::LinearAllocator(allocator.Alloc(size), size);
        if (!alloc.Align(4096)) LogString("Fail to align memory for paging");
        const size_t pageSize = GetPageSize();
        for (uintptr_t i = 0; i < memorySize; i += pageSize)
            if (!Map(alloc, &pml4.table, i)) MathLib::Panic("Failed to identity map 0x"_M + MathLib::ToString(i, 16));
        if (!SetControlRegister(3, (uintptr_t)&pml4)) MathLib::Panic("Failed to map RAM");
    } while (false);
    if (!rsdp) rsdp = FindRSDP();
    if (!InitACPI(rsdp, true)) return false;
    const CMOSFloppyData floppyData = cmos->GetFloppyData();
    LogString("CMOS floppy data: "_M + floppyData.ToString() + '\n');
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
    QEMUFileSystem* qemu = new QEMUFileSystem();
    if (!qemu) return false;
    if (!qemu->IsValid()) delete qemu;
    else if (!vfs.AddFileSystem(MathLib::VFSEntry(qemu, "sys/qemu"_M))) return false;
    return true;
}
[[noreturn]] void ShutdownArch(void) {
    // TODO: Perform ACPI shutdown
    // Bochs shutdown
    WritePort<uint16_t>(0xb004, 0x2000);
    // QEMU shutdown
    WritePort<uint16_t>(0x0604, 0x2000);
    // VirtualBox shutdown
    WritePort<uint16_t>(0x4004, 0x3400);
    // Cloud Hypervisor shutdown
    WritePort<uint16_t>(0x0600, 0x0034);
    mainTimer->Sleep(MathLib::eps);
    MathLib::Panic("It's now safe to turn off your computer");
}
[[noreturn]] void ArchPanic(void) {
    if (cmos) cmos->SetNMI(false);
    ArchSetInterrupts(false);
    while (true) asm volatile("hlt");
}

#endif