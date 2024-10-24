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
    LogString(String("\tSignature: ") + signature + '\n');
    LogString(String("\tChecksum: 0x") + ToString(rsdp->checksum, 16) + '\n');
    LogString(String("\tOEM ID: ") + oemID + '\n');
    LogString(String("\tRevision: 0x") + ToString(rsdp->revision, 16) + '\n');
    LogString(String("\tRSDT address: 0x") + ToString(rsdp->rsdt, 16) + '\n');
    if (rsdp->revision == 2) {
        const RSDP2* rsdp2 = (const RSDP2*)rsdp;
        LogString(String("\tLength: 0x") + ToString(rsdp2->length, 16) + '\n');
        LogString(String("\tXSDT address: 0x") + ToString(rsdp2->xsdt, 16) + '\n');
        LogString(String("\tExtended checksum: 0x") + ToString(rsdp2->extendedChecksum, 16) + '\n');
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
        LogString(String("ACPI table ") + ToString(i) + ": {\n");
        LogString(String("\tSignature: ") + signature + '\n');
        LogString(String("\tLength: 0x") + ToString(table->length, 16) + '\n');
        LogString(String("\tRevision: 0x") + ToString(table->revision, 16) + '\n');
        LogString(String("\tChecksum: 0x") + ToString(table->checksum, 16) + '\n');
        LogString(String("\tOEM ID: ") + oemID + '\n');
        LogString(String("\tOEM table ID: ") + oemTableID + '\n');
        LogString(String("\tOEM revision: 0x") + ToString(table->oemRevision, 16) + '\n');
        LogString(String("\tCreator ID: 0x") + ToString(table->creatorID, 16) + '\n');
        LogString(String("\tCreator revision: 0x") + ToString(table->creatorRevision, 16) + '\n');
        if (String(signature) == String(FADT::expectedSignature)) {
            fadt = (FADT*)table;
            LogString(String("\tFACS: 0x") + ToString(fadt->facs, 16) + '\n');
            LogString(String("\tDSDT: 0x") + ToString(fadt->dsdt, 16) + '\n');
            LogString(String("\tSCI interrupt: 0x") + ToString(fadt->sciInterrupt, 16) + '\n');
            LogString(String("\tSMI command port: 0x") + ToString(fadt->smiCommandPort, 16) + '\n');
            LogString(String("\tACPI enable: 0x") + ToString(fadt->acpiEnable, 16) + '\n');
            LogString(String("\tACPI disable: 0x") + ToString(fadt->acpiDisable, 16) + '\n');
            LogString(String("\tRTC century register: 0x") + ToString((uint8_t)fadt->rtcCenturyRegister, 16) + '\n');
            LogString(String("\tACPI boot architecture flags: 0x") + ToString(fadt->bootArchitectureFlags, 16) + '\n');
            if (fadt->revision >= 0x02) {
                const FADT2* fadt2 = (const FADT2*)fadt;
                LogString(String("\tACPI 2.0 flags: 0x") + ToString(fadt2->flags, 16) + '\n');
                LogString(String("\tACPI 2.0 FACS: 0x") + ToString(fadt2->facs2, 16) + '\n');
                LogString(String("\tACPI 2.0 DSDT: 0x") + ToString(fadt2->dsdt2, 16) + '\n');
            }
        }
        else if (String(signature) == String(MADT::expectedSignature)) {
            madt = (const MADT*)table;
            LogString(String("\tLocal APIC address: 0x") + ToString(madt->localAPIC, 16) + '\n');
        }
        else if (String(signature) == String(MCFG::expectedSignature)) {
            mcfg = (const MCFG*)table;
            LogString(String("\tReserved: 0x") + ToString(mcfg->reserved, 16) + '\n');
        }
        else if (String(signature) == String(HPET::expectedSignature)) {
            const HPET* hpet = (const HPET*)table;
            LogString(String("\tHardware revision: 0x") + ToString(hpet->hardwareRevision, 16) + '\n');
            LogString(String("\tComparator count: 0x") + ToString(hpet->comparatorCount, 16) + '\n');
            LogString(String("\tComparator size: 0x") + ToString(hpet->counterSize, 16) + '\n');
            LogString(String("\tReserved: 0x") + ToString(hpet->reserved, 16) + '\n');
            LogString(String("\tLegacy IRQ replacement: ") + BoolToString(hpet->legacyIRQReplacement) + '\n');
            LogString(String("\tPCI vendor: 0x") + ToString(hpet->pciVendor, 16) + '\n');
            LogString(String("\tNumber: 0x") + ToString(hpet->number, 16) + '\n');
            LogString(String("\tMinimum tick: 0x") + ToString(hpet->minimumTick, 16) + '\n');
            LogString(String("\tPage protection: 0x") + ToString(hpet->pageProtection, 16) + '\n');
        }
        else if (String(signature) == String(WAET::expectedSignature)) {
            const WAET* waet = (const WAET*)table;
            LogString(String("\tEnhanced RTC: ") + BoolToString(waet->enhancedRTC) + '\n');
            LogString(String("\tEnhanced ACPI PM timer: ") + BoolToString(waet->enhancedACPIPMTimer) + '\n');
            LogString(String("\tReserved: 0x") + ToString(waet->reserved, 16) + '\n');
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
                    LogString(String("\tACPI processor ID: 0x") + ToString(lapic->acpiProcessorID, 16) + '\n');
                    LogString(String("\tAPIC ID: 0x") + ToString(lapic->apicID, 16) + '\n');
                    LogString(String("\tFlags: 0x") + ToString(lapic->flags, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::IOAPIC: {
                    const MADTIOAPIC* ioapic = (const MADTIOAPIC*)entry;
                    LogString("MADT IO APIC: {\n");
                    LogString(String("\tIO APIC ID: 0x") + ToString(ioapic->ioAPICID, 16) + '\n');
                    LogString(String("\tIO APIC address: 0x") + ToString(ioapic->ioAPIC, 16) + '\n');
                    LogString(String("\tGlobal system interrupt address: 0x") + ToString(ioapic->gsiBase, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::IOAPICInterruptSourceOverride: {
                    const MADTIOAPICInterruptSourceOverride* ioapicISO = (const MADTIOAPICInterruptSourceOverride*)entry;
                    LogString("MADT IO APIC interrupt source override: {\n");
                    LogString(String("\tBus source: 0x") + ToString(ioapicISO->busSource, 16) + '\n');
                    LogString(String("\tIRQ source: 0x") + ToString(ioapicISO->irqSource, 16) + '\n');
                    LogString(String("\tGlobal system interrupt: 0x") + ToString(ioapicISO->gsi, 16) + '\n');
                    LogString(String("\tFlags: 0x") + ToString(ioapicISO->flags, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::IOAPICNonMaskableInterruptSource: {
                    const MADTIOAPICNonMaskableInterruptSource* ioapicNMISource = (const MADTIOAPICNonMaskableInterruptSource*)entry;
                    LogString("MADT IO APIC non maskable interrupt source: {\n");
                    LogString(String("\tNon maskable interrupt source: 0x") + ToString(ioapicNMISource->nmiSource, 16) + '\n');
                    LogString(String("\tFlags: 0x") + ToString(ioapicNMISource->flags, 16) + '\n');
                    LogString(String("\tGlobal system interrupt: 0x") + ToString(ioapicNMISource->gsi, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::LocalAPICNonMaskableInterrupts: {
                    const MADTLocalAPICNonMaskableInterrupts* lapicNMI = (const MADTLocalAPICNonMaskableInterrupts*)entry;
                    LogString("MADT local APIC non maskable interrupts: {\n");
                    LogString(String("\tACPI processor ID: ") + (lapicNMI->acpiProcessorID == UINT8_MAX ? "all" : String("0x") + ToString(lapicNMI->acpiProcessorID, 16)) + '\n');
                    LogString(String("\tFlags: 0x") + ToString(lapicNMI->flags, 16) + '\n');
                    LogString(String("\tLINT: 0x") + ToString(lapicNMI->lint, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                case MADTEntry::Type::LocalAPICAddressOverride: {
                    lapic = (LocalAPIC*)((const MADTLocalAPICAddressOverride*)entry)->address;
                    LogString(String("MADT local APIC address override: 0x") + ToString((uintptr_t)lapic, 16) + '\n');
                    break;
                }
                case MADTEntry::Type::ProcessorLocalX2APIC: {
                    const MADTProcessorLocalX2APIC* localX2APIC = (const MADTProcessorLocalX2APIC*)entry;
                    LogString("MADT processor local X2 APIC: {\n");
                    LogString(String("\tLocal X2 APIC ID: 0x") + ToString(localX2APIC->localX2APICID, 16) + '\n');
                    LogString(String("\tFlags: 0x") + ToString(localX2APIC->flags, 16) + '\n');
                    LogString(String("\tACPI ID: 0x") + ToString(localX2APIC->acpiID, 16) + '\n');
                    LogString("}\n");
                    break;
                }
                default: {
                    LogString(String("Unknown MADT entry type: ") + ToString((uint8_t)entry->type) + '\n');
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