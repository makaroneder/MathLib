#ifndef MathLib_Interfaces_LinkedList_H
#define MathLib_Interfaces_LinkedList_H
#include "NonLinearCollection.hpp"
#include "../Typedefs.hpp"

namespace MathLib {
    template <typename T>
    struct LinkedList : NonLinearCollection<T> {
        LinkedList(void) : data(), next(nullptr) {
            EmptyBenchmark
        }
        LinkedList(const T& data) : data(data), next(nullptr) {
            EmptyBenchmark
        }
        virtual ~LinkedList(void) override {
            StartBenchmark
            if (next) delete next;
            EndBenchmark
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(next ? next->GetSize() + 1 : 0);
        }
        [[nodiscard]] virtual T At(size_t index) const override {
            StartBenchmark
            if (!index) {
                if (!next) Panic("Index out of bounds");
                ReturnFromBenchmark(next->data);
            }
            ReturnFromBenchmark(next->At(index - 1));
        }
        [[nodiscard]] virtual T& At(size_t index) override {
            StartBenchmark
            if (!index) {
                if (!next) Panic("Index out of bounds");
                ReturnFromBenchmark(next->data);
            }
            ReturnFromBenchmark(next->At(index - 1));
        }
        [[nodiscard]] virtual bool Add(const T& value) override {
            StartBenchmark
            if (next) ReturnFromBenchmark(next->Add(value));
            next = new LinkedList<T>(value);
            ReturnFromBenchmark(next);
        }
        [[nodiscard]] virtual bool Reset(void) override {
            StartBenchmark
            if (next) delete next;
            next = nullptr;
            ReturnFromBenchmark(true);
        }

        private:
        T data;
        LinkedList<T>* next;
    };
}

#endif