#ifndef Freestanding
#ifndef MathLib_Libc_HostCollection_H
#define MathLib_Libc_HostCollection_H
#include "../Interfaces/Collection.hpp"
#include <vector>

namespace MathLib {
    template <typename T>
    struct HostCollection : Collection<T> {
        HostCollection(void) {}
        HostCollection(const std::vector<T>& arr) : buffer(arr) {}
        HostCollection(size_t size) : buffer(std::vector<T>(size)) {}
        HostCollection(const T* arr, size_t size) {
            buffer.reserve(size);
            for (size_t i = 0; i < size; i++) buffer.push_back(arr[i]);
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            return buffer.at(index);
        }
        [[nodiscard]] virtual T& At(size_t index) override {
            return buffer.at(index);
        }
        [[nodiscard]] virtual bool Add(const T& val) override {
            buffer.push_back(val);
            return true;
        }
        [[nodiscard]] virtual bool Reset(void) override {
            buffer.clear();
            return true;
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return buffer.size();
        }
        [[nodiscard]] virtual const T* GetValue(void) const override {
            return buffer.data();
        }
        [[nodiscard]] std::vector<T> ToVector(void) const {
            return buffer;
        }

        private:
        std::vector<T> buffer;
    };
}

#endif
#endif