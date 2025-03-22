#ifndef MathLib_Interfaces_ByteArray_H
#define MathLib_Interfaces_ByteArray_H
#include "Saveable.hpp"

namespace MathLib {
    struct ByteArray : Collection<uint8_t>, ByteDevice, Saveable {
        ByteArray(void);
        ByteArray(size_t size);
        ByteArray(const Sequence<uint8_t>& sequence);
        template <typename T>
        static ByteArray ToByteArray(const Sequence<T>& sequence) {
            StartBenchmark
            ByteArray ret = ByteArray(sequence.GetSize() * sizeof(T));
            const size_t size = sequence.GetSize();
            for (size_t i = 0; i < size; i++) {
                const T tmp = sequence.At(i);
                const uint8_t* buff = (const uint8_t*)&tmp;
                for (size_t j = 0; j < sizeof(T); j++) ret.At(i * sizeof(T) + j) = buff[j];
            }
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] Array<uint8_t> GetArray(void) const;
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual size_t ReadPositionedSizedBuffer(void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual size_t WritePositionedSizedBuffer(const void* buffer, size_t size, size_t position) override;
        [[nodiscard]] virtual uint8_t At(size_t index) const override;
        [[nodiscard]] virtual uint8_t& At(size_t index) override;
        [[nodiscard]] virtual bool Add(const uint8_t& val) override;
        [[nodiscard]] virtual const uint8_t* GetValue(void) const override;
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;

        private:
        Array<uint8_t> array;
    };
}

#endif