#include "SCI.hpp"
#include "ACPI.hpp"
#include "MADT.hpp"
#include "../E9.hpp"
#include "../IO.hpp"
#include "LocalAPIC.hpp"
#include <String.hpp>
#include <Host.hpp>
#include <stddef.h>

SCI sci;
void InitACPI(const RSDP* rsdp) {
    if (!rsdp || !rsdp->IsValid()) return;
    ACPITable* rsdt = (ACPITable*)(uintptr_t)rsdp->rsdt;
    size_t pointerSize = sizeof(uint32_t);
    const bool acpi2 = rsdp->revision == 2;
    if (acpi2) {
        RSDP2* rsdp2 = (RSDP2*)rsdp;
        if (!rsdp2->IsValid()) return;
        rsdt = (ACPITable*)rsdp2->xsdt;
        pointerSize = sizeof(uint64_t);
    }
    const size_t entries = (rsdt->length - sizeof(ACPITable)) / pointerSize;
    E9 e9;
    const FADT* fadt = nullptr;
    const MADT* madt = nullptr;
    for (size_t i = 0; i < entries; i++) {
        const uintptr_t offset = (uintptr_t)rsdt + sizeof(ACPITable) + (i * pointerSize);
        const ACPITable* table = (const ACPITable*)(acpi2 ? *(uint64_t*)offset : (uintptr_t)*(uint32_t*)offset);
        if (!table->IsValid()) continue;
        char signature[5];
        for (size_t i = 0; i < 4; i++) signature[i] = table->signature[i];
        signature[4] = '\0';
        e9.Puts(String("ACPI table ") + ToString(i) + ":\n");
        e9.Puts(String("\tSignature: ") + signature + '\n');
        e9.Puts(String("\tLength: 0x") + ToString(table->length, 16) + '\n');
        e9.Puts(String("\tRevision: 0x") + ToString(table->revision, 16) + '\n');
        e9.Puts(String("\tChecksum: 0x") + ToString(table->checksum, 16) + '\n');
        e9.Puts(String("\tOEM ID: ") + table->oemID + '\n');
        e9.Puts(String("\tOEM table ID: ") + table->oemTableID + '\n');
        e9.Puts(String("\tOEM revision: 0x") + ToString(table->oemRevision, 16) + '\n');
        e9.Puts(String("\tCreator ID: 0x") + ToString(table->creatorID, 16) + '\n');
        e9.Puts(String("\tCreator revision: 0x") + ToString(table->creatorRevision, 16) + '\n');
        if (String(signature) == String(FADT::expectedSignature)) {
            fadt = (const FADT*)table;
            e9.Puts(String("\tFACS: 0x") + ToString(fadt->facs, 16) + '\n');
            e9.Puts(String("\tDSDT: 0x") + ToString(fadt->dsdt, 16) + '\n');
            e9.Puts(String("\tSCI interrupt: 0x") + ToString(fadt->sciInterrupt, 16) + '\n');
            e9.Puts(String("\tSMI command port: 0x") + ToString(fadt->smiCommandPort, 16) + '\n');
            e9.Puts(String("\tACPI enable: 0x") + ToString(fadt->acpiEnable, 16) + '\n');
            e9.Puts(String("\tACPI disable: 0x") + ToString(fadt->acpiDisable, 16) + '\n');
            e9.Puts(String("\tRTC century register: 0x") + ToString(fadt->rtcCenturyRegister, 16) + '\n');
            if (acpi2) {
                const FADT2* fadt2 = (const FADT2*)fadt;
                e9.Puts(String("\tACPI 2.0 boot architecture flags: 0x") + ToString(fadt2->bootArchitectureFlags, 16) + '\n');
                e9.Puts(String("\tACPI 2.0 flags: 0x") + ToString(fadt2->flags, 16) + '\n');
                e9.Puts(String("\tACPI 2.0 FACS: 0x") + ToString(fadt2->facs2, 16) + '\n');
                e9.Puts(String("\tACPI 2.0 DSDT: 0x") + ToString(fadt2->dsdt2, 16) + '\n');
            }
        }
        else if (String(signature) == String(MADT::expectedSignature)) {
            madt = (const MADT*)table;
            e9.Puts(String("\tLocal APIC address: 0x") + ToString(madt->localAPIC, 16) + '\n');
            e9.Puts(String("\tFlags: 0x") + ToString(madt->flags, 16) + '\n');
        }
    }
    if (fadt) {
        if (madt && !(madt->flags & (1 << (uint8_t)MADT::Flags::Dual8259))) Panic("ACPI interrupt is not 8259 PIC IRQ");
        sci = SCI(fadt);
    }
    if (madt) {
        LocalAPIC* lapic = (LocalAPIC*)(uintptr_t)madt->localAPIC;
        for (const MADTEntry* entry = (MADTEntry*)((uintptr_t)madt + sizeof(MADT)); (uintptr_t)entry < (uintptr_t)madt + madt->length; entry = (MADTEntry*)((uintptr_t)entry + entry->length)) {
            switch (entry->type) {
                case MADTEntry::Type::ProcessorLocalAPIC: {
                    const MADTProcessorLocalAPIC* lapic = (const MADTProcessorLocalAPIC*)entry;
                    e9.Puts("MADT processor local APIC:\n");
                    e9.Puts(String("\tACPI processor ID: 0x") + ToString(lapic->acpiProcessorID, 16) + '\n');
                    e9.Puts(String("\tAPIC ID: 0x") + ToString(lapic->apicID, 16) + '\n');
                    e9.Puts(String("\tFlags: 0x") + ToString(lapic->flags, 16) + '\n');
                    break;
                }
                case MADTEntry::Type::IOAPIC: {
                    const MADTIOAPIC* ioapic = (const MADTIOAPIC*)entry;
                    e9.Puts("MADT IO APIC:\n");
                    e9.Puts(String("\tIO APIC ID: 0x") + ToString(ioapic->ioAPICID, 16) + '\n');
                    e9.Puts(String("\tIO APIC address: 0x") + ToString(ioapic->ioAPIC, 16) + '\n');
                    e9.Puts(String("\tGlobal system interrupt address: 0x") + ToString(ioapic->gsiBase, 16) + '\n');
                    break;
                }
                case MADTEntry::Type::IOAPICInterruptSourceOverride: {
                    const MADTIOAPICInterruptSourceOverride* ioapicISO = (const MADTIOAPICInterruptSourceOverride*)entry;
                    e9.Puts("MADT IO APIC interrupt source override:\n");
                    e9.Puts(String("\tBus source: 0x") + ToString(ioapicISO->busSource, 16) + '\n');
                    e9.Puts(String("\tIRQ source: 0x") + ToString(ioapicISO->irqSource, 16) + '\n');
                    e9.Puts(String("\tGlobal system interrupt: 0x") + ToString(ioapicISO->gsi, 16) + '\n');
                    e9.Puts(String("\tFlags: 0x") + ToString(ioapicISO->flags, 16) + '\n');
                    break;
                }
                case MADTEntry::Type::IOAPICNonMaskableInterruptSource: {
                    const MADTIOAPICNonMaskableInterruptSource* ioapicNMISource = (const MADTIOAPICNonMaskableInterruptSource*)entry;
                    e9.Puts("MADT IO APIC non maskable interrupt source:\n");
                    e9.Puts(String("\tNon maskable interrupt source: 0x") + ToString(ioapicNMISource->nmiSource, 16) + '\n');
                    e9.Puts(String("\tFlags: 0x") + ToString(ioapicNMISource->flags, 16) + '\n');
                    e9.Puts(String("\tGlobal system interrupt: 0x") + ToString(ioapicNMISource->gsi, 16) + '\n');
                    break;
                }
                case MADTEntry::Type::LocalAPICNonMaskableInterrupts: {
                    const MADTLocalAPICNonMaskableInterrupts* lapicNMI = (const MADTLocalAPICNonMaskableInterrupts*)entry;
                    e9.Puts("MADT local APIC non maskable interrupts:\n");
                    e9.Puts(String("\tACPI processor ID: ") + (lapicNMI->acpiProcessorID == UINT8_MAX ? "all" : String("0x") + ToString(lapicNMI->acpiProcessorID, 16)) + '\n');
                    e9.Puts(String("\tFlags: 0x") + ToString(lapicNMI->flags, 16) + '\n');
                    e9.Puts(String("\tLINT: 0x") + ToString(lapicNMI->lint, 16) + '\n');
                    break;
                }
                case MADTEntry::Type::LocalAPICAddressOverride: {
                    lapic = (LocalAPIC*)((const MADTLocalAPICAddressOverride*)entry)->address;
                    e9.Puts(String("MADT local APIC address override: 0x") + ToString((uintptr_t)lapic, 16) + '\n');
                    break;
                }
                case MADTEntry::Type::ProcessorLocalX2APIC: {
                    const MADTProcessorLocalX2APIC* localX2APIC = (const MADTProcessorLocalX2APIC*)entry;
                    e9.Puts("MADT processor local X2 APIC:\n");
                    e9.Puts(String("\tLocal X2 APIC ID: 0x") + ToString(localX2APIC->localX2APICID, 16) + '\n');
                    e9.Puts(String("\tFlags: 0x") + ToString(localX2APIC->flags, 16) + '\n');
                    e9.Puts(String("\tACPI ID: 0x") + ToString(localX2APIC->acpiID, 16) + '\n');
                    break;
                }
                default: {
                    e9.Puts(String("Unknown MADT entry type: ") + ToString((uint8_t)entry->type) + '\n');
                    break;
                }
            }
        }
    }

    // TODO: Init PS2
    // TODO: Parse AML
    // TODO: Init SMP
    // TODO: Init IO APIC
    // TODO: Init local APIC
    // TODO: Init PCI
    // TODO:    Init USB
}