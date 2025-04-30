#ifndef MathLib_Interfaces_WritableSequence_H
#define MathLib_Interfaces_WritableSequence_H
#include "Sequence.hpp"

namespace MathLib {
    [[noreturn]] void Panic(const char* str);
    template <typename T>
    struct WritableSequence : Sequence<T> {
        [[nodiscard]] virtual bool Set(size_t index, const T& value) = 0;
        [[nodiscard]] virtual bool Add(const T& value) = 0;
        [[nodiscard]] virtual bool Reset(void) = 0;
        [[nodiscard]] bool AddSequence(const Sequence<T>& other) {
            const size_t size = other.GetSize();
            for (size_t i = 0; i < size; i++)
                if (!Add(other.At(i))) return false;
            return true;
        }
        [[nodiscard]] bool Swap(size_t x, size_t y) {
            if (x >= this->GetSize() || y >= this->GetSize()) return false;
            const T tmp = this->At(x);
            return Set(x, this->At(y)) && Set(y, tmp);
        }
        [[nodiscard]] bool Foreach(const Function<T, T>& function) {
            const size_t size = this->GetSize();
            for (size_t i = 0; i < size; i++)
                if (!Set(i, function(this->At(i)))) return false;
            return true;
        }
        [[nodiscard]] bool Reverse(void) {
            return Reverse(0, this->GetSize());
        }
        [[nodiscard]] bool Reverse(size_t start, size_t end) {
            if (end > this->GetSize() || !end) return false;
            while (start < end)
                if (!Swap(start++, --end)) return false;
            return true;
        }
        [[nodiscard]] bool Permutate(const Function<size_t, size_t>& permutation) {
            const size_t size = this->GetSize();
            T buff[size];
            for (size_t i = 0; i < size; i++) {
                const size_t tmp = permutation(i);
                if (tmp >= size) return false;
                buff[tmp] = this->At(i);
            }
            for (size_t i = 0; i < size; i++)
                if (!Set(i, buff[i])) return false;
            return true;
        }
        [[nodiscard]] bool BubbleSort(const Function<bool, T, T>& compare) {
            size_t size = this->GetSize();
            while (size > 1) {
                size_t last = 0;
                for (size_t i = 1; i < size; i++) {
                    if (compare(this->At(i - 1), this->At(i))) {
                        if (!Swap(i - 1, i)) return false;
                        last = i;
                    }
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
                    if (compare(this->At(i - 1), this->At(i))) {
                        if (!Swap(i - 1, i)) return false;
                        last = i;
                    }
                }
                size = last;
                last = 0;
                for (size_t i = size; i >= start + 1; i--) {
                    if (compare(this->At(i - 2), this->At(i - 1))) {
                        if (!Swap(i - 2, i - 1)) return false;
                        last = i;
                    }
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
                    if (compare(this->At(i), this->At(i + 1))) {
                        if (!Swap(i, i + 1)) return false;
                        sorted = false;
                    }
                }
                for (size_t i = 0; i < size - 1; i += 2) {
                    if (compare(this->At(i), this->At(i + 1))) {
                        if (!Swap(i, i + 1)) return false;
                        sorted = false;
                    }
                }
                if (sorted) break;
            }
            return true;
        }
        [[nodiscard]] bool InsertationSort(const Function<bool, T, T>& compare) {
            const size_t size = this->GetSize();
            for (size_t i = 1; i < size; i++) {
                const T tmp = this->At(i);
                size_t j = i;
                while (j > 0 && compare(this->At(j - 1), tmp)) {
                    if (!Swap(j - 1, j)) return false;
                    j--;
                }
                if (!Set(j, tmp)) return false;
            }
            return true;
        }
        WritableSequence<T>& operator+=(const T& val) {
            if (!Add(val)) Panic("Failed to append value to writable sequence");
            return *this;
        }
        WritableSequence<T>& operator+=(const Sequence<T>& other) {
            if (!AddSequence(other)) Panic("Failed to append sequence to writable sequence");
            return *this;
        }

        private:
        [[nodiscard]] bool StoogeSort(const Function<bool, T, T>& compare, size_t start, size_t end) {
            if (compare(this->At(start), this->At(end)) && !Swap(start, end)) return false;
            if (end - start > 1) {
                const size_t t = (end - start + 1) / 3;
                if (!StoogeSort(compare, start, end - t) || !StoogeSort(compare, start + t, end) || !StoogeSort(compare, start, end - t)) return false;
            }
            return true;
        }
    };
}

#endif