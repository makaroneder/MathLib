#include <MathLib.hpp>
#include <iostream>

template <typename T>
struct Function {
    [[nodiscard]] virtual T Get(const T& x) const = 0;
    [[nodiscard]] T GetFromDerivative(const T& x, size_t derivative) const {
        return derivative ? (GetFromDerivative(x + MathLib::eps, derivative - 1) - GetFromDerivative(x, derivative - 1)) / MathLib::eps : Get(x);
    }
    [[nodiscard]] T NewtonsMethod(const T& x0) const {
        T ret = x0;
        while (!MathLib::IsNaN(ret) && !MathLib::IsInf(ret) && !MathLib::FloatsEqual<T>(Get(ret), 0))
            ret -= Get(ret) / GetFromDerivative(ret, 1);
        return ret;
    }
    [[nodiscard]] T HalleysRationalMethod(const T& x0) const {
        T ret = x0;
        while (!MathLib::IsNaN(ret) && !MathLib::IsInf(ret) && !MathLib::FloatsEqual<T>(Get(ret), 0))
            ret -= Get(ret) * GetFromDerivative(ret, 1) * 2 / (MathLib::Pow(GetFromDerivative(ret, 1), 2) * 2 - Get(ret) * GetFromDerivative(ret, 2));
        return ret;
    }
    [[nodiscard]] T HalleysIrrationalMethod(const T& x0) const {
        T ret = x0;
        while (!MathLib::IsNaN(ret) && !MathLib::IsInf(ret) && !MathLib::FloatsEqual<T>(Get(ret), 0)) {
            ret -= (GetFromDerivative(ret, 1) - MathLib::Sqrt(MathLib::Pow(GetFromDerivative(ret, 1), 2) - Get(ret) * GetFromDerivative(ret, 2) * 2)) / GetFromDerivative(ret, 2);
        }
        return ret;
    }
};
template <typename T>
struct Polynomial : Function<T> {
    Polynomial(const MathLib::Array<T>& coefficients) : coefficients(coefficients) {}
    [[nodiscard]] virtual T Get(const T& x) const override {
        T ret = 0;
        for (size_t i = 0; i < coefficients.GetSize(); i++) ret += coefficients.At(i) * MathLib::Pow(x, i);
        return ret;
    }

    private:
    MathLib::Array<T> coefficients;
};
template <typename T>
struct TestFunction : Function<T> {
    TestFunction(const T& c) : c(c) {}
    [[nodiscard]] virtual T Get(const T& w) const override {
        const T wSquared = w * w;
        const T x = (wSquared * -2 + w * c * 3 + w * MathLib::Sqrt(wSquared * 4 * (1 - 3 * MathLib::Pow(10, -1.42)) + w * 4 * (c * -3 - MathLib::Pow(10, -4.71)) + c * c * 9)) / (w * 6 + 2 * MathLib::Pow(10, -3.29));
        const T a = MathLib::Pow(w, 3) * -2 * MathLib::Pow(10, -6.58) + w * 2 * MathLib::Pow(10, -6.58);
        const T b = MathLib::Pow(w, 5) * 4 * MathLib::Pow(10, -3.29) + MathLib::Pow(w, 3) * (2 * MathLib::Pow(10, -6.58) - 4 * MathLib::Pow(10, -3.29));
        const T c = MathLib::Pow(w, 6) * 8 * MathLib::Pow(10, -4.71) - MathLib::Pow(w, 4) * 8 * MathLib::Pow(10, -4.71) - MathLib::Pow(w, 7) * 2 - MathLib::Pow(w, 5) * (2 - 2 * MathLib::Pow(10, -3.29)) + 3 * MathLib::Pow(10, -9.69);
        const T d = MathLib::Pow(w, 8) * -8 * MathLib::Pow(10, -1.42) + MathLib::Pow(w, 6) * (-4 * MathLib::Pow(10, -4.71) + 8 * MathLib::Pow(10, -1.42)) - w * 3 * MathLib::Pow(10, -6.4) + wSquared * 3 * MathLib::Pow(10, -6.4) - 3 * MathLib::Pow(10, -9.69);
        const T e = MathLib::Pow(w, 9) * -8 * MathLib::Pow(10, -2.84) + MathLib::Pow(w, 7) * 8 * + MathLib::Pow(10, -2.84) - wSquared * 6 * + MathLib::Pow(10, -7.82);
        return MathLib::Pow(x, 4) * a + MathLib::Pow(x, 3) * b + x * x * c + x * d + e;
    }

    private:
    T c;
};
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        TestFunction<MathLib::complex_t> func = TestFunction<MathLib::complex_t>(1);
        const MathLib::complex_t x0 = 0.5;
        std::cout << func.NewtonsMethod(x0) << std::endl;
        std::cout << func.HalleysRationalMethod(x0) << std::endl;
        std::cout << func.HalleysIrrationalMethod(x0) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}