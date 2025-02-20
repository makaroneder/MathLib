#ifndef MathLib_Math_Orderable_H
#define MathLib_Math_Orderable_H
#include "../Typedefs.hpp"

namespace MathLib {
    struct Orderable : Allocatable {
        [[nodiscard]] bool operator<=(const Orderable& other) const {
            return LessThanEqual(other);
        }
        [[nodiscard]] bool operator>=(const Orderable& other) const {
            return other.LessThanEqual(*this);
        }
        [[nodiscard]] bool operator==(const Orderable& other) const {
            return LessThanEqual(other) && other.LessThanEqual(*this);
        }
        [[nodiscard]] bool operator!=(const Orderable& other) const {
            return !LessThanEqual(other) || !other.LessThanEqual(*this);
        }
        [[nodiscard]] bool operator<(const Orderable& other) const {
            return LessThanEqual(other) && *this != other;
        }
        [[nodiscard]] bool operator>(const Orderable& other) const {
            return other.LessThanEqual(*this) && *this != other;
        }
        [[nodiscard]] bool Uncomparable(const Orderable& other) const {
            return !LessThanEqual(other) && !other.LessThanEqual(*this);
        }

        protected:
        [[nodiscard]] virtual bool LessThanEqual(const Orderable& other) const = 0;
    };
}

#endif