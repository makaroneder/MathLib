#include <MathLib.hpp>
#include <iostream>
#include <numeric>

template <typename T>
T GreatestCommonDivisor(T a, T b) {
    while (MathLib::Abs(b) > MathLib::eps) {
        const T tmp = b;
        b = std::fmod(a, b);
        a = tmp;
    }
    return a;
}
template <typename T>
T LeastCommonMultiple(const T& a, const T& b) {
    if (MathLib::FloatsEqual<T>(a, 0)) return b;
    else if (MathLib::FloatsEqual<T>(b, 0)) return a;
    else return MathLib::Abs(a * b) / GreatestCommonDivisor<T>(a, b);
}
template <typename T>
T LeastCommonMultiple(const MathLib::Array<T>& array) {
    if (array.IsEmpty()) return 0;
    T ret = array.At(0);
    for (size_t i = 1; i < array.GetSize(); i++) ret = LeastCommonMultiple<T>(ret, array.At(i));
    return ret;
}
MathLib::Array<ssize_t> GetDivisors(size_t p) {
    if (!p) return MathLib::MakeArrayFromSingle<ssize_t>(0);
    MathLib::Array<ssize_t> ret;
    for (size_t i = 1; i <= p; i++) {
        if (!(p % i)) {
            ret.Add(i);
            ret.Add(-i);
        }
    }
    return ret;
}

