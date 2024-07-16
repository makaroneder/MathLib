#ifndef Renderer_H
#define Renderer_H
#include "Font.hpp"
#include "Image.hpp"
#include "Event.hpp"
#include "Color.hpp"
#include "Threads.hpp"
#include "Function.hpp"
#include "Saveable.hpp"
#include "Quaternion.hpp"
#include "ComplexPosition.hpp"
#include "Geometry/LineShape.hpp"

struct Renderer : Image {
    /// @brief Creates a new renderer
    /// @param w Width of the window
    /// @param h Height of the window
    Renderer(const size_t& w, const size_t& h);
    virtual ~Renderer(void) override;
    /// @brief Updates renderer
    /// @return Status
    virtual bool Update(void) = 0;
    /// @brief Gets current event
    /// @return Event
    virtual Event GetEvent(void) = 0;
    /// @brief Waits for current event
    /// @return Event
    Event WaitForEvent(void);
    /// @brief Renders pixel
    /// @tparam T Type of number
    /// @param pixel Pixel to render
    /// @param color Color of the pixel
    template <typename T>
    void SetPixel(const Matrix<T>& pixel, const uint32_t& color) {
        SetPixelInternal<T>(ProjectVector<T>(pixel - ConvertMatrix<num_t, T>(position)) * pointMultiplier, color);
    }
    /// @brief Returns pixel
    /// @tparam T Type of number
    /// @param pixel Position of the pixel
    /// @return Color of the pixel
    template <typename T>
    uint32_t GetPixel(const Matrix<T>& pixel) {
        uint32_t* color = GetPixelInternal<T>(ProjectVector<T>(pixel - ConvertMatrix<num_t, T>(position)) * pointMultiplier);
        return color ? *color : 0;
    }
    /// @brief Copies pixels from renderer to this renderer
    /// @tparam T Type of number
    /// @param renderer Renderer to copy pixels from
    /// @param rotation Vector containing axis to rotate around
    /// @return Status
    template <typename T>
    bool DrawImage(const Renderer& renderer, const Matrix<T>& rotation) {
        if (pointMultiplier != renderer.pointMultiplier) return false;
        const T w = renderer.GetWidth() / (pointMultiplier * 2);
        const T h = renderer.GetHeight() / (pointMultiplier * 2);
        for (T y = -h; y <= h; y += 1 / pointMultiplier) {
            for (T x = -w; x <= w; x += 1 / pointMultiplier) {
                const Matrix<T> pos = CreateVector<T>(x, y, 0);
                SetPixel<T>(RotateVector<T>(pos + renderer.position, renderer.position, rotation), renderer.GetPixel<T>(pos));
            }
        }
        return true;
    }
    /// @brief Draws a line
    /// @tparam T Type of number
    /// @param line Line to draw
    /// @param color Color of the line
    template <typename T>
    void DrawLine(const Line<T>& line, const uint32_t& color) {
        Matrix<ssize_t> startVector = ConvertMatrix<T, ssize_t>(ProjectVector<T>(line.start - ConvertMatrix<num_t, T>(position)) * pointMultiplier);
        Matrix<ssize_t> endVector = ConvertMatrix<T, ssize_t>(ProjectVector<T>(line.end - ConvertMatrix<num_t, T>(position)) * pointMultiplier);
        bool steep = false;
        ssize_t x0 = GetX(startVector);
        ssize_t x1 = GetX(endVector);
        ssize_t y0 = GetY(startVector);
        ssize_t y1 = GetY(endVector);
        if (Abs(x0 - x1) < Abs(y0 - y1)) {
            Swap(x0, y0);
            Swap(x1, y1);
            steep = true;
        }
        if (x0 > x1) {
            Swap(x0, x1);
            Swap(y0, y1);
        }
        const ssize_t dx = x1 - x0;
        const ssize_t derr = Abs(y1 - y0) * 2;
        ssize_t err = 0;
        ssize_t y = y0;
        for (ssize_t x = x0; x <= x1; x++) {
            if (steep) SetPixelInternal<ssize_t>(CreateVector<ssize_t>(y, x, 0), color);
            else SetPixelInternal<ssize_t>(CreateVector<ssize_t>(x, y, 0), color);
            err += derr;
            if (err > dx) {
                y += (y1 > y0 ? 1 : -1);
                err -= dx * 2;
            }
        }
    }
    /// @brief Renders specified shape
    /// @tparam T Type of number
    /// @param shape Shape to draw
    /// @param rotation Vector containing axis to rotate around
    /// @param color Color of the shape
    template <typename T>
    void DrawShape(const LineShape<T>& shape, const Matrix<T>& rotation, const uint32_t& color) {
        const Array<Line<T>> lines = shape.ToLines(rotation);
        for (size_t i = 0; i < lines.GetSize(); i++) DrawLine<T>(Line<T>(lines.At(i).start, lines.At(i).end), color);
    }
    /// @brief Renders strings
    /// @tparam T Type of number
    /// @param str Strings to render
    /// @param font Font to renderer strings with
    /// @param pos Position of the strings
    /// @param rotation Vector containing axis to rotate around
    /// @param scale Scale of the strings
    /// @param color Color to render strings with
    /// @return Status
    template <typename T>
    bool Puts(const Array<String>& strs, const PSF1* font, Matrix<T> pos, const Matrix<T>& rotation, const Matrix<size_t>& scale, const uint32_t& color) {
        if (!font || !font->IsValid()) return false;
        const T w = font->GetWidth() / 2;
        const T h = font->GetHeight() / 2;
        const size_t sx = GetX(scale);
        const size_t sy = GetY(scale);
        const size_t sz = GetZ(scale);
        GetY(pos) -= strs.GetSize() * font->GetHeight() * sy / (pointMultiplier * 2);
        Matrix<T> tmp = pos;
        for (size_t i = 0; i < strs.GetSize(); i++) {
            pos = tmp;
            GetX(pos) -= strs.At(i).GetSize() * font->GetWidth() * sx / (pointMultiplier * 2);
            const Matrix<T> center = pos;
            for (size_t j = 0; j < strs.At(i).GetSize(); j++) {
                const uint8_t* fontPtr = font->GetGlyph(strs.At(i).At(j));
                for (T y = -h; y < h; y++) {
                    for (T x = -w; x < w; x++)
                        if (*fontPtr & (1 << (7 - (size_t)(x + w))))
                            for (size_t nz = 0; nz < sz; nz++)
                                for (size_t ny = 0; ny < sy; ny++)
                                    for (size_t nx = 0; nx < sx; nx++)
                                        SetPixel<T>(RotateVector<T>(CreateVector<T>(nx + x, ny - y, nz) / pointMultiplier + pos, center, rotation), color);
                    fontPtr++;
                }
                GetX(pos) += font->GetWidth() * sx / pointMultiplier;
            }
            GetY(tmp) -= font->GetHeight() * sy / pointMultiplier;
        }
        return true;
    }
    /// @brief Calculates start of the graph
    /// @tparam T Type of number
    /// @return Start of graph
    template <typename T>
    Matrix<T> GetStart(void) const {
        return -GetEnd<T>();
    }
    /// @brief Calculates end of the graph
    /// @tparam T Type of number
    /// @return End of graph
    template <typename T>
    Matrix<T> GetEnd(void) const {
        return CreateVector<T>(pixels.GetWidth() / 2 / pointMultiplier, pixels.GetHeight() / 2 / pointMultiplier, 0);
    }
    /// @brief Draw x and y axis
    /// @param axisColor Color of the axis
    /// @param cellColor Color of the cells
    void DrawAxis(const uint32_t& axisColor, const uint32_t& cellColor);
    /// @brief Draws complex function based on its values
    /// @tparam T Type of number
    /// @param values Values generated by function
    template <typename T>
    void DrawComplexFunction(const Array<ComplexPosition<T>>& values) {
        for (size_t i = 0; i < values.GetSize(); i++) SetPixel<T>(values.At(i).GetPosition(), values.At(i).GetColor());
    }
    /// @brief Draws function based on its values
    /// @tparam T Type of number
    /// @param values Values generated by function
    /// @param color Color of function
    template <typename T>
    void DrawFunction(const Array<Line<T>>& values, const uint32_t& color) {
        for (size_t i = 0; i < values.GetSize(); i++) {
            #ifdef FillGapsInFunctions
            if (!IsNaN(GetX(values.At(i).start))) DrawLine<T>(Line<T>(values.At(i).start, values.At(i).end), color);
            #else
            SetPixel<T>(values.At(i).end, color);
            #endif
        }
    }
    /// @brief f(x + y * i)
    /// @tparam T Type of number
    /// @param f Complex function to calculate color of the graph
    /// @return Result of complex function
    template <typename T>
    Array<ComplexPosition<T>> GenerateComplexFunction(const Function<Complex<T>, Complex<T>>& f) {
        // TODO: Make this multithreaded
        Array<ComplexPosition<T>> ret;
        const Matrix<ssize_t> start = ConvertMatrix<T, ssize_t>((GetStart<T>() + ConvertMatrix<num_t, T>(position)) * pointMultiplier);
        const Matrix<ssize_t> end = ConvertMatrix<T, ssize_t>((GetEnd<T>() + ConvertMatrix<num_t, T>(position)) * pointMultiplier);
        for (ssize_t i = GetY(start); i <= GetY(end); i++) {
            for (ssize_t r = GetX(start); r <= GetX(end); r++) {
                const Complex<T> pos = Complex<T>(r, i) / pointMultiplier;
                const Complex<T> val = f(pos);
                if (!(IsNaN(val) || IsInf(val) || IsNaN(pos) || IsInf(pos)))
                    if (!ret.Add(ComplexPosition<T>(pos, val))) return {};
            }
        }
        return ret;
    }
    /// @brief y = f(x)
    /// @tparam T Type of number
    /// @param f Function that calculates x or y depending on the 'axis' parameter
    /// @param inAxis Axis we are using for input values
    /// @param outAxis Axis we are using for output values
    /// @return Result of function
    template <typename T>
    Array<Line<T>> GenerateMultiFunction(const Function<Array<T>, T>& f, const VectorAxis& inAxis = VectorAxis::X, const VectorAxis& outAxis = VectorAxis::Y) {
        Array<Matrix<T>> prev;
        Array<Line<T>> ret;
        const T start = GetVectorAxis(GetStart<T>(), inAxis) + (T)GetX(position);
        const T end = GetVectorAxis(GetEnd<T>(), inAxis) + (T)GetX(position);
        for (T i = start; i <= end; i += 1 / pointMultiplier) {
            Matrix<T> curr = CreateVector<T>(0, 0, 0);
            const Array<T> arr = f(i);
            if (prev.IsEmpty())
                for (T j = 0; j < arr.GetSize(); j++)
                    if (!prev.Add(CreateVector<T>(MakeNaN(), MakeNaN(), MakeNaN()))) return Array<Line<T>>();
            for (T j = 0; j < arr.GetSize(); j++) {
                const T tmp = arr.At(j);
                if (IsNaN(i) || IsInf(i) || IsNaN(tmp) || IsInf(tmp)) {
                    prev.At(j) = CreateVector<num_t>(MakeNaN(), MakeNaN(), MakeNaN());
                    continue;
                }
                GetVectorAxis(curr, inAxis) = i;
                GetVectorAxis(curr, outAxis) = tmp;
                ret.Add(Line<T>(prev.At(j), curr));
                prev.At(j) = curr;
            }
        }
        return ret;
    }
    template <typename T>
    Matrix<T> IndexToPosition(const Matrix<T>& index) const {
        return CreateVector<T>((GetX(index) - pixels.GetWidth() / 2) / pointMultiplier, (pixels.GetHeight() / 2 - GetY(index)) / pointMultiplier, 0);
    }
    template <typename T, typename... Args>
    bool SetImage(Args... args) {
        T* tmp = new T(GetWidth(), GetHeight(), args...);
        if (!tmp) return false;
        if (image) delete image;
        image = tmp;
        return true;
    }

