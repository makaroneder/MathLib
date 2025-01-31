#ifndef MathLib_Math_Set_H
#define MathLib_Math_Set_H
#include "../Expected.hpp"

namespace MathLib {
    template <typename T>
    struct Set : Printable {
        Set(void) {}
        Set(const Array<T>& data) : data(data) {}
        size_t GetSize(void) const {
            return data.GetSize();
        }
        bool Contains(const T& x) const {
            return data.Contains(x);
        }
        Array<T> GetData(void) const {
            return data;
        }
        Expected<Set<T>> Negate(const Set<T>& universalSet) const {
            return universalSet - *this;
        }
        Expected<Set<Set<T>>> PowerSet(void) const {
            if (!data.GetSize()) return Expected<Set<Set<T>>>(Set<Set<T>>(MakeArrayFromSingle<Set<T>>(Set<T>())));
            else {
                Array<T> tmp = Array<T>(data.GetSize() - 1);
                for (size_t i = 0; i < tmp.GetSize(); i++) tmp.At(i) = data.At(i);
                const Expected<Set<Set<T>>> pow = Set<T>(tmp).PowerSet();
                if (!pow.HasValue()) return Expected<Set<Set<T>>>();
                const Set<T> rest = Set<T>(MakeArrayFromSingle<T>(data.At(data.GetSize() - 1)));
                Array<Set<T>> ret = pow.Get().GetData();
                const Array<Set<T>> powArr = pow.Get().GetData();
                for (size_t i = 0; i < powArr.GetSize(); i++) {
                    const Expected<Set<T>> tmp = powArr.At(i) + rest;
                    if (!tmp.HasValue() || !ret.Add(tmp.Get())) return Expected<Set<Set<T>>>();
                }
                return Set<Set<T>>(ret);
            }
        }
        Expected<Set<T>> SymetricDifference(const Set<T>& other) const {
            const Expected<Set<T>> tmp1 = *this + other;
            if (!tmp1.HasValue()) return Expected<Set<T>>();
            const Expected<Set<T>> tmp2 = *this * other;
            return tmp2.HasValue() ? tmp1.Get() - tmp2.Get() : Expected<Set<T>>();
        }
        Expected<Set<Array<T>>> CartesianProduct(const Set<T>& other) const {
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
        bool operator==(const Set<T>& other) const {
            if (data.GetSize() != other.GetSize()) return false;
            for (size_t i = 0; i < data.GetSize(); i++)
                if (!other.Contains(data.At(i))) return false;
            return true;
        }
        Expected<Set<T>> operator+(const Set<T>& other) const {
            Set<T> ret = *this;
            for (const T& x : other.data)
                if (!ret.data.Contains(x) && !ret.data.Add(x)) return Expected<Set<T>>();
            return Expected<Set<T>>(ret);
        }
        Expected<Set<T>> operator-(const Set<T>& other) const {
            Set<T> ret;
            for (const T& x : data)
                if (!other.data.Contains(x) && !ret.data.Add(x)) return Expected<Set<T>>();
            return Expected<Set<T>>(ret);
        }
        Expected<Set<T>> operator*(const Set<T>& other) const {
            Set<T> ret;
            for (const T& x : data)
                if (other.data.Contains(x) && !ret.data.Add(x)) return Expected<Set<T>>();
            return Expected<Set<T>>(ret);
        }
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const String& padding = "") const override {
            String ret = padding + '{';
            const size_t size = data.GetSize();
            for (size_t i = 0; i < size; i++) ret += ToString(data.At(i)) + (((i + 1) == size) ? "" : ", ");
            return ret + '}';
        }

        private:
        Array<T> data;
    };
}

#endif