template <typename T>
struct Polynomial : MathLib::Printable {
    CreateOperators(Polynomial<T>, T)
    CreateExponential(Polynomial<T>, true, Polynomial<T>(MathLib::MakeArrayFromSingle<T>(1)))
    Polynomial(void) {}
    Polynomial(MathLib::Array<T> coefficients) : coefficients(coefficients) {}
    size_t GetDegree(void) const {
        return coefficients.GetSize() - 1;
    }
    size_t GetTermCount(void) const {
        size_t ret = 0;
        for (const T& x : coefficients) ret += !MathLib::FloatsEqual<T>(x, 0);
        return ret;
    }
    T At(size_t x) const {
        return coefficients.At(x);
    }
    template <typename F>
    F Calculate(const F& x) const {
        F ret = 0;
        for (size_t i = 0; i < coefficients.GetSize(); i++)
            ret += MathLib::Pow(x, i) * coefficients.At(i);
        return ret;
    }
    void Divide(const Polynomial<T>& other, Polynomial<T>& div, Polynomial<T>& mod) const {
        ssize_t degree = GetDegree() - other.GetDegree();
        div = Polynomial<T>(degree <= 0 ? 1 : degree + 1);
        mod = *this;
        while (true) {
            degree = mod.GetDegree() - other.GetDegree();
            if (degree < 0) break;
            const T ret = mod.coefficients.At(mod.GetDegree()) / other.coefficients.At(other.GetDegree());
            div.coefficients.At(degree) += ret;
            MathLib::Array<T> coeffs = MathLib::Array<T>(degree + 1);
            coeffs.At(degree) = ret;
            mod -= other * Polynomial<T>(coeffs);
            if (!degree) break;
        }
    }
    template <typename F>
    MathLib::Array<F> Solve(void) const {
        Polynomial<T> tmp = *this * LeastCommonMultiple<T>(coefficients);
        MathLib::Array<F> ret;
        while (true) {
            const MathLib::Array<ssize_t> pDivs = GetDivisors(MathLib::Abs(tmp.coefficients.At(0)));
            const MathLib::Array<ssize_t> qDivs = GetDivisors(MathLib::Abs(tmp.coefficients.At(tmp.GetDegree())));
            bool any = false;
            for (const ssize_t& p : pDivs) {
                for (const ssize_t& q : qDivs) {
                    const F t = p / q;
                    if (MathLib::FloatsEqual<F>(Calculate(t), 0) && !ret.Contains(t)) {
                        ret.Add(t);
                        any = true;
                        MathLib::Array<T> arr = MathLib::Array<T>(2);
                        arr.At(0) = -(T)t;
                        arr.At(1) = 1;
                        tmp /= Polynomial<T>(arr);
                    }
                }
            }
            if (!any) {
                if (tmp.GetDegree() == 2) {
                    const F delta = MathLib::Pow(tmp.coefficients.At(1), 2) - tmp.coefficients.At(2) * tmp.coefficients.At(0) * 4;
                    const F x1 = (-F(tmp.coefficients.At(1)) - MathLib::Sqrt(delta)) / (tmp.coefficients.At(2) * 2);
                    const F x2 = (-F(tmp.coefficients.At(1)) + MathLib::Sqrt(delta)) / (tmp.coefficients.At(2) * 2);
                    if (!ret.Contains(x1)) ret.Add(x1);
                    if (!ret.Contains(x2)) ret.Add(x2);
                }
                return ret;
            }
        }
    }
    MathLib::Expected<Polynomial<T>> Composition(const Polynomial<T>& other) const {
        Polynomial<T> ret = MathLib::Array<T>(coefficients.GetSize() + other.coefficients.GetSize() - 1);
        for (size_t i = 0; i < coefficients.GetSize(); i++) {
            const MathLib::Expected<Polynomial<T>> tmp = other.UnsignedPow(i);
            if (!tmp.HasValue()) return MathLib::Expected<Polynomial<T>>();
            else ret += tmp.Get() * coefficients.At(i);
        }
        return MathLib::Expected<Polynomial<T>>(ret);
    }
    Polynomial<T> Derivative(void) const {
        Polynomial<T> ret = Polynomial<T>(GetDegree());
        for (size_t i = 1; i < coefficients.GetSize(); i++)
            ret.coefficients.At(i - 1) += coefficients.At(i) * i;
        return ret;
    }
    Polynomial<T> AntiDerivative(const T& constantOfIntegration) const {
        Polynomial<T> ret = Polynomial<T>(coefficients.GetSize() + 1);
        ret.coefficients.At(0) = constantOfIntegration;
        for (size_t i = 0; i < coefficients.GetSize(); i++)
            ret.coefficients.At(i + 1) += coefficients.At(i) / (i + 1);
        return ret;
    }
    Polynomial<T> operator*(const Polynomial<T>& other) const {
        Polynomial<T> ret = MathLib::Array<T>(coefficients.GetSize() + other.coefficients.GetSize() - 1);
        for (size_t i = 0; i < coefficients.GetSize(); i++)
            for (size_t j = 0; j < other.coefficients.GetSize(); j++)
                ret.coefficients.At(i + j) += coefficients.At(i) * other.coefficients.At(j);
        return ret;
    }
    Polynomial<T> operator/(const Polynomial<T>& other) const {
        Polynomial<T> unused;
        Polynomial<T> ret;
        Divide(other, ret, unused);
        return ret;
    }
    Polynomial<T> operator%(const Polynomial<T>& other) const {
        Polynomial<T> unused;
        Polynomial<T> ret;
        Divide(other, unused, ret);
        return ret;
    }
    Polynomial<T>& operator*=(const Polynomial<T>& other) {
        return *this = *this * other;
    }
    Polynomial<T>& operator/=(const Polynomial<T>& other) {
        return *this = *this / other;
    }
    Polynomial<T>& operator%=(const Polynomial<T>& other) {
        return *this = *this % other;
    }
    bool operator==(const Polynomial<T>& other) const {
        if (coefficients.GetSize() != other.coefficients.GetSize()) return false;
        for (size_t i = 0; i < coefficients.GetSize(); i++)
            if (!MathLib::FloatsEqual<T>(coefficients.At(i), other.coefficients.At(i))) return false;
        return true;
    }
    /// @brief Converts struct to string
    /// @param padding String to pad with
    /// @return String representation
    virtual MathLib::String ToString(const MathLib::String& padding = "") const override {
        MathLib::String ret;
        for (size_t i = GetDegree(); i; i--) {
            const MathLib::String tmp = MathLib::CoefficientToString(coefficients.At(i), "x");
            if (!tmp.IsEmpty()) ret += padding + tmp + (i == 1 ? "" : (MathLib::String('^') + MathLib::ToString(i, 10))) + " + ";
        }
        return !ret.IsEmpty() && MathLib::FloatsEqual<T>(coefficients.At(0), 0) ? MathLib::SubString(ret, 0, ret.GetSize() - 3) : ret + padding + MathLib::ToString(coefficients.At(0));
    }

    private:
    MathLib::Array<T> coefficients;

