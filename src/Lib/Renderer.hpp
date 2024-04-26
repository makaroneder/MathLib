#ifndef Renderer_H
#define Renderer_H
#include "Set.hpp"
#include "Font.hpp"
#include "Event.hpp"
#include "Color.hpp"
#include "Geometry/Shape.hpp"
#include "ComplexPosition.hpp"
#include <functional>

struct Renderer {
    /// @brief Creates a new renderer
    /// @param w Width of the window
    /// @param h Height of the window
    Renderer(size_t w, size_t h);
    /// @brief Destroys the renderer
    virtual ~Renderer(void);
    /// @brief Updates renderer
    /// @param pixels Pixel to render
    /// @return Status
    virtual bool UpdateInternal(Matrix<uint32_t> pixels) = 0;
    /// @brief Gets current event
    /// @return Event
    virtual Event GetEvent(void) = 0;
    /// @brief Calculates width of the renderer window
    /// @return Width of the renderer window
    size_t GetWidth(void) const;
    /// @brief Calculates height of the renderer window
    /// @return Height of the renderer window
    size_t GetHeight(void) const;
    /// @brief Waits for current event
    /// @return Event
    Event WaitForEvent(void);
    /// @brief Flushes renderer buffer
    /// @return Status
    bool Update(void);
    /// @brief Fills screen
    /// @param color Color to fill the screen with
    void Fill(uint32_t color);
    /// @brief Renders pixel
    /// @tparam T Type of number
    /// @param pixel Pixel to render
    /// @param color Color of the pixel
    template <typename T>
    void SetPixel(Matrix<T> pixel, uint32_t color) {
        pixel = ConvertVectorToVector2<T>(pixel - ConvertMatrix<num_t, T>(position)) * pointMultiplier;
        const T x = GetX(pixel) + width / 2;
        const T y = height / 2 - GetY(pixel);
        if (x > 0 && x < width && y > 0 && y < height) pixels.At(x, y) = BlendColor(pixels.At(x, y), color);
    }
    /// @brief Draws a line
    /// @tparam T Type of number
    /// @param line Line to draw
    /// @param color Color of the line
    template <typename T>
    void DrawLine(Line<T> line, uint32_t color) {
        const Matrix<T> diff = line.end - line.start;
        const T dx = GetX(diff);
        const T dy = GetY(diff);
        const T dz = GetZ(diff);
        VectorAxis inputAxis = VectorAxis::AxisCount;
        std::array<VectorAxis, (size_t)VectorAxis::AxisCount - 1> outputAxis;
        if (dx == 0 && dy == 0 && dz == 0) {
            SetPixel<T>(line.start, color);
            return;
        }
        else if (std::abs(dx) > std::abs(dy) && std::abs(dx) > std::abs(dz)) inputAxis = VectorAxis::X;
        else if (std::abs(dy) > std::abs(dx) && std::abs(dy) > std::abs(dz)) inputAxis = VectorAxis::Y;
        else inputAxis = VectorAxis::Z;
        for (size_t i = 0; i < (size_t)VectorAxis::AxisCount; i++)
            if (i != (size_t)inputAxis) outputAxis[i - (i > (size_t)inputAxis)] = (VectorAxis)i;
        if (GetVectorAxis(line.start, inputAxis) > GetVectorAxis(line.end, inputAxis)) std::swap(line.start, line.end);
        for (T i = GetVectorAxis(line.start, inputAxis); i <= GetVectorAxis(line.end, inputAxis); i += 1 / pointMultiplier) {
            Matrix<T> pos = CreateVector<T>(0, 0, 0);
            GetVectorAxis(pos, inputAxis) = i;
            for (const VectorAxis& out : outputAxis)
                GetVectorAxis(pos, out) = (GetVectorAxis(diff, out) * (i - GetVectorAxis(line.start, inputAxis)) / GetVectorAxis(diff, inputAxis) + GetVectorAxis(line.start, out));
            SetPixel<T>(pos, color);
        }
    }
    /// @brief Renders specified shape
    /// @tparam T Type of number
    /// @param shape Shape to draw
    /// @param angle Angle to rotate the shape by
    /// @param axis Normalized vector containing axis to rotate around
    /// @param color Color of the shape
    template <typename T>
    void DrawShape(const Shape<T>& shape, T angle, Matrix<T> axis, uint32_t color) {
        std::vector<Line<T>> lines = shape.ToLines(angle, axis);
        for (const Line<T>& line : lines)
            DrawLine<T>(Line<T>(line.start, line.end), color);
    }
    /// @brief Renders character
    /// @tparam T Type of number
    /// @param chr Character to render
    /// @param pos Position of the character
    /// @param font Font to renderer character with
    /// @param color Color to render character with
    template <typename T>
    void Putc(char chr, Matrix<T> pos, const PSF1* font, uint32_t color) {
        const T w = font->GetWidth() / 2;
        const T h = font->GetHeight() / 2;
        const uint8_t* fontPtr = font->GetGlyph(chr);
        for (T y = -h; y < h; y++) {
            for (T x = -w; x < w; x++)
                if (*fontPtr & (1 << (7 - (size_t)(x + w))))
                    SetPixel<T>(CreateVector<T>(x, -y, 0) / pointMultiplier + pos, color);
            fontPtr++;
        }
    }
    /// @brief Renders string
    /// @tparam T Type of number
    /// @param str String to render
    /// @param pos Position of the string
    /// @param startX First x position to render
    /// @param font Font to renderer string with
    /// @param color Color to render string with
    template <typename T>
    void Puts(std::string str, Matrix<T> pos, T startX, PSF1* font, uint32_t color) {
        const T w = GetX(GetEnd<T>());
        for (const char& chr : str) {
            switch (chr) {
                case ' ': {
                    GetX(pos) += font->GetWidth() / pointMultiplier;
                    break;
                }
                case '\n': {
                    GetY(pos) -= font->GetHeight() / pointMultiplier;
                    GetX(pos) = startX;
                    break;
                }
                default: {
                    Putc(chr, pos, font, color);
                    GetX(pos) += font->GetWidth() / pointMultiplier;
                    break;
                }
            }
            if (GetX(pos) >= w) {
                GetY(pos) -= font->GetHeight() / pointMultiplier;
                GetX(pos) = startX;
            }
        }
    }
    /// @brief Calculates start of the graph
    /// @tparam T Type of number
    /// @return Start of graph
    template <typename T>
    Matrix<T> GetStart(void) const {
        return CreateVector<T>(-(width / 2 / pointMultiplier), -(height / 2 / pointMultiplier), 0);
    }
    /// @brief Calculates end of the graph
    /// @tparam T Type of number
    /// @return End of graph
    template <typename T>
    Matrix<T> GetEnd(void) const {
        return CreateVector<T>(width / 2 / pointMultiplier, height / 2 / pointMultiplier, 0);
    }
    /// @brief Creates set of every pixel on the screen
    /// @tparam T Type of number
    /// @param axis Axis we are using
    /// @return Set of every pixel on the screen
    template <typename T>
    std::vector<T> CreateRealNumberSet(void) const {
        const bool isFloat = IsFloat<T>();
        const T div = isFloat ? 1 : pointMultiplier;
        const Matrix<T> startArr = GetStart<T>();
        const Matrix<T> endArr = GetEnd<T>();
        return CreateSet<T>(
            std::max<T>(GetX(startArr), GetY(startArr)) * div,
            std::max<T>(GetX(endArr), GetY(endArr)) * div,
            1 / (isFloat ? pointMultiplier : 1)
        );
    }
    /// @brief Draw x and y axis
    /// @param axisColor Color of the axis
    /// @param cellColor Color of the cells
    void DrawAxis(uint32_t axisColor, uint32_t cellColor);
    /// @brief Draws complex function based on its values
    /// @tparam T Type of number
    /// @param values Values generated by function
    template <typename T>
    void DrawComplexFunction(std::vector<ComplexPosition<T>> values) {
        for (const ComplexPosition<T>& val : values) SetPixel<T>(val.GetPosition(), val.GetColor());
    }
    /// @brief Draws function based on its values
    /// @tparam T Type of number
    /// @param values Values generated by function
    /// @param color Color of function
    template <typename T>
    void DrawFunction(std::vector<Line<T>> values, uint32_t color) {
        const T div = IsFloat<T>() ? 1 : pointMultiplier;
        for (const Line<T>& val : values) {
            #ifdef FillGapsInFunctions
            if (!std::isnan(GetX(val.start))) DrawLine<T>(Line<T>(val.start / div, val.end / div), color);
            #else
            SetPixel<T>(val.end / div, color);
            #endif
        }
    }
    /// @brief f(x + y * i)
    /// @tparam T Type of number
    /// @param f Complex function to calculate color of the graph
    /// @param inSet Set of number values we allow as input
    /// @return Result of complex function
    template <typename T>
    std::vector<ComplexPosition<T>> GenerateComplexFunction(std::function<std::complex<T>(std::complex<T>)> f, std::vector<T> inSet) {
        std::vector<ComplexPosition<T>> ret;
        for (T& i : inSet) {
            for (T& r : inSet) {
                const std::complex<T> pos = std::complex<T>(r, i);
                const std::complex<T> val = f(pos);
                if (!(std::isnan(val.real()) || std::isinf(val.real()) || std::isnan(val.imag()) || std::isinf(val.imag())))
                    ret.push_back(ComplexPosition<T>(pos, val));
            }
        }
        return ret;
    }
    /// @brief y = f(x)
    /// @tparam T Type of number
    /// @param f Function that calculates x or y depending on the 'axis' parameter
    /// @param inSet Set of number values we allow as input
    /// @param inAxis Axis we are using for input values
    /// @param outAxis Axis we are using for output values
    /// @return Result of function
    template <typename T>
    std::vector<Line<T>> GenerateFunction(std::function<std::vector<T>(T)> f, std::vector<T> inSet, VectorAxis inAxis = VectorAxis::X, VectorAxis outAxis = VectorAxis::Y) {
        std::vector<Matrix<T>> prev = std::vector<Matrix<T>>();
        std::vector<Line<T>> ret = std::vector<Line<T>>();
        for (const T& i : inSet) {
            Matrix<T> curr = CreateVector<T>(0, 0, 0);
            const std::vector<T> arr = f(i);
            if (prev.empty())
                for (T j = 0; j < arr.size(); j++) prev.push_back(CreateVector<T>(NAN, NAN, NAN));
            for (T j = 0; j < arr.size(); j++) {
                const T tmp = arr.at(j);
                if (std::isnan(i) || std::isinf(i) || std::isnan(tmp) || std::isinf(tmp)) {
                    prev.at(j) = CreateVector<num_t>(NAN, NAN, NAN);
                    continue;
                }
                GetVectorAxis(curr, inAxis) = i;
                GetVectorAxis(curr, outAxis) = tmp;
                ret.push_back(Line<T>(prev.at(j), curr));
                prev.at(j) = curr;
            }
        }
        return ret;
    }
    /// @brief y = f(x)
    /// @tparam T Type of number
    /// @param f Function that calculates x or y depending on the 'axis' parameter
    /// @param inSet Set of number values we allow as input
    /// @param inAxis Axis we are using for input values
    /// @param outAxis Axis we are using for output values
    /// @return Result of function
    template <typename T>
    std::vector<Line<T>> GenerateFunction(std::function<T(T)> f, std::vector<T> inSet, VectorAxis inAxis = VectorAxis::X, VectorAxis outAxis = VectorAxis::Y) {
        return GenerateFunction<T>([f](T x) {
            std::vector<T> ret = { f(x), };
            return ret;
        }, inSet, inAxis, outAxis);
    }
    /// @brief Current position
    Matrix<num_t> position;
    /// @brief Scale
    num_t pointMultiplier;

    private:
    /// @brief Width of the window
    size_t width;
    /// @brief Height of the window
    size_t height;
    /// @brief Pixels
    Matrix<uint32_t> pixels;
};

#endif