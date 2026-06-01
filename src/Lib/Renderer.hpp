#ifndef MathLib_Renderer_H
#define MathLib_Renderer_H
#include "Event.hpp"
#include "Color.hpp"
#include "String.hpp"
#include "Threads.hpp"
#include "FunctionT.hpp"
#include "Font/Font.hpp"
#include "Image/Video.hpp"
#include "Math/Quaternion.hpp"
#include "Geometry/LineShape.hpp"
#include "Image/SaveableImage.hpp"
#include "Math/ComplexPosition.hpp"

namespace MathLib {
    struct Renderer : SaveableImage {
        Renderer(size_t width, size_t height);
        virtual ~Renderer(void) override;
        [[nodiscard]] virtual bool Update(void) = 0;
        [[nodiscard]] virtual Event GetEvent(void) = 0;
        [[nodiscard]] Event WaitForEvent(void);
        template <typename T>
        void SetPixel(const Matrix<T>& pixel, uint32_t color) {
            const Matrix<T> tmp = ProjectVector<T>(pixel - ConvertMatrix<num_t, T>(position));
            SetPixelInternal(GetX(tmp) * pointMultiplier, GetY(tmp) * pointMultiplier, color);
        }
        template <typename T>
        [[nodiscard]] uint32_t GetPixel(const Matrix<T>& pixel) {
            const Matrix<T> tmp = ProjectVector<T>(pixel - ConvertMatrix<num_t, T>(position));
            const uint32_t* color = GetPixelInternal(GetX(tmp) * pointMultiplier, GetY(tmp) * pointMultiplier);
            return color ? *color : 0;
        }
        template <typename T>
        [[nodiscard]] bool DrawImage(Renderer& renderer, const Matrix<T>& rotation) {
            if (pointMultiplier != renderer.pointMultiplier) return false;
            const T div = 1 / pointMultiplier;
            const T w = renderer.GetWidth() * div / 2;
            const T h = renderer.GetHeight() * div / 2;
            for (T y = -h; y <= h; y += div) {
                for (T x = -w; x <= w; x += div) {
                    const Matrix<T> pos = CreateVector<T>(x + GetX(renderer.position), y + GetY(renderer.position), GetZ(renderer.position));
                    SetPixel<T>(RotateVector<T>(pos, renderer.position, rotation), renderer.GetPixel<T>(pos));
                }
            }
            return true;
        }
        template <typename T>
        void DrawImage(const Image& image, const Matrix<T>& pos) {
            const size_t width = image.GetWidth();
            const size_t height = image.GetHeight();
            const Matrix<T> tmp1 = pos - ConvertMatrix<num_t, T>(position);
            const Matrix<ssize_t> tmp2 = PositionToIndexInternal(GetX(tmp1) * pointMultiplier, GetY(tmp1) * pointMultiplier);
            const ssize_t tmpX = GetX(tmp2) - width / 2;
            const ssize_t tmpY = GetY(tmp2) - height / 2;
            const ssize_t maxX = Min<ssize_t>(GetWidth() - tmpX, width);
            const ssize_t maxY = Min<ssize_t>(GetHeight() - tmpY, height);
            const size_t minX = Max<ssize_t>(-tmpY, 0);
            const size_t minY = Max<ssize_t>(-tmpX, 0);
            for (ssize_t y = minY; y < maxY; y++) {
                const ssize_t wy = tmpY + y;
                for (ssize_t x = minX; x < maxX; x++) {
                    const ssize_t wx = tmpX + x;
                    AtUnsafe(wx, wy) = BlendColor(AtUnsafe(wx, wy), image.AtUnsafe(x, y), alphaPosition);
                }
            }
        }
        void DrawImage(const Image& image, ssize_t centerX, ssize_t centerY);
        [[nodiscard]] bool DrawPartialImage(const Image& image, size_t startX, size_t endX, size_t startY, size_t endY, ssize_t centerX, ssize_t centerY);
        void DrawStackedImage(const Video& images, ssize_t centerX, ssize_t centerY, ssize_t diff);
        void FillRectangle(ssize_t centerX, ssize_t centerY, size_t width, size_t height, uint32_t color);
        void DrawRectangle(ssize_t centerX, ssize_t centerY, size_t width, size_t height, uint32_t color);
        void DrawLinePararellToOX(ssize_t startX, ssize_t endX, ssize_t y, uint32_t color);
        void DrawLinePararellToOY(ssize_t startY, ssize_t endY, ssize_t x, uint32_t color);
        void DrawGrid(ssize_t centerX, ssize_t centerY, size_t width, size_t height, size_t cellsX, size_t cellsY, uint32_t color);
        template <typename T>
        void DrawLine(const Line<T>& line, uint32_t color) {
            const size_t width = GetWidth();
            const size_t height = GetHeight();
            const size_t width2 = width / 2;
            const size_t height2 = height / 2;
            const Matrix<ssize_t> startVector = ConvertMatrix<T, ssize_t>(ProjectVector<T>(line.start - ConvertMatrix<num_t, T>(position)) * pointMultiplier);
            const Matrix<ssize_t> endVector = ConvertMatrix<T, ssize_t>(ProjectVector<T>(line.end - ConvertMatrix<num_t, T>(position)) * pointMultiplier);
            bool steep = false;
            ssize_t x0 = GetX(startVector);
            ssize_t x1 = GetX(endVector);
            ssize_t y0 = GetY(startVector);
            ssize_t y1 = GetY(endVector);
            if (Abs(x0 - x1) < Abs(y0 - y1)) {
                Swap<ssize_t>(x0, y0);
                Swap<ssize_t>(x1, y1);
                steep = true;
            }
            if (x0 > x1) {
                Swap<ssize_t>(x0, x1);
                Swap<ssize_t>(y0, y1);
            }
            const size_t dx = x1 - x0;
            const size_t dx2 = dx * 2;
            const size_t derr = Abs(y1 - y0) * 2;
            ssize_t err = 0;
            ssize_t y = y0;
            if (steep) {
                y += width2;
                const int8_t dy = (y1 > y0) * 2 - 1;
                const ssize_t max = Min<ssize_t>(x1, height2);
                for (ssize_t x = x0; x <= max; ++x) {
                    const ssize_t wy = height2 - x;
                    if (y >= 0 && (size_t)y < width && (size_t)wy < height) AtUnsafe(y, wy) = BlendColor(AtUnsafe(y, wy), color, alphaPosition);
                    err += derr;
                    if (err > (ssize_t)dx) {
                        y += dy;
                        err -= dx2;
                    }
                }
            }
            else {
                y = height2 - y;
                const int8_t dy = (y1 <= y0) * 2 - 1;
                const ssize_t max = Min<ssize_t>(x1 + 1, width2) + width2;
                for (ssize_t x = x0 + width2; x < max; ++x) {
                    if (x >= 0 && y >= 0 && (size_t)y < height) AtUnsafe(x, y) = BlendColor(AtUnsafe(x, y), color, alphaPosition);
                    err += derr;
                    if (err > (ssize_t)dx) {
                        y += dy;
                        err -= dx2;
                    }
                }
            }
        }
        template <typename T>
        void DrawShape(const LineShape<T>& shape, const Matrix<T>& rotation, uint32_t color) {
            const Array<Line<T>> lines = shape.ToLines(rotation);
            for (size_t i = 0; i < lines.GetSize(); i++) DrawLine<T>(Line<T>(lines.At(i).start, lines.At(i).end), color);
        }
        template <typename T>
        void DrawCircle2D(const Matrix<T>& position, const T& radius, uint32_t color) {
            const T div = 1 / pointMultiplier;
            const T radiusSquared = radius * radius;
            for (T y = -radius; y <= radius; y += div) {
                for (T x = -radius; x <= radius; x += div) {
                    const Matrix<T> offset = CreateVector<T>(x, y, 0);
                    if (FloatsEqual<T>(offset.GetLengthSquared(), radiusSquared, div)) SetPixel<T>(position + offset, color);
                }
            }
        }
        template <typename T>
        void FillCircle2D(const Matrix<T>& position, const T& radius, uint32_t color) {
            const Matrix<T> tmp = ProjectVector<T>(position - ConvertMatrix<num_t, T>(this->position));
            const ssize_t posX = GetX(tmp) * pointMultiplier;
            const ssize_t posY = GetY(tmp) * pointMultiplier;
            const ssize_t r = radius * pointMultiplier;
            const size_t radiusSquared = r * r;
            for (ssize_t y = -r; y <= r; y++) {
                const ssize_t absX = Sqrt(radiusSquared - y * y);
                for (ssize_t x = -absX; x <= absX; x++) SetPixelInternal(posX + x, posY + y, color);
            }
        }
        void Putc(char chr, const Font& font, ssize_t centerX, ssize_t centerY, uint32_t fgColor, uint32_t bgColor);
        template <typename T>
        void Puts(const Sequence<char>& str, const Font& font, Matrix<T> pos, uint32_t fgColor, uint32_t bgColor) {
            const Array<String> strs = Split(str, '\n'_M, false);
            const ssize_t w = font.GetWidth() / 2;
            const ssize_t h = font.GetHeight() / 2;
            const size_t bytesPerGlyph = (w * 2 + 7) / 8;
            const T div = 1 / pointMultiplier;
            GetY(pos) -= strs.GetSize() * h * div;
            Matrix<T> tmp = pos;
            for (const Sequence<char>& s : strs) {
                pos = tmp;
                GetX(pos) -= s.GetSize() * w * div;
                s.Foreach(MakeFunctionT<void, char>([this, &font, &pos, fgColor, bgColor, div, w, h, bytesPerGlyph](char chr) -> void {
                    const uint8_t* fontPtr = font.GetGlyph(chr);
                    for (ssize_t y = -h; y < h; y++) {
                        for (ssize_t x = -w; x < w; x++)
                            SetPixel<T>(CreateVector<T>(div * x, div * -y, 0) + pos, (fontPtr[(x + w) / 8] & (1 << (7 - (x + w) % 8))) ? fgColor : bgColor);
                        fontPtr += bytesPerGlyph;
                    }
                    GetX(pos) += w * 2 * div;
                }));
                GetY(tmp) -= h * 2 * div;
            }
        }
        template <typename T>
        void Puts(const String& str, const Font& font, Matrix<T> pos, const Matrix<T>& rotation, const Matrix<size_t>& scale, uint32_t fgColor, uint32_t bgColor) {
            const Array<String> strs = Split(str, '\n'_M, false);
            const ssize_t w = font.GetWidth() / 2;
            const ssize_t h = font.GetHeight() / 2;
            const size_t bytesPerGlyph = (w * 2 + 7) / 8;
            const size_t sx = GetX(scale);
            const size_t sy = GetY(scale);
            const size_t sz = GetZ(scale);
            const T div = 1 / pointMultiplier;
            GetY(pos) -= strs.GetSize() * h * sy * div;
            Matrix<T> tmp = pos;
            for (const Sequence<char>& s : strs) {
                pos = tmp;
                GetX(pos) -= s.GetSize() * w * sx * div;
                const Matrix<T> center = pos;
                s.Foreach(MakeFunctionT<void, char>([this, &font, &pos, fgColor, bgColor, div, w, h, sx, sy, sz, center, rotation, bytesPerGlyph](char chr) -> void {
                    const uint8_t* fontPtr = font.GetGlyph(chr);
                    for (ssize_t y = -h; y < h; y++) {
                        for (ssize_t x = -w; x < w; x++)
                            for (size_t nz = 0; nz < sz; nz++)
                                for (size_t ny = 0; ny < sy; ny++)
                                    for (size_t nx = 0; nx < sx; nx++)
                                        SetPixel<T>(RotateVector<T>(CreateVector<T>(div * (nx + x), div * (ny - y), div * nz) + pos, center, rotation), (fontPtr[(x + w) / 8] & (1 << (7 - (x + w) % 8))) ? fgColor : bgColor);
                        fontPtr += bytesPerGlyph;
                    }
                    GetX(pos) += w * 2 * sx * div;
                }));
                GetY(tmp) -= h * 2 * sy * div;
            }
        }
        template <typename T>
        [[nodiscard]] Matrix<T> GetStart(void) const {
            return -GetEnd<T>();
        }
        template <typename T>
        [[nodiscard]] Matrix<T> GetEnd(void) const {
            return CreateVector<T>(pixels.GetWidth() / (pointMultiplier * 2), pixels.GetHeight() / (pointMultiplier * 2), 0);
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
        template <typename T>
        void DrawAxis(uint32_t axisColor, uint32_t cellColor, const T& cellSize) {
            DrawAxis<T>(axisColor, cellColor, cellSize, GetXInterval<T>(), GetYInterval<T>());
        }
        template <typename T>
        void DrawAxis(uint32_t axisColor, uint32_t cellColor, const T& cellSize, const Interval<T>& x, const Interval<T>& y) {
            const Interval<T> newX = x.Expand(cellSize);
            const Interval<T> newY = y.Expand(cellSize);
            for (T i = newY.GetMin(); i <= newY.GetMax(); i += cellSize)
                DrawLine<T>(Line<T>(CreateVector<T>(newX.GetMin(), i, 0) + position, CreateVector<T>(newX.GetMax(), i, 0) + position), cellColor);
            for (T i = newX.GetMin(); i <= newX.GetMax(); i += cellSize)
                DrawLine<T>(Line<T>(CreateVector<T>(i, newY.GetMin(), 0) + position, CreateVector<T>(i, newY.GetMax(), 0) + position), cellColor);
            DrawLine<T>(Line<T>(CreateVector<T>(x.GetMin() + GetX(position), 0, GetZ(position)), CreateVector<T>(x.GetMax() + GetX(position), 0, GetZ(position))), axisColor);
            DrawLine<T>(Line<T>(CreateVector<T>(0, y.GetMin() + GetY(position), GetZ(position)), CreateVector<T>(0, y.GetMax() + GetY(position), GetZ(position))), axisColor);
        }
        template <typename T>
        void DrawComplexFunction(const Sequence<ComplexPosition<T>>& values) {
            values.Foreach(MakeFunctionT<void, ComplexPosition<T>>([this](ComplexPosition<T> x) -> void {
                SetPixel<T>(x.GetPosition(), x.GetColor());
            }));
        }
        template <typename T>
        void DrawFunction(const Sequence<Line<T>>& values, uint32_t color) {
            values.Foreach(MakeFunctionT<void, Line<T>>([this, color](Line<T> x) -> void {
                if (!fillGapsInFunctions) SetPixel<T>(x.end, color);
                else if (!IsNaN(GetX(x.start))) DrawLine<T>(Line<T>(x.start, x.end), color);
            }));
        }
        template <typename T>
        void DrawFunction(const Sequence<Line<T>>& values, uint32_t color, const Interval<T>& x, const Interval<T>& y, const Interval<T>& z) {
            const Matrix<Interval<T>> interval = CreateVector<Interval<T>>(x, y, z);
            values.Foreach(MakeFunctionT<void, Line<T>>([this, color, interval](Line<T> x) -> void {
                if (x.end.IsInside(interval)) {
                    if (!fillGapsInFunctions) SetPixel<T>(x.end, color);
                    else if (x.start.IsInside(interval)) DrawLine<T>(Line<T>(x.start, x.end), color);
                }
            }));
        }
        template <typename T>
        [[nodiscard]] Array<ComplexPosition<T>> GenerateComplexFunction(const Function<Complex<T>, Complex<T>>& f) {
            Array<ComplexPosition<T>> ret;
            const Matrix<ssize_t> start = ConvertMatrix<T, ssize_t>((GetStart<T>() + ConvertMatrix<num_t, T>(position)) * pointMultiplier);
            const Matrix<ssize_t> end = ConvertMatrix<T, ssize_t>((GetEnd<T>() + ConvertMatrix<num_t, T>(position)) * pointMultiplier);
            const T div = 1 / pointMultiplier;
            for (ssize_t i = GetY(start); i <= GetY(end); i++) {
                for (ssize_t r = GetX(start); r <= GetX(end); r++) {
                    const Complex<T> pos = Complex<T>(r * div, i * div);
                    const Complex<T> val = f(pos);
                    if (!(IsNaN(val) || IsInf(val) || IsNaN(pos) || IsInf(pos)))
                        if (!ret.Add(ComplexPosition<T>(pos, val))) return Array<ComplexPosition<T>>();
                }
            }
            return ret;
        }
        template <typename T>
        [[nodiscard]] Array<Line<T>> GenerateMultiFunction(const Function<Array<T>, T>& f, VectorAxis inAxis = VectorAxis::X, VectorAxis outAxis = VectorAxis::Y) {
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
                    if (!ret.Add(Line<T>(prev.At(j), curr))) return Array<Line<T>>();
                    prev.At(j) = curr;
                }
            }
            return ret;
        }
        template <typename T>
        [[nodiscard]] Matrix<T> IndexToPosition(ssize_t x, ssize_t y) const {
            return CreateVector<T>(((T)x - GetWidth() / 2) / pointMultiplier, (GetHeight() / 2 - (T)y) / pointMultiplier, 0);
        }
        template <typename T, typename... Args>
        [[nodiscard]] bool SetImage(Args... args) {
            T* tmp = new T(0, 0, args...);
            if (!tmp) return false;
            if (image) delete image;
            image = tmp;
            return true;
        }
        [[nodiscard]] virtual bool Save(Writable& file) const override;
        [[nodiscard]] virtual bool Load(Readable& file) override;

        matrix_t position;
        num_t pointMultiplier;
        bool fillGapsInFunctions;
        uint8_t alphaPosition;

        private:
        SaveableImage* image;

        void DrawImage(const Image& image, ssize_t centerX, ssize_t centerY, size_t width, size_t height, size_t startX, size_t startY);
        [[nodiscard]] Matrix<ssize_t> PositionToIndexInternal(ssize_t x, ssize_t y) const {
            return CreateVector<ssize_t>(GetWidth() / 2 + x, GetHeight() / 2 - y, 0);
        }
        [[nodiscard]] uint32_t* GetPixelInternal(ssize_t x, ssize_t y) {
            const Matrix<ssize_t> tmp = PositionToIndexInternal(x, y);
            return IsBetween(GetX(tmp), 0, (ssize_t)GetWidth() - 1) && IsBetween(GetY(tmp), 0, (ssize_t)GetHeight() - 1) ? &AtUnsafe(GetX(tmp), GetY(tmp)) : nullptr;
        }
        void SetPixelInternal(ssize_t x, ssize_t y, uint32_t color) {
            uint32_t* pixel = GetPixelInternal(x, y);
            if (pixel) *pixel = BlendColor(*pixel, color, alphaPosition);
        }
    };
}

#endif