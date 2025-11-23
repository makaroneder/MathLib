#ifndef CSV_H
#define CSV_H
#include <Interfaces/Printable.hpp>

struct CSV : MathLib::Printable {
    CSV(void);
    CSV(const MathLib::Sequence<char>& str);
    [[nodiscard]] virtual MathLib::String ToString(const MathLib::Sequence<char>& padding = ""_M) const override;
    [[nodiscard]] size_t GetHeight(void) const;
    [[nodiscard]] size_t GetWidth(size_t y) const;
    [[nodiscard]] CSV Reduce(const MathLib::Sequence<MathLib::String>& fields) const;
    [[nodiscard]] MathLib::String At(size_t x, size_t y) const;
    [[nodiscard]] MathLib::String& At(size_t x, size_t y);
    [[nodiscard]] MathLib::String At(const MathLib::Sequence<char>& key, size_t y) const;
    [[nodiscard]] MathLib::String& At(const MathLib::Sequence<char>& key, size_t y);

    private:
    MathLib::Array<MathLib::Array<MathLib::String>> data;
};

#endif