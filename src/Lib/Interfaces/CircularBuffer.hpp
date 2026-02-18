#ifndef MathLib_Interfaces_CircularBuffer_H
#define MathLib_Interfaces_CircularBuffer_H
#include "../Typedefs.hpp"

namespace MathLib {
    template <typename T>
    struct CircularBuffer {
        CircularBuffer(size_t size) : data(size), readPosition(0), writePosition(0) {}
        CircularBuffer(const Sequence<T>& data) : data(CollectionToArray<T>(data)), readPosition(0), writePosition(0) {}
        [[nodiscard]] T Get(void) {
            const T ret = data.At(readPosition);
            readPosition = (readPosition + 1) % data.GetSize();
            return ret;
        }
        void Set(const T& value) {
            data.At(writePosition) = value;
            writePosition = (writePosition + 1) % data.GetSize();
        }
        void SetPosition(size_t position, bool read) {
            position %= data.GetSize();
            if (read) readPosition = position;
            else writePosition = position;
        }

        private:
        Array<T> data;
        size_t readPosition;
        size_t writePosition;
    };
}

#endif