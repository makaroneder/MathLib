#ifndef MathLib_Geometry_Line_H
#define MathLib_Geometry_Line_H
#include "../Math/Vector.hpp"

namespace MathLib {
    /// @brief Line
    /// @tparam T Type of number
    template <typename T>
    struct Line : Allocatable {
        /// @brief Starting point of line
        Matrix<T> start;
        /// @brief Ending point of line
        Matrix<T> end;

        /// @brief Creates a new line
        /// @param s Starting point of line
        /// @param e Ending point of line
        Line(const Matrix<T>& s = CreateVector<T>(0, 0, 0), const Matrix<T>& e = CreateVector<T>(0, 0, 0)) : start(s), end(e) {}
        [[nodiscard]] bool operator==(const Line<T>& other) const {
            return start == other.start && end == other.end;
        }
        [[nodiscard]] bool operator!=(const Line<T>& other) const {
            return !(*this == other);
        }
    };
    /// @brief Converts a line from one type to another
    /// @tparam T Old type
    /// @tparam F New type
    /// @param line line to convert
    /// @return Converted line
    template <typename T, typename F>
    [[nodiscard]] Line<F> ConvertLine(const Line<T>& line) {
        return Line<F>(ConvertMatrix<T, F>(line.start), ConvertMatrix<T, F>(line.end));
    }
}

#endif