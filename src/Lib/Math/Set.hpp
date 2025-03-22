#ifndef MathLib_Math_Set_H
#define MathLib_Math_Set_H
#include "../Expected.hpp"
#include "Orderable.hpp"

namespace MathLib {
    template <typename T>
    struct Set : Collection<T>, Orderable {
        template <typename F>
        friend struct Set;

        Set(void) {}
        Set(size_t size) : data(size) {}
        Set(const T& data) : data(MakeArray<T>(data)) {}
        Set(const Sequence<T>& data) : data(CollectionToArray<T>(data)) {}
        [[nodiscard]] virtual T At(size_t index) const override {
            return data.At(index);
        }
        [[nodiscard]] virtual T& At(size_t index) override {
            return data.At(index);
        }
        [[nodiscard]] virtual bool Add(const T& val) override {
            return data.Contains(val) ? true : data.Add(val);
        }
        [[nodiscard]] virtual const T* GetValue(void) const override {
            return data.GetValue();
        }
        [[nodiscard]] virtual size_t GetSize(void) const override {
            return data.GetSize();
        }
        [[nodiscard]] bool Contains(const T& x) const {
            return data.Contains(x);
        }
        [[nodiscard]] Array<T> GetData(void) const {
            return data;
        }
        [[nodiscard]] Expected<Set<T>> Negate(const Set<T>& universalSet) const {
            return universalSet - *this;
        }
        [[nodiscard]] Expected<Set<Set<T>>> PowerSet(void) const {
            if (!data.GetSize()) return Expected<Set<Set<T>>>(Set<Set<T>>(MakeArray<Set<T>>(Set<T>())));
            else {
                Array<T> tmp = Array<T>(data.GetSize() - 1);
                for (size_t i = 0; i < tmp.GetSize(); i++) tmp.At(i) = data.At(i);
                const Expected<Set<Set<T>>> pow = Set<T>(tmp).PowerSet();
                if (!pow.HasValue()) return Expected<Set<Set<T>>>();
                const Set<T> rest = Set<T>(MakeArray<T>(data.At(data.GetSize() - 1)));
                Array<Set<T>> ret = pow.Get().GetData();
                const Array<Set<T>> powArr = pow.Get().GetData();
                for (size_t i = 0; i < powArr.GetSize(); i++) {
                    const Expected<Set<T>> tmp = powArr.At(i) + rest;
                    if (!tmp.HasValue() || !ret.Add(tmp.Get())) return Expected<Set<Set<T>>>();
                }
                return Set<Set<T>>(ret);
            }
        }
        [[nodiscard]] Expected<Set<T>> SymetricDifference(const Set<T>& other) const {
            const Expected<Set<T>> tmp1 = *this + other;
            if (!tmp1.HasValue()) return Expected<Set<T>>();
            const Expected<Set<T>> tmp2 = *this * other;
            return tmp2.HasValue() ? tmp1.Get() - tmp2.Get() : Expected<Set<T>>();
        }
        [[nodiscard]] Expected<Set<Array<T>>> CartesianProduct(const Set<T>& other) const {
            Set<Array<T>> ret;
            for (const T& x : data) {
                Array<T> tmp = Array<T>(2);
                tmp.At(0) = x;
                for (const T& y : other.data) {
                    tmp.At(1) = y;
                    if (!ret.data.Add(tmp)) return Expected<Set<Array<T>>>();
                }
            }
            return ret;
        }
        [[nodiscard]] Expected<Set<Array<T>>> CartesianProduct(const Sequence<Set<T>>& others) const {
            if (others.IsEmpty()) return Expected<Set<Array<T>>>(Set<Array<T>>());
            Set<Array<T>> ret;
            const Expected<Set<Array<T>>> tmp = CartesianProduct(others.At(0));
            if (!tmp.HasValue()) return Expected<Set<Array<T>>>();
            ret = tmp.Get();
            for (size_t i = 1; i < others.GetSize(); i++) {
                Array<Array<T>> data = ret.GetData();
                for (Array<T>& x : data)
                    if (!x.Add(others.At(i))) return Expected<Set<Array<T>>>();
                ret = Set<Array<T>>(data);        
            }
            return ret;
        }
        [[nodiscard]] Expected<Set<T>> operator+(const Set<T>& other) const {
            Set<T> ret = *this;
            for (const T& x : other.data)
                if (!ret.data.Contains(x) && !ret.data.Add(x)) return Expected<Set<T>>();
            return Expected<Set<T>>(ret);
        }
        [[nodiscard]] Expected<Set<T>> operator-(const Set<T>& other) const {
            Set<T> ret;
            for (const T& x : data)
                if (!other.data.Contains(x) && !ret.data.Add(x)) return Expected<Set<T>>();
            return Expected<Set<T>>(ret);
        }
        [[nodiscard]] Expected<Set<T>> operator*(const Set<T>& other) const {
            Set<T> ret;
            for (const T& x : data)
                if (other.data.Contains(x) && !ret.data.Add(x)) return Expected<Set<T>>();
            return Expected<Set<T>>(ret);
        }

        protected:
        [[nodiscard]] virtual bool LessThanEqual(const Orderable& other_) const override {
            const Set<T>& other = (const Set<T>&)other_;
            for (const T& x : data)
                if (!other.Contains(x)) return false;
            return true;
        }

        private:
        Array<T> data;
    };
    template <typename T>
    [[nodiscard]] String ToString(const Set<T>& set) {
        return MathLib::ToString<T>(set.GetData());
    }
}

#endif