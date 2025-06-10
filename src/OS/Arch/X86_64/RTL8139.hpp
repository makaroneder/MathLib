#ifdef __x86_64__
#ifndef RTL8139_H
#define RTL8139_H
#include "PCI/PCIHeader.hpp"

struct RTL8139 {
    enum class IOOffset : uint8_t {
        ID0 = 0x00,
        ID1,
        ID2,
        ID3,
        ID4,
        ID5,
        Reserved1,
        Reserved2,
        Multicast0,
        Multicast1,
        Multicast2,
        Multicast3,
        Multicast4,
        Multicast5,
        Multicast6,
        Multicast7,
        TransmitStatusOfDescriptor00,
        TransmitStatusOfDescriptor01,
        TransmitStatusOfDescriptor02,
        TransmitStatusOfDescriptor03,
        TransmitStatusOfDescriptor10,
        TransmitStatusOfDescriptor11,
        TransmitStatusOfDescriptor12,
        TransmitStatusOfDescriptor13,
        TransmitStatusOfDescriptor20,
        TransmitStatusOfDescriptor21,
        TransmitStatusOfDescriptor22,
        TransmitStatusOfDescriptor23,
        TransmitStatusOfDescriptor30,
        TransmitStatusOfDescriptor31,
        TransmitStatusOfDescriptor32,
        TransmitStatusOfDescriptor33,
        TransmitStartOfDescriptor00,
        TransmitStartOfDescriptor01,
        TransmitStartOfDescriptor02,
        TransmitStartOfDescriptor03,
        TransmitStartOfDescriptor10,
        TransmitStartOfDescriptor11,
        TransmitStartOfDescriptor12,
        TransmitStartOfDescriptor13,
        TransmitStartOfDescriptor20,
        TransmitStartOfDescriptor21,
        TransmitStartOfDescriptor22,
        TransmitStartOfDescriptor23,
        TransmitStartOfDescriptor30,
        TransmitStartOfDescriptor31,
        TransmitStartOfDescriptor32,
        TransmitStartOfDescriptor33,
        ReceiveBufferStart0,
        ReceiveBufferStart1,
        ReceiveBufferStart2,
        ReceiveBufferStart3,
        EarlyReceiveByteCount0,
        EarlyReceiveByteCount1,
        EarlyReceiveStatus,
        Command,
        PacketReadAddress0,
        PacketReadAddress1,
        BufferAddress0,
        BufferAddress1,
        InterruptMask0,
        InterruptMask1,
        InterruptStatus0,
        InterruptStatus1,
        TransmitConfiguration0,
        TransmitConfiguration1,
        TransmitConfiguration2,
        TransmitConfiguration3,
        ReceiveConfiguration0,
        ReceiveConfiguration1,
        ReceiveConfiguration2,
        ReceiveConfiguration3,
        TimerCount0,
        TimerCount1,
        TimerCount2,
        TimerCount3,
        MissedPacketCounter0,
        MissedPacketCounter1,
        MissedPacketCounter2,
        MissedPacketCounter3,
        Command93c46,
        Configuration0,
        Configuration1,
        Reserved3,
        TimerInterrupt0,
        TimerInterrupt1,
        TimerInterrupt2,
        TimerInterrupt3,
        MediaStatus,
        Configuration3,
        Configuration4,
        Reserved4,
        MultipleInterruptSelect0,
        MultipleInterruptSelect1,
        PCIRevisionID,
        Reserved5,
        TransmitStatusOfAllDescriptors0,
        TransmitStatusOfAllDescriptors1,
        BasicModeControl0,
        BasicModeControl1,
        BasicModeStatus0,
        BasicModeStatus1,
        // TODO: 0x66-0xff
    };
    RTL8139(PCIHeader* header);

    private:
    template <typename T>
    T Read(IOOffset offset) const {
        return *(const T*)(base + (uint8_t)offset);
    }
    template <typename T>
    void Write(IOOffset offset, const T& value) {
        *(T*)(base + (uint8_t)offset) = value;
    }

    uintptr_t base;
};

#endif
#endif