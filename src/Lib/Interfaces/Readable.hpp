#ifndef MathLib_Interfaces_Readable_H
#define MathLib_Interfaces_Readable_H
#include "../Expected.hpp"

namespace MathLib {
    struct Readable : Allocatable {
        [[nodiscard]] virtual size_t ReadSizedBuffer(void* buffer, size_t size) = 0;
        [[nodiscard]] virtual bool Skip(size_t size) = 0;
        [[nodiscard]] bool ReadBuffer(void* buffer, size_t size);
        [[nodiscard]] Expected<char> Getc(void);
        [[nodiscard]] String ReadUntil(char end);
        template <typename T>
        [[nodiscard]] bool Read(T& value) {
            return ReadBuffer(&value, sizeof(T));
        }
        template <typename T>
        [[nodiscard]] Expected<T> Read(void) {
            T ret;
            return Read<T>(ret) ? Expected<T>(ret) : Expected<T>();
        }
        template <typename T>
        [[nodiscard]] bool ReadCollection(Collection<T>& sequence) {
            return ReadBuffer(sequence.GetValue(), sequence.GetSize() * sizeof(T));
        }

        protected:
        [[nodiscard]] bool DefaultSkip(size_t size);
    };
}

#endif