#ifndef MathLib_Renderer_H
#define MathLib_Renderer_H
#include "Font.hpp"
#include "Event.hpp"
#include "Color.hpp"
#include "String.hpp"
#include "Threads.hpp"
#include "Math/Quaternion.hpp"
#include "Geometry/LineShape.hpp"
#include "Image/SaveableImage.hpp"
#include "Interfaces/Function.hpp"
#include "Math/ComplexPosition.hpp"

namespace MathLib {
    struct Renderer : SaveableImage {
        /// @brief Creates a new renderer
        /// @param width Width of the window
        /// @param height Height of the window
        Renderer(size_t width, size_t height);
        /// @brief Destroys renderer
        virtual ~Renderer(void) override;
        /// @brief Updates renderer
        /// @return Status
        [[nodiscard]] virtual bool Update(void) = 0;
        /// @brief Gets current event
        /// @return Event
        [[nodiscard]] virtual Event GetEvent(void) = 0;
        /// @brief Waits for current event
        /// @return Event
        [[nodiscard]] Event WaitForEvent(void);
        /// @brief Renders pixel
        /// @tparam T Type of number
        /// @param pixel Pixel to render
        /// @param color Color of the pixel
        template <typename T>
        void SetPixel(const Matrix<T>& pixel, uint32_t color) {
            StartBenchmark
            SetPixelInternal<T>(ProjectVector<T>(pixel - ConvertMatrix<num_t, T>(position)) * pointMultiplier, color);
            EndBenchmark
        }
        /// @brief Returns pixel
        /// @tparam T Type of number
        /// @param pixel Position of the pixel
        /// @return Color of the pixel
        template <typename T>
        [[nodiscard]] uint32_t GetPixel(const Matrix<T>& pixel) {
            StartBenchmark
            const uint32_t* color = GetPixelInternal<T>(ProjectVector<T>(pixel - ConvertMatrix<num_t, T>(position)) * pointMultiplier);
            ReturnFromBenchmark(color ? *color : 0);
        }
        /// @brief Copies pixels from renderer to this renderer
        /// @tparam T Type of number
        /// @param renderer Renderer to copy pixels from
        /// @param rotation Vector containing axis to rotate around
        /// @return Status
        template <typename T>
        [[nodiscard]] bool DrawImage(Renderer& renderer, const Matrix<T>& rotation) {
            StartBenchmark
            if (pointMultiplier != renderer.pointMultiplier) ReturnFromBenchmark(false);
            const T div = 1 / pointMultiplier;
            const T w = renderer.GetWidth() * div / 2;
            const T h = renderer.GetHeight() * div / 2;
            for (T y = -h; y <= h; y += div) {
                for (T x = -w; x <= w; x += div) {
                    const Matrix<T> pos = CreateVector<T>(x + GetX(renderer.position), y + GetY(renderer.position), GetZ(renderer.position));
                    SetPixel<T>(RotateVector<T>(pos, renderer.position, rotation), renderer.GetPixel<T>(pos));
                }
            }
            ReturnFromBenchmark(true);
        }
        template <typename T>
        void DrawImage(const Image& image, const Matrix<T>& pos) {
            StartBenchmark
            Matrix<ssize_t> tmp = ConvertMatrix<T, ssize_t>(PositionToIndex<T>(pos - ConvertMatrix<num_t, T>(position)));
            GetX(tmp) -= image.GetWidth() / 2;
            GetY(tmp) -= image.GetHeight() / 2;
            for (size_t y = 0; y < image.GetHeight(); y++) {
                const ssize_t wy = GetY(tmp) + y;
                if (wy < 0) continue;
                else if ((size_t)wy >= GetHeight()) break;
                for (size_t x = 0; x < image.GetWidth(); x++) {
                    const ssize_t wx = GetX(tmp) + x;
                    if (wx < 0) continue;
                    else if ((size_t)wx >= GetWidth()) break;
                    else At(wx, wy) = image.At(x, y);
                }
            }
            EndBenchmark
        }
        /// @brief Draws a line
        /// @tparam T Type of number
        /// @param line Line to draw
        /// @param color Color of the line
        template <typename T>
        void DrawLine(const Line<T>& line, uint32_t color) {
            StartBenchmark
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
            EndBenchmark
        }
        /// @brief Renders specified shape
        /// @tparam T Type of number
        /// @param shape Shape to draw
        /// @param rotation Vector containing axis to rotate around
        /// @param color Color of the shape
        template <typename T>
        void DrawShape(const LineShape<T>& shape, const Matrix<T>& rotation, uint32_t color) {
            StartBenchmark
            const Array<Line<T>> lines = shape.ToLines(rotation);
            for (size_t i = 0; i < lines.GetSize(); i++) DrawLine<T>(Line<T>(lines.At(i).start, lines.At(i).end), color);
            EndBenchmark
        }
        /// @brief Renders 2D circle
        /// @tparam T Type of number
        /// @param position Position of the circle
        /// @param radius Radius of the circle
        /// @param color Color of the circle
        template <typename T>
        void DrawCircle2D(const Matrix<T>& position, const T& radius, uint32_t color) {
            StartBenchmark
            const T div = 1 / pointMultiplier;
            const T radiusSquared = radius * radius;
            for (T y = -radius; y <= radius; y += div) {
                for (T x = -radius; x <= radius; x += div) {
                    const Matrix<T> offset = CreateVector<T>(x, y, 0);
                    if (FloatsEqual<T>(offset.GetLengthSquared(), radiusSquared, div)) SetPixel<T>(position + offset, color);
                }
            }
            EndBenchmark
        }
        /// @brief Renders filled 2D circle
        /// @tparam T Type of number
        /// @param position Position of the circle
        /// @param radius Radius of the circle
        /// @param color Color of the circle
        template <typename T>
        void FillCircle2D(const Matrix<T>& position, const T& radius, uint32_t color) {
            StartBenchmark
            const T div = 1 / pointMultiplier;
            const T radiusSquared = radius * radius;
            for (T y = -radius; y <= radius; y += div) {
                for (T x = -radius; x <= radius; x += div) {
                    const Matrix<T> offset = CreateVector<T>(x, y, 0);
                    if (offset.GetLengthSquared() <= radiusSquared) SetPixel<T>(position + offset, color);
                }
            }
            EndBenchmark
        }
        /// @brief Renders strings
        /// @tparam T Type of number
        /// @param strs Strings to render
        /// @param font Font to renderer strings with
        /// @param pos Position of the strings
        /// @param fgColor Foreground color to render strings with
        /// @param bgColor Background color to render strings with
        /// @return Status
        template <typename T>
        [[nodiscard]] bool Puts(const String& str, const PSF1* font, Matrix<T> pos, uint32_t fgColor, uint32_t bgColor) {
            StartBenchmark
            if (!font || !font->IsValid()) ReturnFromBenchmark(false);
            const Array<String> strs = Split(str, "\n", false);
            const T w = font->GetWidth() / 2;
            const T h = font->GetHeight() / 2;
            const size_t bytesPerGlyph = (size_t)(w * 2) / 8 + !!((size_t)(w * 2) % 8);
            const T div = 1 / pointMultiplier;
            GetY(pos) -= strs.GetSize() * h * div;
            Matrix<T> tmp = pos;
            for (const String& s : strs) {
                pos = tmp;
                GetX(pos) -= s.GetSize() * w * div;
                for (const char& chr : s) {
                    const uint8_t* fontPtr = font->GetGlyph(chr);
                    for (T y = -h; y < h; y++) {
                        for (T x = -w; x < w; x++)
                            SetPixel<T>(CreateVector<T>(x * div, -y * div, 0) + pos, (fontPtr[(size_t)(x + w) / 8] & (1 << (7 - (size_t)(x + w)))) ? fgColor : bgColor);
                        fontPtr += bytesPerGlyph;
                    }
                    GetX(pos) += w * 2 * div;
                }
                GetY(tmp) -= h * 2 * div;
            }
            ReturnFromBenchmark(true);
        }
        /// @brief Renders strings
        /// @tparam T Type of number
        /// @param strs Strings to render
        /// @param font Font to renderer strings with
        /// @param pos Position of the strings
        /// @param rotation Vector containing axis to rotate around
        /// @param scale Scale of the strings
        /// @param fgColor Foreground color to render strings with
        /// @param bgColor Background color to render strings with
        /// @return Status
        template <typename T>
        [[nodiscard]] bool Puts(const String& str, const PSF1* font, Matrix<T> pos, const Matrix<T>& rotation, const Matrix<size_t>& scale, uint32_t fgColor, uint32_t bgColor) {
            StartBenchmark
            if (!font || !font->IsValid()) ReturnFromBenchmark(false);
            const Array<String> strs = Split(str, "\n", false);
            const T w = font->GetWidth() / 2;
            const T h = font->GetHeight() / 2;
            const size_t bytesPerGlyph = (size_t)(w * 2) / 8 + !!((size_t)(w * 2) % 8);
            const size_t sx = GetX(scale);
            const size_t sy = GetY(scale);
            const size_t sz = GetZ(scale);
            const T div = 1 / pointMultiplier;
            GetY(pos) -= strs.GetSize() * h * sy * div;
            Matrix<T> tmp = pos;
            for (const String& s : strs) {
                pos = tmp;
                GetX(pos) -= s.GetSize() * w * sx * div;
                const Matrix<T> center = pos;
                for (const char& chr : s) {
                    const uint8_t* fontPtr = font->GetGlyph(chr);
                    for (T y = -h; y < h; y++) {
                        for (T x = -w; x < w; x++)
                            for (size_t nz = 0; nz < sz; nz++)
                                for (size_t ny = 0; ny < sy; ny++)
                                    for (size_t nx = 0; nx < sx; nx++)
                                        SetPixel<T>(RotateVector<T>(CreateVector<T>((nx + x) * div, (ny - y) * div, nz * div) + pos, center, rotation), (fontPtr[(size_t)(x + w) / 8] & (1 << (7 - (size_t)(x + w)))) ? fgColor : bgColor);
                        fontPtr += bytesPerGlyph;
                    }
                    GetX(pos) += w * 2 * sx * div;
                }
                GetY(tmp) -= h * 2 * sy * div;
            }
            ReturnFromBenchmark(true);
        }
        /// @brief Calculates start of the graph
        /// @tparam T Type of number
        /// @return Start of graph
        template <typename T>
        [[nodiscard]] Matrix<T> GetStart(void) const {
            StartAndReturnFromBenchmark(-GetEnd<T>());
        }
        /// @brief Calculates end of the graph
        /// @tparam T Type of number
        /// @return End of graph
        template <typename T>
        [[nodiscard]] Matrix<T> GetEnd(void) const {
            StartAndReturnFromBenchmark(CreateVector<T>(pixels.GetWidth() / (pointMultiplier * 2), pixels.GetHeight() / (pointMultiplier * 2), 0));
        }
        template <typename T>
        [[nodiscard]] Interval<T> GetXInterval(void) const {
            const T tmp = pixels.GetWidth() / (pointMultiplier * 2);
            return Interval<T>(-tmp, tmp);
        }
        template <typename T>
        [[nodiscard]] Interval<T> GetYInterval(void) const {
            const T tmp = pixels.GetHeight() / (pointMultiplier * 2);
            return Interval<T>(-tmp, tmp);
        }
        /// @brief Draw x and y axis
        /// @param axisColor Color of the axis
        /// @param cellColor Color of the cells
        template <typename T>
        void DrawAxis(uint32_t axisColor, uint32_t cellColor, const T& cellSize) {
            StartBenchmark
            DrawAxis<T>(axisColor, cellColor, cellSize, GetXInterval<T>(), GetYInterval<T>());
            EndBenchmark
        }
        /// @brief Draw x and y axis
        /// @param axisColor Color of the axis
        /// @param cellColor Color of the cells
        /// @param x X size
        /// @param y y Size
        template <typename T>
        void DrawAxis(uint32_t axisColor, uint32_t cellColor, const T& cellSize, const Interval<T>& x, const Interval<T>& y) {
            StartBenchmark
            const Interval<T> newX = x.Expand(cellSize);
            const Interval<T> newY = y.Expand(cellSize);
            for (T i = newY.GetMin(); i <= newY.GetMax(); i += cellSize)
                DrawLine<T>(Line<T>(CreateVector<T>(newX.GetMin(), i, 0) + position, CreateVector<T>(newX.GetMax(), i, 0) + position), cellColor);
            for (T i = newX.GetMin(); i <= newX.GetMax(); i += cellSize)
                DrawLine<T>(Line<T>(CreateVector<T>(i, newY.GetMin(), 0) + position, CreateVector<T>(i, newY.GetMax(), 0) + position), cellColor);
            DrawLine<T>(Line<T>(CreateVector<T>(x.GetMin() + GetX(position), 0, 0), CreateVector<T>(x.GetMax() + GetX(position), 0, 0)), axisColor);
            DrawLine<T>(Line<T>(CreateVector<T>(0, y.GetMin() + GetY(position), 0), CreateVector<T>(0, y.GetMax() + GetY(position), 0)), axisColor);
            EndBenchmark
        }
        /// @brief Draws complex function based on its values
        /// @tparam T Type of number
        /// @param values Values generated by function
        template <typename T>
        void DrawComplexFunction(const Collection<ComplexPosition<T>>& values) {
            StartBenchmark
            for (const ComplexPosition<T>& x : values) SetPixel<T>(x.GetPosition(), x.GetColor());
            EndBenchmark
        }
        /// @brief Draws function based on its values
        /// @tparam T Type of number
        /// @param values Values generated by function
        /// @param color Color of function
        template <typename T>
        void DrawFunction(const Collection<Line<T>>& values, uint32_t color) {
            StartBenchmark
            for (const Line<T>& x : values) {
                if (!fillGapsInFunctions) SetPixel<T>(x.end, color);
                else if (!IsNaN(GetX(x.start))) DrawLine<T>(Line<T>(x.start, x.end), color);
            }
            EndBenchmark
        }
        /// @brief Draws function based on its values
        /// @tparam T Type of number
        /// @param values Values generated by function
        /// @param color Color of function
        /// @param x X size
        /// @param y Y size
        /// @param z Z size
        template <typename T>
        void DrawFunction(const Collection<Line<T>>& values, uint32_t color, const Interval<T>& x, const Interval<T>& y, const Interval<T>& z) {
            StartBenchmark
            const Matrix<Interval<T>> interval = MathLib::CreateVector<Interval<T>>(x, y, z);
            for (const Line<T>& x : values) {
                if (x.end.IsInside(interval)) {
                    if (!fillGapsInFunctions) SetPixel<T>(x.end, color);
                    else if (x.start.IsInside(interval)) DrawLine<T>(Line<T>(x.start, x.end), color);
                }
            }
            EndBenchmark
        }
        /// @brief f(x + y * i)
        /// @tparam T Type of number
        /// @param f Complex function to calculate color of the graph
        /// @return Result of complex function
        template <typename T>
        [[nodiscard]] Array<ComplexPosition<T>> GenerateComplexFunction(const Function<Complex<T>, Complex<T>>& f) {
            // TODO: Make this multithreaded
            StartBenchmark
            Array<ComplexPosition<T>> ret;
            const Matrix<ssize_t> start = ConvertMatrix<T, ssize_t>((GetStart<T>() + ConvertMatrix<num_t, T>(position)) * pointMultiplier);
            const Matrix<ssize_t> end = ConvertMatrix<T, ssize_t>((GetEnd<T>() + ConvertMatrix<num_t, T>(position)) * pointMultiplier);
            const T div = 1 / pointMultiplier;
            for (ssize_t i = GetY(start); i <= GetY(end); i++) {
                for (ssize_t r = GetX(start); r <= GetX(end); r++) {
                    const Complex<T> pos = Complex<T>(r * div, i * div);
                    const Complex<T> val = f(pos);
                    if (!(IsNaN(val) || IsInf(val) || IsNaN(pos) || IsInf(pos)))
                        if (!ret.Add(ComplexPosition<T>(pos, val))) ReturnFromBenchmark(Array<ComplexPosition<T>>());
                }
            }
            ReturnFromBenchmark(ret);
        }
        /// @brief y = f(x)
        /// @tparam T Type of number
        /// @param f Function that calculates x or y depending on the 'axis' parameter
        /// @param inAxis Axis we are using for input values
        /// @param outAxis Axis we are using for output values
        /// @return Result of function
        template <typename T>
        [[nodiscard]] Array<Line<T>> GenerateMultiFunction(const Function<Array<T>, T>& f, VectorAxis inAxis = VectorAxis::X, VectorAxis outAxis = VectorAxis::Y) {
            StartBenchmark
            Array<Matrix<T>> prev;
            Array<Line<T>> ret;
            const T start = GetVectorAxis(GetStart<T>(), inAxis) + (T)GetX(position);
            const T end = GetVectorAxis(GetEnd<T>(), inAxis) + (T)GetX(position);
            for (T i = start; i <= end; i += 1 / pointMultiplier) {
                Matrix<T> curr = CreateVector<T>(0, 0, 0);
                const Array<T> arr = f(i);
                if (prev.IsEmpty()) {
                    prev = Array<Matrix<T>>(arr.GetSize());
                    for (Matrix<T>& x : prev) x = CreateVector<T>(nan, nan, nan);
                }
                for (T j = 0; j < arr.GetSize(); j++) {
                    const T tmp = arr.At(j);
                    if (IsNaN(i) || IsInf(i) || IsNaN(tmp) || IsInf(tmp)) {
                        prev.At(j) = CreateVector<num_t>(nan, nan, nan);
                        continue;
                    }
                    GetVectorAxis(curr, inAxis) = i;
                    GetVectorAxis(curr, outAxis) = tmp;
                    if (!ret.Add(Line<T>(prev.At(j), curr))) ReturnFromBenchmark(Array<Line<T>>());
                    prev.At(j) = curr;
                }
            }
            ReturnFromBenchmark(ret);
        }
        /// @brief Converts index to position
        /// @tparam T Type of number
        /// @param index Index to convert
        /// @return Position
        template <typename T>
        [[nodiscard]] Matrix<T> IndexToPosition(const Matrix<T>& index) const {
            StartAndReturnFromBenchmark(CreateVector<T>((GetX(index) - GetWidth() / 2) / pointMultiplier, (GetHeight() / 2 - GetY(index)) / pointMultiplier, 0));
        }
        /// @brief Converts position to index
        /// @tparam T Type of number
        /// @param pos Position to convert
        /// @return Index
        template <typename T>
        [[nodiscard]] Matrix<T> PositionToIndex(const Matrix<T>& pos) const {
            StartAndReturnFromBenchmark(PositionToIndexInternal(pos * pointMultiplier));
        }
        /// @brief Sets image loading/saving interface
        /// @tparam T Type of image interface
        /// @tparam ...Args Type of arguments for image constructor
        /// @param ...args Arguments for image constructor
        /// @return Status
        template <typename T, typename... Args>
        [[nodiscard]] bool SetImage(Args... args) {
            StartBenchmark
            T* tmp = new T(0, 0, args...);
            if (!tmp) ReturnFromBenchmark(false);
            if (image) delete image;
            image = tmp;
            ReturnFromBenchmark(true);
        }
        /// @brief Saves data as image
        /// @param file File to save data into
        /// @return Status
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        /// @brief Loads data from image
        /// @param file File to load data from
        /// @return Status
        [[nodiscard]] virtual bool Load(Readable& file) override;

