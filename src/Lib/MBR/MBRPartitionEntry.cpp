#include "MBRPartitionEntry.hpp"

MBRPartitionEntry::MBRPartitionEntry(void) : MBRPartitionEntry(Interval<uint32_t>(), ID::Empty) {}
MBRPartitionEntry::MBRPartitionEntry(const Interval<uint32_t>& lba, const ID& id, const Type& type, const uint8_t& heads, const uint8_t& sectors) : type(type), startCHS(MBRPartitionEntryCHS(lba.GetMin(), heads, sectors)), id(id), endCHS(MBRPartitionEntryCHS(lba.GetMax(), heads, sectors)), startLBA(lba.GetMin()), sectorCount(lba.GetSize()) {}
bool MBRPartitionEntry::IsValid(void) const {
    return (type == Type::Normal || type == Type::Active) && !(id == ID::Empty && type == Type::Active);
}