    void Add(const Polynomial<T>& other) {
        if (coefficients.GetSize() < other.coefficients.GetSize()) {
            const MathLib::Array<T> tmp = coefficients;
            coefficients = MathLib::Array<T>(other.coefficients.GetSize());
            for (size_t i = 0; i < tmp.GetSize(); i++) coefficients.At(i) = tmp.At(i);
        }
        for (size_t i = 0; i < other.coefficients.GetSize(); i++)
            coefficients.At(i) += other.coefficients.At(i);
        size_t reduce = 0;
        for (size_t i = GetDegree(); i; i--) {
            if (MathLib::FloatsEqual<T>(coefficients.At(i), 0)) reduce++;
            else break;
        }
        const MathLib::Array<T> tmp = coefficients;
        coefficients = MathLib::Array<T>(coefficients.GetSize() - reduce);
        for (size_t i = 0; i < coefficients.GetSize(); i++) coefficients.At(i) = tmp.At(i);
    }
    void Multiply(const T& scalar) {
        if (MathLib::FloatsEqual<T>(scalar, 0)) coefficients = MathLib::MakeArrayFromSingle<T>(0);
        else for (T& x : coefficients) x *= scalar;
    }
};
template <typename T>
MathLib::Expected<Polynomial<T>> SplitPolynomial(const MathLib::Node* node) {
    const MathLib::Expected<Polynomial<T>> left = node->left ? SplitPolynomial<T>(node->left) : MathLib::Expected<Polynomial<T>>();
    const MathLib::Expected<Polynomial<T>> right = node->right ? SplitPolynomial<T>(node->right) : MathLib::Expected<Polynomial<T>>();
    switch (node->type) {
        case MathLib::Node::Type::Add: return left.HasValue() && right.HasValue() ? MathLib::Expected<Polynomial<T>>(left.Get() + right.Get()) : MathLib::Expected<Polynomial<T>>();
        case MathLib::Node::Type::Sub: return left.HasValue() && right.HasValue() ? MathLib::Expected<Polynomial<T>>(left.Get() - right.Get()) : MathLib::Expected<Polynomial<T>>();
        case MathLib::Node::Type::Mul: return left.HasValue() && right.HasValue() ? MathLib::Expected<Polynomial<T>>(left.Get() * right.Get()) : MathLib::Expected<Polynomial<T>>();
        case MathLib::Node::Type::Div: return left.HasValue() && right.HasValue() ? MathLib::Expected<Polynomial<T>>(left.Get() / right.Get()) : MathLib::Expected<Polynomial<T>>();
        case MathLib::Node::Type::Pow: return left.HasValue() && right.HasValue() && !right.Get().GetDegree() ? left.Get().UnsignedPow((size_t)right.Get().At(0)) : MathLib::Expected<Polynomial<T>>();
        case MathLib::Node::Type::Constant: return MathLib::Expected<Polynomial<T>>(Polynomial<T>(MathLib::MakeArrayFromSingle<T>((T)node->ToNumber().At(0))));
        case MathLib::Node::Type::Variable: {
            MathLib::Array<T> tmp = MathLib::Array<T>(2);
            tmp.At(1) = 1;
            return MathLib::Expected<Polynomial<T>>(Polynomial<T>(tmp));
        }
        default: return MathLib::Expected<Polynomial<T>>();
    }
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int argc, char** argv) {
    try {
        if (argc < 2) MathLib::Panic(MathLib::String("Usage: ") + argv[0] + " <input file>");
        MathLib::HostFileSystem fs;
        MathLib::Node* root = MathLib::Tokenize(MathLib::Preproces(fs, argv[1]));
        #ifdef Debug
        std::cout << "Generated nodes:\n" << *root << std::endl;
        #endif
        MathLib::Optimizer optimizer = MathLib::Optimizer();
        MathLib::Node* optimizedRoot = optimizer.Optimize(root);
        delete root;
        #ifdef Debug
        std::cout << "Optimized nodes:\n" << *optimizedRoot << std::endl;
        #endif
        delete optimizedRoot;
        optimizer.runtime = true;
        const Polynomial<MathLib::num_t> w = SplitPolynomial<MathLib::num_t>(optimizer.GetFunction("w").body).Get("Failed to read polynomials from file");
        const Polynomial<MathLib::num_t> q = SplitPolynomial<MathLib::num_t>(optimizer.GetFunction("q").body).Get("Failed to read polynomials from file");
        optimizer.Destroy();
        std::cout << "W(x) = " << w << '\n';
        std::cout << "Q(x) = " << q << '\n';
        std::cout << "W(x) + Q(x) = " << w + q << '\n';
        std::cout << "W(x) - Q(x) = " << w - q << '\n';
        std::cout << "W(x) * Q(x) = " << w * q << '\n';
        std::cout << "W(x) / Q(x) = " << w / q << '\n';
        std::cout << "W(x) % Q(x) = " << w % q << '\n';
        std::cout << "W(Q(x)) = " << w.Composition(q).Get("Failed to compose 2 polynomials") << '\n';
        std::cout << "W'(x) = " << w.Derivative() << '\n';
        std::cout << "integral(W(x) dx) = " << w.AntiDerivative(0) << " + C\n";
        const MathLib::Array<MathLib::complex_t> tmp = w.Solve<MathLib::complex_t>();
        std::cout << "W(x) = 0 for x = {\n";
        for (const MathLib::complex_t& x : tmp) std::cout << '\t' << x << '\n';
        std::cout << '}' << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}