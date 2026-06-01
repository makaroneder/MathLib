#ifndef MathLib_Interfaces_Sequence_ExternString_H
#define MathLib_Interfaces_Sequence_ExternString_H
#include "Collection.hpp"

namespace MathLib {
    struct ExternString : Collection<char> {
        ExternString(char* buffer);
        [[nodiscard]] virtual bool Add(const char& chr) override;
        [[nodiscard]] virtual size_t GetSize(void) const override;
        [[nodiscard]] virtual char* GetValue(void) override;
        [[nodiscard]] virtual const char* GetValue(void) const override;
        [[nodiscard]] virtual bool Reset(void) override;

        private:
        char* buffer;
    };
}

#endif