#ifndef MathLib_CSV_H
#define MathLib_CSV_H
#include "Interfaces/Printable.hpp"

namespace MathLib {
    struct CSV : Printable {
        CSV(void);
        CSV(const Sequence<char>& str);
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;
        [[nodiscard]] size_t GetHeight(void) const;
        [[nodiscard]] size_t GetWidth(size_t y) const;
        [[nodiscard]] size_t GetMaxWidth(void) const;
        [[nodiscard]] size_t GetKeyIndex(const Sequence<char>& key) const;
        [[nodiscard]] CSV Reduce(const Sequence<String>& fields) const;
        [[nodiscard]] String At(size_t x, size_t y) const;
        [[nodiscard]] String& At(size_t x, size_t y);

        private:
        Array<Array<String>> data;
    };
}

#endif