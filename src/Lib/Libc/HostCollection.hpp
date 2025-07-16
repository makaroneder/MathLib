#ifndef Freestanding
#ifndef MathLib_Libc_HostCollection_H
#define MathLib_Libc_HostCollection_H
#include "../Interfaces/Collection.hpp"
#include <vector>

namespace MathLib {
    template <typename T>
    struct HostCollection : Collection<T> {
        HostCollection(void) {
            EmptyBenchmark
        }
        HostCollection(const std::vector<T>& arr) : buffer(arr) {
            EmptyBenchmark
        }
        HostCollection(size_t size) : buffer(std::vector<T>(size)) {
            EmptyBenchmark
        }
        HostCollection(const T* arr, size_t size) {
            StartBenchmark
            buffer.reserve(size);
            for (size_t i = 0; i < size; i++) buffer.push_back(arr[i]);
            EndBenchmark
        }
        [[nodiscard]] virtual bool Add(const T& val) override {
            StartBenchmark
            buffer.push_back(val);
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] virtual bool Reset(void) override {
            StartBenchmark
            buffer.clear();
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(buffer.size());
        }
        [[nodiscard]] virtual T* GetValue(void) override {
            StartAndReturnFromBenchmark(buffer.data());
        }
        [[nodiscard]] virtual const T* GetValue(void) const override {
            StartAndReturnFromBenchmark(buffer.data());
        }
        [[nodiscard]] std::vector<T> ToVector(void) const {
            StartAndReturnFromBenchmark(buffer);
        }

        private:
        std::vector<T> buffer;
    };
}

#endif
#endif