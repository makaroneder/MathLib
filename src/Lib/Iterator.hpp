#ifndef Iterator_H
#define Iterator_H
#include "Interfaces/Allocatable.hpp"

template <typename T>
struct Iterator : Allocatable {
    Iterator(T* data) : data(data) {}
    T& operator*(void) const {
        return *data;
    }
    T* operator->(void) {
        return data;
    }
    Iterator<T>& operator++(void) {
        data++;
        return *this;
    }
    Iterator<T> operator++(int) {
        Iterator<T> tmp = *this;
        data++;
        return tmp;
    }
    bool operator==(const Iterator<T>& other) const {
        return data == other.data;
    }
    bool operator!=(const Iterator<T>& other) const {
        return data != other.data;
    }

    private:
    T* data;
};
template <typename T>
struct Iteratable : Allocatable {
    virtual Iterator<const T> begin(void) const = 0;
    virtual Iterator<const T> end(void) const = 0;
    virtual Iterator<T> begin(void) = 0;
    virtual Iterator<T> end(void) = 0;
};

#endif