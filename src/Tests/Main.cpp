#include "Test.hpp"
#include <Factorial.hpp>
#include <Physics/Equations.hpp>
#include <iostream>
#define TestOperation(expr) if (!(expr)) test.failed++

// TODO: Add more test cases

/// @brief Tests whether const function return values are correct
/// @tparam T Type of number
/// @param test Test case
/// @param inputSet Input set
/// @param f Function to test
template <typename T>
void TestFunction(Test& test, std::vector<T> inputSet, std::function<T(T)> f) {
    test.DrawFunction<T>(test.GenerateFunction<num_t>(f, inputSet, VectorAxis::X, VectorAxis::Y), UINT32_MAX);
    for (Matrix<T>& val : test.values) std::swap(GetX(val), GetY(val));
    test.DrawFunction<T>(test.GenerateFunction<num_t>(f, inputSet, VectorAxis::Y, VectorAxis::X), UINT32_MAX);
}
/// @brief Tests whether const function return values are correct
/// @tparam T Type of number
/// @param test Test case
/// @param inputSet Input set
/// @param c Const value
template <typename T>
void TestConstFunction(Test& test, std::vector<T> inputSet, T c) {
    test.values = std::vector<Matrix<T>>();
    for (T& x : inputSet) test.values.push_back(CreateVector<T>(x, c, 0));
    TestFunction<T>(test, inputSet, [c](T) -> T { return c; });
}
int main(void) {
    try {
        Test test = Test(800, 800);
        const Kilogram<num_t> mass = Kilogram<num_t>(1);
        const Kilogram<num_t> otherMass = Kilogram<num_t>(1);
        const Metre<num_t> r = Metre<num_t>(1);
        const Newton<num_t> force = GravitationalForce<num_t>(Newton<num_t>(NAN), mass, otherMass, r);
        TestOperation(GravitationalForce<num_t>(force, Kilogram<num_t>(NAN), otherMass, r) == mass);
        TestOperation(GravitationalForce<num_t>(force, mass, Kilogram<num_t>(NAN), r) == otherMass);
        TestOperation(GravitationalForce<num_t>(force, mass, otherMass, Metre<num_t>(NAN)) == r);
        const MetrePerSecondSquared<num_t> acceleration = ForceMassAcceleration<num_t>(force, mass, MetrePerSecondSquared<num_t>(NAN));
        TestOperation(ForceMassAcceleration<num_t>(Newton<num_t>(NAN), mass, acceleration) == force);
        TestOperation(ForceMassAcceleration<num_t>(force, Kilogram<num_t>(NAN), acceleration) == mass);
        TestOperation(Kilometre<num_t>(1) == Metre<num_t>(1000));
        TestOperation((Second<num_t>(1) * Second<num_t>(1)) == Second<num_t>(1).Pow(2));
        std::vector<num_t> inputSet = test.CreateRealNumberSet<num_t>();
        for (num_t& i : inputSet) TestOperation(IsInsideSet<num_t>(inputSet, i));
        TestOperation(Sigmoid<num_t>(0) == 0.5);
        TestOperation(Factorial<num_t>(0) == 1);
        TestOperation(Matrix<num_t>(1, 1, { 2, }).Normalize().GetLength() == 1);
        TestOperation(IsInsideSet<num_t>({ }, 0));
        TestOperation(!IsInsideSet<num_t>({ NAN, }, 0));
        TestOperation(!IsInsideSet<num_t>({ INFINITY, }, 0));
        TestOperation(!IsInsideSet<num_t>({ }, NAN));
        TestOperation(!IsInsideSet<num_t>({ }, INFINITY));
        TestOperation(!IsInsideSet<num_t>({ NAN, }, NAN));
        TestOperation(!IsInsideSet<num_t>({ INFINITY, }, INFINITY));
        TestConstFunction<num_t>(test, inputSet, 0);
        if (test.failed != 0) std::cout << test.failed << " test(s) failed" << std::endl;
        return test.failed;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}