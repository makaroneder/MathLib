#include "MBRPartitionEntryCHS.hpp"
#include "../Typedefs.hpp"

namespace MathLib {
    MBRPartitionEntryCHS::MBRPartitionEntryCHS(uint32_t lba, uint8_t heads, uint8_t sectors) {
        StartBenchmark
        cylinder = lba / (heads * sectors);
        head = lba % (heads * sectors) / sectors;
        sector = lba % sectors + 1;
        EndBenchmark
    }
    bool MBRPartitionEntryCHS::operator==(const MBRPartitionEntryCHS& other) const {
        StartAndReturnFromBenchmark(head == other.head && sector == other.sector && cylinder == other.cylinder);
    }
    bool MBRPartitionEntryCHS::operator!=(const MBRPartitionEntryCHS& other) const {
        StartAndReturnFromBenchmark(!(*this == other));
    }
}