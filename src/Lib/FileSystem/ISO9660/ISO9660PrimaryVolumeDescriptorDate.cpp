#include "ISO9660PrimaryVolumeDescriptorDate.hpp"
#include "../../Host.hpp"

namespace MathLib {
    bool ISO9660PrimaryVolumeDescriptorDate::IsValid(void) const {
        StartBenchmark
        const bool isZero = !StringToNumber(String(year, SizeOfArray(year)));
        uint8_t tmp = StringToNumber(String(month, SizeOfArray(month)));
        if (tmp > 12 || (isZero && tmp) || (!isZero && !tmp)) ReturnFromBenchmark(false);
        tmp = StringToNumber(String(day, SizeOfArray(day)));
        if (tmp > 31 || (isZero && tmp) || (!isZero && !tmp)) ReturnFromBenchmark(false);
        tmp = StringToNumber(String(hour, SizeOfArray(hour)));
        if (tmp > 23 || (isZero && tmp)) ReturnFromBenchmark(false);
        tmp = StringToNumber(String(minute, SizeOfArray(minute)));
        if (tmp > 59 || (isZero && tmp)) ReturnFromBenchmark(false);
        tmp = StringToNumber(String(second, SizeOfArray(second)));
        if (tmp > 59 || (isZero && tmp)) ReturnFromBenchmark(false);
        tmp = StringToNumber(String(hundredthsOfSecond, SizeOfArray(hundredthsOfSecond)));
        ReturnFromBenchmark(!(tmp > 59 || (isZero && tmp)));
    }
}