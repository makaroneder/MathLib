#ifdef __x86_64__
#include "SCI.hpp"
#include "ACPI.hpp"
#include "MADT.hpp"
#include "HPET.hpp"
#include "WAET.hpp"
#include "../RTC.hpp"
#include "LocalAPIC.hpp"
#include "../PCI/PCI.hpp"
#include "../PS2/PS2.hpp"
#include <Logger.hpp>
#include <String.hpp>
#include <Host.hpp>
#include <stddef.h>

SCI sci;
[[nodiscard]] bool InitRTC(bool nmi, CMOS::Register centuryRegister) {
    RTC* rtc = new RTC(nmi, centuryRegister);
    if (!rtc) return false;
    cmos = rtc;
    dateKeeper = rtc;
    return true;
}
[[nodiscard]] bool NoFADTInit(bool nmi) {
    if (!InitRTC(nmi, (CMOS::Register)0)) return false;
    InitPS2();
    return true;
}
bool InitACPI(const RSDP* rsdp, bool nmi) {
    if (!rsdp || !rsdp->IsValid()) return NoFADTInit(nmi);
    char signature[9];
    for (size_t i = 0; i < 8; i++) signature[i] = rsdp->signature[i];
    signature[8] = '\0';
    char oemID[7];
    for (size_t i = 0; i < 6; i++) oemID[i] = rsdp->oemID[i];
    oemID[6] = '\0';
    LogString("RSDP: {\n");
    LogString("\tSignature: "_M + signature + '\n');
    LogString("\tChecksum: 0x"_M + MathLib::ToString(rsdp->checksum, 16) + '\n');
    LogString("\tOEM ID: "_M + oemID + '\n');
    LogString("\tRevision: 0x"_M + MathLib::ToString(rsdp->revision, 16) + '\n');
    LogString("\tRSDT address: 0x"_M + MathLib::ToString(rsdp->rsdt, 16) + '\n');
    if (rsdp->revision == 2) {
        const RSDP2* rsdp2 = (const RSDP2*)rsdp;
        LogString("\tLength: 0x"_M + MathLib::ToString(rsdp2->length, 16) + '\n');
        LogString("\tXSDT address: 0x"_M + MathLib::ToString(rsdp2->xsdt, 16) + '\n');
        LogString("\tExtended checksum: 0x"_M + MathLib::ToString(rsdp2->extendedChecksum, 16) + '\n');
    }
    LogString("}\n");
    MathLib::ACPITable* rsdt = (MathLib::ACPITable*)(uintptr_t)rsdp->rsdt;
    size_t pointerSize = sizeof(uint32_t);
    if (rsdp->revision == 2) {
        RSDP2* rsdp2 = (RSDP2*)rsdp;
        if (rsdp2->IsValid()) {
            rsdt = (MathLib::ACPITable*)rsdp2->xsdt;
            pointerSize = sizeof(uint64_t);
        }
    }
    const size_t entries = (rsdt->length - sizeof(MathLib::ACPITable)) / pointerSize;
    FADT* fadt = nullptr;
    const MADT* madt = nullptr;
    const MCFG* mcfg = nullptr;
    for (size_t i = 0; i < entries; i++) {
        const uintptr_t offset = (uintptr_t)rsdt + sizeof(MathLib::ACPITable) + (i * pointerSize);
        const MathLib::ACPITable* table = (const MathLib::ACPITable*)(rsdp->revision == 2 ? *(uint64_t*)offset : (uintptr_t)*(uint32_t*)offset);
        if (!table->IsValid()) continue;
        char signature[5];
        for (size_t i = 0; i < SizeOfArray(table->signature); i++) signature[i] = table->signature[i];
        signature[4] = '\0';
        char oemID[7];
        for (size_t i = 0; i < SizeOfArray(table->oemID); i++) oemID[i] = table->oemID[i];
        oemID[6] = '\0';
        char oemTableID[9];
        for (size_t i = 0; i < SizeOfArray(table->oemTableID); i++) oemTableID[i] = table->oemTableID[i];
        oemTableID[8] = '\0';
        LogString("ACPI table "_M + MathLib::ToString(i) + ": {\n");
        LogString("\tSignature: "_M + signature + '\n');
        LogString("\tLength: 0x"_M + MathLib::ToString(table->length, 16) + '\n');
        LogString("\tRevision: 0x"_M + MathLib::ToString(table->revision, 16) + '\n');
        LogString("\tChecksum: 0x"_M + MathLib::ToString(table->checksum, 16) + '\n');
        LogString("\tOEM ID: "_M + oemID + '\n');
        LogString("\tOEM table ID: "_M + oemTableID + '\n');
        LogString("\tOEM revision: 0x"_M + MathLib::ToString(table->oemRevision, 16) + '\n');
        LogString("\tCreator ID: 0x"_M + MathLib::ToString(table->creatorID, 16) + '\n');
        LogString("\tCreator revision: 0x"_M + MathLib::ToString(table->creatorRevision, 16) + '\n');
        if (MathLib::String(signature) == MathLib::String(FADT::expectedSignature)) {
            fadt = (FADT*)table;
            LogString("\tFACS: 0x"_M + MathLib::ToString(fadt->facs, 16) + '\n');
            LogString("\tDSDT: 0x"_M + MathLib::ToString(fadt->dsdt, 16) + '\n');
            LogString("\tSCI interrupt: 0x"_M + MathLib::ToString(fadt->sciInterrupt, 16) + '\n');
            LogString("\tSMI command port: 0x"_M + MathLib::ToString(fadt->smiCommandPort, 16) + '\n');
            LogString("\tACPI enable: 0x"_M + MathLib::ToString(fadt->acpiEnable, 16) + '\n');
            LogString("\tACPI disable: 0x"_M + MathLib::ToString(fadt->acpiDisable, 16) + '\n');
            LogString("\tRTC century register: 0x"_M + MathLib::ToString((uint8_t)fadt->rtcCenturyRegister, 16) + '\n');
            LogString("\tACPI boot architecture flags: 0x"_M + MathLib::ToString(fadt->bootArchitectureFlags, 16) + '\n');
            if (fadt->revision >= 0x02) {
                const FADT2* fadt2 = (const FADT2*)fadt;
                LogString("\tACPI 2.0 flags: 0x"_M + MathLib::ToString(fadt2->flags, 16) + '\n');
                LogString("\tACPI 2.0 FACS: 0x"_M + MathLib::ToString(fadt2->facs2, 16) + '\n');
                LogString("\tACPI 2.0 DSDT: 0x"_M + MathLib::ToString(fadt2->dsdt2, 16) + '\n');
            }
        }
        else if (MathLib::String(signature) == MathLib::String(MADT::expectedSignature)) {
            madt = (const MADT*)table;
            LogString("\tLocal APIC address: 0x"_M + MathLib::ToString(madt->localAPIC, 16) + '\n');
        }
        else if (MathLib::String(signature) == MathLib::String(MCFG::expectedSignature)) {
            mcfg = (const MCFG*)table;
            LogString("\tReserved: 0x"_M + MathLib::ToString(mcfg->reserved, 16) + '\n');
        }
        else if (MathLib::String(signature) == MathLib::String(HPET::expectedSignature)) {
            const HPET* hpet = (const HPET*)table;
            LogString("\tHardware revision: 0x"_M + MathLib::ToString(hpet->hardwareRevision, 16) + '\n');
            LogString("\tComparator count: 0x"_M + MathLib::ToString(hpet->comparatorCount, 16) + '\n');
            LogString("\tComparator size: 0x"_M + MathLib::ToString(hpet->counterSize, 16) + '\n');
            LogString("\tReserved: 0x"_M + MathLib::ToString(hpet->reserved, 16) + '\n');
            LogString("\tLegacy IRQ replacement: "_M + MathLib::BoolToString(hpet->legacyIRQReplacement) + '\n');
            LogString("\tPCI vendor: 0x"_M + MathLib::ToString(hpet->pciVendor, 16) + '\n');
            LogString("\tNumber: 0x"_M + MathLib::ToString(hpet->number, 16) + '\n');
            LogString("\tMinimum tick: 0x"_M + MathLib::ToString(hpet->minimumTick, 16) + '\n');
            LogString("\tPage protection: 0x"_M + MathLib::ToString(hpet->pageProtection, 16) + '\n');
        }
        else if (MathLib::String(signature) == MathLib::String(WAET::expectedSignature)) {
            const WAET* waet = (const WAET*)table;
            LogString("\tEnhanced RTC: "_M + MathLib::BoolToString(waet->enhancedRTC) + '\n');
            LogString("\tEnhanced ACPI PM timer: "_M + MathLib::BoolToString(waet->enhancedACPIPMTimer) + '\n');
            LogString("\tReserved: 0x"_M + MathLib::ToString(waet->reserved, 16) + '\n');
        }
        LogString("}\n");
    }
    if (madt) {
        if (!madt->dual8259) return false;
        LocalAPIC* lapic = (LocalAPIC*)(uintptr_t)madt->localAPIC;
        for (const MADTEntry* entry = (MADTEntry*)((uintptr_t)madt + sizeof(MADT)); (uintptr_t)entry < (uintptr_t)madt + madt->length; entry = (MADTEntry*)((uintptr_t)entry + entry->length)) {
            switch (entry->type) {
                case MADTEntry::Type::ProcessorLocalAPIC: {
                    const MADTProcessorLocalAPIC* lapic = (const MADTProcessorLocalAPIC*)entry;
                    LogString("MADT processor local APIC: {\n");
                    LogString("\tACPI processor ID: 0x"_M + MathLib::ToString(lapic->acpiProcessorID, 16) + '\n');
                    LogString("\tAPIC ID: 0x"_M + MathLib::ToString(lapic->apicID, 16) + '\n');
                    LogString("\tFlags: 0x"_M + MathLib::ToString(lapic->flags, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::IOAPIC: {
                    const MADTIOAPIC* ioapic = (const MADTIOAPIC*)entry;
                    LogString("MADT IO APIC: {\n");
                    LogString("\tIO APIC ID: 0x"_M + MathLib::ToString(ioapic->ioAPICID, 16) + '\n');
                    LogString("\tIO APIC address: 0x"_M + MathLib::ToString(ioapic->ioAPIC, 16) + '\n');
                    LogString("\tGlobal system interrupt address: 0x"_M + MathLib::ToString(ioapic->gsiBase, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::IOAPICInterruptSourceOverride: {
                    const MADTIOAPICInterruptSourceOverride* ioapicISO = (const MADTIOAPICInterruptSourceOverride*)entry;
                    LogString("MADT IO APIC interrupt source override: {\n");
                    LogString("\tBus source: 0x"_M + MathLib::ToString(ioapicISO->busSource, 16) + '\n');
                    LogString("\tIRQ source: 0x"_M + MathLib::ToString(ioapicISO->irqSource, 16) + '\n');
                    LogString("\tGlobal system interrupt: 0x"_M + MathLib::ToString(ioapicISO->gsi, 16) + '\n');
                    LogString("\tFlags: 0x"_M + MathLib::ToString(ioapicISO->flags, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::IOAPICNonMaskableInterruptSource: {
                    const MADTIOAPICNonMaskableInterruptSource* ioapicNMISource = (const MADTIOAPICNonMaskableInterruptSource*)entry;
                    LogString("MADT IO APIC non maskable interrupt source: {\n");
                    LogString("\tNon maskable interrupt source: 0x"_M + MathLib::ToString(ioapicNMISource->nmiSource, 16) + '\n');
                    LogString("\tFlags: 0x"_M + MathLib::ToString(ioapicNMISource->flags, 16) + '\n');
                    LogString("\tGlobal system interrupt: 0x"_M + MathLib::ToString(ioapicNMISource->gsi, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::LocalAPICNonMaskableInterrupts: {
                    const MADTLocalAPICNonMaskableInterrupts* lapicNMI = (const MADTLocalAPICNonMaskableInterrupts*)entry;
                    LogString("MADT local APIC non maskable interrupts: {\n");
                    LogString("\tACPI processor ID: "_M + (lapicNMI->acpiProcessorID == UINT8_MAX ? "all" : "0x"_M + MathLib::ToString(lapicNMI->acpiProcessorID, 16)) + '\n');
                    LogString("\tFlags: 0x"_M + MathLib::ToString(lapicNMI->flags, 16) + '\n');
                    LogString("\tLINT: 0x"_M + MathLib::ToString(lapicNMI->lint, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::LocalAPICAddressOverride: {
                    lapic = (LocalAPIC*)((const MADTLocalAPICAddressOverride*)entry)->address;
                    LogString("MADT local APIC address override: 0x"_M + MathLib::ToString((uintptr_t)lapic, 16) + '\n');
                    break;
                }
                case MADTEntry::Type::ProcessorLocalX2APIC: {
                    const MADTProcessorLocalX2APIC* localX2APIC = (const MADTProcessorLocalX2APIC*)entry;
                    LogString("MADT processor local X2 APIC: {\n");
                    LogString("\tLocal X2 APIC ID: 0x"_M + MathLib::ToString(localX2APIC->localX2APICID, 16) + '\n');
                    LogString("\tFlags: 0x"_M + MathLib::ToString(localX2APIC->flags, 16) + '\n');
                    LogString("\tACPI ID: 0x"_M + MathLib::ToString(localX2APIC->acpiID, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                default: {
                    LogString("Unknown MADT entry type: "_M + MathLib::ToString((uint8_t)entry->type) + '\n');
                    break;
                }
            }
        }
    }
    if (mcfg && !InitPCI(mcfg)) return false;
    if (fadt) {
        sci = SCI(fadt);
        if (fadt->bootArchitectureFlags & (1 << 1) || !fadt->bootArchitectureFlags) InitPS2();
        if (fadt->bootArchitectureFlags & (1 << 5)) {
            cmos = new CMOS(nmi);
            if (!cmos) return false;
        }
        else if (!InitRTC(nmi, fadt->rtcCenturyRegister)) return false;
    }
    else if (!NoFADTInit(nmi)) return false;
    // TODO: Parse AML
    // TODO: Init SMP
    // TODO: Init IO APIC
    // TODO: Init local APIC
    return true;
}

#endif