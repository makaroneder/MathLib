#ifndef Pattern_H
#define Pattern_H
#include <Math/Matrix.hpp>

struct Pattern {
    Pattern(const MathLib::Matrix<uint8_t>& data, const MathLib::Matrix<ssize_t>& position);
    [[nodiscard]] MathLib::Matrix<ssize_t> GetPosition(void) const;
    [[nodiscard]] size_t GetWidth(void) const;
    [[nodiscard]] size_t GetHeight(void) const;
    [[nodiscard]] uint8_t At(size_t x, size_t y) const;
    [[nodiscard]] uint8_t& At(size_t x, size_t y);

    private:
    MathLib::Matrix<uint8_t> data;
    MathLib::Matrix<ssize_t> position;
};

#endif