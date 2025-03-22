#ifndef MathLib_Bitmap_H
#define MathLib_Bitmap_H
#include "Interfaces/ByteArray.hpp"

namespace MathLib {
    struct Bitmap : Sequence<bool>, Saveable {
        Bitmap(void);
        Bitmap(size_t size);
        Bitmap(const Sequence<uint8_t>& array);
        Bitmap(const Sequence<uint8_t>& array, uint8_t lastByteSize);
        void Fill(bool value);
        [[nodiscard]] bool IsFilledWith(bool value) const;
        [[nodiscard]] Array<uint8_t> GetArray(void) const;
        [[nodiscard]] uint8_t GetLastByteSize(void) const;
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual bool At(size_t index) const override;
        [[nodiscard]] bool Reverse(void);
        [[nodiscard]] bool Add(bool value);
        [[nodiscard]] bool Add(const Sequence<bool>& other);
        [[nodiscard]] bool Flip(size_t index);
        [[nodiscard]] Expected<bool> Get(size_t index) const;
        [[nodiscard]] bool Set(size_t index, bool value);
        [[nodiscard]] uint64_t Read(size_t index, uint8_t bits) const;
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
        [[nodiscard]] Bitmap operator~(void) const;
        [[nodiscard]] Bitmap operator^(const Bitmap& other) const;
        [[nodiscard]] Bitmap operator|(const Bitmap& other) const;
        [[nodiscard]] Bitmap operator&(const Bitmap& other) const;
        Bitmap& operator^=(const Bitmap& other);
        Bitmap& operator|=(const Bitmap& other);
        Bitmap& operator&=(const Bitmap& other);

        private:
        [[nodiscard]] uint8_t GetLastByteMask(void) const;

        ByteArray array;
        uint8_t lastByteSize;
    };
}

#endif