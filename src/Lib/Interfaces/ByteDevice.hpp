#ifndef MathLib_Interfaces_ByteDevice_H
#define MathLib_Interfaces_ByteDevice_H
#include "../SeekMode.hpp"
#include "Readable.hpp"
#include "Writable.hpp"

namespace MathLib {
    struct ByteDevice : Readable, Writable {
        ByteDevice(void);
        [[nodiscard]] virtual size_t GetSize(void) const = 0;
        [[nodiscard]] virtual size_t ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) = 0;
        [[nodiscard]] virtual size_t WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) = 0;
        [[nodiscard]] virtual size_t ReadSizedBuffer(void* buffer, size_t size) override;
        [[nodiscard]] virtual size_t WriteSizedBuffer(const void* buffer, size_t size) override;
        [[nodiscard]] virtual bool Skip(size_t size) override;
        [[nodiscard]] bool ReadPositionedBuffer(void* buffer, size_t size, size_t position);
        [[nodiscard]] bool WritePositionedBuffer(const void* buffer, size_t size, size_t position);
        [[nodiscard]] bool Seek(ssize_t offset, SeekMode mode);
        [[nodiscard]] size_t Tell(void) const;
        [[nodiscard]] size_t GetSizeLeft(void) const;
        [[nodiscard]] bool CopyTo(Writable& dst);
        [[nodiscard]] Array<uint8_t> ReadAll(void);
        template <typename T>
        [[nodiscard]] bool ReadPositioned(T& value, size_t position) {
            return ReadPositionedBuffer(&value, sizeof(T), position);
        }
        template <typename T>
        [[nodiscard]] bool WritePositioned(const T& value, size_t position) {
            return WritePositionedBuffer(&value, sizeof(T), position);
        }
        template <typename T>
        [[nodiscard]] Expected<T> ReadPositioned(size_t position) {
            T ret;
            return ReadPositioned<T>(ret, position) ? Expected<T>(ret) : Expected<T>();
        }
        template <typename T>
        [[nodiscard]] bool ReadPositionedCollection(Collection<T>& sequence, size_t position) {
            return ReadPositionedBuffer(sequence.GetValue(), sequence.GetSize() * sizeof(T), position);
        }

        private:
        size_t position;
    };
}

#endif