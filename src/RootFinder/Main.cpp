#include <MathLib.hpp>
#include <iostream>

template <typename T>
struct Function {
    virtual T Get(const T& x) const = 0;
    T GetFromDerivative(const T& x, size_t derivative) const {
        return derivative ? (GetFromDerivative(x + MathLib::eps, derivative - 1) - GetFromDerivative(x, derivative - 1)) / MathLib::eps : Get(x);
    }
    T NewtonsMethod(const T& x0) const {
        T ret = x0;
        while (!MathLib::IsNaN(ret) && !MathLib::IsInf(ret) && !MathLib::FloatsEqual<T>(Get(ret), 0))
            ret -= Get(ret) / GetFromDerivative(ret, 1);
        return ret;
    }
    T HalleysRationalMethod(const T& x0) const {
        T ret = x0;
        while (!MathLib::IsNaN(ret) && !MathLib::IsInf(ret) && !MathLib::FloatsEqual<T>(Get(ret), 0))
            ret -= Get(ret) * GetFromDerivative(ret, 1) * 2 / (MathLib::Pow(GetFromDerivative(ret, 1), 2) * 2 - Get(ret) * GetFromDerivative(ret, 2));
        return ret;
    }
    T HalleysIrrationalMethod(const T& x0) const {
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
    virtual T Get(const T& x) const override {
        T ret = 0;
        for (size_t i = 0; i < coefficients.GetSize(); i++) ret += coefficients.At(i) * MathLib::Pow(x, i);
        return ret;
    }

    private:
    MathLib::Array<T> coefficients;
};

/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        Polynomial<MathLib::complex_t> func = Polynomial<MathLib::complex_t>(std::vector<MathLib::complex_t> {
            -1, 0, 0, 1,
        });
        std::cout << func.NewtonsMethod(100) << std::endl;
        std::cout << func.HalleysRationalMethod(100) << std::endl;
        std::cout << func.HalleysIrrationalMethod(100) << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}