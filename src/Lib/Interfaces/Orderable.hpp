#ifndef MathLib_Interfaces_Orderable_H
#define MathLib_Interfaces_Orderable_H
#include "Comparable.hpp"

namespace MathLib {
    struct Orderable;
    struct Orderable : Comparable<Orderable> {
        [[nodiscard]] bool operator<=(const Orderable& other) const {
            return LessThanEqual(other);
        }
        [[nodiscard]] bool operator>=(const Orderable& other) const {
            return other.LessThanEqual(*this);
        }
        [[nodiscard]] bool operator<(const Orderable& other) const {
            return LessThanEqual(other) && !other.LessThanEqual(*this);
        }
        [[nodiscard]] bool operator>(const Orderable& other) const {
            return other.LessThanEqual(*this) && !LessThanEqual(other);
        }
        [[nodiscard]] bool operator!=(const Orderable& other) const {
            return !LessThanEqual(other) || !other.LessThanEqual(*this);
        }
        [[nodiscard]] bool Uncomparable(const Orderable& other) const {
            return !LessThanEqual(other) && !other.LessThanEqual(*this);
        }

        protected:
        [[nodiscard]] virtual bool Equals(const Orderable& other) const override {
            return LessThanEqual(other) && other.LessThanEqual(*this);
        }
        [[nodiscard]] virtual bool LessThanEqual(const Orderable& other) const = 0;
    };
}

#endif