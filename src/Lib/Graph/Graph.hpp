#ifndef MathLib_Graph_H
#define MathLib_Graph_H
#include "../Math/Matrix.hpp"
#include "../Permutation.hpp"
#include "Tree.hpp"

namespace MathLib {
    template <typename T>
    struct DijkstrasAlgorithmData {
        bool visited;
        T distance;
        size_t prev;

        DijkstrasAlgorithmData(bool visited = false, const T& distance = MakeInf(), size_t prev = SIZE_MAX) : visited(visited), distance(distance), prev(prev) {}
    };
    template <typename T>
    struct Graph : Printable {
        Graph(void) : data(Matrix<T>()) {}
        Graph(const Matrix<T>& data) : data(data) {}
        Graph(size_t points) : data(Matrix<T>(points, points)) {
            for (T& x : data) x = 0;
        }
        [[nodiscard]] size_t GetPointCount(void) const {
            return data.GetWidth();
        }
        [[nodiscard]] T& At(size_t from, size_t to) {
            return data.At(to, from);
        }
        [[nodiscard]] T At(size_t from, size_t to) const {
            return data.At(to, from);
        }
        void SetWeight(size_t from, size_t to, const T& value, bool twoWay) {
            At(from, to) = value;
            if (twoWay) At(to, from) = value;
        }
        void AddPoint(void) {
            const Matrix<T> tmp = data;
            data = Matrix<T>(tmp.GetWidth() + 1, tmp.GetHeight() + 1);
            for (size_t y = 0; y < tmp.GetHeight(); y++)
                for (size_t x = 0; x < tmp.GetWidth(); x++) data.At(x, y) = tmp.At(x, y);
            for (size_t y = 0; y < 1; y++)
                for (size_t x = 0; x < 1; x++) data.At(x + tmp.GetWidth(), y + tmp.GetHeight()) = 0;
        }
        [[nodiscard]] size_t ConnectGraph(const Graph<T>& other) {
            const size_t ret = GetPointCount();
            MathLib::Matrix<T> tmp = MathLib::Matrix<T>(ret + other.GetPointCount(), ret + other.GetPointCount());
            for (size_t y = 0; y < ret; y++)
                for (size_t x = 0; x < ret; x++) tmp.At(x, y) = data.At(x, y);
            for (size_t y = 0; y < other.GetPointCount(); y++)
                for (size_t x = 0; x < other.GetPointCount(); x++) tmp.At(x + ret, y + ret) = other.data.At(x, y);
            data = tmp;
            return ret;
        }
        [[nodiscard]] Expected<Graph<T>> Permutate(const Matrix<T>& permutation) const {
            if (!permutation.IsSquare() || permutation.GetWidth() != GetPointCount()) return Expected<Graph<T>>();
            Expected<Matrix<T>> tmp = permutation * data;
            if (!tmp.HasValue()) return Expected<Graph<T>>();
            tmp = tmp.Get() * permutation.GetTranspose();
            if (!tmp.HasValue()) return Expected<Graph<T>>();
            return Expected<Graph<T>>(Graph<T>(tmp.Get()));
        }
        [[nodiscard]] Array<size_t> GetNeighbours(size_t point) const {
            Array<size_t> ret;
            for (size_t i = 0; i < GetPointCount(); i++)
                if (At(point, i) && !ret.Add(i)) return Array<size_t>();
            return ret;
        }
        [[nodiscard]] Expected<Tree<T>> DijkstrasAlgorithm(size_t start) const {
            Array<DijkstrasAlgorithmData<T>> data = Array<DijkstrasAlgorithmData<T>>(GetPointCount());
            data.At(start) = DijkstrasAlgorithmData<T>(true, 0, SIZE_MAX);
            size_t curr = start;
            while (true) {
                const Array<size_t> neighbours = GetNeighbours(curr);
                for (const size_t& x : neighbours) {
                    const T dist = At(curr, x) + data.At(curr).distance;
                    if (dist < data.At(x).distance) {
                        data.At(x).distance = dist;
                        data.At(x).prev = curr;
                    }
                }
                size_t next = SIZE_MAX;
                for (size_t i = 0; i < data.GetSize(); i++) {
                    if (data.At(i).visited) continue;
                    if (next == SIZE_MAX) next = i;
                    else if (data.At(i).distance < data.At(next).distance) next = i;
                }
                if (next == SIZE_MAX) break;
                data.At(next).visited = true;
                curr = next;
            }
            return ToTree(data, start, 0);
        }
        /// @brief Converts struct to string
        /// @param padding String to pad with
        /// @return String representation
        [[nodiscard]] virtual String ToString(const String& padding = "") const override {
            return data.ToString(padding);
        }
        [[nodiscard]] Expected<bool> operator==(const Graph<T>& other) const {
            const size_t points = GetPointCount();
            Array<size_t> perm = Array<size_t>(points);
            for (size_t i = 0; i < points; i++) perm.At(i) = i;
            while (true) {
                Matrix<T> permutation = Matrix<T>(points, points);
                for (size_t i = 0; i < points; i++) permutation.At(perm.At(i), i) = 1;
                const Expected<Graph<T>> tmp = Permutate(permutation);
                if (!tmp.HasValue()) return Expected<bool>();
                if (tmp.Get().data == other.data) return Expected<bool>(true);
                const Expected<bool> t = NextPermutation<size_t>(perm);
                if (!t.HasValue()) return Expected<bool>();
                if (!t.Get()) return Expected<bool>(false);
            }
        }

        private:
        [[nodiscard]] Expected<Tree<T>> ToTree(const Array<DijkstrasAlgorithmData<T>>& data, size_t start, T prevDist) const {
            Tree<T> ret = Tree<T>(MathLib::ToString(start, 10), data.At(start).distance - prevDist);
            for (size_t i = 0; i < data.GetSize(); i++) {
                if (data.At(i).prev == start) {
                    const Expected<Tree<T>> tmp = ToTree(data, i, data.At(start).distance);
                    if (!tmp.HasValue() || !ret.Add(tmp.Get())) return Expected<Tree<T>>();
                }
            }
            return Expected<Tree<T>>(ret);
        }
        Matrix<T> data;
    };
}

#endif