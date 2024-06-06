#ifndef Renderer_H
#define Renderer_H
#include "Set.hpp"
#include "Font.hpp"
#include "Event.hpp"
#include "Color.hpp"
#include "Saveable.hpp"
#include "Quaternion.hpp"
#include "ComplexPosition.hpp"
#include "Geometry/LineShape.hpp"

struct Renderer : Saveable {
    /// @brief Creates a new renderer
    /// @param w Width of the window
    /// @param h Height of the window
    Renderer(size_t w, size_t h);
    /// @brief Destroys the renderer
    virtual ~Renderer(void);
    /// @brief Updates renderer
    /// @return Status
    virtual bool Update(void) = 0;
    /// @brief Gets current event
    /// @return Event
    virtual Event GetEvent(void) = 0;
    /// @brief Calculates width of the renderer window
    /// @return Width of the renderer window
    size_t GetWidth(void) const;
    /// @brief Calculates height of the renderer window
    /// @return Height of the renderer window
    size_t GetHeight(void) const;
    /// @brief Returns pixel buffer
    /// @return Framebuffer
    Matrix<uint32_t> GetPixels(void) const;
    /// @brief Waits for current event
    /// @return Event
    Event WaitForEvent(void);
    /// @brief Fills screen
    /// @param color Color to fill the screen with
    void Fill(uint32_t color);
    /// @brief Renders pixel
    /// @tparam T Type of number
    /// @param pixel Pixel to render
    /// @param color Color of the pixel
    template <typename T>
    void SetPixel(Matrix<T> pixel, uint32_t color) {
        SetPixelInternal<T>(ConvertVectorToVector2<T>(pixel - ConvertMatrix<num_t, T>(position)) * pointMultiplier, color);
    }
    /// @brief Returns pixel
    /// @tparam T Type of number
    /// @param pixel Position of the pixel
    /// @return Color of the pixel
    template <typename T>
    uint32_t GetPixel(Matrix<T> pixel) {
        uint32_t* color = GetPixelInternal<T>(ConvertVectorToVector2<T>(pixel - ConvertMatrix<num_t, T>(position)) * pointMultiplier);
        return color ? *color : 0;
    }
    /// @brief Copies pixels from renderer to this renderer
    /// @tparam T Type of number
    /// @param renderer Renderer to copy pixels from
    /// @param rotation Vector containing axis to rotate around
    /// @return Status
    template <typename T>
    bool DrawImage(const Renderer& renderer, Matrix<T> rotation) {
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
    void DrawLine(Line<T> line, uint32_t color) {
        Matrix<ssize_t> startVector = ConvertMatrix<T, ssize_t>(ConvertVectorToVector2<T>(line.start - ConvertMatrix<num_t, T>(position)) * pointMultiplier);
        Matrix<ssize_t> endVector = ConvertMatrix<T, ssize_t>(ConvertVectorToVector2<T>(line.end - ConvertMatrix<num_t, T>(position)) * pointMultiplier);
        const Matrix<ssize_t> diff = endVector - startVector;
        const ssize_t dx = GetX(diff);
        const ssize_t dy = GetY(diff);
        bool steep = false;
        ssize_t start;
        ssize_t end;
        if (dx == 0 && dy == 0) {
            SetPixelInternal<ssize_t>(startVector, color);
            return;
        }
        else if (Abs(dx) > Abs(dy)) {
            start = GetX(startVector);
            end = GetX(endVector);
        }
        else {
            steep = true;
            start = GetY(startVector);
            end = GetY(endVector);
        }
        if (start > end) {
            Swap(startVector, endVector);
            Swap(start, end);
        }
        for (ssize_t i = start; i <= end; i++)
            SetPixelInternal<ssize_t>(steep ? CreateVector<ssize_t>(GetX(diff) * (i - start) / GetY(diff) + GetX(startVector), i, 0) : CreateVector<ssize_t>(i, GetY(diff) * (i - start) / GetX(diff) + GetY(startVector), 0), color);
    }
    /// @brief Renders specified shape
    /// @tparam T Type of number
    /// @param shape Shape to draw
    /// @param rotation Vector containing axis to rotate around
    /// @param color Color of the shape
    template <typename T>
    void DrawShape(const LineShape<T>& shape, Matrix<T> rotation, uint32_t color) {
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
    bool Puts(Array<String> strs, PSF1* font, Matrix<T> pos, Matrix<T> rotation, Matrix<size_t> scale, uint32_t color) {
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
    /// @brief Creates set of every pixel on the screen
    /// @tparam T Type of number
    /// @param axis Axis we are using
    /// @return Set of every pixel on the screen
    template <typename T>
    Array<T> CreateRealNumberSet(void) const {
        const Matrix<T> startArr = GetStart<T>();
        const Matrix<T> endArr = GetEnd<T>();
        return CreateSet<T>(
            Max(GetX(startArr), GetY(startArr)),
            Max(GetX(endArr), GetY(endArr)),
            1 / pointMultiplier
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
    void DrawComplexFunction(Array<ComplexPosition<T>> values) {
        for (size_t i = 0; i < values.GetSize(); i++) SetPixel<T>(values.At(i).GetPosition(), values.At(i).GetColor());
    }
    /// @brief Draws function based on its values
    /// @tparam T Type of number
    /// @param values Values generated by function
    /// @param color Color of function
    template <typename T>
    void DrawFunction(Array<Line<T>> values, uint32_t color) {
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
    /// @param inSet Set of number values we allow as input
    /// @return Result of complex function
    template <typename T>
    Array<ComplexPosition<T>> GenerateComplexFunction(std::function<std::complex<T>(std::complex<T>)> f, Array<T> inSet) {
        Array<ComplexPosition<T>> ret;
        for (size_t i = 0; i < inSet.GetSize(); i++) {
            for (size_t r = 0; r < inSet.GetSize(); r++) {
                const std::complex<T> pos = std::complex<T>(inSet.At(r), inSet.At(i));
                const std::complex<T> val = f(pos);
                if (!(IsNaN(val) || IsInf(val) || IsNaN(pos) || IsInf(pos)))
                    if (!ret.Add(ComplexPosition<T>(pos, val))) return {};
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
    Array<Line<T>> GenerateMultiFunction(std::function<Array<T>(T)> f, Array<T> inSet, VectorAxis inAxis = VectorAxis::X, VectorAxis outAxis = VectorAxis::Y) {
        Array<Matrix<T>> prev;
        Array<Line<T>> ret;
        for (size_t i = 0; i < inSet.GetSize(); i++) {
            Matrix<T> curr = CreateVector<T>(0, 0, 0);
            const Array<T> arr = f(inSet.At(i));
            if (prev.IsEmpty())
                for (T j = 0; j < arr.GetSize(); j++)
                    if (!prev.Add(CreateVector<T>(NAN, NAN, NAN))) return Array<Line<T>>();
            for (T j = 0; j < arr.GetSize(); j++) {
                const T tmp = arr.At(j);
                if (IsNaN(inSet.At(i)) || IsInf(inSet.At(i)) || IsNaN(tmp) || IsInf(tmp)) {
                    prev.At(j) = CreateVector<num_t>(NAN, NAN, NAN);
                    continue;
                }
                GetVectorAxis(curr, inAxis) = inSet.At(i);
                GetVectorAxis(curr, outAxis) = tmp;
                ret.Add(Line<T>(prev.At(j), curr));
                prev.At(j) = curr;
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
    Array<Line<T>> GenerateFunction(std::function<T(T)> f, Array<T> inSet, VectorAxis inAxis = VectorAxis::X, VectorAxis outAxis = VectorAxis::Y) {
        return GenerateMultiFunction<T>([f](T x) -> Array<T> {
            Array<T> ret = Array<T>(1);
            ret.At(0) = f(x);
            return ret;
        }, inSet, inAxis, outAxis);
    }
    /// @brief Saves renderer data as PPM6 image
    /// @param fileSystem File system to save renderer data into
    /// @param file File to save renderer data into
    /// @return Status
    virtual bool Save(FileSystem& fileSystem, size_t file) const override;
    /// @brief Loads renderer data from PPM6 image
    /// @param fileSystem File system to load renderer data from
    /// @param file File to load renderer data from
    /// @return Status
    virtual bool Load(FileSystem& fileSystem, size_t file) override;

    /// @brief Current position
    Matrix<num_t> position;
    /// @brief Scale
    num_t pointMultiplier;

    private:
    /// @brief Returns pixel from index
    /// @tparam T Type of number
    /// @param pos Index of pixel
    /// @return Pixel
    template <typename T>
    uint32_t* GetPixelInternal(Matrix<T> pos) {
        pos = CreateVector<T>(pixels.GetWidth() / 2 + GetX(pos), pixels.GetHeight() / 2 - GetY(pos), 0);
        return (IsBetween(GetX(pos), 0, (ssize_t)pixels.GetWidth() - 1) && IsBetween(GetY(pos), 0, (ssize_t)pixels.GetHeight() - 1)) ? &pixels.At(GetX(pos), GetY(pos)) : nullptr;
    }
    /// @brief Sets pixel from index
    /// @tparam T Type of number
    /// @param pos Index of pixel
    /// @param color Value to set
    template <typename T>
    void SetPixelInternal(Matrix<T> pos, uint32_t color) {
        uint32_t* pixel = GetPixelInternal<T>(pos);
        if (pixel) *pixel = BlendColor(*pixel, color);
    }

    /// @brief Pixels
    Matrix<uint32_t> pixels;
};

#endif