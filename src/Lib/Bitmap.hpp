#ifndef MathLib_Bitmap_H
#define MathLib_Bitmap_H
#include "Interfaces/Sequence/ByteArray.hpp"

namespace MathLib {
    struct Bitmap : WritableSequence<bool>, Saveable {
        using WritableSequence<bool>::Add;
        Bitmap(void);
        Bitmap(size_t size);
        Bitmap(const Sequence<bool>& data);
        Bitmap(const Sequence<uint8_t>& array);
        Bitmap(const Sequence<uint8_t>& array, uint8_t lastByteSize);
        void Fill(bool value);
        [[nodiscard]] bool IsFilledWith(bool value) const;
        [[nodiscard]] Array<uint8_t> GetArray(void) const;
        [[nodiscard]] uint8_t GetLastByteSize(void) const;
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual bool AtUnsafe(size_t index) const override;
        [[nodiscard]] virtual bool Add(const bool& value) override;
        [[nodiscard]] virtual bool Reset(void) override;
        [[nodiscard]] bool Flip(size_t index);
        [[nodiscard]] Expected<bool> Get(size_t index) const;
        [[nodiscard]] virtual bool SetUnsafe(size_t index, const bool& value) override;
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
    Bitmap MakeBitmap(bool arg);
    template <typename... Args>
    Bitmap MakeBitmap(bool arg, Args... args) {
        Bitmap ret = MakeBitmap(arg);
        return ret.AddSequence(MakeBitmap(args...)) ? ret : Bitmap();
    }
}

#endif