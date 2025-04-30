#ifndef MathLib_Automaton_Language_H
#define MathLib_Automaton_Language_H
#include "../Math/Set.hpp"

namespace MathLib {
    template <typename T>
    struct Language : Set<Array<T>> {
        Language(void) {}
        Language(const Sequence<Array<T>>& data) : Set<Array<T>>(data) {}
        Language<T> Concatinate(const Language<T>& other) const {
            Language<T> ret;
            for (const Array<T>& a : *this) {
                for (const NonLinearCollection<T>& b : other) {
                    Array<T> tmp = a;
                    tmp += b;
                    if (!ret.Add(tmp)) return Language<T>();
                }
            }
            return ret;
        }
        Language<T> Concatinate(size_t n) const {
            Language<T> ret = Language(MakeArray<Array<T>>(Array<T>()));
            for (size_t i = 0; i < n; i++) ret = ret.Concatinate(*this);
            return ret;
        }
        Language<T> KleeneStar(size_t n) const {
            Language<T> ret;
            for (size_t i = 0; i < n; i++) ret += Concatinate(i);
            return ret;
        }
        Language<T> KleenePlus(size_t n) const {
            Language<T> ret;
            for (size_t i = 0; i < n; i++) ret += Concatinate(i + 1);
            return ret;
        }
        Language<T> Reverse(void) const {
            Language<T> ret = *this;
            for (WritableSequence<T>& arr : ret)
                if (!arr.Reverse()) return Language<T>();
            return ret;
        }
    };
}

#endif