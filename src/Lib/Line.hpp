#ifndef Line_H
#define Line_H
#include "Vector.hpp"

/// @brief Line
/// @tparam T Type of number
template <typename T>
struct Line {
    /// @brief Starting point of line
    Vector3<T> start;
    /// @brief Ending point of line
    Vector3<T> end;

    /// @brief Creates a new line
    /// @param s Starting point of line
    /// @param e Ending point of line
    Line(Vector3<T> s, Vector3<T> e) {
        start = s;
        end = e;
    }
};
/// @brief Converts a line from one type to another
/// @tparam T Old type
/// @tparam F New type
/// @param line line to convert
/// @return Converted line
template <typename T, typename F>
Line<F> ConvertLine(Line<T> line) {
    return Line<F>(ConvertMatrix<T, F>(line.start), ConvertMatrix<T, F>(line.end));
}

#endif