#ifndef Renderer_H
#define Renderer_H
#include "Set.hpp"
#include "Vector2.hpp"
#include "Typedefs.hpp"
#include "Summation.hpp"
#include <cmath>

#define ConstFunction(T, c)                         \
[c](T) {                                            \
    return c;                                       \
}
#define IdentityFunction(T)                         \
[](T x) {                                           \
    return x;                                       \
}
#define SignumFunction(T)                           \
[](T x) {                                           \
    return x == 0 ? 0 : (x / abs(x));               \
}
#define PolynomialFunction(T, a)                    \
[a](T x) {                                          \
    return Summation<T>(0, a.size(), [a, x](T n) {  \
        return a[n] * pow(x, n);                    \
    });                                             \
}
#define RationalFunction(T, p, q)                   \
[p, q](T x) {                                       \
    return p(x) / q(x);                             \
}
#define DerivativeFunction(T, f)                    \
[f](T x) {                                          \
    const T h = 1 / pointMultiplier;                \
    return (f(x + h) - f(x)) / h;                   \
}

/// @brief Checks if T is float
/// @tparam T Type of number
/// @return Result
template <typename T>
bool IsFloat(void) {
    return typeid(T) == typeid(float) || typeid(T) == typeid(double) || typeid(T) == typeid(num_t);
}
struct Renderer {
    /// @brief Destroys the renderer
    virtual ~Renderer(void) {}
    /// @brief Flushes renderer buffer
    /// @return Status
    virtual bool Update(void) = 0;
    /// @brief Waits until the renderer is destroyed
    virtual void Quit(void) = 0;
    /// @brief Scale
    static constexpr num_t pointMultiplier = 100;

