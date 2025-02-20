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
}