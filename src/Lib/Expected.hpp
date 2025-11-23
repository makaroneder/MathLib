#ifndef MathLib_Expected_H
#define MathLib_Expected_H
#include "Host.hpp"

namespace MathLib {
    template <typename T>
    struct Expected : Collection<T>, Printable {
        Expected(void) : value(T()), hasValue(false) {}
        Expected(const T& value) : value(value), hasValue(true) {}
        [[nodiscard]] T GetOr(const T& alternative) const {
            return hasValue ? value : alternative;
        }
        [[nodiscard]] T Get(const Sequence<char>& panicStr) const {
            if (hasValue) return value;
            Panic(panicStr);
        }
        [[nodiscard]] T Get(const char* panicStr = "Expected value doesn't exist") const {
            if (hasValue) return value;
            Panic(panicStr);
        }
        [[nodiscard]] bool HasValue(void) const {
            return hasValue;
        }
        template <typename F>
        [[nodiscard]] Expected<F> Convert(void) const {
            return hasValue ? Expected<F>((F)value) : Expected<F>();
        }
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override {
            return CollectionToString(padding) + (hasValue ? Formatter<T>::ToString(Get()) : "No value");
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return hasValue;
        }
        [[nodiscard]] virtual bool Add(const T& x) override {
            if (hasValue) return false;
            value = x;
            hasValue = true;
            return true;
        }
        [[nodiscard]] virtual bool Reset(void) override {
            value = T();
            hasValue = false;
            return true;
        }
        [[nodiscard]] virtual T* GetValue(void) override {
            return &value;
        }
        [[nodiscard]] virtual const T* GetValue(void) const override {
            return &value;
        }

        private:
        T value;
        bool hasValue;
    };
}

#endif