#include "Test.hpp"
#include <Units/Units.hpp>
#include <iostream>
#define TestOperation(expr, expected) if ((expr) != (expected)) test.failed++

// TODO: Add more test cases

/// @brief Tests whether const function return values are correct
/// @tparam T Type of number
/// @param test Test case
/// @param inputSet Input set
/// @param outputSet Output set
/// @param f Function to test
/// @return Status
template <typename T>
bool TestFunction(Test& test, std::vector<T> inputSet, std::vector<T> outputSet, std::function<std::complex<T>(T)> f) {
    if (!test.DrawFunction<T>(test.GenerateFunction<num_t>(f, inputSet, outputSet, VectorAxis::X, VectorAxis::Y), 0)) return false;
    for (Vector3<T>& val : test.values) {
        T tmp = GetVectorAxis(val, VectorAxis::X);
        GetVectorAxis(val, VectorAxis::X) = GetVectorAxis(val, VectorAxis::Y);
        GetVectorAxis(val, VectorAxis::Y) = tmp;
    }
    return test.DrawFunction<T>(test.GenerateFunction<num_t>(f, inputSet, outputSet, VectorAxis::Y, VectorAxis::X), 0);
}
/// @brief Tests whether const function return values are correct
/// @tparam T Type of number
/// @param test Test case
/// @param inputSet Input set
/// @param outputSet Output set
/// @param c Const value
/// @return Status
template <typename T>
bool TestConstFunction(Test& test, std::vector<T> inputSet, std::vector<T> outputSet, T c) {
    test.values = std::vector<Vector3<T>>(inputSet.size());
    for (T& x : inputSet) test.values.push_back(CreateVector3<T>(x, c, 0));
    return TestFunction<T>(test, inputSet, outputSet, ConstFunction(T, c));
}
int main(void) {
    try {
        Test test = Test(800, 800);
        std::vector<num_t> inputSet = test.CreateRealNumberSet<num_t>();
        for (num_t& i : inputSet) TestOperation(IsInsideSet<num_t>(inputSet, i), true);
        TestOperation(IsInsideSet<num_t>({ }, 0), true);
        TestOperation(IsInsideSet<num_t>({ NAN, }, 0), false);
        TestOperation(IsInsideSet<num_t>({ INFINITY, }, 0), false);
        TestOperation(IsInsideSet<num_t>({ }, NAN), false);
        TestOperation(IsInsideSet<num_t>({ }, INFINITY), false);
        TestOperation(IsInsideSet<num_t>({ NAN, }, NAN), false);
        TestOperation(IsInsideSet<num_t>({ INFINITY, }, INFINITY), false);
        TestOperation(Time<num_t>::Day(1), Time<num_t>::Hour(24));
        TestOperation(Time<num_t>::Hour(1), Time<num_t>::Minute(60));
        TestOperation(Time<num_t>::Minute(1), Time<num_t>::Second(60));
        TestOperation(Length<num_t>::Kilometre(1), Length<num_t>::Metre(1000));
        TestOperation(Length<num_t>::Metre(1), Length<num_t>::Centimetre(100));
        TestOperation(Mass<num_t>::Kilogram(1), Mass<num_t>::Gram(1000));
        TestOperation((Length<num_t>::Kilometre(1) / Time<num_t>::Hour(1)), Velocity<num_t>::KilometrePerHour(1));
        TestOperation((Length<num_t>::Metre(1) / Time<num_t>::Second(1)), Velocity<num_t>::MetrePerSecond(1));
        TestOperation((Velocity<num_t>::MetrePerSecond(1) / Time<num_t>::Second(1)), Acceleration<num_t>::MetrePerSecondSquared(1));
        TestOperation((Acceleration<num_t>::MetrePerSecondSquared(1) * Mass<num_t>::Kilogram(1)), Force<num_t>::Newton(1));
        TestOperation((Mass<num_t>::Kilogram(1) * Acceleration<num_t>::MetrePerSecondSquared(1)), Force<num_t>::Newton(1));
        if (!TestConstFunction<num_t>(test, inputSet, {}, 0)) throw std::runtime_error("Failed to test functions");
        if (test.failed != 0) std::cout << test.failed << " test(s) failed" << std::endl;
        return test.failed;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}