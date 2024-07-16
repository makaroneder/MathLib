#include "Test.hpp"
#include <Matrix.hpp>
#include <MathLib.hpp>
#include <Sigmoid.hpp>
#include <Factorial.hpp>
#include <Physics/SIUnits.hpp>
#include <iostream>

/// @brief Entry point for this program
/// @return Status
int main(int, char**) {
    try {
        Test test;
        TestOperation(test, IsNaN(MakeNaN()));
        TestOperation(test, IsInf(MakeInf()));
        TestOperation(test, Kilometre<num_t>(1) == Metre<num_t>(1000));
        TestOperation(test, (Second<num_t>(1) * Second<num_t>(1)) == Second<num_t>(1).Pow(2));
        TestOperation(test, FloatsEqual<num_t>(Sigmoid<num_t>(0), 0.5));
        TestOperation(test, FloatsEqual<num_t>(Factorial<num_t>(0, 1), 1));
        TestOperation(test, Matrix<num_t>(1, 1, std::vector<num_t> { 2, }).Normalize().GetLength() == 1);
        TestOperation(test, FloatsEqual<num_t>(Pow(1, 0), 1));
        TestOperation(test, FloatsEqual<num_t>(Sqrt(1), 1));
        TestOperation(test, FloatsEqual<num_t>(NaturalLog(1), 0));
        TestOperation(test, FloatsEqual<num_t>(Sign<num_t>(1), 1));
        TestOperation(test, FloatsEqual<num_t>(Sign<num_t>(-1), -1));
        TestOperation(test, FloatsEqual<num_t>(Sign<num_t>(0), 0));
        TestOperation(test, FloatsEqual<num_t>(LinearToGamma<num_t>(1), 1));
        TestOperation(test, FloatsEqual<num_t>(LinearToGamma<num_t>(-1), 0));
        const size_t tests = test.GetRecordCount();
        const size_t passed = test.GetPassed();
        #ifdef Debug        
        std::cout << test << passed << "/" << tests << " tests passed" << std::endl;
        #endif
        if (passed != tests) Panic("Some tests failed");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}