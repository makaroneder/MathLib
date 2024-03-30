#ifndef Line_H
#define Line_H
#include "../Vector.hpp"

/// @brief Line
/// @tparam T Type of number
template <typename T>
struct Line {
    /// @brief Starting point of line
    Matrix<T> start;
    /// @brief Ending point of line
    Matrix<T> end;

    /// @brief Creates a new line
    /// @param s Starting point of line
    /// @param e Ending point of line
    Line(Matrix<T> s = CreateVector<T>(0, 0, 0), Matrix<T> e = CreateVector<T>(0, 0, 0)) {
        start = s;
        end = e;
    }
    /// @brief d = sqrt((b_0 - a_0)^2 + ... + (b_n - a_n)^2)
    /// @return Length of line
    T GetLength(void) const {
        return start.GetDistance(end);
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