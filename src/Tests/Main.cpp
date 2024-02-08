#include "Test.hpp"
#include <iostream>

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
    if (!test.DrawFunction<T>(f, 0, inputSet, outputSet, VectorAxis::Y)) return false;
    for (Vector2<T>& val : test.values) {
        T tmp = GetVectorAxis(val, VectorAxis::X);
        GetVectorAxis(val, VectorAxis::X) = GetVectorAxis(val, VectorAxis::Y);
        GetVectorAxis(val, VectorAxis::Y) = tmp;
    }
    return test.DrawFunction<T>(f, 0, inputSet, outputSet, VectorAxis::X);
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
    test.values = std::vector<Vector2<T>>(inputSet.size());
    for (T& x : inputSet) test.values.push_back(CreateVector2<T>(x, c));
    return TestFunction<T>(test, inputSet, outputSet, ConstFunction(T, c));
}
int main(void) {
    try {
        Test test = Test(800, 800);
        std::vector<num_t> inputSet = test.CreateRealNumberSet<num_t>();
        if (!TestConstFunction<num_t>(test, inputSet, {}, 0)) throw std::runtime_error("Failed to test functions");
        if (test.failed != 0) std::cout << test.failed << " test(s) failed" << std::endl;
        return test.failed;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}