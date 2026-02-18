#ifdef __x86_64__
#ifndef HBAPort_H
#define HBAPort_H
#include <stdint.h>
#include <stddef.h>

#define IPMActive 1
#define DETPresent 3

struct HBAPort {
    enum class Type : uint32_t {
        ATAPI = 0xeb140101,
        ATA = 0x00000101,
        SEMB = 0xc33c0101,
        PM = 0x96690101,
    };
    enum class Command : uint8_t {
        ATAReadDMAExt = 0x25,
        ATAWriteDMAExt = 0x35,
        ATAPIPacket = 0xa0,
        ATAPIIdentify,
        ATAIdentify = 0xec,
    };
    enum class ATAPICommand : uint8_t {
        TestUnitReady = 0x00,
        ServiceActionIn12,
        RequestSense = 0x03,
        FormatUnit,
        Inquiry = 0x12,
        EjectDevice = 0x1b,
        SetMediumRemoval = 0x1e,
        ReadFormatCapacities = 0x23,
        ReadCapacity = 0x25,
        Read10 = 0x28,
        Write10 = 0x2a,
        Seek10,
        WriteAndVerify10 = 0x2e,
        Verify10,
        SynchronizeCache = 0x35,
        WriteBuffer = 0x3b,
        ReadBuffer,
        ReadTOC = 0x43,
        GetConfiguration = 0x46,
        GetEventStatusNotification = 0x4a,
        ReadDiscInformation = 0x51,
        ReadTrackInformation,
        ReserveTrack,
        SendOPCInformation,
        SelectMode10,
        RepairTrack = 0x58,
        SenseMode10 = 0x5a,
        CloseTrackSession,
        ReadBufferCapacity,
        SendCueSheet,
        ReportLUNS = 0xa0,
        Blank,
        SecurityProtocolIn,
        SendKey,
        ReportKey,
        LoadMedium = 0xa6,
        SetReadAhead,
        Read12 = 0xa8,
        Write12 = 0xaa,
        ReadMediaSerialNumber,
        GetPerformance,
        ReadDiscStructure,
        SecurityProtocolOut = 0xb5,
        SetStreaming,
        ReadCDMSF = 0xb9,
        SetCDSpeed = 0xbb,
        MechanismStatus = 0xbd,
        ReadCD,
        SendDiscStructure,
    };
    enum class CommandStatusBits : uint8_t {
        ST = 0,
        FRE = 4,
        FR = 14,
        CR,
    };
    enum class InterruptStatusBits : uint8_t {
        FES = 30,
    };
    enum class TaskFileDataBits : uint8_t {
        DRQ = 3,
        Busy = 7,
    };

    [[nodiscard]] bool IsValid(void) const;
    [[nodiscard]] Type GetType(void) const;
    void Configure(uint8_t* buffer);
    [[nodiscard]] bool AccessPort(size_t sector, uint32_t count, void* buff, bool write);
    [[nodiscard]] bool GetSize(size_t& sectors, size_t& bytesPerSector);

    private:
    [[nodiscard]] uint8_t FindCommandSlot(void) const;

    uint64_t commandListBase;
    uint64_t fisBaseAddress;
    uint32_t interruptStatus;
    uint32_t interruptEnable;
    uint32_t commandStatus;
    uint32_t reserved1;
    uint32_t taskFileData;
    Type type;
    uint32_t sataStatus;
    uint32_t sataControl;
    uint32_t sataError;
    uint32_t sataActive;
    uint32_t commandIssue;
    uint32_t sataNotification;
    uint32_t fisSwitchControl;
    uint32_t reserved2[11];
    uint64_t vendor[2];
};

#endif
#endif