#include <Math/Constants.hpp>
#include <Host.hpp>
#include <iostream>

const MathLib::num_t pi2 = MathLib::pi * 2;

template <typename T>
T Sine(const T& x) {
    const size_t n = 10;
    if (x < 0) return Sine<T>(x + pi2);
    if (x >= pi2) return Sine<T>(x - pi2);
    if (x >= MathLib::pi && x < pi2) return -Sine<T>(pi2 - x);
    T ret = 0;
    size_t fact = 1;
    T mul = x;
    for (size_t i = 0; i < n; i++) {
        ret += mul / fact;
        mul *= -x * x;
        fact *= (i * 2 + 2) * (i * 2 + 3);
    }
    return ret;
}
int main(int, char**) {
    try {
        const MathLib::Array<MathLib::num_t> arr = MathLib::MakeArray<MathLib::num_t>(
            0, MathLib::pi / 12, MathLib::pi / 6, MathLib::pi / 4,
            MathLib::pi / 3, MathLib::pi * 5 / 12, MathLib::pi / 2,
            1.0 / 2, 1.0 / 3, 1.0 / 4, 1.0 / 5, 1.0 / 6, 1.0 / 7, 1.0 / 8, 1.0 / 9,
            10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
            -20, -21, -22, -23, -24, -25, -26, -27, -28, -29
        );
        const size_t size = arr.GetSize();
        MathLib::num_t avgDiff = 0;
        MathLib::num_t avgTime1 = 0;
        MathLib::num_t avgTime2 = 0;
        for (size_t i = 0; i < size; i++) {
            const MathLib::num_t x = arr.AtUnsafe(i);
            const MathLib::num_t time1 = MathLib::GetTime();
            const MathLib::num_t sin1 = MathLib::Sin(x);
            const MathLib::num_t time2 = MathLib::GetTime();
            const MathLib::num_t sin2 = Sine<MathLib::num_t>(x);
            const MathLib::num_t time3 = MathLib::GetTime();
            const MathLib::num_t diff = MathLib::Abs(sin1 - sin2);
            const MathLib::num_t t1 = time2 - time1;
            const MathLib::num_t t2 = time3 - time2;
            std::cout << "Sample " << i + 1 << ": {\n";
            std::cout << "\tX: " << x << '\n';
            std::cout << "\tDifference: " << diff << '\n';
            std::cout << "\tTime difference: " << t2 - t1 << '\n';
            std::cout << "\tSin 1: " << sin1 << '\n';
            std::cout << "\tSin 2: " << sin2 << '\n';
            std::cout << "\tTime 1: " << t1 << '\n';
            std::cout << "\tTime 2: " << t2 << '\n';
            std::cout << "}\n";
            avgDiff = (i * avgDiff + diff) / (i + 1);
            avgTime1 = (i * avgTime1 + t1) / (i + 1);
            avgTime2 = (i * avgTime2 + t2) / (i + 1);
        }
        std::cout << "Samples: " << size << '\n';
        std::cout << "Average difference: " << avgDiff << '\n';
        std::cout << "Average time 1: " << avgTime1 << '\n';
        std::cout << "Average time 2: " << avgTime2 << '\n';
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}