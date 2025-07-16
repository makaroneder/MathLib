#ifndef MathLib_Interfaces_WritableSequence_H
#define MathLib_Interfaces_WritableSequence_H
#include "Sequence.hpp"

namespace MathLib {
    template <typename T>
    struct WritableSequence : Sequence<T> {
        [[nodiscard]] virtual bool Set(size_t index, const T& value) = 0;
        [[nodiscard]] virtual bool Add(const T& value) = 0;
        [[nodiscard]] virtual bool Reset(void) = 0;
        [[nodiscard]] bool AddSequence(const Sequence<T>& other) {
            StartBenchmark
            const size_t size = other.GetSize();
            for (size_t i = 0; i < size; i++)
                if (!Add(other.At(i))) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool Swap(size_t x, size_t y) {
            StartBenchmark
            if (x >= this->GetSize() || y >= this->GetSize()) ReturnFromBenchmark(false);
            const T tmp = this->At(x);
            ReturnFromBenchmark(Set(x, this->At(y)) && Set(y, tmp));
        }
        [[nodiscard]] bool Copy(WritableSequence<T>& sequence, size_t start, size_t n) const {
            StartBenchmark
            if (start + n >= this->GetSize()) ReturnFromBenchmark(false);
            const size_t size = sequence.GetSize();
            if (n >= size) ReturnFromBenchmark(false);
            for (size_t i = 0; i < size; i++)
                if (!sequence.Set(i, this->At(start + i))) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool Foreach(const Function<T, T>& function) {
            StartBenchmark
            const size_t size = this->GetSize();
            for (size_t i = 0; i < size; i++)
                if (!Set(i, function(this->At(i)))) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool Reverse(void) {
            StartAndReturnFromBenchmark(Reverse(0, this->GetSize()));
        }
        [[nodiscard]] bool Reverse(size_t start, size_t end) {
            StartBenchmark
            if (end > this->GetSize() || !end) ReturnFromBenchmark(false);
            while (start < end)
                if (!Swap(start++, --end)) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool Permutate(const Function<size_t, size_t>& permutation) {
            StartBenchmark
            const size_t size = this->GetSize();
            T buff[size];
            for (size_t i = 0; i < size; i++) {
                const size_t tmp = permutation(i);
                if (tmp >= size) ReturnFromBenchmark(false);
                buff[tmp] = this->At(i);
            }
            for (size_t i = 0; i < size; i++)
                if (!Set(i, buff[i])) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool BubbleSort(const Function<bool, T, T>& compare) {
            StartBenchmark
            size_t size = this->GetSize();
            while (size > 1) {
                size_t last = 0;
                for (size_t i = 1; i < size; i++) {
                    if (compare(this->At(i - 1), this->At(i))) {
                        if (!Swap(i - 1, i)) ReturnFromBenchmark(false);
                        last = i;
                    }
                }
                size = last;
            }
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool CocktailShakerSort(const Function<bool, T, T>& compare) {
            StartBenchmark
            size_t size = this->GetSize();
            size_t start = 1;
            while (size > start) {
                size_t last = 0;
                for (size_t i = start; i < size; i++) {
                    if (compare(this->At(i - 1), this->At(i))) {
                        if (!Swap(i - 1, i)) ReturnFromBenchmark(false);
                        last = i;
                    }
                }
                size = last;
                last = 0;
                for (size_t i = size; i >= start + 1; i--) {
                    if (compare(this->At(i - 2), this->At(i - 1))) {
                        if (!Swap(i - 2, i - 1)) ReturnFromBenchmark(false);
                        last = i;
                    }
                }
                start = last - 1;
            }
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool StoogeSort(const Function<bool, T, T>& compare) {
            StartBenchmark
            const size_t size = this->GetSize();
            ReturnFromBenchmark(size <= 1 || StoogeSort(compare, 0, size - 1));
        }
        [[nodiscard]] bool OddEvenSort(const Function<bool, T, T>& compare) {
            StartBenchmark
            const size_t size = this->GetSize();
            if (size <= 1) ReturnFromBenchmark(true);
            while (true) {
                bool sorted = true;
                for (size_t i = 1; i < size - 1; i += 2) {
                    if (compare(this->At(i), this->At(i + 1))) {
                        if (!Swap(i, i + 1)) ReturnFromBenchmark(false);
                        sorted = false;
                    }
                }
                for (size_t i = 0; i < size - 1; i += 2) {
                    if (compare(this->At(i), this->At(i + 1))) {
                        if (!Swap(i, i + 1)) ReturnFromBenchmark(false);
                        sorted = false;
                    }
                }
                if (sorted) break;
            }
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] bool InsertationSort(const Function<bool, T, T>& compare) {
            StartBenchmark
            const size_t size = this->GetSize();
            for (size_t i = 1; i < size; i++) {
                const T tmp = this->At(i);
                size_t j = i;
                while (j > 0 && compare(this->At(j - 1), tmp)) {
                    if (!Swap(j - 1, j)) ReturnFromBenchmark(false);
                    j--;
                }
                if (!Set(j, tmp)) ReturnFromBenchmark(false);
            }
            return true;
        }
        WritableSequence<T>& operator+=(const T& val) {
            StartBenchmark
            if (!Add(val)) Panic("Failed to append value to writable sequence");
            ReturnFromBenchmark(*this);
        }
        WritableSequence<T>& operator+=(const Sequence<T>& other) {
            StartBenchmark
            if (!AddSequence(other)) Panic("Failed to append sequence to writable sequence");
            ReturnFromBenchmark(*this);
        }

        private:
        [[nodiscard]] bool StoogeSort(const Function<bool, T, T>& compare, size_t start, size_t end) {
            StartBenchmark
            if (compare(this->At(start), this->At(end)) && !Swap(start, end)) ReturnFromBenchmark(false);
            if (end - start > 1) {
                const size_t t = (end - start + 1) / 3;
                if (!StoogeSort(compare, start, end - t) || !StoogeSort(compare, start + t, end) || !StoogeSort(compare, start, end - t)) ReturnFromBenchmark(false);
            }
            ReturnFromBenchmark(true);
        }
    };
}

#endif