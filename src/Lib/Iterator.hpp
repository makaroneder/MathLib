#ifndef MathLib_Iterator_H
#define MathLib_Iterator_H
#include "Allocator/Allocatable.hpp"

namespace MathLib {
    template <typename T>
    struct Iterator : Allocatable {
        Iterator(T* data) : data(data) {}
        [[nodiscard]] T& operator*(void) const {
            return *data;
        }
        [[nodiscard]] T* operator->(void) {
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
        [[nodiscard]] bool operator==(const Iterator<T>& other) const {
            return data == other.data;
        }
        [[nodiscard]] bool operator!=(const Iterator<T>& other) const {
            return data != other.data;
        }

        private:
        T* data;
    };
}

#endif