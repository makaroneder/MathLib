#ifndef Geometry_WavefrontObject_H
#define Geometry_WavefrontObject_H
#include "LineShape.hpp"
#include "../Quaternion.hpp"
#include "../FileSystems/File.hpp"
#include "../EquationSolver/Tokenizer.hpp"

template <typename T>
struct WavefrontObject : LineShape<T> {
    Array<Matrix<T>> verticies;
    Array<Array<size_t>> faces;
    Array<size_t> lines;

    WavefrontObject(const Matrix<T>& pos, FileSystem& fileSystem, const String& path) : LineShape<T>(pos) {
        File file = fileSystem.Open(path, OpenMode::Read);
        const size_t size = file.GetSize();
        while (file.Tell() < size) {
            String line = file.ReadUntil('\n');
            if (line[0] == 'v' && line[1] == ' ') {
                size_t i = 2;
                Matrix<T> v = CreateVector<T>(0, 0, 0);
                for (size_t axis = (size_t)VectorAxis::X; axis < (size_t)VectorAxis::AxisCount; axis++) {
                    String tmp = "";
                    SkipWhiteSpace(line, i);
                    while (i < line.GetSize() && (IsDigit(line[i]) || line[i] == '-' || line[i] == '.' || line[i] == 'e' || line[i] == 'E')) tmp += line[i++];
                    GetVectorAxis(v, axis) = StringToNumber(tmp);
                }
                verticies.Add(v);
            }
            else if (line[0] == 'f' && line[1] == ' ') {
                Array<size_t> face;
                size_t i = 2;
                while (i < line.GetSize()) {
                    String tmp = "";
                    SkipWhiteSpace(line, i);
                    while (i < line.GetSize() && IsDigit(line[i])) tmp += line[i++];
                    face.Add(StringToNumber(tmp) - 1);
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
                faces.Add(face);
            }
            else if (line[0] == 'l' && line[1] == ' ') {
                size_t i = 2;
                for (size_t j = 0; j < 2; j++) {
                    String tmp = "";
                    SkipWhiteSpace(line, i);
                    while (i < line.GetSize() && IsDigit(line[i])) tmp += line[i++];
                    lines.Add(StringToNumber(tmp) - 1);
                }
            }
        }
    }
    virtual bool CollidesWith(const Shape<T>&) const override {
        // TODO:
        return false;
    }
    virtual Array<Line<T>> ToLines(const Matrix<T>& rotation) const override {
        Array<Line<T>> ret;
        for (const Array<size_t>& face : faces) {
            Array<Matrix<T>> verts = Array<Matrix<T>>(face.GetSize());
            for (size_t i = 0; i < verts.GetSize(); i++)
                verts.At(i) = RotateVector<T>(verticies.At(face.At(i)) + this->position, this->position, rotation);
            for (size_t i = 0; i < verts.GetSize(); i++) ret.Add(Line<T>(verts.At(i), verts.At((i + 1) % verts.GetSize())));
        }
        for (size_t i = 0; i < lines.GetSize(); i += 2) {
            Matrix<T> tmp[2];
            for (size_t j = 0; j < 2; j++) tmp[j] = RotateVector<T>(verticies.At(lines.At(i + j)) + this->position, this->position, rotation);
            ret.Add(Line<T>(tmp[0], tmp[1]));
        }
        return ret;
    }
};

#endif