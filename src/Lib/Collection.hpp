#ifndef Collection_H
#define Collection_H
#include <stddef.h>

template <typename T>
struct Collection {
    virtual T At(size_t index) const = 0;
    virtual T& At(size_t index) = 0;
    virtual bool Add(T val) = 0;
    virtual size_t GetSize(void) const = 0;
    virtual const T* GetValue(void) const = 0;
    bool IsEmpty(void) const {
        return GetSize() == 0;
    }
    T operator[](size_t index) const {
        return At(index);
    }
    T& operator[](size_t index) {
        return At(index);
    }
};

#endif