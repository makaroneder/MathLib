#include "MBRPartitionEntryCHS.hpp"

MBRPartitionEntryCHS::MBRPartitionEntryCHS(const uint32_t& lba, const uint8_t& heads, const uint8_t& sectors) {
    cylinder = lba / (heads * sectors);
    head = lba % (heads * sectors) / sectors;
    sector = lba % sectors + 1;
}