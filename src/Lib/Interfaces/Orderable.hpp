#ifndef MathLib_Interfaces_Orderable_H
#define MathLib_Interfaces_Orderable_H
#include "Comparable.hpp"

namespace MathLib {
    template <typename Self>
    struct Orderable : Comparable<Self> {
        [[nodiscard]] bool operator<=(const Self& other) const {
            return LessThanEqual(other);
        }
        [[nodiscard]] bool operator>=(const Self& other) const {
            return other.LessThanEqual((const Self&)*this);
        }
        [[nodiscard]] bool operator<(const Self& other) const {
            return LessThanEqual(other) && !other.LessThanEqual((const Self&)*this);
        }
        [[nodiscard]] bool operator>(const Self& other) const {
            return other.LessThanEqual((const Self&)*this) && !LessThanEqual(other);
        }
        [[nodiscard]] bool operator!=(const Self& other) const {
            return !LessThanEqual(other) || !other.LessThanEqual((const Self&)*this);
        }
        [[nodiscard]] bool Uncomparable(const Self& other) const {
            return !LessThanEqual(other) && !other.LessThanEqual((const Self&)*this);
        }
        [[nodiscard]] virtual bool Equals(const Self& other) const override {
            return LessThanEqual(other) && other.LessThanEqual((const Self&)*this);
        }
        [[nodiscard]] virtual bool LessThanEqual(const Self& other) const = 0;
    };
}

#endif