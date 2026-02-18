#ifndef MathLib_Interfaces_Sequence_ByteDevice_H
#define MathLib_Interfaces_Sequence_ByteDevice_H
#include "WritableSequence.hpp"
#include "../ByteDevice.hpp"

namespace MathLib {
    template <typename T>
    struct ByteDeviceSequence : WritableSequence<T> {
        ByteDeviceSequence(ByteDevice& device) : device(device) {}
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return device.GetSize() / sizeof(T);
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return device.ReadPositioned<T>(index * sizeof(T)).GetOr(T());
        }
        [[nodiscard]] virtual bool SetUnsafe(size_t index, const T& value) override {
            return device.WritePositioned<T>(value, index * sizeof(T));
        }
        [[nodiscard]] virtual bool Add(const T& value) override {
            return device.WritePositioned<T>(value, device.GetSize());
        }
        [[nodiscard]] virtual bool Reset(void) override {
            return false;
        }

        private:
        ByteDevice& device;
    };
}

#endif