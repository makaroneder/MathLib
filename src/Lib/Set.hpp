#ifndef Set_H
#define Set_H
#include "Host.hpp"

/// @brief Creates a new set
/// @tparam T Type of number
/// @param start Start of the set
/// @param end End of the set
/// @param separator Separator between elements
/// @return New set
template <typename T>
Array<T> CreateSet(T start, T end, T separator) {
    Array<T> ret;
    for (T i = start; i <= end; i += separator) ret.Add(i);
    return ret;
}
/// @brief Checks whether element is inside the set
/// @tparam T Type of number
/// @param set Set in which we are looking for the element
/// @param element Element we are looking for in the set
/// @param eps_ Maximum error tolerance
/// @return Result
template <typename T>
bool IsInsideSet(Array<T> set, T element, T eps_ = eps) {
    if (IsNaN(element) || IsInf(element)) return false;
    if (set.GetSize() == 0) return true;
    for (size_t i = 0; i < set.GetSize(); i++)
        if (FloatsEqual<T>(set.At(i), element, eps_)) return true;
    return false;
}

#endif