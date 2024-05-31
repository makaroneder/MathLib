#ifndef Buffer_H
#define Buffer_H
#include "Collection.hpp"
#include "Host.hpp"

template <typename T>
struct Buffer : Collection<T> {
    Buffer(void) : buffer(nullptr), size(0) {}
    Buffer(size_t size_) : buffer(new T[size_]), size(size_) {}
    Buffer(const Buffer<T>& other) : buffer(new T[other.size]), size(other.size) {
        if (other.size != 0) {
            if (!buffer || !other.buffer) Panic("Buffer allocation failed");
            for (size_t i = 0; i < size; i++) buffer[i] = other.buffer[i];
        }
    }
    virtual ~Buffer(void) {
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
    virtual bool Add(T val) override {
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
    Buffer<T>& operator=(const Buffer<T>& other) {
        delete [] buffer;
        size = other.size;
        buffer = new T[size];
        if (!buffer || !other.buffer) Panic("Buffer allocation failed");
        for (size_t i = 0; i < size; i++) buffer[i] = other.buffer[i];
        return *this;
    }

    private:
    T* buffer;
    size_t size;
};

#endif