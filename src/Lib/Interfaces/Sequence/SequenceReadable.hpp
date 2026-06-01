#ifndef MathLib_Interfaces_Sequence_Readable_H
#define MathLib_Interfaces_Sequence_Readable_H
#include "../Readable.hpp"

namespace MathLib {
    struct SequenceReadable : Readable {
        SequenceReadable(const Sequence<uint8_t>& sequence);
        void ResetPosition(void);
        [[nodiscard]] virtual size_t ReadSizedBuffer(void* buffer, size_t size) override;
        [[nodiscard]] virtual bool Skip(size_t size) override;

        private:
        const Sequence<uint8_t>& sequence;
        size_t position;
    };
}

#endif