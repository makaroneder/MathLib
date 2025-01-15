#ifdef __x86_64__
#include "PCI.hpp"
#include "PCIHeader.hpp"
#include "../USB/UHCI.hpp"
#include "../AHCI/AHCI.hpp"
#include <Logger.hpp>
#include <String.hpp>

[[nodiscard]] MathLib::String PCIAddressToString(uint16_t segment, uint8_t bus, uint8_t device, uint8_t function) {
    return "PCI "_M + MathLib::ToString(segment) + '/' + MathLib::ToString(bus) + '/' + MathLib::ToString(device) + '/' + MathLib::ToString(function);
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
                                LogString("Found non VGA compatible unclassified device on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString("Found VGA compatible unclassified device on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown unclassified device on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x01: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString("Found SCSI bus controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: switch (functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found IDE controller in ISA compatibility mode on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x05: {
                                    LogString("Found IDE controller in PCI native mode on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x0a: {
                                    LogString("Found ISA compatible IDE controller with PCI native mode on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x0f: {
                                    LogString("Found PCI native IDE controller with ISA compatibility mode on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x80: {
                                    LogString("Found IDE controller in ISA compatibility mode with bus mastering on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x85: {
                                    LogString("Found IDE controller in PCI native mode with bus mastering on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x8a: {
                                    LogString("Found ISA compatible IDE controller with PCI native mode with bus mastering on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x8f: {
                                    LogString("Found PCI native IDE controller with ISA compatibility mode with bus mastering on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown IDE controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x02: {
                                LogString("Found floppy disk controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString("Found IPI bus controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x04: {
                                LogString("Found RAID controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x05: switch (functionEntry->programInterface) {
                                case 0x20: {
                                    LogString("Found single DMA ATA controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x30: {
                                    LogString("Found chained DMA ATA controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown ATA controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x06: switch (functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found vendor specific serial ATA interface on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString("Found AHCI 1.0 on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    if (!InitAHCI(functionEntry)) return false;
                                    break;
                                }
                                case 0x02: {
                                    LogString("Found serial ATA storage bus on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown serial ATA controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x07: switch (functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found SCSI SAS controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString("Found SCSI serial storage bus on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown serial SCSI controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x08: switch (functionEntry->programInterface) {
                                case 0x01: {
                                    LogString("Found NVMHCI on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString("Found NVM express on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown non volatile memory controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            default: {
                                LogString("Found unknown mass storage controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x02: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString("Found ethernet controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString("Found token ring controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString("Found FDDI controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString("Found ATM controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x04: {
                                LogString("Found ISDN controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x05: {
                                LogString("Found WorldFip controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x06: {
                                LogString("Found PICMG 2.14 multi computing controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x07: {
                                LogString("Found InfiniBand controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x08: {
                                LogString("Found fabric controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown network controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x03: switch (functionEntry->subClass) {
                            case 0x00: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found VGA controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString("Found 8514 compatible controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown VGA compatible controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x01: {
                                LogString("Found XGA controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString("Found non VGA compatible 3D controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown display controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x04: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString("Found multimedia video controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString("Found multimedia audio controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString("Found computer telephony device on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString("Found audio device on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown multimedia controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x05: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString("Found RAM controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString("Found flash controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown memory controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x06: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString("Found host bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString("Found ISA bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString("Found EISA bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString("Found MCA bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x04: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found normal PCI to PCI bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString("Found subtractive PCI to PCI bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown PCI to PCI bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x05: {
                                LogString("Found PCMCIA bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x06: {
                                LogString("Found NuBus bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x07: {
                                LogString("Found CardBus bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x08: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found transparent RACEway bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString("Found endpoint RACEway bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown RACEway bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x09: switch(functionEntry->programInterface) {
                                case 0x40: {
                                    LogString("Found semi transparent primary bus to host CPU bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x80: {
                                    LogString("Found semi transparent secondary bus to host CPU bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown PCI to PCI bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x0a: {
                                LogString("Found InfiniBand to PCI host bridge on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown bridge device on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x07: switch (functionEntry->subClass) {
                            case 0x00: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found 8250 compatible serial controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString("Found 16450 compatible serial controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString("Found 16550 compatible serial controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x03: {
                                    LogString("Found 16650 compatible serial controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x04: {
                                    LogString("Found 16750 compatible serial controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x05: {
                                    LogString("Found 16850 compatible serial controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x06: {
                                    LogString("Found 16950 compatible serial controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown serial controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x01: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found standard parallel port on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString("Found bi-directional parallel port on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString("Found ECP 1.X compliant parallel port on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x03: {
                                    LogString("Found IEEE 1284 controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0xfe: {
                                    LogString("Found IEEE 1284 target device on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown parallel controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x02: {
                                LogString("Found multiport serial controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found generic modem on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString("Found Hayes 16450 compatible interface on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString("Found Hayes 16550 compatible interface on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x03: {
                                    LogString("Found Hayes 16650 compatible interface on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x04: {
                                    LogString("Found Hayes 16750 compatible interface on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown modem on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x04: {
                                LogString("Found IEEE 488.1/2 controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x05: {
                                LogString("Found smart card controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown communication controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x08: switch (functionEntry->subClass) {
                            case 0x00: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found generic 8259 compatible PIC on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString("Found ISA compatible PIC on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString("Found EISA compatible PIC on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x10: {
                                    LogString("Found IO APIC controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x20: {
                                    LogString("Found IOx APIC controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown PIC controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x01: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found generic 8237 compatible DMA controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString("Found ISA compatible DMA controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString("Found EISA compatible DMA controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown DMA controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x02: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found 8254 compatible timer on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString("Found ISA compatible timer on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString("Found EISA compatible timer on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x03: {
                                    LogString("Found HPET on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown timer on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x03: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found generic RTC controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString("Found ISA compatible RTC controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown RTC controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x04: {
                                LogString("Found PCI hot plug controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x05: {
                                LogString("Found SD host controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x06: {
                                LogString("Found IOMMU on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown system peripheral on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x09: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString("Found keyboard controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString("Found digitizer pen on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString("Found mouse controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString("Found scanner controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x04: switch(functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found generic gameport controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x10: {
                                    LogString("Found extended gameport controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown gameport controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            default: {
                                LogString("Found unknown input device on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x0a: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString("Found generic docking station on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown docking station on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x0b: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString("Found i386 processor on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString("Found i486 processor on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString("Found pentium processor on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString("Found pentium pro processor on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x10: {
                                LogString("Found alpha processor on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x20: {
                                LogString("Found power PC processor on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x30: {
                                LogString("Found MIPS processor on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x40: {
                                LogString("Found co-processor on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown processor on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x0c: switch (functionEntry->subClass) {
                            case 0x00: switch (functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found generic FireWire controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x10: {
                                    LogString("Found OHCI FireWire controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown FireWire controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x01: {
                                LogString("Found ACCESS bus controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString("Found SSA on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: switch (functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found UHCI controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    new UHCI(functionEntry);
                                    break;
                                }
                                case 0x10: {
                                    LogString("Found OHCI controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x20: {
                                    LogString("Found EHCI controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x30: {
                                    LogString("Found XHCI controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0xfe: {
                                    LogString("Found USB device on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown USB controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x04: {
                                LogString("Found fibre channel on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x05: {
                                LogString("Found SMBus controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x06: {
                                LogString("Found InfiniBand controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x07: switch (functionEntry->programInterface) {
                                case 0x00: {
                                    LogString("Found SMIC interface on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x01: {
                                    LogString("Found keyboard controller style interface on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                case 0x02: {
                                    LogString("Found block transfer interface on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                                default: {
                                    LogString("Found unknown IPMI interface on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                    break;
                                }
                            } break;
                            case 0x08: {
                                LogString("Found SERCOS interface on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x09: {
                                LogString("Found CANbus controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown serial bus controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x0d: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString("Found iRDA compatible controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString("Found consumer IR controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x10: {
                                LogString("Found RF controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x11: {
                                LogString("Found bluetooth controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x12: {
                                LogString("Found broadband controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x20: {
                                LogString("Found 802.1a ethernet controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x21: {
                                LogString("Found 802.1b ethernet controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown intelligent controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x0e: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString("Found I20 intelligent controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown intelligent controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x0f: switch (functionEntry->subClass) {
                            case 0x01: {
                                LogString("Found satellite TV controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x02: {
                                LogString("Found satellite audio controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x03: {
                                LogString("Found satellite voice controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x04: {
                                LogString("Found satellite data controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown satellite controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x10: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString("Found network and computing encryption controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x10: {
                                LogString("Found entertainment encryption controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown encryption controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x11: switch (functionEntry->subClass) {
                            case 0x00: {
                                LogString("Found DPIO module on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x01: {
                                LogString("Found performance counter on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x10: {
                                LogString("Found communication synchronizer on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            case 0x20: {
                                LogString("Found signal processing managment on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                            default: {
                                LogString("Found unknown signal processing controller on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                                break;
                            }
                        } break;
                        case 0x12: {
                            LogString("Found processing accelerator on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                            break;
                        }
                        case 0x13: {
                            LogString("Found non essential instrumentation on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                            break;
                        }
                        case 0x40: {
                            LogString("Found co-processor on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + '\n');
                            break;
                        }
                        default: {
                            LogString("Found unknown device on "_M + PCIAddressToString(mcfg->entries[i].pciSegment, bus, device, function) + ": {\n");
                            LogString("\tClass: 0x"_M + MathLib::ToString(functionEntry->classCode, 16) + '\n');
                            LogString("\tSub class: 0x"_M + MathLib::ToString(functionEntry->subClass, 16) + '\n');
                            LogString("\tProgram interface: 0x"_M + MathLib::ToString(functionEntry->programInterface, 16) + '\n');
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