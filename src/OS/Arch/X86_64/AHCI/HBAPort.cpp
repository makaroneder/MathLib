#ifdef __x86_64__
#include "HBAPort.hpp"
#include "FISRegH2D.hpp"
#include "HBACommandTable.hpp"
#include "HBACommandHeader.hpp"
#include <Utils.hpp>

bool HBAPort::IsValid(void) const {
    return (sataStatus & 0x0f) == DETPresent && ((sataStatus >> 8) & 0x0f) == IPMActive;
}
HBAPort::Type HBAPort::GetType(void) const {
    return type;
}
void HBAPort::Configure(uint8_t* buffer) {
    commandStatus &= ~(1 << (uint8_t)CommandStatusBits::ST | 1 << (uint8_t)CommandStatusBits::FRE);
    while (commandStatus & 1 << (uint8_t)CommandStatusBits::FR || commandStatus & 1 << (uint8_t)CommandStatusBits::CR) {}
    commandListBase = (uintptr_t)buffer;
    fisBaseAddress = (uintptr_t)buffer + 1024;
    HBACommandHeader* header = (HBACommandHeader*)buffer;
    for (uint8_t i = 0; i < 32; i++) {
        header[i].prdtLen = 8;
        header[i].cmdTableBaseAddress = (uintptr_t)buffer + 1024 + 256 * (i + 1);
    }
    while (commandStatus & 1 << (uint8_t)CommandStatusBits::CR) {}
    commandStatus |= 1 << (uint8_t)CommandStatusBits::ST | 1 << (uint8_t)CommandStatusBits::FRE;
}
bool HBAPort::AccessPort(size_t sector, uint32_t count, void* buff, bool write) {
    interruptStatus = UINT32_MAX;
    HBACommandHeader* cmdHeader = (HBACommandHeader*)commandListBase;
    cmdHeader->cmdFisLen = sizeof(FISRegH2D) / sizeof(uint32_t);
    cmdHeader->write = write;
    cmdHeader->prdtLen = 1;
    HBACommandTable* commandTable = (HBACommandTable*)cmdHeader->cmdTableBaseAddress;
    commandTable->entry[0].dataAddress = (uintptr_t)buff;
    commandTable->entry[0].count = (count << 9) - 1;
    commandTable->entry[0].interruptOnCompletion = true;
    FISRegH2D* fis = (FISRegH2D*)&commandTable->commandFis;
    fis->type = FISType::RegH2D;
    fis->commandControl = 1;
    fis->deviceReg = 1 << 6;
    if (type == Type::ATA) {
        fis->command = (uint8_t)(write ? Command::ATAWriteDMAExt : Command::ATAReadDMAExt);
        fis->lba0 = sector;
        fis->lba1 = sector >> 8;
        fis->lba2 = sector >> 16;
        fis->lba3 = sector >> 32;
        fis->lba4 = sector >> 40;
        fis->lba5 = sector >> 48;
        fis->count = count;
    }
    else if (type == Type::ATAPI) {
        fis->featureLow = 1 << 2 | 1 << 0;
        fis->command = (uint8_t)Command::ATAPIPacket;
        cmdHeader->atapi = true;
        commandTable->atapiCommand[0] = (uint8_t)(write ? ATAPICommand::Write12 : ATAPICommand::Read12);
        commandTable->atapiCommand[1] = 0;
        commandTable->atapiCommand[2] = sector >> 24;
        commandTable->atapiCommand[3] = sector >> 16;
        commandTable->atapiCommand[4] = sector >> 8;
        commandTable->atapiCommand[5] = sector;
        commandTable->atapiCommand[6] = count >> 24;
        commandTable->atapiCommand[7] = count >> 16;
        commandTable->atapiCommand[8] = count >> 8;
        commandTable->atapiCommand[9] = count;
        commandTable->atapiCommand[10] = 0;
        commandTable->atapiCommand[11] = 0;
    }
    else return false;
    size_t spin = 0;
    while (taskFileData & (1 << (uint8_t)TaskFileDataBits::DRQ | 1 << (uint8_t)TaskFileDataBits::Busy) && spin < 1000000) spin++;
    if (spin == 1000000) return false;
    commandIssue = 1;
    while (commandIssue)
        if (interruptStatus & 1 << (uint8_t)InterruptStatusBits::FES) return false;
    return true;
}
bool HBAPort::GetSize(size_t& sectors, size_t& bytesPerSector) {
    uint8_t buff[512];
    interruptStatus = (uint32_t)-1;
    HBACommandHeader* cmdHeader = (HBACommandHeader*)commandListBase;
    cmdHeader->cmdFisLen = sizeof(FISRegH2D) / sizeof(uint32_t);
    cmdHeader->write = false;
    cmdHeader->prdtLen = 1;
    HBACommandTable* commandTable = (HBACommandTable*)cmdHeader->cmdTableBaseAddress;
    commandTable->entry[0].dataAddress = (uintptr_t)buff;
    commandTable->entry[0].count = 512 - 1;
    commandTable->entry[0].interruptOnCompletion = true;
    FISRegH2D* fis = (FISRegH2D*)&commandTable->commandFis;
    fis->type = FISType::RegH2D;
    fis->commandControl = 1;
    fis->deviceReg = 0;
    fis->lba0 = 0;
    fis->lba1 = 0;
    fis->lba2 = 0;
    fis->lba3 = 0;
    fis->lba4 = 0;
    fis->lba5 = 0;
    fis->count = 0;
    if (type == Type::ATA) fis->command = (uint8_t)Command::ATAIdentify;
    else if (type == Type::ATAPI) {
        fis->featureLow = 1 << 0 | 1 << 2;
        fis->command = (uint8_t)Command::ATAPIPacket;
        cmdHeader->atapi = true;
        commandTable->atapiCommand[0] = (uint8_t)ATAPICommand::ReadCapacity;
        commandTable->atapiCommand[1] = 0;
        commandTable->atapiCommand[2] = 0;
        commandTable->atapiCommand[3] = 0;
        commandTable->atapiCommand[4] = 0;
        commandTable->atapiCommand[5] = 0;
        commandTable->atapiCommand[6] = 0;
        commandTable->atapiCommand[7] = 0;
        commandTable->atapiCommand[8] = 0;
        commandTable->atapiCommand[9] = 16;
        commandTable->atapiCommand[10] = 0;
        commandTable->atapiCommand[11] = 0;
    }
    size_t spin = 0;
    while (taskFileData & (1 << (uint8_t)TaskFileDataBits::DRQ | 1 << (uint8_t)TaskFileDataBits::Busy) && spin < 1000000) spin++;
    if (spin == 1000000) return false;
    commandIssue = 1;
    while (commandIssue)
        if (interruptStatus & 1 << (uint8_t)InterruptStatusBits::FES) return false;
    switch (type) {
        case Type::ATA: {
            bytesPerSector = 512;
            const uint64_t lba48SectorCount = *(const uint64_t*)&buff[200];
            sectors = lba48SectorCount ? lba48SectorCount : *(const uint16_t*)&buff[120];
            break;
        }
        case Type::ATAPI: {
            const uint32_t* tmp = (const uint32_t*)buff;
            sectors = MathLib::SwapEndian32(tmp[0]) + 1;
            bytesPerSector = MathLib::SwapEndian32(tmp[1]);
            break;
        }
        default: return false;
    }
    return true;
}
uint8_t HBAPort::FindCommandSlot(void) const {
    uint32_t slots = sataActive | commandIssue;
    for (uint8_t i = 0; i < 32; i++)
        if (!(slots >> i & 1)) return i;
    return 32;
}

#endif