#include "MBRPartitionEntryCHS.hpp"

namespace MathLib {
    MBRPartitionEntryCHS::MBRPartitionEntryCHS(uint32_t lba, uint8_t heads, uint8_t sectors) {
        cylinder = lba / (heads * sectors);
        head = lba % (heads * sectors) / sectors;
        sector = lba % sectors + 1;
    }
    bool MBRPartitionEntryCHS::operator==(const MBRPartitionEntryCHS& other) const {
        return head == other.head && sector == other.sector && cylinder == other.cylinder;
    }
    bool MBRPartitionEntryCHS::operator!=(const MBRPartitionEntryCHS& other) const {
        return !(*this == other);
    }
}