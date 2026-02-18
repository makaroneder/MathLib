#include <Tests/Test.hpp>
#include <iostream>

size_t Problem1(size_t limit) {
    const size_t limit3 = --limit / 3;
    const size_t limit5 = limit / 5;
    const size_t limit15 = limit / 15;
    const size_t sum1 = limit3 * (limit3 + 1);
    const size_t sum2 = limit5 * (limit5 + 1);
    const size_t sum3 = limit15 * (limit15 + 1);
    return (sum1 * 3 + sum2 * 5 - sum3 * 15) / 2;
}
size_t Problem2(size_t limit) {
    size_t ret = 0;
    size_t prev = 1;
    size_t curr = 2;
    for (size_t i = 0; curr < limit; i++) {
        if (!(curr % 2)) ret += curr;
        const size_t tmp = curr;
        curr += prev;
        prev = tmp;
    }
    return ret;
}
size_t Problem3(size_t limit) {
    const size_t lim = MathLib::Sqrt(limit);
    for (size_t i = 2; i < lim; i++) {
        if (!(limit % i)) {
            while (true) {
                limit /= i;
                if (limit % i) break;
            }
            const size_t tmp = Problem3(limit);
            return tmp == 1 ? limit : tmp;
        }
    }
    return 1;
}
/// @brief Entry point for this program
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments
/// @return Status
int main(int, char**) {
    try {
        MathLib::Test test;
        TestOperation(test, Problem1(10) == 23);
        TestOperation(test, Problem2(90) == 2 + 8 + 34);
        TestOperation(test, Problem3(13195) == 29);
        std::cout << "1) " << Problem1(1000) << std::endl;
        std::cout << "2) " << Problem2(4000000) << std::endl;
        std::cout << "3) " << Problem3(600851475143) << std::endl;
        std::cout << "-----------" << std::endl;
        std::cout << test << std::endl;
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}