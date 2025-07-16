#ifndef MathLib_Buffer_H
#define MathLib_Buffer_H
#include "Interfaces/Collection.hpp"

namespace MathLib {
    [[noreturn]] void Panic(const char*);
    template <typename T>
    struct Buffer : Collection<T> {
        Buffer(void) : buffer(nullptr), size(0) {
            EmptyBenchmark
        }
        Buffer(size_t size) : buffer(size ? new T[size] : nullptr), size(size) {
            EmptyBenchmark
        }
        Buffer(const T* arr, size_t size) : Buffer(size) {
            StartBenchmark
            for (size_t i = 0; i < size; i++) this->At(i) = arr[i];
            EndBenchmark
        }
        Buffer(const Buffer<T>& other) : Buffer(other.size) {
            StartBenchmark
            if (size) {
                if (!buffer || !other.buffer) Panic("Buffer allocation failed");
                for (size_t i = 0; i < size; i++) buffer[i] = other.buffer[i];
            }
            EndBenchmark
        }
        virtual ~Buffer(void) override {
            StartBenchmark
            if (buffer) delete [] buffer;
            EndBenchmark
        }
        [[nodiscard]] virtual bool Add(const T& val) override {
            StartBenchmark
            T* tmp = new T[size + 1];
            if (!tmp) ReturnFromBenchmark(false);
            for (size_t i = 0; i < size; i++) tmp[i] = buffer[i];
            tmp[size++] = val;
            if (buffer) delete [] buffer;
            buffer = tmp;
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] virtual bool Reset(void) override {
            StartBenchmark
            if (buffer) delete [] buffer;
            size = 0;
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(size);
        }
        [[nodiscard]] virtual T* GetValue(void) override {
            StartAndReturnFromBenchmark(buffer);
        }
        [[nodiscard]] virtual const T* GetValue(void) const override {
            StartAndReturnFromBenchmark(buffer);
        }
        Buffer<T>& operator=(const Buffer<T>& other) {
            StartBenchmark
            if (buffer) delete [] buffer;
            size = other.size;
            if (size) {
                buffer = new T[size];
                if (!buffer || !other.buffer) Panic("Buffer allocation failed");
                for (size_t i = 0; i < size; i++) buffer[i] = other.buffer[i];
            }
            else buffer = nullptr;
            ReturnFromBenchmark(*this);
        }

        protected:
        T* buffer;
        size_t size;
    };
}

#endif