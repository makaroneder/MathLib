#ifndef Interval_H
#define Interval_H
#include "Typedefs.hpp"

template <typename T>
struct Interval : Allocatable {
    Interval(const T& min_ = 0, const T& max_ = 0) : min(min_), max(max_) {}
    T GetMin(void) const {
        return min;
    }
    T GetMax(void) const {
        return max;
    }
    T GetSize(void) const {
        return max - min;
    }
    bool Contains(const T& x) const {
        return IsBetween(x, min, max);
    }
    T Clamp(const T& x) const {
        if (x < min) return min;
        else if (x > max) return max;
        else return x;
    }

    private:
    T min;
    T max;
};

#endif