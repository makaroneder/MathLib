#ifndef Set_H
#define Set_H
#include "Typedefs.hpp"
#include <vector>

/// @brief Creates a new set
/// @tparam T Type of number
/// @param start Start of the set
/// @param end End of the set
/// @param separator Separator between elements
/// @return New set
template <typename T>
std::vector<T> CreateSet(T start, T end, T separator) {
    std::vector<T> ret;
    for (T i = start; i <= end; i += separator) ret.push_back(i);
    return ret;
}
/// @brief Checks whether element is inside the set
/// @tparam T Type of number
/// @param set Set in which we are looking for the element
/// @param element Element we are looking for in the set
/// @param eps_ Maximum error tolerance
/// @return Result
template <typename T>
bool IsInsideSet(std::vector<T> set, T element, T eps_ = eps) {
    if (std::isnan(element) || std::isinf(element)) return false;
    if (set.size() == 0) return true;
    for (T& i : set)
        if (FloatsEqual<T>(i, element, eps_)) return true;
    return false;
}

#endif