#ifndef MathLib_Geometry_Cuboid_H
#define MathLib_Geometry_Cuboid_H
#include "LineShape.hpp"
#include "../Math/Quaternion.hpp"

namespace MathLib {
    template <typename T>
    struct Cuboid : LineShape<T> {
        Matrix<T> sizes;

        Cuboid(const Matrix<T>& pos = CreateVector<T>(0, 0, 0), const Matrix<T>& s = CreateVector<T>(0, 0, 0)) : LineShape<T>(pos), sizes(s) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual bool CollidesWith(const Shape<T>& other_) const override {
            StartBenchmark
            const Cuboid<T>& other = (const Cuboid<T>&)other_;
            for (size_t x = 0; x < this->position.GetWidth(); x++)
                if (Abs(this->position.At(x, 0) - other.position.At(x, 0)) > (sizes.At(x, 0) + other.sizes.At(x, 0)) / 2) ReturnFromBenchmark(false);
            ReturnFromBenchmark(true);
        }
        [[nodiscard]] virtual Array<Line<T>> ToLines(const Matrix<T>& rotation) const override {
            StartBenchmark
            const Array<Matrix<T>> points = GenerateVertices(sizes / 2);
            const Matrix<T> p1 = RotateVector<T>(this->position + points.At(0), this->position, rotation);
            const Matrix<T> p2 = RotateVector<T>(this->position + points.At(1), this->position, rotation);
            const Matrix<T> p3 = RotateVector<T>(this->position + points.At(2), this->position, rotation);
            const Matrix<T> p4 = RotateVector<T>(this->position + points.At(3), this->position, rotation);
            const Matrix<T> p5 = RotateVector<T>(this->position + points.At(4), this->position, rotation);
            const Matrix<T> p6 = RotateVector<T>(this->position + points.At(5), this->position, rotation);
            const Matrix<T> p7 = RotateVector<T>(this->position + points.At(6), this->position, rotation);
            const Matrix<T> p8 = RotateVector<T>(this->position + points.At(7), this->position, rotation);
            Array<Line<T>> ret = Array<Line<T>>(12);
            ret.At(0) = Line<T>(p1, p2);
            ret.At(1) = Line<T>(p1, p3);
            ret.At(2) = Line<T>(p2, p4);
            ret.At(3) = Line<T>(p3, p4);
            ret.At(4) = Line<T>(p1, p5);
            ret.At(5) = Line<T>(p2, p6);
            ret.At(6) = Line<T>(p3, p7);
            ret.At(7) = Line<T>(p4, p8);
            ret.At(8) = Line<T>(p5, p6);
            ret.At(9) = Line<T>(p5, p7);
            ret.At(10) = Line<T>(p6, p8);
            ret.At(11) = Line<T>(p7, p8);
            ReturnFromBenchmark(ret);
        }
        [[nodiscard]] static Array<Matrix<T>> GenerateVertices(const Matrix<T>& dimensions) {
            StartBenchmark
            Array<Matrix<T>> ret = Array<Matrix<T>>(2);
            ret.At(0) = Matrix<T>(1, 1, MakeArray<T>(GetX(dimensions)));
            ret.At(1) = Matrix<T>(1, 1, MakeArray<T>(-GetX(dimensions)));
            for (size_t i = 1; i < dimensions.GetWidth(); i++) ret = GenerateVertices(ret, dimensions.At(i, 0));
            ReturnFromBenchmark(ret);
        }

        private:
        [[nodiscard]] static Array<Matrix<T>> GenerateVertices(const Array<Matrix<T>>& prev, const T& dimension) {
            StartBenchmark
            Array<Matrix<T>> ret = Array<Matrix<T>>(prev.GetSize() * 2);
            for (size_t i = 0; i < prev.GetSize(); i++) {
                Array<T> tmp = Array<T>(prev.At(i).GetWidth() + 1);
                for (size_t j = 0; j < prev.At(i).GetWidth(); j++) tmp.At(j) = prev.At(i).At(j, 0);
                Matrix<T> point = Matrix<T>(tmp.GetSize(), 1, tmp);
                point.At(prev.At(i).GetWidth(), 0) = dimension;
                ret.At(2 * i) = point;
                point.At(prev.At(i).GetWidth(), 0) = -dimension;
                ret.At(2 * i + 1) = point;
            }
            ReturnFromBenchmark(ret);
        }
    };
}

#endif