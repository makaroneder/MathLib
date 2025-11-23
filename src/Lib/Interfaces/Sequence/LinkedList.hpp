#ifndef MathLib_Interfaces_Sequence_LinkedList_H
#define MathLib_Interfaces_Sequence_LinkedList_H
#include "NonLinearCollection.hpp"

namespace MathLib {
    template <typename T>
    struct LinkedList : NonLinearCollection<T> {
        LinkedList(void) : data(), next(nullptr) {}
        LinkedList(const T& data) : data(data), next(nullptr) {}
        virtual ~LinkedList(void) override {
            if (next) delete next;
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return next ? next->GetSize() + 1 : 0;
        }
        [[nodiscard]] virtual T AtUnsafe(size_t index) const override {
            return index ? next->At(index - 1) : next->data;
        }
        [[nodiscard]] virtual T& AtUnsafe(size_t index) override {
            return index ? next->At(index - 1) : next->data;
        }
        [[nodiscard]] virtual bool Add(const T& value) override {
            if (next) return next->Add(value);
            next = new LinkedList<T>(value);
            return next;
        }
        [[nodiscard]] virtual bool Reset(void) override {
            if (next) delete next;
            next = nullptr;
            return true;
        }

        private:
        T data;
        LinkedList<T>* next;
    };
}

#endif