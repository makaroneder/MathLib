#ifndef MathLib_Interfaces_CountedReadable_H
#define MathLib_Interfaces_CountedReadable_H
#include "Readable.hpp"

namespace MathLib {
    struct CountedReadable : Readable {
        CountedReadable(Readable& base);
        [[nodiscard]] virtual size_t ReadSizedBuffer(void* buffer, size_t size) override;
        [[nodiscard]] virtual bool Skip(size_t size) override;
        [[nodiscard]] size_t GetReadBytes(void) const;
        void ResetReadBytes(void);

        private:
        Readable& base;
        size_t read;
    };
}

#endif