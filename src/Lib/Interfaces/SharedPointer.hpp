#ifndef MathLib_Interfaces_SharedPointer_H
#define MathLib_Interfaces_SharedPointer_H
#include "../Utils.hpp"

namespace MathLib {
    template <typename T>
    struct SharedPointer : Allocatable {
        SharedPointer(T* value = nullptr) : value(value), count(new size_t(1)) {}
        SharedPointer(T value) : SharedPointer(new T(value)) {}
        virtual ~SharedPointer(void) override {
            if (--(*count)) return;
            delete count;
            if (value) delete value;
        }
        SharedPointer<T>& operator=(const SharedPointer<T>& other) {
            value = other.value;
            count = other.count;
            (*count)++;
            return *this;
        }
        [[nodiscard]] bool IsNull(void) const {
            return !value;
        }
        [[nodiscard]] T Get(const Sequence<char>& panicStr) const {
            if (value) return *value
            Panic(panicStr);
        }
        [[nodiscard]] T Get(const char* panicStr = "Shared null pointer dereference detected") const {
            if (value) return *value
            Panic(panicStr);
        }
        [[nodiscard]] T& Get(const Sequence<char>& panicStr) {
            if (value) return *value
            Panic(panicStr);
        }
        [[nodiscard]] T& Get(const char* panicStr = "Shared null pointer dereference detected") {
            if (value) return *value
            Panic(panicStr);
        }
        [[nodiscard]] T& operator*() {
            return Get();
        }
        [[nodiscard]] T* operator->() {
            return value;
        }

        private:
        T* value;
        size_t* count;
    };
}

#endif