        /// @brief Current position
        matrix_t position;
        /// @brief Scale
        num_t pointMultiplier;
        bool fillGapsInFunctions;

        private:
        /// @brief Image for saving and loading
        SaveableImage* image;

        /// @brief Converts position to index (assumes pos is multiplied by pointMultiplier)
        /// @tparam T Type of number
        /// @param pos Position to convert
        /// @return Index
        template <typename T>
        [[nodiscard]] Matrix<T> PositionToIndexInternal(const Matrix<T>& pos) const {
            StartAndReturnFromBenchmark(CreateVector<T>(GetWidth() / 2 + GetX(pos), GetHeight() / 2 - GetY(pos), 0));
        }
        /// @brief Returns pixel from index
        /// @tparam T Type of number
        /// @param pos Index of pixel
        /// @return Pixel
        template <typename T>
        [[nodiscard]] uint32_t* GetPixelInternal(const Matrix<T>& pos) {
            StartBenchmark
            const Matrix<T> tmp = PositionToIndexInternal<T>(pos);
            ReturnFromBenchmark((IsBetween(GetX(tmp), 0, (ssize_t)GetWidth() - 1) && IsBetween(GetY(tmp), 0, (ssize_t)GetHeight() - 1)) ? &At(GetX(tmp), GetY(tmp)) : nullptr);
        }
        /// @brief Sets pixel from index
        /// @tparam T Type of number
        /// @param pos Index of pixel
        /// @param color Value to set
        template <typename T>
        void SetPixelInternal(const Matrix<T>& pos, uint32_t color) {
            StartBenchmark
            uint32_t* pixel = GetPixelInternal<T>(pos);
            if (pixel) *pixel = BlendColor(*pixel, color);
            EndBenchmark
        }
    };
}

#endif