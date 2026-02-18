#ifndef MathLib_Interfaces_Sequence_Writable_H
#define MathLib_Interfaces_Sequence_Writable_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T>
    struct WritableSequence : Sequence<T> {
        [[nodiscard]] virtual bool SetUnsafe(size_t index, const T& value) = 0;
        [[nodiscard]] virtual bool Add(const T& value) = 0;
        [[nodiscard]] virtual bool Reset(void) = 0;
        [[nodiscard]] bool Set(size_t index, const T& value) {
            if (index < this->GetSize()) return SetUnsafe(index, value);
            Panic("Index out of bounds");
        }
        [[nodiscard]] bool AddSequence(const Sequence<T>& other) {
            const size_t size = other.GetSize();
            for (size_t i = 0; i < size; i++)
                if (!Add(other.AtUnsafe(i))) return false;
            return true;
        }
        [[nodiscard]] bool AddUnique(const T& value) {
            return this->Contains(value) || Add(value);
        }
        [[nodiscard]] bool SwapUnsafe(size_t x, size_t y) {
            const T tmp = this->AtUnsafe(x);
            return SetUnsafe(x, this->AtUnsafe(y)) && SetUnsafe(y, tmp);
        }
        [[nodiscard]] bool Swap(size_t x, size_t y) {
            return x < this->GetSize() && y < this->GetSize() && SwapUnsafe(x, y);
        }
        [[nodiscard]] bool Fill(const T& value) {
            const size_t size = this->GetSize();
            for (size_t i = 0; i < size; i++)
                if (!SetUnsafe(i, value)) return false;
            return true;
        }
        [[nodiscard]] bool Foreach(const Function<T, T>& function) {
            const size_t size = this->GetSize();
            for (size_t i = 0; i < size; i++)
                if (!SetUnsafe(i, function(this->AtUnsafe(i)))) return false;
            return true;
        }
        [[nodiscard]] bool Reverse(void) {
            return Reverse(0, this->GetSize());
        }
        [[nodiscard]] bool Reverse(size_t start, size_t end) {
            if (end > this->GetSize() || !end) return false;
            while (start < end)
                if (!SwapUnsafe(start++, --end)) return false;
            return true;
        }
        [[nodiscard]] bool Permutate(const Function<size_t, size_t>& permutation) {
            const size_t size = this->GetSize();
            T buff[size];
            for (size_t i = 0; i < size; i++) {
                const size_t tmp = permutation(i);
                if (tmp >= size) return false;
                buff[tmp] = this->AtUnsafe(i);
            }
            for (size_t i = 0; i < size; i++)
                if (!SetUnsafe(i, buff[i])) return false;
            return true;
        }
        [[nodiscard]] bool BubbleSort(const Function<bool, T, T>& compare) {
            size_t size = this->GetSize();
            while (size > 1) {
                size_t last = 0;
                for (size_t i = 1; i < size; i++) {
                    if (!compare(this->AtUnsafe(i - 1), this->AtUnsafe(i))) continue;
                    if (!SwapUnsafe(i - 1, i)) return false;
                    last = i;
                }
                size = last;
            }
            return true;
        }
        [[nodiscard]] bool CocktailShakerSort(const Function<bool, T, T>& compare) {
            size_t size = this->GetSize();
            size_t start = 1;
            while (size > start) {
                size_t last = 0;
                for (size_t i = start; i < size; i++) {
                    if (!compare(this->AtUnsafe(i - 1), this->AtUnsafe(i))) continue;
                    if (!SwapUnsafe(i - 1, i)) return false;
                    last = i;
                }
                size = last;
                last = 0;
                for (size_t i = size; i >= start + 1; i--) {
                    if (!compare(this->AtUnsafe(i - 2), this->AtUnsafe(i - 1))) continue;
                    if (!SwapUnsafe(i - 2, i - 1)) return false;
                    last = i;
                }
                start = last - 1;
            }
            return true;
        }
        [[nodiscard]] bool StoogeSort(const Function<bool, T, T>& compare) {
            const size_t size = this->GetSize();
            return size <= 1 || StoogeSort(compare, 0, size - 1);
        }
        [[nodiscard]] bool OddEvenSort(const Function<bool, T, T>& compare) {
            const size_t size = this->GetSize();
            if (size <= 1) return true;
            while (true) {
                bool sorted = true;
                for (size_t i = 1; i < size - 1; i += 2) {
                    if (!compare(this->AtUnsafe(i), this->AtUnsafe(i + 1))) continue;
                    if (!SwapUnsafe(i, i + 1)) return false;
                    sorted = false;
                }
                for (size_t i = 0; i < size - 1; i += 2) {
                    if (!compare(this->AtUnsafe(i), this->AtUnsafe(i + 1))) continue;
                    if (!SwapUnsafe(i, i + 1)) return false;
                    sorted = false;
                }
                if (sorted) break;
            }
            return true;
        }
        [[nodiscard]] bool InsertationSort(const Function<bool, T, T>& compare) {
            const size_t size = this->GetSize();
            for (size_t i = 1; i < size; i++) {
                const T tmp = this->AtUnsafe(i);
                size_t j = i;
                while (j && compare(this->AtUnsafe(j - 1), tmp)) {
                    if (!SwapUnsafe(j - 1, j)) return false;
                    j--;
                }
                if (!SetUnsafe(j, tmp)) return false;
            }
            return true;
        }
        WritableSequence<T>& operator+=(const T& val) {
            if (Add(val)) return *this;
            Panic("Failed to append value to writable sequence");
        }
        WritableSequence<T>& operator+=(const Sequence<T>& other) {
            if (AddSequence(other)) return *this;
            Panic("Failed to append sequence to writable sequence");
        }

        private:
        [[nodiscard]] bool StoogeSort(const Function<bool, T, T>& compare, size_t start, size_t end) {
            if (compare(this->AtUnsafe(start), this->AtUnsafe(end)) && !SwapUnsafe(start, end)) return false;
            if (end - start <= 1) return true;
            const size_t t = (end - start + 1) / 3;
            return StoogeSort(compare, start, end - t) && StoogeSort(compare, start + t, end) && StoogeSort(compare, start, end - t);
        }
    };
}

#endif