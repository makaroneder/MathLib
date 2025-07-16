#ifndef MathLib_Interfaces_ByteDeviceSequence_H
#define MathLib_Interfaces_ByteDeviceSequence_H
#include "WritableSequence.hpp"
#include "ByteDevice.hpp"

namespace MathLib {
    template <typename T>
    struct ByteDeviceSequence : WritableSequence<T> {
        ByteDeviceSequence(ByteDevice& device) : device(device) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(device.GetSize() / sizeof(T));
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            StartAndReturnFromBenchmark(device.ReadPositioned<T>(index * sizeof(T)).GetOr(T()));
        }
        [[nodiscard]] virtual bool Set(size_t index, const T& value) override {
            StartAndReturnFromBenchmark(device.WritePositioned<T>(value, index * sizeof(T)));
        }
        [[nodiscard]] virtual bool Add(const T& value) override {
            StartAndReturnFromBenchmark(device.WritePositioned<T>(value, device.GetSize()));
        }
        [[nodiscard]] virtual bool Reset(void) override {
            StartAndReturnFromBenchmark(false);
        }

        private:
        ByteDevice& device;
    };
}

#endif