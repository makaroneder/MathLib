#ifndef Collection_H
#define Collection_H
#include "Iterator.hpp"

template <typename T>
struct Collection : Allocatable {
    virtual T At(const size_t& index) const = 0;
    virtual T& At(const size_t& index) = 0;
    virtual bool Add(const T& val) = 0;
    virtual size_t GetSize(void) const = 0;
    virtual const T* GetValue(void) const = 0;
    bool IsEmpty(void) const {
        return GetSize() == 0;
    }
    Iterator<const T> begin(void) const {
        return Iterator<const T>(GetValue());
    }
    Iterator<const T> end(void) const {
        return Iterator<const T>(GetValue() + GetSize());
    }
    Iterator<T> begin(void) {
        return Iterator<T>((T*)GetValue());
    }
    Iterator<T> end(void) {
        return Iterator<T>((T*)GetValue() + GetSize());
    }
    T operator[](const size_t& index) const {
        return At(index);
    }
    T& operator[](const size_t& index) {
        return At(index);
    }
    Collection<T>& operator+=(const T& val) {
        Add(val);
        return *this;
    }
    Collection<T>& operator+=(const Collection<T>& other) {
        for (size_t i = 0; i < other.GetSize(); i++) Add(other.At(i));
        return *this;
    }
};

#endif