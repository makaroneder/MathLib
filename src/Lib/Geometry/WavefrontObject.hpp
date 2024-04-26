#ifndef Geometry_WavefrontObject_H
#define Geometry_WavefrontObject_H
#include "Shape.hpp"
#include "../Quaternion.hpp"
#include "../EquationSolver/Tokenizer.hpp"
#include <fstream>

template <typename T>
struct WavefrontObject : Shape<T> {
    std::vector<Matrix<T>> verticies;
    std::vector<std::array<size_t, 3>> faces;
    Matrix<T> position;

    WavefrontObject(std::string path, Matrix<T> pos) {
        position = pos;
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
                    while (isdigit(line[i]) || line[i] == '-' || line[i] == '.' || line[i] == 'e' || line[i] == 'E') tmp += line[i++];
                    GetVectorAxis(v, axis) = std::stold(tmp);
                }
                verticies.push_back(v);
            }
            else if (line[0] == 'f' && line[1] == ' ') {
                std::array<size_t, 3> face;
                size_t i = 2;
                for (size_t n = 0; n < 3; n++) {
                    std::string tmp = "";
                    SkipWhiteSpace(line, i);
                    while (isdigit(line[i])) tmp += line[i++];
                    face.at(n) = std::stoull(tmp) - 1;
                    SkipWhiteSpace(line, i);
                    if (line[i] == '/') {
                        i++;
                        SkipWhiteSpace(line, i);
                        while (isdigit(line[i])) tmp += line[i++];
                        SkipWhiteSpace(line, i);
                        if (line[i] == '/') {
                            i++;
                            SkipWhiteSpace(line, i);
                            while (isdigit(line[i])) tmp += line[i++];
                        }
                    }
                }
                faces.push_back(face);
            }
        }
    }
    virtual std::vector<Line<T>> ToLines(T angle, Matrix<T> axis) const override {
        std::vector<Line<T>> lines;
        for (const std::array<size_t, 3>& face : faces) {
            const std::array<Matrix<T>, 3> vert = {
                RotateVector<T>(verticies.at(face.at(0)) + position, position, angle, axis),
                RotateVector<T>(verticies.at(face.at(1)) + position, position, angle, axis),
                RotateVector<T>(verticies.at(face.at(2)) + position, position, angle, axis),
            };
            for (size_t i = 0; i < 3; i++) lines.push_back(Line<T>(vert.at(i), vert.at((i + 1) % 3)));
        }
        return lines;
    }
};

#endif