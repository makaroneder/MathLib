#ifndef Geometry_WavefrontObject_H
#define Geometry_WavefrontObject_H
#include "../Host.hpp"
#include "LineShape.hpp"
#include "../Quaternion.hpp"
#include "../FileSystem.hpp"
#include "../EquationSolver/Tokenizer.hpp"

template <typename T>
struct WavefrontObject : LineShape<T> {
    Array<Matrix<T>> verticies;
    Array<Array<size_t>> faces;
    Array<size_t> lines;

    WavefrontObject(Matrix<T> pos, FileSystem& fileSystem, String path) : LineShape<T>(pos) {
        const size_t file = fileSystem.Open(path, OpenMode::Read);
        const size_t size = fileSystem.GetFileSize(file);
        while (file != SIZE_MAX && fileSystem.Tell(file) < size) {
            String line = fileSystem.ReadUntil(file, '\n');
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
    virtual Array<Line<T>> ToLines(Matrix<T> rotation) const override {
        Array<Line<T>> ret;
        for (size_t i = 0; i < faces.GetSize(); i++) {
            Array<Matrix<T>> verts = Array<Matrix<T>>(faces.At(i).GetSize());
            for (size_t j = 0; j < verts.GetSize(); j++)
                verts.At(j) = RotateVector<T>(verticies.At(faces.At(i).At(j)) + this->position, this->position, rotation);
            for (size_t j = 0; j < verts.GetSize(); j++) ret.Add(Line<T>(verts.At(j), verts.At((j + 1) % verts.GetSize())));
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