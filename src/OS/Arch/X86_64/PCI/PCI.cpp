#ifdef __x86_64__
#include "PCI.hpp"
#include "PCIHeader.hpp"
#include "../USB/UHCI.hpp"
#include "../AHCI/AHCI.hpp"
#include <Logger.hpp>
#include <String.hpp>

String PCIAddressToString(uint16_t segment, uint8_t bus, uint8_t device, uint8_t function) {
    return String("PCI ") + ToString(segment) + '/' + ToString(bus) + '/' + ToString(device) + '/' + ToString(function);
}
bool InitPCI(const MCFG* mcfg) {
    const size_t entries = (mcfg->length - sizeof(MCFG)) / sizeof(MCFGEntry);
    for (size_t i = 0; i < entries; i++) {
        for (uint8_t bus = mcfg->entries[i].startBus; bus < mcfg->entries[i].endBus; bus++) {
            PCIHeader* busEntry = (PCIHeader*)(mcfg->entries[i].address + (bus << 20));
            if (!busEntry->IsValid()) continue;
            for (uint8_t device = 0; device < 32; device++) {
                PCIHeader* deviceEntry = (PCIHeader*)((uintptr_t)busEntry + (device << 15));
                if (!deviceEntry->IsValid()) continue;
                const uint8_t functions = deviceEntry->multipleFunctions ? 8 : 1;
                for (uint8_t function = 0; function < functions; function++) {
                    PCIHeader* functionEntry = (PCIHeader*)((uintptr_t)deviceEntry + (function << 12));
                    if (!functionEntry->IsValid()) continue;
                    switch (functionEntry->classCode) {
                        case 0x00: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString(String("Found non VGA compatible unclassified device on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString(String("Found VGA compatible unclassified device on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown unclassified device on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x01: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString(String("Found SCSI bus controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: switch (functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found IDE controller in ISA compatibility mode on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x05: {
                                    LogString(String("Found IDE controller in PCI native mode on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x0a: {
                                    LogString(String("Found ISA compatible IDE controller with PCI native mode on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x0f: {
                                    LogString(String("Found PCI native IDE controller with ISA compatibility mode on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x80: {
                                    LogString(String("Found IDE controller in ISA compatibility mode with bus mastering on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x85: {
                                    LogString(String("Found IDE controller in PCI native mode with bus mastering on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x8a: {
                                    LogString(String("Found ISA compatible IDE controller with PCI native mode with bus mastering on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x8f: {
                                    LogString(String("Found PCI native IDE controller with ISA compatibility mode with bus mastering on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown IDE controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x02: {
                                LogString(String("Found floppy disk controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString(String("Found IPI bus controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x04: {
                                LogString(String("Found RAID controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x05: switch (functionEntry->programInterface) {
                                case 0x20: {
                                    LogString(String("Found single DMA ATA controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x30: {
                                    LogString(String("Found chained DMA ATA controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown ATA controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x06: switch (functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found vendor specific serial ATA interface on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString(String("Found AHCI 1.0 on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    if (!InitAHCI(functionEntry)) return false;
                                    break;
                                }
                                case 0x02: {
                                    LogString(String("Found serial ATA storage bus on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown serial ATA controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x07: switch (functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found SCSI SAS controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString(String("Found SCSI serial storage bus on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown serial SCSI controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x08: switch (functionEntry->programInterface) {
                                case 0x01: {
                                    LogString(String("Found NVMHCI on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString(String("Found NVM express on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown non volatile memory controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            default: {
                                LogString(String("Found unknown mass storage controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x02: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString(String("Found ethernet controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString(String("Found token ring controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString(String("Found FDDI controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString(String("Found ATM controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x04: {
                                LogString(String("Found ISDN controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x05: {
                                LogString(String("Found WorldFip controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x06: {
                                LogString(String("Found PICMG 2.14 multi computing controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x07: {
                                LogString(String("Found InfiniBand controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x08: {
                                LogString(String("Found fabric controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown network controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x03: switch (functionEntry->subClass) {
                            case 0x00: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found VGA controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString(String("Found 8514 compatible controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown VGA compatible controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x01: {
                                LogString(String("Found XGA controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString(String("Found non VGA compatible 3D controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown display controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x04: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString(String("Found multimedia video controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString(String("Found multimedia audio controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString(String("Found computer telephony device on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString(String("Found audio device on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown multimedia controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x05: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString(String("Found RAM controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString(String("Found flash controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown memory controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x06: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString(String("Found host bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString(String("Found ISA bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString(String("Found EISA bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString(String("Found MCA bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x04: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found normal PCI to PCI bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString(String("Found subtractive PCI to PCI bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown PCI to PCI bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x05: {
                                LogString(String("Found PCMCIA bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x06: {
                                LogString(String("Found NuBus bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x07: {
                                LogString(String("Found CardBus bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x08: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found transparent RACEway bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString(String("Found endpoint RACEway bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown RACEway bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x09: switch(functionEntry->programInterface) {
                                case 0x40: {
                                    LogString(String("Found semi transparent primary bus to host CPU bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x80: {
                                    LogString(String("Found semi transparent secondary bus to host CPU bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown PCI to PCI bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x0a: {
                                LogString(String("Found InfiniBand to PCI host bridge on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown bridge device on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x07: switch (functionEntry->subClass) {
                            case 0x00: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found 8250 compatible serial controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString(String("Found 16450 compatible serial controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString(String("Found 16550 compatible serial controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x03: {
                                    LogString(String("Found 16650 compatible serial controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x04: {
                                    LogString(String("Found 16750 compatible serial controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x05: {
                                    LogString(String("Found 16850 compatible serial controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x06: {
                                    LogString(String("Found 16950 compatible serial controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown serial controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x01: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found standard parallel port on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString(String("Found bi-directional parallel port on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString(String("Found ECP 1.X compliant parallel port on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x03: {
                                    LogString(String("Found IEEE 1284 controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0xfe: {
                                    LogString(String("Found IEEE 1284 target device on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown parallel controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x02: {
                                LogString(String("Found multiport serial controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found generic modem on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString(String("Found Hayes 16450 compatible interface on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString(String("Found Hayes 16550 compatible interface on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x03: {
                                    LogString(String("Found Hayes 16650 compatible interface on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x04: {
                                    LogString(String("Found Hayes 16750 compatible interface on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown modem on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x04: {
                                LogString(String("Found IEEE 488.1/2 controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x05: {
                                LogString(String("Found smart card controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown communication controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x08: switch (functionEntry->subClass) {
                            case 0x00: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found generic 8259 compatible PIC on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString(String("Found ISA compatible PIC on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString(String("Found EISA compatible PIC on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x10: {
                                    LogString(String("Found IO APIC controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x20: {
                                    LogString(String("Found IOx APIC controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown PIC controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x01: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found generic 8237 compatible DMA controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString(String("Found ISA compatible DMA controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString(String("Found EISA compatible DMA controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown DMA controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x02: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found 8254 compatible timer on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString(String("Found ISA compatible timer on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString(String("Found EISA compatible timer on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x03: {
                                    LogString(String("Found HPET on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown timer on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x03: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found generic RTC controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString(String("Found ISA compatible RTC controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown RTC controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x04: {
                                LogString(String("Found PCI hot plug controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x05: {
                                LogString(String("Found SD host controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x06: {
                                LogString(String("Found IOMMU on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown system peripheral on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x09: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString(String("Found keyboard controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString(String("Found digitizer pen on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString(String("Found mouse controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString(String("Found scanner controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x04: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found generic gameport controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x10: {
                                    LogString(String("Found extended gameport controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown gameport controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            default: {
                                LogString(String("Found unknown input device on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x0a: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString(String("Found generic docking station on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown docking station on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x0b: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString(String("Found i386 processor on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString(String("Found i486 processor on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString(String("Found pentium processor on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString(String("Found pentium pro processor on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x10: {
                                LogString(String("Found alpha processor on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x20: {
                                LogString(String("Found power PC processor on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x30: {
                                LogString(String("Found MIPS processor on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x40: {
                                LogString(String("Found co-processor on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown processor on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x0c: switch (functionEntry->subClass) {
                            case 0x00: switch (functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found generic FireWire controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x10: {
                                    LogString(String("Found OHCI FireWire controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown FireWire controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x01: {
                                LogString(String("Found ACCESS bus controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString(String("Found SSA on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: switch (functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found UHCI controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    new UHCI(functionEntry);
                                    break;
                                }
                                case 0x10: {
                                    LogString(String("Found OHCI controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x20: {
                                    LogString(String("Found EHCI controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x30: {
                                    LogString(String("Found XHCI controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0xfe: {
                                    LogString(String("Found USB device on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown USB controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x04: {
                                LogString(String("Found fibre channel on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x05: {
                                LogString(String("Found SMBus controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x06: {
                                LogString(String("Found InfiniBand controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x07: switch (functionEntry->programInterface) {
                                case 0x00: {
                                    LogString(String("Found SMIC interface on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString(String("Found keyboard controller style interface on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString(String("Found block transfer interface on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString(String("Found unknown IPMI interface on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x08: {
                                LogString(String("Found SERCOS interface on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x09: {
                                LogString(String("Found CANbus controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown serial bus controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x0d: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString(String("Found iRDA compatible controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString(String("Found consumer IR controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x10: {
                                LogString(String("Found RF controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x11: {
                                LogString(String("Found bluetooth controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x12: {
                                LogString(String("Found broadband controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x20: {
                                LogString(String("Found 802.1a ethernet controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x21: {
                                LogString(String("Found 802.1b ethernet controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown intelligent controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x0e: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString(String("Found I20 intelligent controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown intelligent controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x0f: switch (functionEntry->subClass) {
                            case 0x01: {
                                LogString(String("Found satellite TV controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString(String("Found satellite audio controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString(String("Found satellite voice controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x04: {
                                LogString(String("Found satellite data controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown satellite controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x10: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString(String("Found network and computing encryption controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x10: {
                                LogString(String("Found entertainment encryption controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown encryption controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x11: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString(String("Found DPIO module on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString(String("Found performance counter on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x10: {
                                LogString(String("Found communication synchronizer on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x20: {
                                LogString(String("Found signal processing managment on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString(String("Found unknown signal processing controller on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x12: {
                            LogString(String("Found processing accelerator on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                            break;
                        }
                        case 0x13: {
                            LogString(String("Found non essential instrumentation on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                            break;
                        }
                        case 0x40: {
                            LogString(String("Found co-processor on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                            break;
                        }
                        default: {
                            LogString(String("Found unknown device on ") + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + ": {\n");
                            LogString(String("\tClass: 0x") + ToString(functionEntry->classCode, 16) + '\n');
                            LogString(String("\tSub class: 0x") + ToString(functionEntry->subClass, 16) + '\n');
                            LogString(String("\tProgram interface: 0x") + ToString(functionEntry->programInterface, 16) + '\n');
                            LogString("}\n");
                        }
                    }
                }
            }
        }
    }
    return true;
}

#endif