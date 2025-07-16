#ifndef MathLib_Interfaces_ByteArray_H
#define MathLib_Interfaces_ByteArray_H
#include "Saveable.hpp"

namespace MathLib {
    struct ByteArray : Collection<uint8_t>, ByteDevice, Saveable, Printable {
        ByteArray(void);
        ByteArray(size_t size);
        ByteArray(const Sequence<uint8_t>& sequence);
        ByteArray(ByteDevice& device);
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
        [[nodiscard]] virtual bool Add(const uint8_t& val) override;
        [[nodiscard]] virtual bool Reset(void) override;
        [[nodiscard]] virtual uint8_t* GetValue(void) override;
        [[nodiscard]] virtual const uint8_t* GetValue(void) const override;
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        Array<uint8_t> array;
    };
}

#endif