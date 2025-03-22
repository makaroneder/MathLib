#ifndef MathLib_Interfaces_ByteDeviceSequence_H
#define MathLib_Interfaces_ByteDeviceSequence_H
#include "Sequence.hpp"
#include "ByteDevice.hpp"

namespace MathLib {
    template <typename T>
    struct ByteDeviceSequence : Sequence<T> {
        ByteDeviceSequence(ByteDevice& device) : device(device) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(device.GetSize());
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            StartAndReturnFromBenchmark(device.ReadPositioned<T>(index * sizeof(T)).GetOr(T()));
        }

        private:
        ByteDevice& device;
    };
}

#endif