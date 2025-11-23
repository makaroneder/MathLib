#ifndef MathLib_Interfaces_Formatter_H
#define MathLib_Interfaces_Formatter_H
#include "../String.hpp"
#include "Sequence/EmptySequence.hpp"
#ifndef Freestanding
#include <ostream>
#endif

namespace MathLib {
    [[nodiscard]] String ToString(num_t x);
    template <typename T>
    struct Formatter {
        [[nodiscard]] static String ToString(const T&, const Sequence<char>& padding = EmptySequence<char>()) {
            return CollectionToString(padding) + "Unprintable";
        }
    };
    #define MakeNonPrintableFormatter(T, self, padding, code, ...)                                                      \
    template <__VA_ARGS__> struct Formatter<T> {                                                                        \
        [[nodiscard]] static String ToString(const T& self, const Sequence<char>& padding = EmptySequence<char>()) code \
    }
    #ifdef Freestanding
    #define MakeFormatter MakeNonPrintableFormatter
    #define MakeTemplatedFormatter MakeNonPrintableFormatter
    #else
    #define MakeFormatter(T, self, padding, code)                               \
    MakeNonPrintableFormatter(T, self, padding, code);                          \
    static std::ostream& operator<<(std::ostream& stream, const T& printable) { \
        return stream << Formatter<T>::ToString(printable).GetValue();          \
    }
    #define MakeTemplatedFormatter(T, self, padding, code, ...)                                 \
    MakeNonPrintableFormatter(T, self, padding, code, __VA_ARGS__);                             \
    template <__VA_ARGS__> std::ostream& operator<<(std::ostream& stream, const T& printable) { \
        return stream << Formatter<T>::ToString(printable).GetValue();                          \
    }
    #endif
    #define MakeDecimalFormatter(T)                                         \
    MakeNonPrintableFormatter(T, self, padding, {                           \
        return CollectionToString(padding) + MathLib::ToString(self, 10);   \
    })
    #define MakeFloatFormatter(T)                                       \
    MakeNonPrintableFormatter(T, self, padding, {                       \
        return CollectionToString(padding) + MathLib::ToString(self);   \
    })
    #define MakeStringFormatter(T)                  \
    MakeFormatter(T, self, padding, {               \
        return CollectionToString(padding) + self;  \
    })
    MakeNonPrintableFormatter(bool, self, padding, {
        return CollectionToString(padding) + BoolToString(self);
    });
    MakeDecimalFormatter(uint64_t);
    MakeDecimalFormatter(uint32_t);
    MakeDecimalFormatter(uint16_t);
    MakeDecimalFormatter(uint8_t);
    MakeDecimalFormatter(int64_t);
    MakeDecimalFormatter(int32_t);
    MakeDecimalFormatter(int16_t);
    MakeFloatFormatter(num_t);
    MakeFloatFormatter(double);
    MakeFloatFormatter(float);
    MakeStringFormatter(Sequence<char>);
    MakeStringFormatter(WritableSequence<char>);
    MakeStringFormatter(NonLinearCollection<char>);
    MakeStringFormatter(Collection<char>);
    MakeStringFormatter(EmptySequence<char>);
    MakeStringFormatter(String);
}

#endif