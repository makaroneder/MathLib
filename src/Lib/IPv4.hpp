#ifndef MathLib_IPv4_H
#define MathLib_IPv4_H
#include "Interfaces/Saveable.hpp"

namespace MathLib {
    struct IPv4 : Saveable, Printable {
        IPv4(void);
        IPv4(const Sequence<char>& str);
        [[nodiscard]] uint32_t GetRaw(void) const;
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;
        [[nodiscard]] virtual String ToString(const Sequence<char>& padding = ""_M) const override;

        private:
        uint32_t data;
    };
}

#endif