#ifdef __x86_64__
#include "Interrupts/Exceptions/DivisionError.hpp"
#include "Interrupts/Interrupts.hpp"
#include "SerialPort/SerialPort.hpp"
#include "Multiboot/Multiboot1.hpp"
#include "Multiboot/Multiboot2.hpp"
#include "QEMU/QEMUFileSystem.hpp"
#include "ControlRegisters.hpp"
#include "../../VFS.hpp"
#include "ACPI/ACPI.hpp"
#include "GDT/TSS.hpp"
#include "../Arch.hpp"
#include "CPUID.hpp"
#include "CMOS.hpp"
#include "Math.hpp"
#include "MSR.hpp"
#include "PIT.hpp"
#include "E9.hpp"
#include "IO.hpp"
#include <Logger.hpp>
#include <String.hpp>

bool InitLogger(MathLib::Writable* logger, const MathLib::Sequence<char>& str) {
    #ifdef NoLoggerStartMessage
    (void)logger;
    (void)str;
    return true;
    #else
    return logger->Puts(str);
    #endif
}
bool InitArch(uintptr_t signature, void* info) {
    if (E9::IsPresent()) {
        E9* e9 = new E9();
        if (e9 && InitLogger(e9, "E9 logger initialized\n"_M)) MathLib::logger = e9;
        else delete e9;
    }
    do {
        const SerialPort::Port ports[] = {
            SerialPort::Port::COM1, SerialPort::Port::COM2,
            SerialPort::Port::COM3, SerialPort::Port::COM4,
            SerialPort::Port::COM5, SerialPort::Port::COM6,
            SerialPort::Port::COM7, SerialPort::Port::COM8,
        };
        for (size_t i = 0; i < SizeOfArray(ports); i++) {
            SerialPort* serial = new SerialPort(ports[i]);
            if (serial && serial->Init() && InitLogger(serial, "COM"_M + MathLib::ToString(i, 10) + " initialized\n") && !MathLib::logger) MathLib::logger = serial;
            else delete serial;
        }
    } while (false);
    InitTSS();
    if (HasMSR()) {
        const MathLib::Expected<uint64_t> tmp = GetMSR(0xc0000080);
        if (!tmp.HasValue() || !SetMSR(0xc0000080, tmp.Get() | 1 << (uint8_t)EFER::NoExecuteEnable)) return false;
    }
    if (!InitMath()) return false;
    uintptr_t b;
    CPUID(0x7, nullptr, &b, nullptr, nullptr);
    if (b & 1 << (uint8_t)CPUIDBits::B7SMEP) {
        const MathLib::Expected<uintptr_t> tmp = GetControlRegister(4);
        if (!tmp.HasValue() || !SetControlRegister(4, tmp.Get() | 1 << (uint8_t)ControlRegister4::SupervisorModeExecutionsProtectionEnable)) return false;
    }
    if (b & 1 << (uint8_t)CPUIDBits::B7SMAP) {
        const MathLib::Expected<uintptr_t> tmp = GetControlRegister(4);
        if (!tmp.HasValue() || !SetControlRegister(4, tmp.Get() | 1 << (uint8_t)ControlRegister4::SupervisorModeAccessProtectionEnable)) return false;
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
    // TODO: Create new paging structure
    if (!rsdp) rsdp = FindRSDP();
    if (!InitACPI(rsdp, true)) return false;
    const CMOSFloppyData floppyData = cmos->GetFloppyData();
    LogString("CMOS master floppy data: 0x"_M + MathLib::ToString(floppyData.master, 16) + '\n');
    LogString("CMOS slave floppy data: 0x"_M + MathLib::ToString(floppyData.slave, 16) + '\n');
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
    else if (!vfs.AddFileSystem(MathLib::VFSEntry(qemu, "qemu"_M))) return false;
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