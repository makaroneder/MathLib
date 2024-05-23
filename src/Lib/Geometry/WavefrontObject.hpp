#ifndef Geometry_WavefrontObject_H
#define Geometry_WavefrontObject_H
#include "LineShape.hpp"
#include "../Quaternion.hpp"
#include "../EquationSolver/Tokenizer.hpp"
#include "../Host.hpp"
#include <fstream>

template <typename T>
struct WavefrontObject : LineShape<T> {
    std::vector<Matrix<T>> verticies;
    std::vector<std::vector<size_t>> faces;
    std::vector<size_t> lines;

    WavefrontObject(Matrix<T> pos, std::string path) : LineShape<T>(pos) {
        std::ifstream file = std::ifstream(path);
        while (!file.eof()) {
            std::string line;
            std::getline(file, line);
            if (line[0] == 'v' && line[1] == ' ') {
                size_t i = 2;
                Matrix<T> v = CreateVector<T>(0, 0, 0);
                for (size_t axis = (size_t)VectorAxis::X; axis < (size_t)VectorAxis::AxisCount; axis++) {
                    std::string tmp = "";
                    SkipWhiteSpace(line, i);
                    while (IsDigit(line[i]) || line[i] == '-' || line[i] == '.' || line[i] == 'e' || line[i] == 'E') tmp += line[i++];
                    GetVectorAxis(v, axis) = std::stold(tmp);
                }
                verticies.push_back(v);
            }
            else if (line[0] == 'f' && line[1] == ' ') {
                std::vector<size_t> face;
                size_t i = 2;
                while (i < line.size()) {
                    std::string tmp = "";
                    SkipWhiteSpace(line, i);
                    while (IsDigit(line[i])) tmp += line[i++];
                    face.push_back((size_t)std::stold(tmp) - 1);
                    SkipWhiteSpace(line, i);
                    if (line[i] == '/') {
                        i++;
                        SkipWhiteSpace(line, i);
                        while (IsDigit(line[i])) tmp += line[i++];
                        SkipWhiteSpace(line, i);
                        if (line[i] == '/') {
                            i++;
                            SkipWhiteSpace(line, i);
                            while (IsDigit(line[i])) tmp += line[i++];
                        }
                    }
                }
                faces.push_back(face);
            }
            else if (line[0] == 'l' && line[1] == ' ') {
                size_t i = 2;
                for (size_t j = 0; j < 2; j++) {
                    std::string tmp = "";
                    SkipWhiteSpace(line, i);
                    while (IsDigit(line[i])) tmp += line[i++];
                    lines.push_back((size_t)std::stold(tmp) - 1);
                }
            }
        }
    }
    virtual bool CollidesWith(const Shape<T>& other_) const override {
        // TODO:
        (void)other_;
        return false;
    }
    virtual std::vector<Line<T>> ToLines(Matrix<T> rotation) const override {
        std::vector<Line<T>> ret;
        for (const std::vector<size_t>& face : faces) {
            std::vector<Matrix<T>> verts = std::vector<Matrix<T>>(face.size());
            for (size_t i = 0; i < verts.size(); i++)
                verts[i] = RotateVector<T>(verticies.at(face[i]) + this->position, this->position, rotation);
            for (size_t i = 0; i < verts.size(); i++) ret.push_back(Line<T>(verts[i], verts[(i + 1) % verts.size()]));
        }
        for (size_t i = 0; i < lines.size(); i += 2) {
            Matrix<T> tmp[2];
            for (size_t j = 0; j < 2; j++) tmp[j] = RotateVector<T>(verticies.at(lines[i]) + this->position, this->position, rotation);
            ret.push_back(Line<T>(tmp[0], tmp[1]));
        }
        return ret;
    }
};

#endif