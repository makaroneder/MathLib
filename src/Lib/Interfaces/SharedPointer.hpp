#ifndef MathLib_Interfaces_SharedPointer_H
#define MathLib_Interfaces_SharedPointer_H
#include "../Utils.hpp"

namespace MathLib {
    template <typename T>
    struct SharedPointer : Allocatable {
        SharedPointer(T* value = nullptr) : value(value), count(new size_t(1)) {
            EmptyBenchmark
        }
        SharedPointer(T value) : SharedPointer(new T(value)) {
            EmptyBenchmark
        }
        virtual ~SharedPointer(void) override {
            StartBenchmark
            if (!--(*count)) {
                delete count;
                if (value) delete value;
            }
            EndBenchmark
        }
        SharedPointer<T>& operator=(const SharedPointer<T>& other) {
            StartBenchmark
            value = other.value;
            count = other.count;
            (*count)++;
            ReturnFromBenchmark(*this);
        }
        [[nodiscard]] bool IsNull(void) const {
            StartAndReturnFromBenchmark(!value);
        }
        [[nodiscard]] T Get(const Sequence<char>& panicStr) const {
            StartBenchmark
            if (!value) Panic(panicStr);
            ReturnFromBenchmark(*value);
        }
        [[nodiscard]] T Get(const char* panicStr = "Shared null pointer dereference detected") const {
            StartBenchmark
            if (!value) Panic(panicStr);
            ReturnFromBenchmark(*value);
        }
        [[nodiscard]] T& Get(const Sequence<char>& panicStr) {
            StartBenchmark
            if (!value) Panic(panicStr);
            ReturnFromBenchmark(*value);
        }
        [[nodiscard]] T& Get(const char* panicStr = "Shared null pointer dereference detected") {
            StartBenchmark
            if (!value) Panic(panicStr);
            ReturnFromBenchmark(*value);
        }
        [[nodiscard]] T& operator*() {
            StartAndReturnFromBenchmark(Get());
        }
        [[nodiscard]] T* operator->() {
            StartAndReturnFromBenchmark(value);
        }

        private:
        T* value;
        size_t* count;
    };
}

#endif