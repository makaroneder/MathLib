#ifndef MathLib_Automaton_Tape_H
#define MathLib_Automaton_Tape_H
#include "../Typedefs.hpp"

namespace MathLib {
    template <typename T>
    struct Tape {
        Tape(void) : data(), position(0) {
            EmptyBenchmark
        }
        Tape(const Sequence<T>& sequence) : data(CollectionToArray<T>(sequence)), position(data.GetSize()) {
            EmptyBenchmark
        }
        [[nodiscard]] Array<T> GetArray(void) const {
            StartAndReturnFromBenchmark(data);
        }
        [[nodiscard]] bool IsOutside(void) const {
            StartAndReturnFromBenchmark(!position);
        }
        [[nodiscard]] T Get(void) const {
            StartAndReturnFromBenchmark(data.At(position));
        }
        void Set(const T& x) {
            const size_t size = data.GetSize();
            if (position >= size) {
                Array<T> tmp = Array<T>(position + 1);
                for (size_t i = 0; i < size; i++) tmp.At(i) = data.At(i);
                data = tmp;
            }
            data.At(position) = x;
        }
        void Move(ssize_t direction) {
            StartBenchmark
            const size_t size = data.GetSize();
            const ssize_t pos = position + direction;
            position = pos < 0 ? 0 : pos;
            if (pos >= 0 && (size_t)pos <= size) return;
            Array<T> tmp = Array<T>(size + Abs(direction));
            if (direction < 0) for (size_t i = 0; i < size; i++) tmp.At(i - direction) = data.At(i);
            else for (size_t i = 0; i < size; i++) tmp.At(i) = data.At(i);
            data = tmp;
            EndBenchmark
        }
        void Push(const T& x) {
            StartBenchmark
            Set(x);
            Move(1);
            EndBenchmark
        }
        [[nodiscard]] T Pop(void) {
            StartBenchmark
            if (!position) ReturnFromBenchmark(T());
            Move(-1);
            ReturnFromBenchmark(data.At(position));
        }
        bool operator==(const Tape<T>& other) const {
            StartAndReturnFromBenchmark(data == other.data && position == other.position);
        }
        bool operator!=(const Tape<T>& other) const {
            StartAndReturnFromBenchmark(!(*this == other));
        }

        private:
        Array<T> data;
        size_t position;
    };
}

#endif