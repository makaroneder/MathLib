#ifndef MathLib_Geometry_Line_H
#define MathLib_Geometry_Line_H
#include "../Math/Vector.hpp"

namespace MathLib {
    template <typename T>
    struct Line : Allocatable {
        Matrix<T> start;
        Matrix<T> end;

        Line(const Matrix<T>& s = CreateVector<T>(0, 0, 0), const Matrix<T>& e = CreateVector<T>(0, 0, 0)) : start(s), end(e) {}
        [[nodiscard]] bool operator==(const Line<T>& other) const {
            return start == other.start && end == other.end;
        }
        [[nodiscard]] bool operator!=(const Line<T>& other) const {
            return !(*this == other);
        }
    };
    template <typename T, typename F>
    [[nodiscard]] Line<F> ConvertLine(const Line<T>& line) {
        return Line<F>(ConvertMatrix<T, F>(line.start), ConvertMatrix<T, F>(line.end));
    }
}

#endif