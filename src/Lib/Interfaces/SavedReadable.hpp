#ifndef MathLib_Interfaces_SavedReadable_H
#define MathLib_Interfaces_SavedReadable_H
#include "Readable.hpp"
#include "Writable.hpp"

namespace MathLib {
    struct SavedReadable : Readable {
        SavedReadable(Readable& readable, Writable& writable);
        [[nodiscard]] virtual size_t ReadSizedBuffer(void* buffer, size_t size) override;
        [[nodiscard]] virtual bool Skip(size_t size) override;

        private:
        Readable& readable;
        Writable& writable;
    };
}

#endif