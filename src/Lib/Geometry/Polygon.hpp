#ifndef MathLib_Geometry_Polygon_H
#define MathLib_Geometry_Polygon_H
#include "../Math/SystemOfLinearEquations.hpp"
#include "../Math/Vector3.hpp"
#include "../FunctionT.hpp"

namespace MathLib {
    template <typename T, typename Vertex>
    bool LineIntersectsPolygon(const Vector3<T>& start, const Vector3<T>& end, const Sequence<Vertex>& polygon, const Function<Vector3<T>, Vertex>& func) {
        const size_t size = polygon.GetSize();
        SystemOfLinearEquations<T> equations = 2;
        equations.EquationAt(0, 0) = end.x - start.x;
        equations.EquationAt(0, 1) = end.y - start.y;
        for (size_t i = 0; i < size; i++) {
            const Vector3<T> polygonStart = func(polygon.At(i));
            const Vector3<T> polygonEnd = func(polygon.At((i + 1) % size));
            equations.EquationAt(1, 0) = polygonEnd.x - polygonStart.x;
            equations.EquationAt(1, 1) = polygonEnd.y - polygonStart.y;
            equations.ValueAt(0) = polygonStart.x - start.x;
            equations.ValueAt(1) = polygonStart.y - start.y;
            const Expected<Matrix<T>> solution = equations.GetMatrixSolution();
            if (!solution.HasValue()) continue;
            const T u = solution.Get().At(0, 0);
            const T v = solution.Get().At(0, 1);
            if (u > 0 && u < 1 && v > 0 && v < 1 && FloatsEqual<T>((end.z - start.z) * u - (polygonEnd.z - polygonStart.z) * v, polygonStart.z - start.z)) return true;
        }
        return false;
    }
    template <typename T>
    bool LineIntersectsPolygon(const Vector3<T>& start, const Vector3<T>& end, const Sequence<Vector3<T>>& polygon) {
        const auto f = MakeFunctionT<Vector3<T>, Vector3<T>>([](Vector3<T> ret) -> Vector3<T> {
            return ret;
        });
        return LineIntersectsPolygon<T, Vector3<T>>(start, end, polygon, f);
    }
    template <typename T, typename Vertex>
    Array<Vertex> PolygonToTriangles(const Sequence<Vertex>& polygon, const Function<Vector3<T>, Vertex>& func) {
        const size_t size = polygon.GetSize();
        if (!size) return Array<Vertex>();
        if (size == 1) {
            const Vertex vector = polygon.At(0);
            return MakeArray<Vertex>(vector, vector, vector);
        }
        if (size == 2) {
            const Vertex vector = polygon.At(0);
            return MakeArray<Vertex>(vector, vector, polygon.At(1));
        }
        if (size == 3) return MakeArray<Vertex>(polygon.At(0), polygon.At(1), polygon.At(2));
        Array<Vertex> newPolygon;
        for (size_t i = 0; i < size; i++) {
            const Vertex start = polygon.At(i);
            const Vertex end = polygon.At((i + 2) % size);
            if (!newPolygon.Add(start)) return Array<Vertex>();
            if (LineIntersectsPolygon<T, Vertex>(func(start), func(end), polygon, func)) continue;
            if (!newPolygon.Add(end)) return Array<Vertex>();
            for (size_t j = (i + 2) % size + 1; j < size; j++)
                if (!newPolygon.Add(polygon.At(j))) return Array<Vertex>();
            Array<Vertex> ret = PolygonToTriangles<T, Vertex>(newPolygon, func);
            if (!ret.Add(start)) return Array<Vertex>();
            if (!ret.Add(polygon.At((i + 1) % size))) return Array<Vertex>();
            if (!ret.Add(end)) return Array<Vertex>();
            return ret;
        }
        return Array<Vertex>();
    }
    template <typename T>
    Array<Vector3<T>> PolygonToTriangles(const Sequence<Vector3<T>>& polygon) {
        const auto f = MakeFunctionT<Vector3<T>, Vector3<T>>([](Vector3<T> ret) -> Vector3<T> {
            return ret;
        });
        return PolygonToTriangles<T, Vector3<T>>(polygon, f);
    }
}

#endif