    /// @brief Saves data as image
    /// @param file File to save data into
    /// @return Status
    virtual bool Save(ByteDevice& file) const override;
    /// @brief Loads data from image
    /// @param file File to load data from
    /// @return Status
    virtual bool Load(ByteDevice& file) override;

    /// @brief Current position
    Matrix<num_t> position;
    /// @brief Scale
    num_t pointMultiplier;

    private:
    /// @brief Image for saving and loading
    Image* image;

    /// @brief Returns pixel from index
    /// @tparam T Type of number
    /// @param pos Index of pixel
    /// @return Pixel
    template <typename T>
    uint32_t* GetPixelInternal(const Matrix<T>& pos) {
        const Matrix<T> tmp = CreateVector<T>(pixels.GetWidth() / 2 + GetX(pos), pixels.GetHeight() / 2 - GetY(pos), 0);
        return (IsBetween(GetX(tmp), 0, (ssize_t)pixels.GetWidth() - 1) && IsBetween(GetY(tmp), 0, (ssize_t)pixels.GetHeight() - 1)) ? &pixels.At(GetX(tmp), GetY(tmp)) : nullptr;
    }
    /// @brief Sets pixel from index
    /// @tparam T Type of number
    /// @param pos Index of pixel
    /// @param color Value to set
    template <typename T>
    void SetPixelInternal(const Matrix<T>& pos, const uint32_t& color) {
        uint32_t* pixel = GetPixelInternal<T>(pos);
        if (pixel) *pixel = BlendColor(*pixel, color);
    }
};

#endif