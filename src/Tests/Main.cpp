#include "Test.hpp"
#include <Factorial.hpp>
#include <Physics/Equations.hpp>
#include <iostream>
#define TestOperation(expr) if (!(expr)) throw std::runtime_error(#expr)

// TODO: Add more test cases

/// @brief Tests whether const function return values are correct
/// @tparam T Type of number
/// @param test Test case
/// @param inputSet Input set
/// @param outputSet Output set
/// @param f Function to test
/// @return Status
template <typename T>
bool TestFunction(Test& test, std::vector<T> inputSet, std::vector<T> outputSet, std::function<T(T)> f) {
    if (!test.DrawFunction<T>(test.GenerateFunction<num_t>(f, inputSet, outputSet, VectorAxis::X, VectorAxis::Y), 0)) return false;
    for (Matrix<T>& val : test.values) Swap<num_t>(GetX(val), GetY(val));
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
    test.values = std::vector<Matrix<T>>();
    for (T& x : inputSet) test.values.push_back(CreateVector<T>(x, c, 0));
    return TestFunction<T>(test, inputSet, outputSet, ConstFunction(T, c));
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
        if (!TestConstFunction<num_t>(test, inputSet, {}, 0)) throw std::runtime_error("Failed to test functions");
        if (test.failed != 0) std::cout << test.failed << " test(s) failed" << std::endl;
        return test.failed;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}