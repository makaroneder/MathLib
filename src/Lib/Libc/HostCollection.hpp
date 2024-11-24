#ifndef Freestanding
#ifndef Libc_HostCollection_H
#define Libc_HostCollection_H
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
        virtual T At(size_t index) const override {
            return buffer.at(index);
        }
        virtual T& At(size_t index) override {
            return buffer.at(index);
        }
        virtual bool Add(const T& val) override {
            buffer.push_back(val);
            return true;
        }
        virtual size_t GetSize(void) const override {
            return buffer.size();
        }
        virtual const T* GetValue(void) const override {
            return buffer.data();
        }
        std::vector<T> ToVector(void) const {
            return buffer;
        }

        private:
        std::vector<T> buffer;
    };
}

#endif
#endif