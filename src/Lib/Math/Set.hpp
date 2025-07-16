#ifndef MathLib_Math_Set_H
#define MathLib_Math_Set_H
#include "../Expected.hpp"
#include "Orderable.hpp"

namespace MathLib {
    template <typename T>
    struct Set : Collection<T>, Orderable {
        template <typename F>
        friend struct Set;

        Set(void) {
            EmptyBenchmark
        }
        Set(size_t size) : data(size) {
            EmptyBenchmark
        }
        Set(const T& data) : data(MakeArray<T>(data)) {
            EmptyBenchmark
        }
        Set(const Sequence<T>& data) : data(CollectionToArray<T>(data)) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual bool Add(const T& val) override {
            StartAndReturnFromBenchmark(data.Contains(val) ? true : data.Add(val));
        }
        [[nodiscard]] virtual bool Reset(void) override {
            StartAndReturnFromBenchmark(data.Reset());
        }
        [[nodiscard]] virtual T* GetValue(void) override {
            StartAndReturnFromBenchmark(data.GetValue());
        }
        [[nodiscard]] virtual const T* GetValue(void) const override {
            StartAndReturnFromBenchmark(data.GetValue());
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            StartAndReturnFromBenchmark(data.GetSize());
        }
        [[nodiscard]] bool Contains(const T& x) const {
            StartAndReturnFromBenchmark(data.Contains(x));
        }
        [[nodiscard]] Array<T> GetData(void) const {
            StartAndReturnFromBenchmark(data);
        }
        [[nodiscard]] Expected<Set<T>> Negate(const Set<T>& universalSet) const {
            StartAndReturnFromBenchmark(universalSet - *this);
        }
        [[nodiscard]] Expected<Set<Set<T>>> PowerSet(void) const {
            StartBenchmark
            if (data.IsEmpty()) ReturnFromBenchmark(Expected<Set<Set<T>>>(Set<Set<T>>(MakeArray<Set<T>>(Set<T>()))));
            Array<T> tmp = Array<T>(data.GetSize() - 1);
            for (size_t i = 0; i < tmp.GetSize(); i++) tmp.At(i) = data.At(i);
            const Expected<Set<Set<T>>> pow = Set<T>(tmp).PowerSet();
            if (!pow.HasValue()) ReturnFromBenchmark(Expected<Set<Set<T>>>());
            const Set<T> rest = Set<T>(MakeArray<T>(data.At(data.GetSize() - 1)));
            Array<Set<T>> ret = pow.Get().GetData();
            const Array<Set<T>> powArr = pow.Get().GetData();
            for (size_t i = 0; i < powArr.GetSize(); i++) {
                const Expected<Set<T>> tmp = powArr.At(i) + rest;
                if (!tmp.HasValue() || !ret.Add(tmp.Get())) ReturnFromBenchmark(Expected<Set<Set<T>>>());
            }
            ReturnFromBenchmark(Set<Set<T>>(ret));
        }
        [[nodiscard]] Expected<Set<T>> SymetricDifference(const Set<T>& other) const {
            StartBenchmark
            const Expected<Set<T>> tmp1 = *this + other;
            if (!tmp1.HasValue()) ReturnFromBenchmark(Expected<Set<T>>());
            const Expected<Set<T>> tmp2 = *this * other;
            ReturnFromBenchmark(tmp2.HasValue() ? tmp1.Get() - tmp2.Get() : Expected<Set<T>>());
        }
        [[nodiscard]] Expected<Set<Array<T>>> CartesianProduct(const Set<T>& other) const {
            StartBenchmark
            Set<Array<T>> ret;
            for (const T& x : data) {
                Array<T> tmp = Array<T>(2);
                tmp.At(0) = x;
                for (const T& y : other.data) {
                    tmp.At(1) = y;
                    if (!ret.data.Add(tmp)) ReturnFromBenchmark(Expected<Set<Array<T>>>());
                }
            }
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] Expected<Set<Array<T>>> CartesianProduct(const Sequence<Set<T>>& others) const {
            StartBenchmark
            if (others.IsEmpty()) ReturnFromBenchmark(Expected<Set<Array<T>>>(Set<Array<T>>()));
            Set<Array<T>> ret;
            const Expected<Set<Array<T>>> tmp = CartesianProduct(others.At(0));
            if (!tmp.HasValue()) ReturnFromBenchmark(Expected<Set<Array<T>>>());
            ret = tmp.Get();
            for (size_t i = 1; i < others.GetSize(); i++) {
                Array<Array<T>> data = ret.GetData();
                for (WritableSequence<T>& x : data)
                    if (!x.Add(others.At(i))) ReturnFromBenchmark(Expected<Set<Array<T>>>());
                ret = Set<Array<T>>(data);
            }
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] Expected<Set<T>> operator+(const Set<T>& other) const {
            StartBenchmark
            Set<T> ret = *this;
            for (const T& x : other.data)
                if (!ret.data.Contains(x) && !ret.data.Add(x)) ReturnFromBenchmark(Expected<Set<T>>());
            ReturnFromBenchmark(Expected<Set<T>>(ret));
        }
        [[nodiscard]] Expected<Set<T>> operator-(const Set<T>& other) const {
            StartBenchmark
            Set<T> ret;
            for (const T& x : data)
                if (!other.data.Contains(x) && !ret.data.Add(x)) ReturnFromBenchmark(Expected<Set<T>>());
            ReturnFromBenchmark(Expected<Set<T>>(ret));
        }
        [[nodiscard]] Expected<Set<T>> operator*(const Set<T>& other) const {
            StartBenchmark
            Set<T> ret;
            for (const T& x : data)
                if (other.data.Contains(x) && !ret.data.Add(x)) ReturnFromBenchmark(Expected<Set<T>>());
            ReturnFromBenchmark(Expected<Set<T>>(ret));
        }

        protected:
        [[nodiscard]] virtual bool LessThanEqual(const Orderable& other_) const override {
            StartBenchmark
            const Set<T>& other = (const Set<T>&)other_;
            for (const T& x : data)
                if (!other.Contains(x)) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }

        private:
        Array<T> data;
    };
    template <typename T>
    [[nodiscard]] String ToString(const Set<T>& set) {
        StartAndReturnFromBenchmark(MathLib::ToString<T>(set.GetData()));
    }
}

#endif