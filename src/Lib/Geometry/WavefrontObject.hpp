#ifndef MathLib_Geometry_WavefrontObject_H
#define MathLib_Geometry_WavefrontObject_H
#include "String.hpp"
#include "Polygon.hpp"
#include "LineShape.hpp"
#include "../FunctionT.hpp"
#include "../Math/Quaternion.hpp"
#include "../FileSystem/File.hpp"
#include "../EquationSolver/Tokenizer.hpp"

namespace MathLib {
    template <typename T>
    struct WavefrontObject : LineShape<T>, Saveable {
        Array<Matrix<T>> verticies;
        Array<Array<size_t>> faces;
        Array<size_t> lines;

        WavefrontObject(void) : LineShape<T>(Matrix<T>(3, 1)) {}
        WavefrontObject(const Matrix<T>& pos) : LineShape<T>(pos) {}
        [[nodiscard]] WavefrontObject<T> Triangulate(void) const {
            WavefrontObject<T> ret = *this;
            if (!ret.faces.Reset()) return WavefrontObject<T>();
            const auto func = MakeFunctionT<Vector3<T>, size_t>([this](size_t i) -> Vector3<T> {
                return verticies.At(i);
            });
            for (const Array<size_t>& face : faces)
                if (!ret.faces.Add(PolygonToTriangles<T, size_t>(face, func))) return WavefrontObject<T>();
            return ret;
        }
        [[nodiscard]] virtual bool CollidesWith(const Shape<T>&) const override {
            // TODO:
            return false;
        }
        [[nodiscard]] virtual Array<Line<T>> ToLines(const Matrix<T>& rotation) const override {
            Array<Line<T>> ret;
            for (const Sequence<size_t>& face : faces) {
                Array<Matrix<T>> verts = Array<Matrix<T>>(face.GetSize());
                for (size_t i = 0; i < verts.GetSize(); i++)
                    verts.At(i) = RotateVector<T>(verticies.At(face.At(i)) + this->position, this->position, rotation);
                for (size_t i = 0; i < verts.GetSize(); i++)
                    if (!ret.Add(Line<T>(verts.At(i), verts.At((i + 1) % verts.GetSize())))) return Array<Line<T>>();
            }
            for (size_t i = 0; i < lines.GetSize(); i += 2) {
                Matrix<T> tmp[2];
                for (size_t j = 0; j < 2; j++) tmp[j] = RotateVector<T>(verticies.At(lines.At(i + j)) + this->position, this->position, rotation);
                if (!ret.Add(Line<T>(tmp[0], tmp[1]))) return Array<Line<T>>();
            }
            return ret;
        }
        /// @brief Saves data
        /// @param file File to save data into
        /// @return Status
        [[nodiscard]] virtual bool Save(Writable& file) const override {
            for (const Matrix<T>& v : verticies) {
                if (!file.Write<char>('v')) return false;
                for (size_t axis = (size_t)VectorAxis::X; axis < (size_t)VectorAxis::AxisCount; axis++)
                    if (!file.Puts(' '_M + ToString(GetVectorAxis(v, axis)))) return false;
                if (!file.Write<char>('\n')) return false;
            }
            for (const Sequence<size_t>& face : faces) {
                if (!file.Write<char>('f')) return false;
                if (!face.Foreach<bool>(MakeFunctionT<bool, bool, size_t>([&file](bool prev, size_t f) -> bool {
                    return prev && file.Puts(' '_M + ToString(f));
                }), true)) return false;
                if (!file.Write<char>('\n')) return false;
            }
            for (size_t i = 0; i < lines.GetSize(); i += 2)
                if (!file.Puts("l "_M + ToString(lines.At(i)) + ' ' + ToString(lines.At(i + 1)) + '\n')) return false;
            return true;
        }
        /// @brief Loads data
        /// @param file File to load data from
        /// @return Status
        [[nodiscard]] virtual bool Load(Readable& file) override {
            const Array<String> split = Split(file.ReadUntil('\0'), "\n"_M, true);
            for (const Sequence<char>& line : split) {
                if (line[0] == 'v' && line[1] == ' ') {
                    size_t i = 2;
                    Matrix<T> v = CreateVector<T>(0, 0, 0);
                    for (size_t axis = (size_t)VectorAxis::X; axis < (size_t)VectorAxis::AxisCount; axis++) {
                        String tmp = "";
                        SkipWhiteSpace(line, i);
                        while (i < line.GetSize() && (IsDigit(line[i]) || line[i] == '-' || line[i] == '.' || line[i] == 'e' || line[i] == 'E')) tmp += line[i++];
                        GetVectorAxis(v, axis) = StringToNumber(tmp);
                    }
                    if (!verticies.Add(v)) return false;
                }
                else if (line[0] == 'f' && line[1] == ' ') {
                    Array<size_t> face;
                    size_t i = 2;
                    while (i < line.GetSize()) {
                        String tmp = "";
                        SkipWhiteSpace(line, i);
                        while (i < line.GetSize() && IsDigit(line[i])) tmp += line[i++];
                        if (!tmp.IsEmpty()) {
                            if (!face.Add(StringToNumber(tmp) - 1)) return false;
                            SkipWhiteSpace(line, i);
                            if (i < line.GetSize() && line[i] == '/') {
                                i++;
                                SkipWhiteSpace(line, i);
                                while (i < line.GetSize() && IsDigit(line[i])) tmp += line[i++];
                                SkipWhiteSpace(line, i);
                                if (i < line.GetSize() && line[i] == '/') {
                                    i++;
                                    SkipWhiteSpace(line, i);
                                    while (i < line.GetSize() && IsDigit(line[i])) tmp += line[i++];
                                }
                            }
                        }
                    }
                    if (!faces.Add(face)) return false;
                }
                else if (line[0] == 'l' && line[1] == ' ') {
                    size_t i = 2;
                    for (size_t j = 0; j < 2; j++) {
                        String tmp = "";
                        SkipWhiteSpace(line, i);
                        while (i < line.GetSize() && IsDigit(line[i])) tmp += line[i++];
                        if (!lines.Add(StringToNumber(tmp) - 1)) return false;
                    }
                }
            }
            return true;
        }
    };
}

#endif