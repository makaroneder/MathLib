#ifndef Collection_H
#define Collection_H
#include "../Iterator.hpp"

template <typename T>
struct Collection : Iteratable<T> {
    virtual T At(const size_t& index) const = 0;
    virtual T& At(const size_t& index) = 0;
    virtual bool Add(const T& val) = 0;
    virtual size_t GetSize(void) const = 0;
    virtual const T* GetValue(void) const = 0;
    bool IsEmpty(void) const {
        return !GetSize();
    }
    virtual Iterator<const T> begin(void) const override {
        return Iterator<const T>(GetValue());
    }
    virtual Iterator<const T> end(void) const override {
        return Iterator<const T>(GetValue() + GetSize());
    }
    virtual Iterator<T> begin(void) override {
        return Iterator<T>((T*)GetValue());
    }
    virtual Iterator<T> end(void) override {
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