#ifndef Buffer_H
#define Buffer_H
#include "Interfaces/Collection.hpp"

namespace MathLib {
    [[noreturn]] void Panic(const char*);
    template <typename T>
    struct Buffer : Collection<T> {
        Buffer(void) : buffer(nullptr), size(0) {}
        Buffer(size_t size) : buffer(size ? new T[size] : nullptr), size(size) {}
        Buffer(const T* arr, size_t size) : Buffer(size) {
            for (size_t i = 0; i < size; i++) At(i) = arr[i];
        }
        Buffer(const Buffer<T>& other) : Buffer(other.size) {
            if (size) {
                if (!buffer || !other.buffer) Panic("Buffer allocation failed");
                for (size_t i = 0; i < size; i++) buffer[i] = other.buffer[i];
            }
        }
        virtual ~Buffer(void) override {
            delete [] buffer;
        }
        virtual T At(size_t index) const override {
            if (index >= size) Panic("Index out of bounds");
            if (!buffer) Panic("Buffer allocation failed");
            return buffer[index];
        }
        virtual T& At(size_t index) override {
            if (index >= size) Panic("Index out of bounds");
            if (!buffer) Panic("Buffer allocation failed");
            return buffer[index];
        }
        virtual bool Add(const T& val) override {
            T* tmp = new T[size + 1];
            if (!tmp) return false;
            for (size_t i = 0; i < size; i++) tmp[i] = buffer[i];
            tmp[size++] = val;
            delete [] buffer;
            buffer = tmp;
            return true;
        }
        virtual size_t GetSize(void) const override {
            return size;
        }
        virtual const T* GetValue(void) const override {
            return buffer;
        }
        Buffer<T>& operator=(const Buffer<T>& other) {
            delete [] buffer;
            size = other.size;
            if (size) {
                buffer = new T[size];
                if (!buffer || !other.buffer) Panic("Buffer allocation failed");
                for (size_t i = 0; i < size; i++) buffer[i] = other.buffer[i];
            }
            else buffer = nullptr;
            return *this;
        }

        protected:
        T* buffer;
        size_t size;
    };
}

#endif