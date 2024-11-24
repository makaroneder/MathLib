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
bool InitRTC(bool nmi, CMOS::Register centuryRegister) {
    RTC* rtc = new RTC(nmi, centuryRegister);
    if (!rtc) return false;
    cmos = rtc;
    dateKeeper = rtc;
    return true;
}
bool NoFADTInit(bool nmi) {
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
    LogString(MathLib::String("\tSignature: ") + signature + '\n');
    LogString(MathLib::String("\tChecksum: 0x") + MathLib::ToString(rsdp->checksum, 16) + '\n');
    LogString(MathLib::String("\tOEM ID: ") + oemID + '\n');
    LogString(MathLib::String("\tRevision: 0x") + MathLib::ToString(rsdp->revision, 16) + '\n');
    LogString(MathLib::String("\tRSDT address: 0x") + MathLib::ToString(rsdp->rsdt, 16) + '\n');
    if (rsdp->revision == 2) {
        const RSDP2* rsdp2 = (const RSDP2*)rsdp;
        LogString(MathLib::String("\tLength: 0x") + MathLib::ToString(rsdp2->length, 16) + '\n');
        LogString(MathLib::String("\tXSDT address: 0x") + MathLib::ToString(rsdp2->xsdt, 16) + '\n');
        LogString(MathLib::String("\tExtended checksum: 0x") + MathLib::ToString(rsdp2->extendedChecksum, 16) + '\n');
    }
    LogString("}\n");
    ACPITable* rsdt = (ACPITable*)(uintptr_t)rsdp->rsdt;
    size_t pointerSize = sizeof(uint32_t);
    if (rsdp->revision == 2) {
        RSDP2* rsdp2 = (RSDP2*)rsdp;
        if (rsdp2->IsValid()) {
            rsdt = (ACPITable*)rsdp2->xsdt;
            pointerSize = sizeof(uint64_t);
        }
    }
    const size_t entries = (rsdt->length - sizeof(ACPITable)) / pointerSize;
    FADT* fadt = nullptr;
    const MADT* madt = nullptr;
    const MCFG* mcfg = nullptr;
    for (size_t i = 0; i < entries; i++) {
        const uintptr_t offset = (uintptr_t)rsdt + sizeof(ACPITable) + (i * pointerSize);
        const ACPITable* table = (const ACPITable*)(rsdp->revision == 2 ? *(uint64_t*)offset : (uintptr_t)*(uint32_t*)offset);
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
        LogString(MathLib::String("ACPI table ") + MathLib::ToString(i) + ": {\n");
        LogString(MathLib::String("\tSignature: ") + signature + '\n');
        LogString(MathLib::String("\tLength: 0x") + MathLib::ToString(table->length, 16) + '\n');
        LogString(MathLib::String("\tRevision: 0x") + MathLib::ToString(table->revision, 16) + '\n');
        LogString(MathLib::String("\tChecksum: 0x") + MathLib::ToString(table->checksum, 16) + '\n');
        LogString(MathLib::String("\tOEM ID: ") + oemID + '\n');
        LogString(MathLib::String("\tOEM table ID: ") + oemTableID + '\n');
        LogString(MathLib::String("\tOEM revision: 0x") + MathLib::ToString(table->oemRevision, 16) + '\n');
        LogString(MathLib::String("\tCreator ID: 0x") + MathLib::ToString(table->creatorID, 16) + '\n');
        LogString(MathLib::String("\tCreator revision: 0x") + MathLib::ToString(table->creatorRevision, 16) + '\n');
        if (MathLib::String(signature) == MathLib::String(FADT::expectedSignature)) {
            fadt = (FADT*)table;
            LogString(MathLib::String("\tFACS: 0x") + MathLib::ToString(fadt->facs, 16) + '\n');
            LogString(MathLib::String("\tDSDT: 0x") + MathLib::ToString(fadt->dsdt, 16) + '\n');
            LogString(MathLib::String("\tSCI interrupt: 0x") + MathLib::ToString(fadt->sciInterrupt, 16) + '\n');
            LogString(MathLib::String("\tSMI command port: 0x") + MathLib::ToString(fadt->smiCommandPort, 16) + '\n');
            LogString(MathLib::String("\tACPI enable: 0x") + MathLib::ToString(fadt->acpiEnable, 16) + '\n');
            LogString(MathLib::String("\tACPI disable: 0x") + MathLib::ToString(fadt->acpiDisable, 16) + '\n');
            LogString(MathLib::String("\tRTC century register: 0x") + MathLib::ToString((uint8_t)fadt->rtcCenturyRegister, 16) + '\n');
            LogString(MathLib::String("\tACPI boot architecture flags: 0x") + MathLib::ToString(fadt->bootArchitectureFlags, 16) + '\n');
            if (fadt->revision >= 0x02) {
                const FADT2* fadt2 = (const FADT2*)fadt;
                LogString(MathLib::String("\tACPI 2.0 flags: 0x") + MathLib::ToString(fadt2->flags, 16) + '\n');
                LogString(MathLib::String("\tACPI 2.0 FACS: 0x") + MathLib::ToString(fadt2->facs2, 16) + '\n');
                LogString(MathLib::String("\tACPI 2.0 DSDT: 0x") + MathLib::ToString(fadt2->dsdt2, 16) + '\n');
            }
        }
        else if (MathLib::String(signature) == MathLib::String(MADT::expectedSignature)) {
            madt = (const MADT*)table;
            LogString(MathLib::String("\tLocal APIC address: 0x") + MathLib::ToString(madt->localAPIC, 16) + '\n');
        }
        else if (MathLib::String(signature) == MathLib::String(MCFG::expectedSignature)) {
            mcfg = (const MCFG*)table;
            LogString(MathLib::String("\tReserved: 0x") + MathLib::ToString(mcfg->reserved, 16) + '\n');
        }
        else if (MathLib::String(signature) == MathLib::String(HPET::expectedSignature)) {
            const HPET* hpet = (const HPET*)table;
            LogString(MathLib::String("\tHardware revision: 0x") + MathLib::ToString(hpet->hardwareRevision, 16) + '\n');
            LogString(MathLib::String("\tComparator count: 0x") + MathLib::ToString(hpet->comparatorCount, 16) + '\n');
            LogString(MathLib::String("\tComparator size: 0x") + MathLib::ToString(hpet->counterSize, 16) + '\n');
            LogString(MathLib::String("\tReserved: 0x") + MathLib::ToString(hpet->reserved, 16) + '\n');
            LogString(MathLib::String("\tLegacy IRQ replacement: ") + MathLib::BoolToString(hpet->legacyIRQReplacement) + '\n');
            LogString(MathLib::String("\tPCI vendor: 0x") + MathLib::ToString(hpet->pciVendor, 16) + '\n');
            LogString(MathLib::String("\tNumber: 0x") + MathLib::ToString(hpet->number, 16) + '\n');
            LogString(MathLib::String("\tMinimum tick: 0x") + MathLib::ToString(hpet->minimumTick, 16) + '\n');
            LogString(MathLib::String("\tPage protection: 0x") + MathLib::ToString(hpet->pageProtection, 16) + '\n');
        }
        else if (MathLib::String(signature) == MathLib::String(WAET::expectedSignature)) {
            const WAET* waet = (const WAET*)table;
            LogString(MathLib::String("\tEnhanced RTC: ") + MathLib::BoolToString(waet->enhancedRTC) + '\n');
            LogString(MathLib::String("\tEnhanced ACPI PM timer: ") + MathLib::BoolToString(waet->enhancedACPIPMTimer) + '\n');
            LogString(MathLib::String("\tReserved: 0x") + MathLib::ToString(waet->reserved, 16) + '\n');
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
                    LogString(MathLib::String("\tACPI processor ID: 0x") + MathLib::ToString(lapic->acpiProcessorID, 16) + '\n');
                    LogString(MathLib::String("\tAPIC ID: 0x") + MathLib::ToString(lapic->apicID, 16) + '\n');
                    LogString(MathLib::String("\tFlags: 0x") + MathLib::ToString(lapic->flags, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::IOAPIC: {
                    const MADTIOAPIC* ioapic = (const MADTIOAPIC*)entry;
                    LogString("MADT IO APIC: {\n");
                    LogString(MathLib::String("\tIO APIC ID: 0x") + MathLib::ToString(ioapic->ioAPICID, 16) + '\n');
                    LogString(MathLib::String("\tIO APIC address: 0x") + MathLib::ToString(ioapic->ioAPIC, 16) + '\n');
                    LogString(MathLib::String("\tGlobal system interrupt address: 0x") + MathLib::ToString(ioapic->gsiBase, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::IOAPICInterruptSourceOverride: {
                    const MADTIOAPICInterruptSourceOverride* ioapicISO = (const MADTIOAPICInterruptSourceOverride*)entry;
                    LogString("MADT IO APIC interrupt source override: {\n");
                    LogString(MathLib::String("\tBus source: 0x") + MathLib::ToString(ioapicISO->busSource, 16) + '\n');
                    LogString(MathLib::String("\tIRQ source: 0x") + MathLib::ToString(ioapicISO->irqSource, 16) + '\n');
                    LogString(MathLib::String("\tGlobal system interrupt: 0x") + MathLib::ToString(ioapicISO->gsi, 16) + '\n');
                    LogString(MathLib::String("\tFlags: 0x") + MathLib::ToString(ioapicISO->flags, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::IOAPICNonMaskableInterruptSource: {
                    const MADTIOAPICNonMaskableInterruptSource* ioapicNMISource = (const MADTIOAPICNonMaskableInterruptSource*)entry;
                    LogString("MADT IO APIC non maskable interrupt source: {\n");
                    LogString(MathLib::String("\tNon maskable interrupt source: 0x") + MathLib::ToString(ioapicNMISource->nmiSource, 16) + '\n');
                    LogString(MathLib::String("\tFlags: 0x") + MathLib::ToString(ioapicNMISource->flags, 16) + '\n');
                    LogString(MathLib::String("\tGlobal system interrupt: 0x") + MathLib::ToString(ioapicNMISource->gsi, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::LocalAPICNonMaskableInterrupts: {
                    const MADTLocalAPICNonMaskableInterrupts* lapicNMI = (const MADTLocalAPICNonMaskableInterrupts*)entry;
                    LogString("MADT local APIC non maskable interrupts: {\n");
                    LogString(MathLib::String("\tACPI processor ID: ") + (lapicNMI->acpiProcessorID == UINT8_MAX ? "all" : MathLib::String("0x") + MathLib::ToString(lapicNMI->acpiProcessorID, 16)) + '\n');
                    LogString(MathLib::String("\tFlags: 0x") + MathLib::ToString(lapicNMI->flags, 16) + '\n');
                    LogString(MathLib::String("\tLINT: 0x") + MathLib::ToString(lapicNMI->lint, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::LocalAPICAddressOverride: {
                    lapic = (LocalAPIC*)((const MADTLocalAPICAddressOverride*)entry)->address;
                    LogString(MathLib::String("MADT local APIC address override: 0x") + MathLib::ToString((uintptr_t)lapic, 16) + '\n');
                    break;
                }
                case MADTEntry::Type::ProcessorLocalX2APIC: {
                    const MADTProcessorLocalX2APIC* localX2APIC = (const MADTProcessorLocalX2APIC*)entry;
                    LogString("MADT processor local X2 APIC: {\n");
                    LogString(MathLib::String("\tLocal X2 APIC ID: 0x") + MathLib::ToString(localX2APIC->localX2APICID, 16) + '\n');
                    LogString(MathLib::String("\tFlags: 0x") + MathLib::ToString(localX2APIC->flags, 16) + '\n');
                    LogString(MathLib::String("\tACPI ID: 0x") + MathLib::ToString(localX2APIC->acpiID, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                default: {
                    LogString(MathLib::String("Unknown MADT entry type: ") + MathLib::ToString((uint8_t)entry->type) + '\n');
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