    /// @brief Calculates start of the graph
    /// @tparam T Type of number
    /// @return Start of graph
    template <typename T>
    Vector2<T> GetStart(void) {
        return CreateVector2<T>(-(GetWidth() / 2 / pointMultiplier), -(GetHeight() / 2 / pointMultiplier));
    }
    /// @brief Calculates end of the graph
    /// @tparam T Type of number
    /// @return End of graph
    template <typename T>
    Vector2<T> GetEnd(void) {
        return CreateVector2<T>(GetWidth() / 2 / pointMultiplier, GetHeight() / 2 / pointMultiplier);
    }
    /// @brief Draw x and y axis
    /// @param color Color of the axis
    /// @return Status
    bool DrawAxis(uint32_t color) {
        const num_t w = GetWidth() / 2;
        const num_t h = GetHeight() / 2;
        return DrawLine(CreateVector2<num_t>(-w, 0), CreateVector2<num_t>(w, 0), color) && DrawLine(CreateVector2<num_t>(0, -h), CreateVector2<num_t>(0, h), color);
    }
    /// @brief Creates set of every pixel on the screen
    /// @tparam T Type of number
    /// @param axis Axis we are using
    /// @return Set of every pixel on the screen
    template <typename T>
    std::vector<T> CreateRealNumberSet(void) {
        const bool isFloat = IsFloat<T>();
        const T div = isFloat ? 1 : pointMultiplier;
        Vector2<T> startArr = GetStart<T>();
        Vector2<T> endArr = GetEnd<T>();
        return CreateSet<T>(
            std::max<T>(GetVectorAxis(startArr, VectorAxis::X), GetVectorAxis(startArr, VectorAxis::Y)) * div * 2,
            std::max<T>(GetVectorAxis(endArr, VectorAxis::X), GetVectorAxis(endArr, VectorAxis::Y)) * div * 2,
            1 / (isFloat ? pointMultiplier : 1)
        );
    }
    /// @brief y = f(x) or x = f(y)
    /// @tparam T Type of number
    /// @param f Function that calculates x or y depending on the 'axis' parameter
    /// @param color Color of function
    /// @param inSet Set of number values we allow as input
    /// @param outSet Set of number values we allow as output
    /// @param axis Axis we are using
    /// @return Status
    template <typename T>
    bool DrawFunction(std::function<T(T)> f, uint32_t color, std::vector<T> inSet, std::vector<T> outSet, VectorAxis axis = VectorAxis::Y) {
        const bool isFloat = IsFloat<T>();
        const T div = isFloat ? 1 : pointMultiplier;
        Vector2<T> prev;
        for (const T& i : inSet) {
            Vector2<T> curr;
            T tmp = f(i);
            if (std::isnan(i) || std::isinf(i) || !IsInsideSet<T>(outSet, tmp)) {
                prev = CreateVector2<num_t>(NAN, NAN);
                continue;
            }
            GetVectorAxis(curr, axis) = tmp;
            GetVectorAxis(curr, (axis == VectorAxis::X) ? VectorAxis::Y : VectorAxis::X) = i;
            #ifdef FillGapsInFunctions
            if (!std::isnan(GetVectorAxis(prev, VectorAxis::X)) && !DrawLine(CreateVector2<num_t>(GetVectorAxis(prev, VectorAxis::X), GetVectorAxis(prev, VectorAxis::Y)) / div, CreateVector2<num_t>(GetVectorAxis(curr, VectorAxis::X), GetVectorAxis(curr, VectorAxis::Y)) / div, color)) return false;
            prev = curr;
            #else
            if (!SetPixel(CreateVector2<num_t>(GetVectorAxis(curr, VectorAxis::X), GetVectorAxis(curr, VectorAxis::Y)) / div, color)) return false;
            (void)prev;
            #endif
        }
        return true;
    }
    /// @brief f(x) = c
    /// @tparam T Type of number
    /// @param c Constant value
    /// @param color Color of function
    /// @param inSet Set of number values we allow as input
    /// @param outSet Set of number values we allow as output
    /// @param axis Axis we are using
    /// @return Status
    template <typename T>
    bool DrawConstFunction(T c, uint32_t color, std::vector<T> inSet, std::vector<T> outSet, VectorAxis axis = VectorAxis::Y) {
        return DrawFunction<T>(ConstFunction(T, c), color, inSet, outSet, axis);
    }
    /// @brief f(x) = x
    /// @tparam T Type of number
    /// @param color Color of function
    /// @param inSet Set of number values we allow as input
    /// @param outSet Set of number values we allow as output
    /// @param axis Axis we are using
    /// @return Status
    template <typename T>
    bool DrawIdentityFunction(uint32_t color, std::vector<T> inSet, std::vector<T> outSet, VectorAxis axis = VectorAxis::Y) {
        return DrawFunction<T>(IdentityFunction(T), color, inSet, outSet, axis);
    }
    /// @brief f(0) = 0 and f(x) = x / |x|
    /// @tparam T Type of number
    /// @param color Color of function
    /// @param inSet Set of number values we allow as input
    /// @param outSet Set of number values we allow as output
    /// @param axis Axis we are using
    /// @return Status
    template <typename T>
    bool DrawSignumFunction(uint32_t color, std::vector<T> inSet, std::vector<T> outSet, VectorAxis axis = VectorAxis::Y) {
        return DrawFunction<T>(SignumFunction(T), color, inSet, outSet, axis);
    }
    /// @brief f(x) = a_0 * x^0 + ... + a_n * x^n
    /// @tparam T Type of number
    /// @param a Array of numbers by which we multiply the power of x
    /// @param color Color of function
    /// @param inSet Set of number values we allow as input
    /// @param outSet Set of number values we allow as output
    /// @param axis Axis we are using
    /// @return Status
    template <typename T>
    bool DrawPolynomialFunction(std::vector<T> a, uint32_t color, std::vector<T> inSet, std::vector<T> outSet, VectorAxis axis = VectorAxis::Y) {
        return DrawFunction<T>(PolynomialFunction(T, a), color, inSet, outSet, axis);
    }
    /// @brief f(x) = p(x) / q(x)
    /// @tparam T Type of number
    /// @param p Function we are dividing
    /// @param q Function that is dividing
    /// @param color Color of function
    /// @param inSet Set of number values we allow as input
    /// @param outSet Set of number values we allow as output
    /// @param axis Axis we are using
    /// @return Status
    template <typename T>
    bool DrawRationalFunction(std::function<T(T)> p, std::function<T(T)> q, uint32_t color, std::vector<T> inSet, std::vector<T> outSet, VectorAxis axis = VectorAxis::Y) {
        return DrawFunction<T>(RationalFunction(T, p, q), color, inSet, outSet, axis);
    }
    /// @brief f'(x) = (f(x + h) - f(x)) / h
    /// @tparam T Type of number
    /// @param f Function we want the derivative of
    /// @param color Color of the derivative
    /// @param inSet Set of number values we allow as input
    /// @param outSet Set of number values we allow as output
    /// @param axis Axis we are using
    /// @return Status
    template<typename T>
    bool DrawDerivativeFunction(std::function<T(T)> f, uint32_t color, std::vector<T> inSet, std::vector<T> outSet, VectorAxis axis = VectorAxis::Y) {
        return DrawFunction<T>(DerivativeFunction(T, f), color, inSet, outSet, axis);
    }
    /// @brief Draws rectangle
    /// @tparam T Type of number
    /// @param pos Position of the rectangle
    /// @param width Width of the rectangle
    /// @param height Height of the rectangle
    /// @param color Color of the rectangle
    /// @return Status
    template <typename T>
    bool DrawRectangle(Vector2<T> pos, T width, T height, uint32_t color) {
        width /= 2;
        height /= 2;
        return (
            DrawLine(CreateVector2<num_t>(pos.x - width, pos.y + height), CreateVector2<num_t>(pos.x + width, pos.y + height), color) &&
            DrawLine(CreateVector2<num_t>(pos.x - width, pos.y - height), CreateVector2<num_t>(pos.x - width, pos.y + height), color) &&
            DrawLine(CreateVector2<num_t>(pos.x - width, pos.y - height), CreateVector2<num_t>(pos.x + width, pos.y - height), color) &&
            DrawLine(CreateVector2<num_t>(pos.x + width, pos.y - height), CreateVector2<num_t>(pos.x + width, pos.y + height), color)
        );
    }
    /// @brief Calculates width of the renderer window
    /// @return Width of the renderer window
    virtual size_t GetWidth(void) = 0;
    /// @brief Calculates height of the renderer window
    /// @return Height of the renderer window
    virtual size_t GetHeight(void) = 0;
    /// @brief Renders pixel
    /// @param p Position of pixel
    /// @param color Color of pixel
    /// @return Status
    virtual bool SetPixel(Vector2<num_t> p, uint32_t color) = 0;
    /// @brief Draws a line
    /// @param p1 Start of the line
    /// @param p2 End of the line
    /// @param color Color of the line
    /// @return Status
    virtual bool DrawLine(Vector2<num_t> p1, Vector2<num_t> p2, uint32_t color) = 0;
};

#endif