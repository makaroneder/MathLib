#ifndef MathLib_Geometry_Hyperrectangle_H
#define MathLib_Geometry_Hyperrectangle_H
#include "Cuboid.hpp"

namespace MathLib {
    template <typename T>
    struct Hyperrectangle : LineShape<T> {
        Matrix<T> sizes;

        Hyperrectangle(const Matrix<T>& pos = CreateVector<T>(0, 0, 0), const Matrix<T>& s = CreateVector<T>(0, 0, 0)) : LineShape<T>(pos), sizes(s) {}
        [[nodiscard]] virtual bool CollidesWith(const Shape<T>& other_) const override {
            // TODO:
            (void)other_;
            return false;
        }
        [[nodiscard]] virtual Array<Line<T>> ToLines(const Matrix<T>& rotation) const override {
            const Array<Matrix<T>> points = Cuboid<T>::GenerateVertices(sizes / 2);
            Array<Line<T>> ret;
            for (size_t i = 0; i < points.GetSize() - 1; i++) {
                for (size_t j = i + 1; j < points.GetSize(); j++) {
                    size_t diff = 0;
                    for (size_t k = 0; k < 4; k++)
                        if (!FloatsEqual<T>(points.At(i).At(k, 0), points.At(j).At(k, 0))) diff++;
                    if (diff == 1 && !ret.Add(Line<T>(RotateVector<T>(this->position + ConvertVectorDimension<T>(points.At(i), 3), this->position, rotation), RotateVector<T>(this->position + ConvertVectorDimension<T>(points.At(j), 3), this->position, rotation)))) return Array<Line<T>>();
                }
            }
            return ret;
        }
    };
}

#endif